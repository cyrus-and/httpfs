#include "../phpfs.h"

int phpfs_rmdir( const char *path )
{
    PHPFS_DO_SIMPLE_REQUEST( PHPFS_OPCODE_rmdir )
    {

        PHPFS_CHECK_RESPONSE_STATUS;

        PHPFS_CLEANUP;
        return 0;
    }
}
