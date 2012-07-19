#include <stdlib.h>
#include "phpfs.h"

struct phpfs phpfs;

int main( int argc , char *argv[] )
{
    if ( argc != 3 )
    {
        fprintf( stderr , "Usage: phpfs <url> <path>\n" );
        return EXIT_FAILURE;
    }

    /* global context */
    phpfs.php_url = argv[ 1 ];
    phpfs.curl = curl_easy_init();

    if ( !phpfs.curl )
    {
        fprintf( stderr , "Can't initialize cURL\n" );
        return EXIT_FAILURE;
    }

    return phpfs_fuse_start( &phpfs , argv[ 2 ] );
}
