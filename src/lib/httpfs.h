#ifndef _HTTPFS_HTTPFS_H
#define _HTTPFS_HTTPFS_H

/*

  Request format:

  +--------+--------+------+------+
  | opcode | fields | path | data |
  +--------+--------+------+------+

  - opcode: 1 byte that identifies the requested operation (see "operation code"
            enum)

  - fields: arbitrarily long (even 0) packed data in big endian byte order

  - path: absolute Unix-like path ('\0' terminated string)

  - data: arbitrarily long (even 0) raw data

  Response format:

  +--------+------+
  | status | data |
  +--------+------+

  - status: 1 byte that describes the result of the operation (see "response
            status" enum)

  - data: arbitrarily long (even 0) raw data

  NOTE: these messages are carried over HTTP so there's no need to include an
        additional length field

*/

/* common includes for API implementation */
#include <fuse.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include "debug.h"
#include "net.h"

/* convenience macros used to implement the FUSE API functions; 'response' is
   the data received and it's available to the implementation; a structure named
   'header' may be filled with values to be passed to the PHP script before
   calling this macros; a 'struct raw_data raw_data' may contains the additional
   data to pass to the PHP script; this macros expect a following block where
   the logic should be implemented */
#define HTTPFS_DO_SIMPLE_REQUEST( op ) \
    _HTTPFS_DO_REQUEST( op , fuse_get_context()->private_data , \
    httpfs_prepare_request( fuse_get_context()->private_data , \
                            &_in , op , NULL , path , NULL ); )

#define HTTPFS_DO_REQUEST_WITH_HEADER( op ) \
    _HTTPFS_DO_REQUEST( op , fuse_get_context()->private_data , \
    _header_data.payload = ( char * )&header; \
    _header_data.size = sizeof( header ); \
    httpfs_prepare_request( fuse_get_context()->private_data , \
                            &_in , op , &_header_data , path , NULL ); )

#define HTTPFS_DO_REQUEST_WITH_DATA( op ) \
    _HTTPFS_DO_REQUEST( op , fuse_get_context()->private_data , \
    httpfs_prepare_request( fuse_get_context()->private_data , \
                            &_in , op , NULL , path , &raw_data ); )

#define HTTPFS_DO_REQUEST_WITH_HEADER_AND_DATA( op ) \
    _HTTPFS_DO_REQUEST( op , fuse_get_context()->private_data , \
    _header_data.payload = ( char * )&header; \
    _header_data.size = sizeof( header ); \
    httpfs_prepare_request( fuse_get_context()->private_data , \
                            &_in , op , &_header_data , path , &raw_data ); )

/* common */
#define _HTTPFS_DO_REQUEST( op , httpfs , prepare_header ) \
    LOGF( "REQUEST: %s (%i)" , \
          HTTPFS_OPCODE_NAMES[ op ] , op ); \
    struct raw_data _in = { 0 } , _out = { 0 } , _header_data = { 0 }; \
    struct raw_data response = { 0 }; \
    ( void )response; \
    ( void )_header_data; \
    prepare_header \
    DUMP_RAW_DATA( "SENDING " , _in ); \
    if ( CURLE_OK != httpfs_do_post( httpfs , &_in , &_out ) ) { \
        LOG( "REQUEST: failed" ); \
        HTTPFS_CLEANUP; \
        return -ECOMM; \
    } else

/* return errno from FUSE callback functions */
#define HTTPFS_RETURN( errno ) \
    LOGF( "RETURN: %s (%i)" , strerror( errno ) , errno );  \
    return -errno;

/* check the response status and return if an error is occurred */
#define HTTPFS_CHECK_RESPONSE_STATUS \
    response.payload = _out.payload + 1; \
    response.size = _out.size - 1; \
    DUMP_RAW_DATA( "RECEIVED " , _out ); \
    switch ( *_out.payload ) { \
    _HTTPFS_CHECK_HANDLE_ERROR( ENTRY_NOT_FOUND , ENOENT ) \
    _HTTPFS_CHECK_HANDLE_ERROR( CANNOT_ACCESS , EACCES ) \
    _HTTPFS_CHECK_HANDLE_ERROR( NOT_PERMITTED , EPERM ) \
    case HTTPFS_STATUS_OK: _HTTPFS_DUMP_STATUS; break; \
    default: HTTPFS_CLEANUP; HTTPFS_RETURN( EBADMSG ); \
    }

#define _HTTPFS_CHECK_HANDLE_ERROR( status , errno ) \
    case HTTPFS_STATUS_##status: \
    _HTTPFS_DUMP_STATUS; HTTPFS_CLEANUP; \
    HTTPFS_RETURN( errno )

#define _HTTPFS_DUMP_STATUS \
    LOGF( "RESPONSE: %s (%i)" , \
          HTTPFS_STATUS_NAMES[ ( int )*_out.payload ] , *_out.payload ); \

/* to be called before return in FUSE API functions */
#define HTTPFS_CLEANUP \
    free( _in.payload ); \
    free( _out.payload )

/* initialization errors */
enum
{
    HTTPFS_NO_ERROR ,
    HTTPFS_FUSE_ERROR ,
    HTTPFS_CURL_ERROR ,
    HTTPFS_UNREACHABLE_SERVER_ERROR ,
    HTTPFS_WRONG_CHROOT_ERROR ,
    HTTPFS_ERRNO_ERROR
};

/* context */
struct httpfs
{
    const char *url;
    const char *remote_chroot;
    CURL *curl;
};

/* operation codes */
#define _( x ) HTTPFS_OPCODE_##x ,
enum { HTTPFS_OPCODE_NONE ,
#include "fuse_functions.def"
};
extern const char *HTTPFS_OPCODE_NAMES[];

/* response status */
#define _( x ) HTTPFS_STATUS_##x ,
enum {
#include "statuses.def"
};
extern const char *HTTPFS_STATUS_NAMES[];

int httpfs_fuse_start( struct httpfs *httpfs ,
                       const char *url ,
                       const char *remote_chroot ,
                       char *mount_point );

void httpfs_prepare_request( struct httpfs *httpfs ,
                             struct raw_data *in ,
                             uint8_t opcode ,
                             struct raw_data *header ,
                             const char *path ,
                             struct raw_data *data );

#endif
