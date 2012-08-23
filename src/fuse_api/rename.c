#include "../httpfs.h"

int httpfs_rename( const char *path ,
                   const char *newpath )
{
    struct raw_data raw_data = { ( char * )newpath , strlen( newpath ) };

    HTTPFS_DO_REQUEST_WITH_DATA( HTTPFS_OPCODE_rename )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        return 0;
    }
}
