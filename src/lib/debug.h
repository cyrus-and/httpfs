#ifndef _HTTPFS_DEBUG_H
#define _HTTPFS_DEBUG_H

#include <stdio.h>
#include "net.h"

#define DEBUG_PREFIX ">>> "

#define LOG( string ) \
    do if ( HTTPFS_VERBOSE ) fprintf( stderr , DEBUG_PREFIX "%s\n" , string ); \
    while ( 0 )

#define LOGF( format , ... ) \
    do if ( HTTPFS_VERBOSE ) fprintf( stderr , DEBUG_PREFIX format "\n" , ##__VA_ARGS__ ); \
    while ( 0 )

#define DUMP_RAW_DATA( label , raw_data ) \
    do if ( HTTPFS_VERBOSE ) { \
            fprintf( stderr , DEBUG_PREFIX "%s%zu byte: " , \
                     label , raw_data.size ); \
            httpfs_dump_raw_data( &raw_data ); } \
    while ( 0 )

extern int HTTPFS_VERBOSE;

void httpfs_dump_raw_data( struct raw_data *raw_data );

#endif
