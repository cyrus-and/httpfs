#include <ctype.h>
#include "net.h"

void httpfs_dump_raw_data( struct raw_data *raw_data )
{
    int i;

    fprintf( stderr , "'" );

    for ( i = 0 ; i < raw_data->size ; i++ )
    {
        unsigned char c;

        c = raw_data->payload[ i ];
        fprintf( stderr , isprint( c ) ? "%c" : "\\x%02x" , c );
    }

    fprintf( stderr , "'\n" );
}
