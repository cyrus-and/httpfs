#include "../httpfs.h"

int httpfs_mkdir( const char *path ,
                  mode_t mode )
{
    struct
    {
        uint32_t mode;
    }
    header = { htonl( mode | S_IFDIR ) };

    HTTPFS_DO_REQUEST_WITH_HEADER( HTTPFS_OPCODE_mkdir )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        return 0;
    }
}
