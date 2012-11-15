#include <stdio.h>
#include "httpfs.h"
#include "generators.h"

const struct httpfs_generator HTTPFS_GENERATORS[] = {
#define _( x ) { #x , httpfs_generate_##x } ,
#include "generators.def"
    { NULL , NULL }
};

int httpfs_generate( const char *name )
{
    const struct httpfs_generator *generator;

    for ( generator = HTTPFS_GENERATORS ; generator->name ; generator++ )
    {
        if ( strcmp( generator->name , name ) == 0 )
        {
            generator->function();
            return 1;
        }
    }

    return 0;
}

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
                *p , ( int )( p - HTTPFS_STATUS_NAMES ) );
    }

    /* verbose mode */
    if ( HTTPFS_VERBOSE ) printf( "\ndefine( 'VERBOSE' , TRUE );\n\n" );

    printf( "\n?>" );

    /* dump template */
    for ( i = 0 ; i < template_php_len ; i++ )
    {
        fputc( template_php[ i ] , stdout );
    }
}
