#include "../httpfs.h"

int httpfs_readlink( const char *path ,
                     char *buf ,
                     size_t size )
{
    HTTPFS_DO_SIMPLE_REQUEST( HTTPFS_OPCODE_readlink )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;

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

        HTTPFS_CLEANUP;
        return 0;
    }
}
