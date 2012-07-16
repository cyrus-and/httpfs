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
