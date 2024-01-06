
 <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
 <link rel="icon" type="image/x-icon" href="img/icon.png">
 <title>NetMaster</title>
 <style>
        body {
            
            background-image: url('img/bg.jpg');
            background-repeat: no-repeat;
            background-size: cover;
			background-attachment: fixed;
			

            
        }
		.navbar {
            margin-bottom: 40px;
        }
		.titles{
		color:gray; font-family: Arial, sans-serif;}
		td {
            font-size: smaller;
        }
		
		
</style>
<html>

<nav class="navbar navbar-expand-lg navbar-dark bg-dark">
        <a class="navbar-brand" href="about.html">NetMaster</a>
        <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
            <ul class="navbar-nav ml-auto">
                <li class="nav-item">
				<?php if (isset($_SESSION["logged"])){echo '<a class="nav-link" href="#" onclick="logout()">Logout</a>';}
				else {echo '<a class="nav-link" href="login.php">Login</a>';}
				?>
				
                    
					
                </li>
				
				<a class="nav-link" href="index.php">Home</a><a class="nav-link" href="about.php">About</a>
            </ul>
        </div>
    </nav>



