#ifndef _PHPFS_PHPFS_H
#define _PHPFS_PHPFS_H

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
    LOGF( "SEND REQUEST: opcode %i (%s)" , op , __func__ ); \
    struct raw_data _in = { 0 } , _out = { 0 } , response = { 0 }; \
    phpfs_allocate_request( &_in , op , sizeof( header ) , path ); \
    memcpy( _in.payload + 1 , &header , sizeof( header ) ); \
    if ( CURLE_OK != phpfs_do_post( &_in , &_out ) ) { \
        LOG( "SEND REQUEST: failed" ); \
        PHPFS_CLEANUP; \
        return -ECOMM; \
    } else

/* same as above but without header */
#define PHPFS_DO_SIMPLE_REQUEST( op ) \
    LOGF( "SEND REQUEST: opcode %i (%s)" , op , __func__ ); \
    struct raw_data _in = { 0 } , _out = { 0 } , response = { 0 }; \
    phpfs_allocate_request( &_in , op , 0 , path ); \
    if ( CURLE_OK != phpfs_do_post( &_in , &_out ) ) { \
        LOG( "SEND REQUEST: failed" ); \
        PHPFS_CLEANUP; \
        return -ECOMM; \
    } else

/* check the response status and return if an error is occurred */
#define PHPFS_CHECK_RESPONSE_STATUS \
    LOGF( "RESPONSE: status %i" , *_out.payload ); \
    response.payload = _out.payload + 1; \
    response.size = _out.size - 1; \
    switch ( *_out.payload ) { \
    case ENTRY_NOT_FOUND: PHPFS_CLEANUP; return -ENOENT; \
    case NOT_PERMITTED: PHPFS_CLEANUP; return -EPERM; \
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

/* response status */
enum
{
    OK ,
    ENTRY_NOT_FOUND ,
    NOT_PERMITTED ,
    /*...*/
};

/* operation code */
enum
{
    NONE ,
    GETATTR ,
    READDIR ,
    READ ,
    /*...*/
};

int phpfs_fuse_start( struct phpfs *phpfs ,
                      char *mounting_point );

void phpfs_allocate_request( struct raw_data *in ,
                             uint8_t op ,
                             size_t header_length ,
                             const char *path );

#endif
