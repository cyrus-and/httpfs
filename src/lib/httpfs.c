#include "httpfs.h"
#include "fuse_api/fuse_api.h"

const char *HTTPFS_OPCODE_NAMES[] = {
    "NONE" ,
#define _( x ) #x ,
#include "fuse_functions.def"
    NULL
};

const char *HTTPFS_STATUS_NAMES[] = {
#define _( x ) #x ,
#include "statuses.def"
    NULL
};

static int check_remote_availability( struct httpfs *httpfs )
{
    _HTTPFS_DO_REQUEST( HTTPFS_OPCODE_getattr , httpfs ,
                        httpfs_prepare_request( httpfs , &_in , HTTPFS_OPCODE_getattr ,
                                                NULL , "/" , NULL ); )
    {
        HTTPFS_CHECK_RESPONSE_STATUS;
        HTTPFS_CLEANUP;
        HTTPFS_RETURN( 0 );
    }
}

int httpfs_fuse_start( struct httpfs *httpfs ,
                       const char *url ,
                       const char *remote_chroot ,
                       char *mount_point )
{
    int argc;
    char *argv[ 4 ];
    int rv;

    const struct fuse_operations operations = {
#define _( x ) .x = httpfs_##x ,
#include "fuse_functions.def"
    };

    httpfs->url = url;
    httpfs->remote_chroot = remote_chroot;

    /* initialize curl */
    httpfs->curl = curl_easy_init();
    if ( !httpfs->curl )
    {
        return HTTPFS_CURL_ERROR;
    }

    /* check remote availability before mounting */
    rv = -check_remote_availability( httpfs );
    switch ( rv )
    {
    case 0: break;
    case ECOMM: return HTTPFS_UNREACHABLE_SERVER_ERROR;
    case ENOENT: return HTTPFS_WRONG_CHROOT_ERROR;
    default:
        {
            errno = rv;
            return HTTPFS_ERRNO_ERROR;
        }
    }

    /* fuse arguments */
    argc = 0;
    argv[ argc++ ] = "httpfs";
    argv[ argc++ ] = "-s"; /* single thread */
    if ( HTTPFS_VERBOSE ) argv[ argc++ ] = "-d"; /* debug and core dump */
    argv[ argc++ ] = mount_point;

    /* start loop */
    if ( fuse_main( argc , argv , &operations , httpfs ) )
    {
        return HTTPFS_FUSE_ERROR;
    }

    return HTTPFS_NO_ERROR;
}

void httpfs_prepare_request( struct httpfs *httpfs ,
                             struct raw_data *in ,
                             uint8_t opcode ,
                             struct raw_data *header ,
                             const char *path ,
                             struct raw_data *data )
{
    size_t offset , header_size , remote_chroot_length , path_length , data_size;

    header_size = ( header ? header->size : 0 );
    remote_chroot_length = ( httpfs->remote_chroot ? strlen( httpfs->remote_chroot ) : 0 );
    path_length = strlen( path ) + 1;
    data_size = ( data ? data->size : 0 );

    in->size = 1 + header_size + remote_chroot_length + path_length + data_size;
    in->payload = malloc( in->size );

    /* opcode */
    *in->payload = opcode;

    /* header */
    offset = 1;
    if ( header )
    {
        memcpy( in->payload + offset , header->payload , header->size );
    }

    /* path */
    offset += header_size;
    memcpy( in->payload + offset , httpfs->remote_chroot , remote_chroot_length );
    offset += remote_chroot_length;
    memcpy( in->payload + offset , path , path_length );

    /* data */
    if ( data )
    {
        offset += path_length;
        memcpy( in->payload + offset , data->payload , data->size );
    }
}
