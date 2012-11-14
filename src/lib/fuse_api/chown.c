#include "httpfs.h"

int httpfs_chown( const char *path ,
                  uid_t uid ,
                  gid_t gid )
{
    struct
    {
        uint32_t uid;
        uint32_t gid;
    }
    header = { htonl( uid ) ,
               htonl( gid ) };

    HTTPFS_DO_REQUEST_WITH_HEADER( HTTPFS_OPCODE_chown )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        return 0;
    }
}
