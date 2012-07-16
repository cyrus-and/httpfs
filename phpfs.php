<?php

// TODO this is a stub

$post = file_get_contents('php://input');

$arr = unpack("Cop", $post);
$post = substr( $post , 1 );

file_put_contents( "/tmp/phpfs.log" , ">>> OP\n" . print_r( $arr , true ) , FILE_APPEND );

ob_start();

switch ( $arr['op'] )
{
case 1: // getattr
    $arr = unpack("a*path", $post);
    $path = $arr['path'];
    @ $s = stat( $path );
    if ( $s === FALSE )
    {
        echo pack('III', 0,0,0);
        // echo '0:0:0\n';
    }
    else
    {
        echo pack('III', $s['mode'] , $s['nlink'] , $s['size']);
        // echo '$s[mode]:$s[nlink]:$s[size]\n';
    }
    break;

case 2: // ls
    $arr = unpack("a*path", $post);
    $path = $arr['path'];
    foreach ( scandir( $path ) as $entry )
    {
        /* echo pack( 'a*' , $entry ); */
        echo $entry; printf( '%c' , 0 );
    }
    break;

case 3: // read
    $arr = unpack("Lsize/Loffset/a*path", $post);
    file_put_contents( "/tmp/phpfs.log" , ">>> REQUEST\n" . print_r( $arr , true ) , FILE_APPEND );
    $fd = fopen( $arr['path'] , 'r' );
    fseek( $fd , $arr['offset'] );
    echo fread( $fd , $arr['size'] );
    fclose( $fd );
    break;

default:
}

$output = ob_get_flush();
file_put_contents( "/tmp/phpfs.log" , ">>> RESPONSE\n" . $output , FILE_APPEND );

?>
