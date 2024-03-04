function load() {
	filelist();
	getFile("eviltwin.txt", function (res) {
		document.getElementById("eviltwinpath").value = res;
	});
	getFile("log.json", function (res) {
		var array = JSON.parse(res)
		var logDiv = document.getElementById("log");
		logDiv.innerHTML = "<th>SSID</th> <th>Password</th>";
		for (let index = 0; index < array.length; index++) {
			logDiv.innerHTML += "<tr><td>" + array[index].ssid + "</td><td>" + array[index].pass + "</td></tr>";
		}
	});
	getFile("fsinfo", function (res) {
		document.getElementById("fsinfo").innerHTML = res;
	});
}

function format() {
	getFile("format", function (res) {
		var out;
		if (res == "OK") out = "Format Success. Restarting...";
		else out = "Format failed!!!"
		alert(out);
	});
}

function filelist() {
	getFile("filecli?cmd=listfile", function (res) {
		var array = JSON.parse(res)
		var listView = document.getElementById("fileList");
		for (let index = 0; index < array.length; index++) {
			listView.innerHTML += "<tr><td>" + array[index].name + "</td><td>" + array[index].size + "</td><td><button onclick='deleteFile(\""+ array[index].name +"\");' class='red'>DELETE</button></td></tr>";
		}
	});
}
function deleteFile(filePath) {
	getFile("filecli?cmd=" + filePath, function (res) {
		if (res == "OK"){
			alert("Success delete file : " + filePath);
			window.location.reload();
		}else{
			alert("Failed delete file");
		}
	});
}