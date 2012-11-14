#include "httpfs.h"

int httpfs_truncate( const char *path ,
                     off_t offset )
{
    struct
    {
        uint32_t offset;
    }
    header = { htonl( offset ) };

    HTTPFS_DO_REQUEST_WITH_HEADER( HTTPFS_OPCODE_truncate )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        return 0;
    }
}
