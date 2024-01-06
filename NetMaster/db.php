<?php

$host="localhost";
$username="root";
$password="";
$dbname="NetMaster";
$con = mysqli_connect($host,$username,$password,$dbname);
if (!$con) {
  echo "Error connecting to MySQL: ";
  exit();
}

?>