<?php

error_reporting( 0 );

function check_file_exists( $path )
{
    if ( file_exists( $path ) === FALSE )
    {
        printf( '%c' , ENTRY_NOT_FOUND );
        exit;
    }
}

// TODO this is a stub

$post = file_get_contents('php://input');

$arr = unpack("Cop", $post);
$post = substr( $post , 1 );

file_put_contents( "/tmp/phpfs.log" , ">>> OP\n" . print_r( $arr , true ) , FILE_APPEND );

ob_start();

switch ( $arr['op'] )
{
case phpfs_getattr:
    $arr = unpack("a*path", $post);
    $path = $arr['path'];
    check_file_exists( $path );
    $s = stat( $path );
    if ( $s === FALSE )
    {
        printf( '%c' , NOT_PERMITTED );
    }
    else
    {
        printf( '%c' , OK );
        echo pack('NNN', $s['mode'] , $s['nlink'] , $s['size']);
    }
    break;

case phpfs_readdir:
    $arr = unpack("a*path", $post);
    $path = $arr['path'];
    check_file_exists( $path );
    $d = scandir( $path );
    if ( $d === FALSE )
    {
        printf( '%c' , NOT_PERMITTED );
    }
    else
    {
        printf( '%c' , OK );
        foreach ( $d as $entry )
        {
            /* echo pack( 'a*' , $entry ); */
            echo $entry; printf( '%c' , 0 );
        }
    }
    break;

case phpfs_read:
    $arr = unpack("Nsize/Noffset/a*path", $post);
    $path = $arr['path'];
    check_file_exists( $path );
    $fd = fopen( $path , 'r' );
    if ( fseek( $fd , $arr['offset'] ) == -1 )
    {
        printf( '%c' , NOT_PERMITTED );
    }
    else
    {
        printf( '%c' , OK );
        echo fread( $fd , $arr['size'] );
        fclose( $fd );
    }
    break;

default:
}

$output = ob_get_flush();
file_put_contents( "/tmp/phpfs.log" , ">>> RESPONSE\n" . $output , FILE_APPEND );

?>
