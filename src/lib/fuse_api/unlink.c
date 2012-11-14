#include "httpfs.h"

int httpfs_unlink( const char *path )
{
    HTTPFS_DO_SIMPLE_REQUEST( HTTPFS_OPCODE_unlink )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}
