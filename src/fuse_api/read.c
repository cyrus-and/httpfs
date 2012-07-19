#include "../phpfs.h"

int phpfs_read( const char *path ,
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

    PHPFS_DO_REQUEST( READ )
    {
        PHPFS_CHECK_RESPONSE_STATUS;

        /* TODO check chunk size */
        memcpy( buf , response.payload , response.size );

        PHPFS_CLEANUP;
        return response.size;
    }
}
