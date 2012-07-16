#include "phpfs.h"
#include "fuse_api/fuse_api.h"

#define FUSE_OPERATION( op ) .op = phpfs_##op

int phpfs_fuse_start( struct phpfs *phpfs , char *mounting_point )
{
    int argc;
    char *argv[ 3 ];

    const struct fuse_operations operations = {
        FUSE_OPERATION( getattr ) ,
        FUSE_OPERATION( readdir ) ,
        FUSE_OPERATION( read ) ,
        /*...*/
    };

    argc = 0;
    argv[ argc++ ] = "phpfs";
    argv[ argc++ ] = "-s"; /* single thread */
    argv[ argc++ ] = mounting_point;

    return fuse_main( argc , argv , &operations , phpfs );
}

void phpfs_allocate_request( struct raw_data *in , uint8_t op , size_t header_length , const char *path )
{
    /* produce OP<-- header_length -->path */
    in->size = 1 + header_length + strlen( path );
    in->payload = malloc( in->size );
    *in->payload = op;
    strcpy( in->payload + 1 + header_length , path );
}
