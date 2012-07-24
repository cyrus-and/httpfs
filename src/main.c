#include <stdlib.h>
#include "generator.h"
#include "phpfs.h"

struct phpfs phpfs;

static void usage()
{
    fprintf( stderr ,
             "Usage:\n\n"
             "    phpfs generate\n"
             "    phpfs mount <url> <mount_point> [<remote_chroot>]\n" );
    exit( EXIT_FAILURE );
}

int main( int argc , char *argv[] )
{
    if ( argc == 1 ) usage();

    if ( strcmp( argv[ 1 ] , "generate" ) == 0 )
    {
        if ( argc != 2 ) usage();
        phpfs_generate_php();
        return EXIT_SUCCESS;
    }
    else if ( strcmp( argv[ 1 ] , "mount" ) == 0 )
    {
        if ( argc != 4 && argc != 5 ) usage();

        /* global context */
        phpfs.php_url = argv[ 2 ];
        phpfs.remote_chroot = ( argc == 5 ? argv[ 4 ] : NULL );
        phpfs.curl = curl_easy_init();

        if ( !phpfs.curl )
        {
            fprintf( stderr , "Can't initialize cURL\n" );
            return EXIT_FAILURE;
        }

        return phpfs_fuse_start( &phpfs , argv[ 3 ] );
    }
    else usage();

    return EXIT_SUCCESS;
}
