#include "../phpfs.h"

int phpfs_getattr( const char *path ,
                   struct stat *stbuf )
{
    PHPFS_DO_SIMPLE_REQUEST( GETATTR )
    {
        /* TODO check type */
        struct attrs
        {
            int st_mode;
            int st_nlink;
            int st_size;
        }
        attrs;

        PHPFS_CHECK_RESPONSE_STATUS;

        memset( stbuf , 0 , sizeof( struct stat ) );

        attrs = *( struct attrs * )out.payload;
        stbuf->st_mode = attrs.st_mode;
        stbuf->st_nlink = attrs.st_nlink;
        stbuf->st_size = attrs.st_size;

        /* LOGF( "'%s': mode %o ; nlink %i ; size %i" , path , attrs.st_mode , attrs.st_nlink , attrs.st_size ); */

        PHPFS_CLEANUP;
        return 0;
    }
}
