<?php

error_reporting( 0 );

/* UTILITY */

function dump_status( $status )
{
    printf( '%c' , $status );
}

function check_file_exists( $path )
{
    if ( !file_exists( $path ) )
    {
        dump_status( ENTRY_NOT_FOUND );
        exit;
    }
}

/* FUSE API */

function phpfs_getattr( $data )
{
    $fields = unpack( 'a*path' , $data );
    check_file_exists( $fields[ 'path' ] );

    $s = stat( $fields[ 'path' ] );
    if ( $s )
    {
        dump_status( OK );
        echo pack( 'NNN' , $s[ 'mode' ] , $s[ 'nlink' ] , $s[ 'size' ] );
    }
    else
    {
        dump_status( NOT_PERMITTED );
    }
}

function phpfs_readdir( $data )
{
    $fields = unpack( 'a*path' , $data );
    check_file_exists( $fields[ 'path' ] );

    $d = scandir( $fields[ 'path' ] );
    if ( $d )
    {
        dump_status( OK );
        foreach ( $d as $entry )
        {
            printf( "$entry%c" , 0 );
        }
    }
    else
    {
        dump_status( NOT_PERMITTED );
    }
}

function phpfs_read( $data )
{
    $fields = unpack( 'Nsize/Noffset/a*path' , $data );
    check_file_exists( $fields[ 'path' ] );

    $f = fopen( $fields[ 'path' ] , 'r' );
    if ( $f )
    {
        dump_status( OK );
        fseek( $f , $fields[ 'offset' ] );
        echo fread( $f , $fields[ 'size' ] );
        fclose( $f );
    }
    else
    {
        dump_status( NOT_PERMITTED );
    }
}

/*...*/

/* MAIN */

$post = file_get_contents( 'php://input' );
$opcode = ord( $post );
$function_name = $PHPFS_OPCODE_NAMES[ $opcode ];
$data = substr( $post , 1 );
call_user_func( $function_name , $data );

?>
