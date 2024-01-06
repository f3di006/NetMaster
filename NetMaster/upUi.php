
 <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="css/style.css">




<!-- The Modal -->
<div class="container">
<div id="myModal" class="modal">
<input type="text" id="botid" hidden>
    <span class="close" onclick="closeModal('myModal')">&times;</span>
    <h5 class="inline-header" >Upload File : </h5><h5 style="color:red;" class="inline-header" id="botname"></h5><br><br>
    <form id="uploadForm">
        <label for="file">Select File:</label>
        <input type="file" id="file" name="file">
        <br>
        
        <br>
        
    </form>
	
	<input id="chkbox" type='checkbox' onchange='checkbox();'>Upload From URL<br><input class="form-control" hidden type="text" placeholder="https://example.com/test.zip" id="link"/>
	<input id="run" type='checkbox'> Execute<br><br><button class="btn btn-primary" onclick="uploadFile()";id="linkup" >Submit</button> 
</div>
</div>

<script src="js/main.js"></script>

