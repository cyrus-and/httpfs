#ifndef _HTTPFS_NET_H
#define _HTTPFS_NET_H

#include <curl/curl.h>

struct httpfs;

struct raw_data
{
    char *payload;
    size_t size;
};

CURLcode httpfs_do_post( struct httpfs *httpfs ,
                         const struct raw_data *in ,
                         struct raw_data *out );

#endif
