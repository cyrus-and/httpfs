#include "../phpfs.h"

int phpfs_readdir( const char *path ,
                   void *buf ,
                   fuse_fill_dir_t filler ,
                   off_t offset ,
                   struct fuse_file_info *fi )
{
    struct raw_data in , out;

    in.size = 1 + strlen( path );
    in.payload = malloc( in.size );
    *in.payload = READDIR;
    memcpy( in.payload + 1 , path , in.size - 1 );

    if ( CURLE_OK == phpfs_do_post( PHPFS( curl ) , PHPFS( php_url ) , &in , &out ) )
    {
        char *p;

        for ( p = out.payload ; p - out.payload < out.size ; p += strlen( p ) + 1 )
        {
            /* LOGF( "dir entry: '%s'" , p ); */
            filler( buf , p , NULL , 0 );
        }

        free( in.payload );
        free( out.payload );
        return 0;
    }

    free( in.payload );
    return -ECOMM;
}
