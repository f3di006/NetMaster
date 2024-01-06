<?php


include"db.php";
header("Content-type: text/plain");
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
	if(!isset($_GET["id"])){exit();}
	$id=mysqli_real_escape_string($con,$_GET["id"]);
	$result=mysqli_query($con,"SELECT hosts FROM host_filter where id='$id'");
	if($result){
		 $row = mysqli_fetch_assoc($result);

    if ($row) {
        echo str_replace("\r\n", "\n", $row['hosts']);
    }
		
		
	}
	
	
} elseif ($_SERVER['REQUEST_METHOD'] === 'POST') {
	if(!isset($_GET["id"])){exit();}
	session_start();
	if(!isset($_SESSION['logged'])){echo"<script>window.location.href = 'login.php';</script>";exit();}
	$id=mysqli_real_escape_string($con,$_GET["id"]);
	$hosts=mysqli_real_escape_string($con,$_POST["hosts"]);
	$query = "INSERT INTO host_filter (id, hosts) VALUES ('$id', '$hosts') ON DUPLICATE KEY UPDATE hosts='$hosts'";
    if(mysqli_query($con,$query)){
		
		mysqli_query($con,"INSERT INTO commands VALUES ('$id','7|null')");
		
	}
} else {
    exit();
}



?>