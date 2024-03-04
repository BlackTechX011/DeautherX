/* This software is licensed under the MIT License: https://github.com/spacehuhntech/esp8266_deauther */

var attackJSON = [[false, 0, 0], [false, 0, 0], [false, 0, 0], [false, 0, 0], [false, "[Nothing]"]];

function draw() {
	getE("deauth").innerHTML = attackJSON[0][0] ? lang("stop") : lang("start");
	getE("beacon").innerHTML = attackJSON[1][0] ? lang("stop") : lang("start");
	getE("probe").innerHTML = attackJSON[2][0] ? lang("stop") : lang("start");
	getE("deauthAll").innerHTML = attackJSON[3][0] ? lang("stop") : lang("start");
	getE("evilTwin").innerHTML = attackJSON[4][0] ? lang("stop") : lang("start");

	getE("deauthTargets").innerHTML = esc(attackJSON[0][1] + "");
	getE("beaconTargets").innerHTML = esc(attackJSON[1][1] + "");
	getE("probeTargets").innerHTML = esc(attackJSON[2][1] + "");
	getE("deauthAllTargets").innerHTML = esc(attackJSON[3][1] + "");
	getE("evilTwinTargets").innerHTML = esc(attackJSON[4][1] + "");

	getE("deauthPkts").innerHTML = esc(attackJSON[0][2] + "/" + attackJSON[0][3]);
	getE("beaconPkts").innerHTML = esc(attackJSON[1][2] + "/" + attackJSON[1][3]);
	getE("probePkts").innerHTML = esc(attackJSON[2][2] + "/" + attackJSON[2][3]);
	getE("deauthAllPkts").innerHTML = esc(attackJSON[3][2] + "/" + attackJSON[3][3]);

	getE("allpkts").innerHTML = esc(attackJSON[5] + "");
}

function stopAll() {
	getFile("run?cmd=stop attack", function () {
		load();
	});
}

function start(mode) {
	switch (mode) {
		case 0:
			attackJSON[0][0] = !attackJSON[0][0];
			break;
		case 1:
			attackJSON[1][0] = !attackJSON[1][0];
			break;
		case 2:
			attackJSON[2][0] = !attackJSON[2][0];
			break;
		case 3:
			attackJSON[3][0] = !attackJSON[3][0];
			break;
		case 4:
			attackJSON[4][0] = !attackJSON[4][0];
			break;
	}
	getFile("run?cmd=attack" + (attackJSON[0][0] ? " -d" : "") + (attackJSON[1][0] ? " -b" : "") + (attackJSON[2][0] ? " -p" : "") + (attackJSON[3][0] ? " -da" : "") + (attackJSON[4][0] ? " -et" : ""), function () {
		setTimeout(load, 2000);
		draw();
	});
}

function load() {
	getFile("attack.json", function (response) {
		attackJSON = JSON.parse(response);
		console.log(response);
		showMessage("connected");
		draw();
	});
}
