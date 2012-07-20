#ifndef _PHPFS_PHPFS_H
#define _PHPFS_PHPFS_H

/*

  Request format:

  +--------+--------+------+
  | opcode | fields | path |
  +--------+--------+------+

  - opcode: 1 byte that identifies the requested operation (see "operation code"
            enum)

  - fields: arbitrarily long (even 0) packed data in big endian byte order

  - path: absolute Unix-like path

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

/* access to the global context */
#define PHPFS( field ) \
    ( ( struct phpfs * )fuse_get_context()->private_data )->field

/* convenience macro used implement a FUSE API function; 'response' is the data
   received and it's available to the implementation; a structure named header
   must be filled with values to be passed to the PHP script before calling this
   macro; this macro expects a following block where the logic should be
   implemented */
#define PHPFS_DO_REQUEST( op ) \
    _PHPFS_DO_REQUEST( op , \
    phpfs_allocate_request( &_in , op , sizeof( header ) , path ); \
    memcpy( _in.payload + 1 , &header , sizeof( header ) ); )

/* same as above but without header */
#define PHPFS_DO_SIMPLE_REQUEST( op ) \
    _PHPFS_DO_REQUEST( op , \
    phpfs_allocate_request( &_in , op , 0 , path ); )

/* common */
#define _PHPFS_DO_REQUEST( op , prepare_header ) \
    LOGF( "SEND REQUEST: %s (%i)" , \
          PHPFS_OPCODE_NAMES[ op ] , op ); \
    struct raw_data _in = { 0 } , _out = { 0 } , response = { 0 }; \
    prepare_header \
    if ( CURLE_OK != phpfs_do_post( &_in , &_out ) ) { \
        LOG( "SEND REQUEST: failed" ); \
        PHPFS_CLEANUP; \
        return -ECOMM; \
    } else

/* check the response status and return if an error is occurred */
#define PHPFS_CHECK_RESPONSE_STATUS \
    LOGF( "RESPONSE: %s (%i)" , \
          PHPFS_STATUS_NAMES[ ( int )*_out.payload ] , *_out.payload ); \
    response.payload = _out.payload + 1; \
    response.size = _out.size - 1; \
    switch ( *_out.payload ) { \
    case PHPFS_STATUS_ENTRY_NOT_FOUND: PHPFS_CLEANUP; return -ENOENT; \
    case PHPFS_STATUS_NOT_PERMITTED: PHPFS_CLEANUP; return -EPERM; \
    }

/* to be called before return in FUSE API functions */
#define PHPFS_CLEANUP \
    free( _in.payload ); \
    free( _out.payload )

/* global context */
struct phpfs
{
    const char *php_url;
    CURL *curl;
};

/* operation codes */
#define _PHPFS_FUSE_FUNCTIONS \
    _( getattr ) \
    _( readdir ) \
    _( read )
    /*...*/

#define _( x ) PHPFS_OPCODE_##x ,
enum { PHPFS_OPCODE_NONE , _PHPFS_FUSE_FUNCTIONS };
#undef _

extern const char *PHPFS_OPCODE_NAMES[];

/* response status */
#define _PHPFS_STATUSES \
    _( OK ) \
    _( ENTRY_NOT_FOUND ) \
    _( NOT_PERMITTED )
    /*...*/

#define _( x ) PHPFS_STATUS_##x ,
enum { _PHPFS_STATUSES };
#undef _

extern const char *PHPFS_STATUS_NAMES[];

int phpfs_fuse_start( struct phpfs *phpfs ,
                      char *mounting_point );

void phpfs_allocate_request( struct raw_data *in ,
                             uint8_t op ,
                             size_t header_length ,
                             const char *path );

#endif
