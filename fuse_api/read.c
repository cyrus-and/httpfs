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
    header;

    LOGF( "request read: size %lu ; offset %lu" , size , offset );

    PHPFS_DO_REQUEST( READ )
    {
        LOGF( "read: %lu bytes" , out.size );

        /* TODO check chunk size */
        memcpy( buf , out.payload , out.size );

        PHPFS_CLEANUP;
        return out.size;
    }
}
