#include <stdio.h>
#include "phpfs.h"

void phpfs_generate_php()
{
#include "template.php.h"
    int i;
    const char **p;

    printf( "<?php\n" );

    for ( p = PHPFS_OPCODE_NAMES ; *p ; p++ )
    {
        printf( "define( 'phpfs_%s' , %i );\n" ,
                *p , ( int )( p - PHPFS_OPCODE_NAMES ) );
    }

    for ( p = PHPFS_STATUS_NAMES ; *p ; p++ )
    {
        printf( "define( '%s' , %i );\n" ,
                *p , ( int )(p - PHPFS_STATUS_NAMES ) );
    }

    printf( "?>\n" );

    for ( i = 0 ; i < template_php_len ; i++ )
    {
        fputc( template_php[ i ] , stdout );
    }
}
