<?php
session_start();
if(!isset($_SESSION['logged'])){echo"<script>window.location.href = 'login.php';</script>";exit();}
include"db.php";

$ttime=time()-40;
$result = mysqli_query($con, "SELECT * FROM clients where LastSeen > $ttime");
if ($result) {
    $rows = mysqli_fetch_all($result, MYSQLI_ASSOC);
    $jsonResult = json_encode($rows);
    echo $jsonResult;
    mysqli_free_result($result);
} else {
    echo "Error: " . mysqli_error($conn);
}
mysqli_close($con);



?>