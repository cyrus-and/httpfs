#include "httpfs.h"

int httpfs_write( const char *path ,
                  const char *buf ,
                  size_t size ,
                  off_t offset ,
                  struct fuse_file_info *fi )
{
    struct raw_data raw_data = { ( char * )buf , size };
    struct
    {
        uint32_t size;
        uint32_t offset;
    }
    header = { htonl( size ) ,
               htonl( offset ) };

    HTTPFS_DO_REQUEST_WITH_HEADER_AND_DATA( HTTPFS_OPCODE_write )
    {
        uint32_t write_size;

        HTTPFS_CHECK_RESPONSE_STATUS;
        if ( response.size != sizeof( uint32_t ) )
        {
            HTTPFS_CLEANUP;
            HTTPFS_RETURN( EBADMSG );
        }

        write_size = ntohl( *( uint32_t * )response.payload );
        HTTPFS_CLEANUP;
        return write_size;
    }
}
