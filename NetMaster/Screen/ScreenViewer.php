<?php
session_start();
if(!isset($_SESSION['logged'])){header("Location:../login.php");exit();}
session_write_close();
include"srv.php";
set_time_limit(0);
header("HTTP/1.1 200 OK");
header("Content-Type: multipart/x-mixed-replace; boundary=--boundary");
session_write_close();
$buffer = "";
while(1){

$n=@socket_recv ( $clientsock , $buffer , 1024 ,0 );
if( $n== false || $n==0){break;}
echo $buffer ;
flush();

}
socket_close($clientsock);
exit();
?>