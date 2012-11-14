#include "httpfs.h"

int httpfs_chmod( const char *path ,
                  mode_t mode )
{
    struct
    {
        uint32_t mode;
    }
    header = { htonl( mode ) };

    HTTPFS_DO_REQUEST_WITH_HEADER( HTTPFS_OPCODE_chmod )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}
