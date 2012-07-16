#ifndef _PHPFS_PHPFS_H
#define _PHPFS_PHPFS_H

/* common includes for API implementation */
#include <fuse.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "debug.h"
#include "net.h"

/* access to the global context */
#define PHPFS( field ) \
    ( ( struct phpfs * )fuse_get_context()->private_data )->field

/* global context */
struct phpfs
{
    const char *php_url;
    CURL *curl;
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

int phpfs_fuse_start( struct phpfs *phpfs , char *mounting_point );

#endif
