#include <stdlib.h>
#include "generators.h"
#include "httpfs.h"
#include "fuse_api/fuse_api.h"
#include "version.h"

static void usage()
{
    fprintf( stderr ,
             "Usage:\n\n"
             "    httpfs --help\n"
             "    httpfs --version\n"
             "    httpfs generators\n"
             "    httpfs generate <generator>\n"
             "    httpfs mount <url> <mount_point> [<remote_chroot>]\n" );
}

static void info()
{
    fprintf( stderr , "httpfs " HTTPFS_VERSION "\n" );
}

int main( int argc , char *argv[] )
{
    if ( argc == 2 && strcmp( argv[ 1 ] , "--version" ) == 0 )
    {
        info();
    }
    else if ( argc == 2 && strcmp( argv[ 1 ] , "--help" ) == 0 )
    {
        usage();
    }
    else if ( argc == 2 && strcmp( argv[ 1 ] , "generators" ) == 0 )
    {
        const struct httpfs_generator *generator;

        for ( generator = HTTPFS_GENERATORS ; generator->name ; generator++ )
        {
            printf( "%s\n" , generator->name );
        }
    }
    else if ( argc == 3 && strcmp( argv[ 1 ] , "generate" ) == 0 )
    {
        if ( !httpfs_generate( argv[ 2 ] ) )
        {
            usage();
            return EXIT_FAILURE;
        }
    }
    else if ( ( argc == 4 || argc == 5 ) &&
              strcmp( argv[ 1 ] , "mount" ) == 0 )
    {
        struct httpfs httpfs;

        httpfs.php_url = argv[ 2 ];
        httpfs.remote_chroot = ( argc == 5 ? argv[ 4 ] : NULL );
        httpfs.curl = curl_easy_init();

        if ( !httpfs.curl )
        {
            fprintf( stderr , "Can't initialize cURL\n" );
            return EXIT_FAILURE;
        }

        /* check_remote_availability(); */
        return httpfs_fuse_start( &httpfs , argv[ 3 ] );
    }
    else
    {
        usage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
