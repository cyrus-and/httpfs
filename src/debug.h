#ifndef _PHPFS_DEBUG_H
#define _PHPFS_DEBUG_H

#ifndef NDEBUG

#include <stdio.h>

#define DEBUG_PREFIX ">>> "

#define LOG( string ) \
    do fprintf( stderr , DEBUG_PREFIX "%s\n" , string ); \
    while ( 0 )

#define LOGF( format , ... ) \
    do fprintf( stderr , DEBUG_PREFIX format "\n" , ##__VA_ARGS__ ); \
    while ( 0 )

#else /* not DEBUG */

#define LOG( string ) do ; while ( 0 )
#define LOGF( format , ... ) do ; while ( 0 )

#endif

#endif
