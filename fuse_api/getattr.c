#include "../phpfs.h"

int phpfs_getattr( const char *path ,
                   struct stat *stbuf )
{
    struct raw_data in , out;

    in.size = 1 + strlen( path );
    in.payload = malloc( in.size );
    *in.payload = GETATTR;
    memcpy( in.payload + 1 , path , in.size - 1 );

    if ( CURLE_OK == phpfs_do_post( &in , &out ) )
    {
        /* TODO check type */
        struct attrs
        {
            int st_mode;
            int st_nlink;
            int st_size;
        }
        attrs;

        memset( stbuf , 0 , sizeof( struct stat ) );

        attrs = *( struct attrs * )out.payload;
        stbuf->st_mode = attrs.st_mode;
        stbuf->st_nlink = attrs.st_nlink;
        stbuf->st_size = attrs.st_size;

        /* LOGF( "'%s': mode %o ; nlink %i ; size %i" , path , attrs.st_mode , attrs.st_nlink , attrs.st_size ); */

        free( in.payload );
        free( out.payload );
        return 0;
    }

    free( in.payload );
    return -ECOMM;
}
