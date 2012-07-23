#include "../phpfs.h"

int phpfs_truncate( const char *path ,
                    off_t offset )
{
    struct raw_data raw_data = { 0 };
    struct
    {
        uint32_t offset;
    }
    header = { htonl( offset ) };

    PHPFS_DO_REQUEST( PHPFS_OPCODE_truncate )
    {
        PHPFS_CHECK_RESPONSE_STATUS;
        PHPFS_CLEANUP;
        return 0;
    }
}
