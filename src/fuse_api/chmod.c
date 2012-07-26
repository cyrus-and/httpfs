#include "../phpfs.h"

int phpfs_chmod( const char *path ,
                  mode_t mode )
{
    struct
    {
        uint32_t mode;
    }
    header = { htonl( mode ) };

    PHPFS_DO_REQUEST_WITH_HEADER( PHPFS_OPCODE_chmod )
    {
        PHPFS_CHECK_RESPONSE_STATUS;
        PHPFS_CLEANUP;
        return 0;
    }
}
