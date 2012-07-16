#ifndef _PHPFS_NET_H
#define _PHPFS_NET_H

#include <curl/curl.h>

struct raw_data
{
    char *payload;
    size_t size;
};

CURLcode phpfs_do_post( CURL *curl ,
                        const char *url ,
                        const struct raw_data *in ,
                        struct raw_data *out );

#endif
