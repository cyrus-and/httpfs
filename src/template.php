<?php

error_reporting( 0 );
if ( DEBUG ) set_error_handler( 'store_error' );

/* UTILITY */

function store_error( $errno , $error )
{
    global $error_message;
    $error_message = $error;
}

function dump_ok()
{
    printf( '%c' , OK );
}

function dump_error( $error , $custom_error_message = null )
{
    global $error_message;
    printf( '%c' , $error );
    $message = $custom_error_message ? $custom_error_message : $error_message;
    if ( $message ) printf( "$message%c" , 0 );
}

function check_file_exists( $path )
{
    if ( !file_exists( $path ) )
    {
        dump_error( ENTRY_NOT_FOUND );
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
        dump_ok();
        echo pack( 'NNN' , $s[ 'mode' ] , $s[ 'nlink' ] , $s[ 'size' ] );
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_readdir( $data )
{
    $fields = unpack( 'a*path' , $data );
    check_file_exists( $fields[ 'path' ] );

    $d = scandir( $fields[ 'path' ] );
    if ( $d )
    {
        dump_ok();
        foreach ( $d as $entry )
        {
            printf( "$entry%c" , 0 );
        }
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_read( $data )
{
    $fields = unpack( 'Nsize/Noffset/a*path' , $data );
    check_file_exists( $fields[ 'path' ] );

    $f = fopen( $fields[ 'path' ] , 'r' );
    if ( $f )
    {
        dump_ok();
        fseek( $f , $fields[ 'offset' ] );
        echo fread( $f , $fields[ 'size' ] );
        fclose( $f );
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_unlink( $data )
{
    $fields = unpack( 'a*path' , $data );
    check_file_exists( $fields[ 'path' ] );

    $u = unlink( $fields[ 'path' ] );
    if ( $u )
    {
        dump_ok();
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_rmdir( $data )
{
    $fields = unpack( 'a*path' , $data );
    check_file_exists( $fields[ 'path' ] );

    $u = rmdir( $fields[ 'path' ] );
    if ( $u )
    {
        dump_ok();
    }
    else
    {
        dump_error( NOT_PERMITTED );
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
