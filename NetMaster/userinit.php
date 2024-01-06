<?php

include 'db.php';


if(isset($_POST["id"]) and isset($_POST["name"]) ){

$id=mysqli_real_escape_string($con,$_POST["id"]);
$ip=mysqli_real_escape_string($con,$_SERVER['REMOTE_ADDR']);
$name=mysqli_real_escape_string($con,$_POST["name"]);
}
else {exit();}
$i=time();
mysqli_query($con,"DELETE FROM clients where id='$id'");
mysqli_query($con,"INSERT INTO clients VALUES ('$id','$name','$ip','-','-','-','-','-','-',$i)");




?>