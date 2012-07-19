#include "../phpfs.h"

int phpfs_getattr( const char *path ,
                   struct stat *stbuf )
{
    PHPFS_DO_SIMPLE_REQUEST( GETATTR )
    {
        /* TODO check type */
        struct attrs
        {
            uint32_t st_mode;
            uint32_t st_nlink;
            uint32_t st_size;
        }
        attrs;

        PHPFS_CHECK_RESPONSE_STATUS;

        memset( stbuf , 0 , sizeof( struct stat ) );

        attrs = *( struct attrs * )response.payload;
        stbuf->st_mode = ntohl( attrs.st_mode );
        stbuf->st_nlink = ntohl( attrs.st_nlink );
        stbuf->st_size = ntohl( attrs.st_size );

        /* LOGF( "'%s': mode %o ; nlink %i ; size %i" , path , attrs.st_mode , attrs.st_nlink , attrs.st_size ); */

        PHPFS_CLEANUP;
        return 0;
    }
}
