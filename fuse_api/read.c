#include "../phpfs.h"

int phpfs_read( const char *path ,
                char *buf ,
                size_t size ,
                off_t offset ,
                struct fuse_file_info *fi )
{
    struct raw_data in , out;

    LOGF( "request read: size %lu ; offset %lu" , size , offset );

    in.size = 1 + 2 * sizeof( uint32_t ) + strlen( path ); /* TODO type size */
    in.payload = malloc( in.size );

    *in.payload = READ;
    memcpy( in.payload + 1 , &size , 4 );
    memcpy( in.payload + 1 + 4 , &offset , 4 );
    memcpy( in.payload + 1 + 2 * sizeof( uint32_t ) , path , in.size - (1 + 2 * sizeof( uint32_t ) ) );

    if ( CURLE_OK == phpfs_do_post( &in , &out ) )
    {
        LOGF( "read: %lu bytes" , out.size );

        /* TODO check chunk size */
        memcpy( buf , out.payload , out.size );

        free( in.payload );
        free( out.payload );
        return out.size;
    }

    free( in.payload );
    return -ECOMM;
}
