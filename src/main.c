#include <stdlib.h>
#include "generators.h"
#include "httpfs.h"
#include "fuse_api/fuse_api.h"
#include "version.h"

struct httpfs httpfs;

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
            fprintf( stderr , "errno (%i) %s\n" , errno , strerror( errno ) );
        }

        exit( EXIT_FAILURE );
    }
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
#define _( x ) printf( #x "\n" );
#include "generators.def"
    }
    else if ( argc == 3 && strcmp( argv[ 1 ] , "generate" ) == 0 )
    {
        int i;
        struct generator
        {
            const char *name;
            void ( *function )();
        }
        generators[] = {
#define _( x ) { #x , httpfs_generate_##x } ,
#include "generators.def"
        };

        for ( i = 0 ; i < sizeof( generators ) / sizeof( struct generator ) ; i++ )
        {
            if ( strcmp( generators[ i ].name , argv[ 2 ] ) == 0 )
            {
                generators[ i ].function();
                return EXIT_SUCCESS;
            }
        }

        usage();
        return EXIT_FAILURE;
    }
    else if ( ( argc == 4 || argc == 5 ) &&
              strcmp( argv[ 1 ] , "mount" ) == 0 )
    {
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
    else
    {
        usage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
