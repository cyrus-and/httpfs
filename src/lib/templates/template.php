<?php

error_reporting( 0 );
if ( VERBOSE ) set_error_handler( 'store_error' );

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
    if ( $message ) echo $message;
}

/* FUSE API */

function httpfs_getattr( $data )
{
    $path = explode( "\x00" , $data , 2 )[ 0 ];

    $s = lstat( $path );
    if ( $s )
    {
        dump_ok();
        echo pack( 'NNNNNNNNNNNNN' ,
                   $s[ 'dev' ] ,
                   $s[ 'ino' ] ,
                   $s[ 'mode' ] ,
                   $s[ 'nlink' ] ,
                   $s[ 'uid' ] ,
                   $s[ 'gid' ] ,
                   $s[ 'rdev' ] ,
                   $s[ 'size' ] ,
                   $s[ 'atime' ] ,
                   $s[ 'mtime' ] ,
                   $s[ 'ctime' ] ,
                   $s[ 'blksize' ] ,
                   $s[ 'blocks' ] );
    }
    else
    {
        dump_error( ENTRY_NOT_FOUND );
    }
}

function httpfs_readdir( $data )
{
    $path = explode( "\x00" , $data , 2 )[ 0 ];

    $d = scandir( $path );
    if ( $d )
    {
        dump_ok();
        foreach ( $d as $entry )
        {
            echo "$entry\x00";
        }
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_read( $data )
{
    $fields = unpack( 'Nsize/Noffset' , $data );
    $path = explode( "\x00" , substr( $data , 8 ) , 2 )[ 0 ];

    $f = fopen( $path , 'r' );
    if ( $f )
    {
        dump_ok();
        fseek( $f , $fields[ 'offset' ] );
        echo fread( $f , $fields[ 'size' ] );
        fclose( $f );
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_write( $data )
{
    $fields = unpack( 'Nsize/Noffset' , $data );
    list( $path , $write_data ) = explode( "\x00" , substr( $data , 8 ) , 2 );

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
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_truncate( $data )
{
    $fields = unpack( 'Noffset' , $data );
    $path = explode( "\x00" , substr( $data , 4 ) , 2 )[ 0 ];

    $f = fopen( $path , 'r+' );
    if ( $f )
    {
        dump_ok();
        ftruncate( $f , $fields[ 'offset' ] );
        fclose( $f );
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_create( $data )
{
    $fields = unpack( 'Nmode' , $data );
    $path = explode( "\x00" , substr( $data , 4 ) , 2 )[ 0 ];

    $f = fopen( $path , 'w' );
    if ( $f )
    {
        dump_ok();
        fclose( $f );
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_unlink( $data )
{
    $path = explode( "\x00" , $data , 2 )[ 0 ];

    $u = unlink( $path );
    if ( $u )
    {
        dump_ok();
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_mkdir( $data )
{
    $fields = unpack( 'Nmode' , $data );
    $path = explode( "\x00" , substr( $data , 4 ) , 2 )[ 0 ];

    $m = mkdir( $path , $fields[ 'mode' ] );
    if ( $m )
    {
        dump_ok();
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_rmdir( $data )
{
    $path = explode( "\x00" , $data , 2 )[ 0 ];

    $u = rmdir( $path );
    if ( $u )
    {
        dump_ok();
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_rename( $data )
{
    list( $path , $newpath ) = explode( "\x00" , $data , 2 );
    $r = rename( $path , $newpath );
    if ( $r )
    {
        dump_ok();
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_link( $data )
{
    list( $path , $newpath ) = explode( "\x00" , $data , 2 );
    $r = link( $path , $newpath );
    if ( $r )
    {
        dump_ok();
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_readlink( $data )
{
    $path = explode( "\x00" , $data , 2 )[ 0 ];

    $r = readlink( $path );
    if ( $r )
    {
        dump_ok();
        echo $r;
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_symlink( $data )
{
    list( $path , $newpath ) = explode( "\x00" , $data , 2 );

    $s = symlink( $path , $newpath );
    if ( $s )
    {
        dump_ok();
    }
    else
    {
        dump_error( CANNOT_ACCESS );
    }
}

function httpfs_chmod( $data )
{
    $fields = unpack( 'Nmode' , $data );
    $path = explode( "\x00" , substr( $data , 4 ) , 2 )[ 0 ];

    $c = chmod( $path , $fields[ 'mode' ] );
    if ( $c )
    {
        dump_ok();
    }
    else
    {
        dump_error( NOT_PERMITTED );
    }
}

function httpfs_chown( $data )
{
    $fields = unpack( 'Nuid/Ngid' , $data );
    $path = explode( "\x00" , substr( $data , 8 ) , 2 )[ 0 ];

    if ( $fields[ 'uid' ] != 0xffffffff )
    {
        $u = chown( $path , $fields[ 'uid' ] );
        $g = TRUE;
    }

    if ( $fields[ 'gid' ] != 0xffffffff )
    {
        $g = chgrp( $path , $fields[ 'gid' ] );
        $u = TRUE;
    }

    if ( $u && $g )
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
$function_name = $HTTPFS_OPCODE_NAMES[ $opcode ];
$data = substr( $post , 1 );
call_user_func( $function_name , $data );

?>
