#include "../phpfs.h"

int phpfs_chown( const char *path ,
                uid_t uid ,
                gid_t gid )
{

    struct
    {
        uint32_t uid;
        uint32_t gid;
    }
    header = { htonl( uid ) ,
               htonl( gid ) };

    PHPFS_DO_REQUEST_WITH_HEADER( PHPFS_OPCODE_chown )
    {
        PHPFS_CHECK_RESPONSE_STATUS;

        PHPFS_CLEANUP;
        return 0;
    }
}
