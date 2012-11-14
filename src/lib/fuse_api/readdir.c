#include "httpfs.h"

int httpfs_readdir( const char *path ,
                    void *buf ,
                    fuse_fill_dir_t filler ,
                    off_t offset ,
                    struct fuse_file_info *fi )
{
    HTTPFS_DO_SIMPLE_REQUEST( HTTPFS_OPCODE_readdir )
    {
        char *p;

        HTTPFS_CHECK_RESPONSE_STATUS;

        for ( p = response.payload ;
              p - response.payload < response.size ;
              p += strnlen( p , response.size - ( p - response.payload ) ) + 1 )
        {
            filler( buf , p , NULL , 0 );
        }

        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}
