#ifndef _PHPFS_DEBUG_H
#define _PHPFS_DEBUG_H

#ifdef DEBUG

#include <syslog.h>

#define LOG( string ) \
    do syslog( LOG_USER | LOG_DEBUG , "%s\n" , string ); \
    while ( 0 )

#define LOGF( format , ... ) \
    do syslog( LOG_USER | LOG_DEBUG , format "\n" , ##__VA_ARGS__ ); \
    while ( 0 )

#else /* not DEBUG */

#define LOG( string ) do ; while ( 0 )
#define LOGF( format , ... ) do ; while ( 0 )

#endif

#endif
