#include <stdio.h>
#include "phpfs.h"

void phpfs_generate_php()
{
#include "template.php.h"
    int i;
    const char **p;

    /* opcode names array */
    printf( "<?php\n\n" );
    printf( "$PHPFS_OPCODE_NAMES = array(\n" );

    for ( p = PHPFS_OPCODE_NAMES ; *p ; p++ )
    {
        printf( "    'phpfs_%s' ,\n" , *p );
    }

    printf( ");\n\n" );

    /* status codes numeric constants */
    for ( p = PHPFS_STATUS_NAMES ; *p ; p++ )
    {
        printf( "define( '%s' , %i );\n" ,
                *p , ( int )(p - PHPFS_STATUS_NAMES ) );
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
