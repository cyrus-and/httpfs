#ifndef _PHPFS_PHPFS_H
#define _PHPFS_PHPFS_H

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
#define PHPFS_DO_SIMPLE_REQUEST( op ) \
    _PHPFS_DO_REQUEST( op , \
    phpfs_prepare_request( &_in , op , NULL , path , NULL ); )

#define PHPFS_DO_REQUEST_WITH_HEADER( op ) \
    _PHPFS_DO_REQUEST( op , \
    _header_data.payload = ( char * )&header; \
    _header_data.size = sizeof( header ); \
    phpfs_prepare_request( &_in , op , &_header_data , path , NULL ); )

#define PHPFS_DO_REQUEST_WITH_DATA( op ) \
    _PHPFS_DO_REQUEST( op , \
    phpfs_prepare_request( &_in , op , NULL , path , &raw_data ); )

#define PHPFS_DO_REQUEST_WITH_HEADER_AND_DATA( op ) \
    _PHPFS_DO_REQUEST( op , \
    _header_data.payload = ( char * )&header; \
    _header_data.size = sizeof( header ); \
    phpfs_prepare_request( &_in , op , &_header_data , path , &raw_data ); )

/* common */
#define _PHPFS_DO_REQUEST( op , prepare_header ) \
    LOGF( "SEND REQUEST: %s (%i)" , \
          PHPFS_OPCODE_NAMES[ op ] , op ); \
    struct raw_data _in = { 0 } , _out = { 0 } , _header_data = { 0 }; \
    struct raw_data response = { 0 }; \
    ( void )response; \
    ( void )_header_data; \
    prepare_header \
    if ( CURLE_OK != phpfs_do_post( &_in , &_out ) ) { \
        LOG( "SEND REQUEST: failed" ); \
        PHPFS_CLEANUP; \
        return -ECOMM; \
    } else

/* check the response status and return if an error is occurred */
#define PHPFS_CHECK_RESPONSE_STATUS \
    LOGF( "RESPONSE: %s (%i) %s" , \
          PHPFS_STATUS_NAMES[ ( int )*_out.payload ] , *_out.payload , \
          _out.size > 1 ? _out.payload + 1 : "" ); \
    response.payload = _out.payload + 1; \
    response.size = _out.size - 1; \
    switch ( *_out.payload ) { \
    _PHPFS_CHECK_HANDLE_ERROR( ENTRY_NOT_FOUND , ENOENT ) \
    _PHPFS_CHECK_HANDLE_ERROR( CANNOT_ACCESS , EACCES ) \
    _PHPFS_CHECK_HANDLE_ERROR( NOT_PERMITTED , EPERM ) \
    case 0: break; \
    default: PHPFS_CLEANUP; return -EBADMSG; \
    }

#define _PHPFS_CHECK_HANDLE_ERROR( status , errno ) \
    case PHPFS_STATUS_##status: PHPFS_CLEANUP; return -errno;

/* to be called before return in FUSE API functions */
#define PHPFS_CLEANUP \
    free( _in.payload ); \
    free( _out.payload )

/* global context */
extern struct phpfs
{
    const char *php_url;
    const char *remote_chroot;
    CURL *curl;
}
phpfs;

/* operation codes */
#define _( x ) PHPFS_OPCODE_##x ,
enum { PHPFS_OPCODE_NONE ,
#include "fuse_functions.def"
};
extern const char *PHPFS_OPCODE_NAMES[];

/* response status */
#define _( x ) PHPFS_STATUS_##x ,
enum {
#include "statuses.def"
};
extern const char *PHPFS_STATUS_NAMES[];

int phpfs_fuse_start( struct phpfs *phpfs ,
                      char *mount_point );

void phpfs_prepare_request( struct raw_data *in ,
                            uint8_t opcode ,
                            struct raw_data *header ,
                            const char *path ,
                            struct raw_data *data );

#endif
