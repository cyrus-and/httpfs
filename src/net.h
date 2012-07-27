#ifndef _HTTPFS_NET_H
#define _HTTPFS_NET_H

#include <curl/curl.h>

struct raw_data
{
    char *payload;
    size_t size;
};

CURLcode httpfs_do_post( const struct raw_data *in ,
                         struct raw_data *out );

#endif
