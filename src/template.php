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

/* FUSE API */

function phpfs_getattr( $data )
{
    $fields = unpack( 'a*path' , $data );

    $s = lstat( $fields[ 'path' ] );
    if ( $s )
    {
        dump_ok();
        echo pack( 'NNN' , $s[ 'mode' ] , $s[ 'nlink' ] , $s[ 'size' ] );
    }
    else
    {
        dump_error( ENTRY_NOT_FOUND );
    }
}

function phpfs_readdir( $data )
{
    $fields = unpack( 'a*path' , $data );

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

function phpfs_write( $data )
{
    $fields = unpack( 'Nsize/Noffset' , $data );
    list( $path , $write_data ) = explode ( "\x00" , substr( $data , 8 ) , 2 );

    $f = fopen( $path , 'a' );
    if ( $f )
    {
        dump_ok();
        fseek( $f , $fields[ 'offset' ] );
        $write_size = fwrite( $f , $write_data , $fields[ 'size' ] );
        fclose( $f );
        echo pack( 'N' , $write_size );
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_truncate( $data )
{
    $fields = unpack( 'Noffset/a*path' , $data );

    $f = fopen( $fields[ 'path' ] , 'r+' );
    if ( $f )
    {
        dump_ok();
        ftruncate( $f , $fields[ 'offset' ] );
        fclose( $f );
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_create( $data )
{
    $fields = unpack( 'Nmode/a*path' , $data );

    $f = fopen( $fields[ 'path' ] , 'w' );
    if ( $f )
    {
        dump_ok();
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

function phpfs_mkdir( $data )
{
    $fields = unpack( 'Nmode/a*path' , $data );

    $m = mkdir( $fields[ 'path' ] , $fields[ 'mode' ] );
    if ( $m )
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

function phpfs_rename( $data )
{
    list( $path , $newpath ) = explode ( "\x00" , $data, 2 );
    $r = rename( $path , $newpath );
    if ( $r )
    {
        dump_ok();
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_link( $data )
{
    list( $path , $newpath ) = explode ( "\x00" , $data, 2 );
    $r = link( $path , $newpath );
    if ( $r )
    {
        dump_ok();
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_readlink( $data )
{
    $fields = unpack( 'a*path' , $data );

    $r = readlink( $fields[ 'path' ] );
    if ( $r )
    {
        dump_ok();
        echo $r;
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function phpfs_symlink( $data )
{
    list( $path , $newpath ) = explode ( "\x00" , $data , 2 );

    $s = symlink( $path , $newpath );
    if ( $s )
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
