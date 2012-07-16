#include <stdlib.h>
#include <string.h>
#include "net.h"

static size_t retrieve_chunk( char *ptr ,
                              size_t size ,
                              size_t nmemb ,
                              void *userdata )
{
    struct raw_data *data;
    size_t chunk_size;

    chunk_size = size * nmemb;
    data = ( struct raw_data * )userdata;
    data->payload = realloc( data->payload , data->size + chunk_size );
    memcpy( data->payload + data->size , ptr , chunk_size );
    data->size += chunk_size;

    return chunk_size;
}

CURLcode phpfs_do_post( CURL *curl ,
                        const char *url ,
                        const struct raw_data *in ,
                        struct raw_data *out )
{
    out->payload = malloc( 1 );
    out->size = 0;

    curl_easy_setopt( curl , CURLOPT_URL , url );
    curl_easy_setopt( curl , CURLOPT_POSTFIELDS , in->payload );
    curl_easy_setopt( curl , CURLOPT_POSTFIELDSIZE , in->size );
    curl_easy_setopt( curl , CURLOPT_WRITEFUNCTION , retrieve_chunk );
    curl_easy_setopt( curl , CURLOPT_WRITEDATA , out );

    return curl_easy_perform( curl );
}
