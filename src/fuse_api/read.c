#include "../httpfs.h"

int httpfs_read( const char *path ,
                 char *buf ,
                 size_t size ,
                 off_t offset ,
                 struct fuse_file_info *fi )
{
    struct
    {
        uint32_t size;
        uint32_t offset;
    }
    header = { htonl( size ) ,
               htonl( offset ) };

    HTTPFS_DO_REQUEST_WITH_HEADER( HTTPFS_OPCODE_read )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;

        /* TODO check chunk size */
        memcpy( buf , response.payload , response.size );

        HTTPFS_CLEANUP;
        return response.size;
    }
}
