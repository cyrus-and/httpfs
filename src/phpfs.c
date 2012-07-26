#include "phpfs.h"
#include "fuse_api/fuse_api.h"

const char *PHPFS_OPCODE_NAMES[] = {
    "NONE" ,
#define _( x ) #x ,
#include "fuse_functions.def"
    NULL
};

const char *PHPFS_STATUS_NAMES[] = {
#define _( x ) #x ,
#include "statuses.def"
    NULL
};

int phpfs_fuse_start( struct phpfs *phpfs ,
                      char *mount_point )
{
    int argc;
    char *argv[ 4 ];

    const struct fuse_operations operations = {
#define _( x ) .x = phpfs_##x ,
#include "fuse_functions.def"
    };

    argc = 0;
    argv[ argc++ ] = "phpfs";
    argv[ argc++ ] = "-s"; /* single thread */
#ifndef NDEBUG
    argv[ argc++ ] = "-d"; /* debug and core dump */
#endif
    argv[ argc++ ] = mount_point;

    return fuse_main( argc , argv , &operations , phpfs );
}

void phpfs_prepare_request( struct raw_data *in ,
                            uint8_t opcode ,
                            struct raw_data *header ,
                            const char *path ,
                            struct raw_data *data )
{
    size_t offset , header_size , remote_chroot_length , path_length , data_size;

    header_size = ( header ? header->size : 0 );
    remote_chroot_length = ( phpfs.remote_chroot ? strlen( phpfs.remote_chroot ) : 0 );
    path_length = strlen( path ) + 1;
    data_size = ( data ? data->size : 0 );

    in->size = 1 + header_size + remote_chroot_length + path_length + data_size;
    in->payload = malloc( in->size );

    /* opcode */
    *in->payload = opcode;

    /* header */
    offset = 1;
    if ( header )
    {
        memcpy( in->payload + offset , header->payload , header->size );
    }

    /* path */
    offset += header_size;
    memcpy( in->payload + offset , phpfs.remote_chroot , remote_chroot_length );
    offset += remote_chroot_length;
    memcpy( in->payload + offset , path , path_length );

    /* data */
    if ( data )
    {
        offset += path_length;
        memcpy( in->payload + offset , data->payload , data->size );
    }
}
