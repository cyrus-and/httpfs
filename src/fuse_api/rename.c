#include "../phpfs.h"

int phpfs_rename( const char *path , const char *newpath)
{
    struct raw_data raw_data = { ( char * )newpath, strlen(newpath) };

    PHPFS_DO_REQUEST_WITH_DATA( PHPFS_OPCODE_rename )
    {
        PHPFS_CHECK_RESPONSE_STATUS;

        PHPFS_CLEANUP;
        return 0;
    }
}
