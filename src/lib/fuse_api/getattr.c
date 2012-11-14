#include "httpfs.h"

int httpfs_getattr( const char *path ,
                    struct stat *stbuf )
{
    HTTPFS_DO_SIMPLE_REQUEST( HTTPFS_OPCODE_getattr )
    {
        struct attrs
        {
            uint32_t dev;
            uint32_t ino;
            uint32_t mode;
            uint32_t nlink;
            uint32_t uid;
            uint32_t gid;
            uint32_t rdev;
            uint32_t size;
            uint32_t atime;
            uint32_t mtime;
            uint32_t ctime;
            uint32_t blksize;
            uint32_t blocks;
        }
        attrs;

        HTTPFS_CHECK_RESPONSE_STATUS;
        if ( response.size != sizeof( struct attrs ) )
        {
            HTTPFS_CLEANUP;
            HTTPFS_RETURN( EBADMSG );
        }

        memset( stbuf , 0 , sizeof( struct stat ) );
        attrs = *( struct attrs * )response.payload;
        stbuf->st_dev = ntohl( attrs.dev );
        stbuf->st_ino = ntohl( attrs.ino );
        stbuf->st_mode = ntohl( attrs.mode );
        stbuf->st_nlink = ntohl( attrs.nlink );
        stbuf->st_uid = ntohl( attrs.uid );
        stbuf->st_gid = ntohl( attrs.gid );
        stbuf->st_rdev = ntohl( attrs.rdev );
        stbuf->st_size = ntohl( attrs.size );
        stbuf->st_atime = ntohl( attrs.atime );
        stbuf->st_mtime = ntohl( attrs.mtime );
        stbuf->st_ctime = ntohl( attrs.ctime );
        stbuf->st_blksize = ntohl( attrs.blksize );
        stbuf->st_blocks = ntohl( attrs.blocks );

        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}
