#include "../phpfs.h"

int phpfs_readlink( const char *path ,
                    char *buf ,
                    size_t size )
{
    PHPFS_DO_SIMPLE_REQUEST( PHPFS_OPCODE_readlink )
    {
        PHPFS_CHECK_RESPONSE_STATUS;

        /* see man 2 readlink */
        if ( response.size <= size - 1 )
        {
            memcpy( buf , response.payload , response.size );
            *( buf + response.size ) = '\0';
        }
        else
        {
            memcpy( buf , response.payload , size );
        }

        PHPFS_CLEANUP;
        return 0;
    }
}
