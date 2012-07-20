#include "../phpfs.h"

int phpfs_readdir( const char *path ,
                   void *buf ,
                   fuse_fill_dir_t filler ,
                   off_t offset ,
                   struct fuse_file_info *fi )
{
    PHPFS_DO_SIMPLE_REQUEST( PHPFS_OPCODE_readdir )
    {
        char *p;

        PHPFS_CHECK_RESPONSE_STATUS;

        for ( p = response.payload ; p - response.payload < response.size ; p += strlen( p ) + 1 )
        {
            filler( buf , p , NULL , 0 );
        }

        PHPFS_CLEANUP;
        return 0;
    }
}
