#include "httpfs.h"

int httpfs_symlink( const char *path ,
                    const char *newpath )
{
    struct httpfs *httpfs = fuse_get_context()->private_data;
    char aux[ 4096 ] = { 0 };
    struct raw_data raw_data = { aux , 0 };

    /* prepend remote chroot */
    if ( httpfs->remote_chroot )
    {
        raw_data.size = strlen( httpfs->remote_chroot );
        strcat( raw_data.payload , httpfs->remote_chroot );
    }

    /* append the other path  */
    strcat( raw_data.payload , newpath );
    raw_data.size += strlen( newpath );

    HTTPFS_DO_REQUEST_WITH_DATA( HTTPFS_OPCODE_symlink )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}
