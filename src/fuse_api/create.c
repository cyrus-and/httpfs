#include "../phpfs.h"

int phpfs_create( const char *path ,
                  mode_t mode ,
                  struct fuse_file_info *fi )
{
    struct
    {
        uint32_t mode;
    }
    header = { htonl( mode ) };

    PHPFS_DO_REQUEST_WITH_HEADER( PHPFS_OPCODE_create )
    {
        PHPFS_CHECK_RESPONSE_STATUS;
        PHPFS_CLEANUP;
        return 0;
    }
}
