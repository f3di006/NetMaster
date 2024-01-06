<?php
$password="admin";
session_start();


if(isset($_POST["password"])){
	
	if($_POST["password"]==$password){
		$_SESSION["logged"]="1";
		header("Location: ./index.php");exit();
		
	}
	else {echo"<script>alert('wrong password!');window.location.href = 'login.php';</script>";}
	exit();
	
}


if (isset($_SESSION["logged"])) {
    unset($_SESSION["logged"]);
}

include"header.php";
?>

<div class="container mt-5">
    <div class="row justify-content-center">
        <div class="col-md-6">
            <div class="card">
                <div class="card-body">
                    <h5 class="card-title">Login</h5>
                    <form method="post">
                        <div class="form-group">
                            <label for="password">Password</label>
                            <input type="password" class="form-control" name="password" required>
                        </div>
                        <button type="submit" class="btn btn-primary">Login</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<?php




?>