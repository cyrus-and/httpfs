#include "httpfs.h"

int httpfs_create( const char *path ,
                   mode_t mode ,
                   struct fuse_file_info *fi )
{
    struct
    {
        uint32_t mode;
    }
    header = { htonl( mode ) };

    HTTPFS_DO_REQUEST_WITH_HEADER( HTTPFS_OPCODE_create )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}
