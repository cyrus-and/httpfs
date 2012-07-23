#include "../phpfs.h"

int phpfs_write( const char *path ,
                 const char *buf ,
                 size_t size ,
                 off_t offset ,
                 struct fuse_file_info *fi )
{
    struct raw_data raw_data = { buf , size };
    struct
    {
        uint32_t size;
        uint32_t offset;
    }
    header = { htonl( size ) ,
               htonl( offset ) };

    PHPFS_DO_REQUEST( PHPFS_OPCODE_write )
    {
        uint32_t write_size;

        PHPFS_CHECK_RESPONSE_STATUS;
        write_size = ntohl( *( uint32_t * )response.payload );
        PHPFS_CLEANUP;
        return write_size;
    }
}
