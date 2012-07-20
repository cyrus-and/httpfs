#include "phpfs.h"
#include "fuse_api/fuse_api.h"

#define _( x ) #x ,
const char *PHPFS_OPCODE_NAMES[] = { _PHPFS_FUSE_FUNCTIONS };
const char *PHPFS_STATUS_NAMES[] = { _PHPFS_STATUSES };
#undef _

int phpfs_fuse_start( struct phpfs *phpfs ,
                      char *mounting_point )
{
    int argc;
    char *argv[ 4 ];

    const struct fuse_operations operations = {
#define _( x ) .x = phpfs_##x ,
        _PHPFS_FUSE_FUNCTIONS
#undef _
    };

    argc = 0;
    argv[ argc++ ] = "phpfs";
#ifndef NDEBUG
    argv[ argc++ ] = "-s"; /* single thread */
    argv[ argc++ ] = "-d"; /* debug and core dump */
#endif
    argv[ argc++ ] = mounting_point;

    return fuse_main( argc , argv , &operations , phpfs );
}

void phpfs_allocate_request( struct raw_data *in ,
                             uint8_t op ,
                             size_t header_length ,
                             const char *path )
{
    /* produce OP<-- header_length -->path */
    in->size = 1 + header_length + strlen( path );
    in->payload = malloc( in->size );
    *in->payload = op;
    memcpy( in->payload + 1 + header_length , path , in->size - ( 1 + header_length ) );
}
