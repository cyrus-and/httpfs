#include "httpfs.h"

int httpfs_symlink( const char *path ,
                    const char *newpath )
{
    struct raw_data raw_data = { ( char * )newpath , strlen( newpath ) };

    HTTPFS_DO_REQUEST_WITH_DATA( HTTPFS_OPCODE_symlink )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}
