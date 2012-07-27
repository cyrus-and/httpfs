#include <stdlib.h>
#include "generator.h"
#include "httpfs.h"
#include "fuse_api/fuse_api.h"

struct httpfs httpfs;

static void usage()
{
    fprintf( stderr ,
             "Usage:\n\n"
             "    httpfs generate\n"
             "    httpfs mount <url> <mount_point> [<remote_chroot>]\n" );
    exit( EXIT_FAILURE );
}

static void check_remote_availability()
{
    struct stat ss;
    int errno;

    if ( errno = -httpfs_getattr( "/" , &ss ) , errno )
    {
        fprintf( stderr , "Unable to mount: " );

        switch ( errno )
        {
        case ECOMM:
            fprintf( stderr , "cannot reach the remote server\n" );
            break;

        case ENOENT:
            fprintf( stderr , "cannot find the remote path\n" );
            break;

        default:
            fprintf( stderr , "errno (%i) %s\n" , errno , sys_errlist[ errno ]);
        }

        exit( EXIT_FAILURE );
    }
}

int main( int argc , char *argv[] )
{
    if ( argc == 1 ) usage();

    if ( strcmp( argv[ 1 ] , "generate" ) == 0 )
    {
        if ( argc != 2 ) usage();
        httpfs_generate_php();
        return EXIT_SUCCESS;
    }
    else if ( strcmp( argv[ 1 ] , "mount" ) == 0 )
    {
        if ( argc != 4 && argc != 5 ) usage();

        /* global context */
        httpfs.php_url = argv[ 2 ];
        httpfs.remote_chroot = ( argc == 5 ? argv[ 4 ] : NULL );
        httpfs.curl = curl_easy_init();

        if ( !httpfs.curl )
        {
            fprintf( stderr , "Can't initialize cURL\n" );
            return EXIT_FAILURE;
        }

        check_remote_availability();
        return httpfs_fuse_start( &httpfs , argv[ 3 ] );
    }
    else usage();

    return EXIT_SUCCESS;
}
