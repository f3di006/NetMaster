
 <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="css/style.css">




<!-- The Modal -->
<div class="container">
<div id="hostsModal" class="modal">
<input type="text" id="botidhosts" hidden>
    <span class="close" onclick="closeModal('hostsModal')">&times;</span>
    <h5 class="inline-header" >Block Hosts : </h5><h5 style="color:red;" class="inline-header" id="botnamehosts"></h5><br><br>
    
	
	<br><textarea style="width:400px;height:100px;" id="hosts"></textarea>
	<br><br><button class="btn btn-primary" onclick="HostsBlock()">Block</button> 
</div>
</div>

<script src="js/main.js"></script>

