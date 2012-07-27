#include <stdio.h>
#include "httpfs.h"

void httpfs_generate_php()
{
#include "template.php.h"
    int i;
    const char **p;

    /* opcode names array */
    printf( "<?php\n\n" );
    printf( "$HTTPFS_OPCODE_NAMES = array(\n" );

    for ( p = HTTPFS_OPCODE_NAMES ; *p ; p++ )
    {
        printf( "    'httpfs_%s' ,\n" , *p );
    }

    printf( ");\n\n" );

    /* status codes numeric constants */
    for ( p = HTTPFS_STATUS_NAMES ; *p ; p++ )
    {
        printf( "define( '%s' , %i );\n" ,
                *p , ( int )(p - HTTPFS_STATUS_NAMES ) );
    }

#ifndef NDEBUG
    /* debug mode */
    printf( "\ndefine( 'DEBUG' , TRUE );\n\n" );
#endif

    printf( "\n?>" );

    /* dump template */
    for ( i = 0 ; i < template_php_len ; i++ )
    {
        fputc( template_php[ i ] , stdout );
    }
}
