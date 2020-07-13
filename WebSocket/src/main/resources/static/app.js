var stompClient = null;
var map = null;
var location_data = [];
var pin_data = [];
var list_mission = [];
var check_mission = 0;
var string_mission = "";
var count_mission = 0;
var current_loc = null;
var current_pin = null;

function GetMap() {
    // Initialize the map
    map = new Microsoft.Maps.Map(document.getElementById("mapDiv"), {credentials: "AuMaHi1MAXAciqXe2eAYNSGq-2S52QQGPnbjp19MSG0j2G2lozrh5A7hRTqkMNEO"});
    // var loc = new Microsoft.Maps.Location(47.3977, 8.54559);
    // var pin = new Microsoft.Maps.Pushpin(loc, {
    //     icon: '<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20"><circle cx="9" cy="9" r="7" stroke="green" stroke-width="4" fill="yellow" /></svg>',
    //     anchor: new Microsoft.Maps.Point(9, 9)
    // });
    // list_mission.push(loc);
    // map.entities.push(pin);
    // map.setView({center: loc, zoom: 17});
}

function changeCheck() {

    //Add handler for the map click event.
    checkMission = Microsoft.Maps.Events.addHandler(map, 'click', function (e) {
        count_mission++;
        var pin = new Microsoft.Maps.Pushpin(e.location, {
            icon: '<svg xmlns="http://www.w3.org/2000/svg" width="15" height="15"><circle cx="8" cy="8" r="5" stroke="red" stroke-width="4" fill="yellow" /></svg>',
            anchor: new Microsoft.Maps.Point(8, 8),
            title: "" + count_mission
        });
        map.entities.push(pin);
        list_mission.push(e.location);
        var line = new Microsoft.Maps.Polyline(list_mission, {strokeColor: 'yellow'});
        map.entities.push(line);
        console.log(e.location.latitude + " " + e.location.longitude);
    });
    document.getElementById("infoState").innerHTML = "Create mission mode";
}

function addToMission() {
    var lat = document.getElementById("addLat").value;
    var lon = document.getElementById("addLon").value;
    if (lat != "" && lon != "") {
        count_mission++;
        var locat = new Microsoft.Maps.Location(lat, lon);
        var pin = new Microsoft.Maps.Pushpin(locat, {
            icon: '<svg xmlns="http://www.w3.org/2000/svg" width="15" height="15"><circle cx="8" cy="8" r="5" stroke="red" stroke-width="4" fill="yellow" /></svg>',
            anchor: new Microsoft.Maps.Point(8, 8),
            title: "" + count_mission
        });
        map.entities.push(pin);
        list_mission.push(locat);
        var line = new Microsoft.Maps.Polyline(list_mission, {strokeColor: 'yellow'});
        map.entities.push(line);
        console.log(lat + " " + lon);
    } else {
        document.getElementById("infoState").innerHTML = "You must enter both Latitude and Longitude";
    }
}

function disnableMision() {
    document.getElementById("infoState").innerHTML = "Close create mission mode";
    Microsoft.Maps.Events.removeHandler(checkMission);
}

function resetProperties(connected) {
    $("#button-connect").prop("disabled", connected);
    $("#button-disconnect").prop("disabled", !connected);
    if (connected) {
        $("#infotext").html("You are connected");
    } else {
        $("#infotext").html("You are not connected");
    }
}

function connect() {
    var socket = new SockJS('/chat-websocket');
    stompClient = Stomp.over(socket);
    stompClient.connect({}, function (frame) {
        resetProperties(true);
        console.log("reset...");
        stompClient.subscribe('/chat/sendMessage', function (data) {
            createTextNode(JSON.parse(data.body));
        });
        stompClient.subscribe('/chat/location', function (data) {
            createFirstLocation(JSON.parse(data.body));
        });
    });
}

function getCurrentLocation() {
    location_data = [];
    pin_data = [];
    list_mission = [];
    check_mission = 0;
    count_mission = 0;
    string_mission = "";
    removePushpin();
    stompClient.send("/app/clearmission", {});
    current_loc = null;
    current_pin = null;
    stompClient.send("/app/getLocation", {});
}

function createFirstLocation(loca) {
    var lat = Number(loca.lat);
    var lon = Number(loca.lon);
    console.log(loca.lat);
    console.log(loca.lon);

    current_loc = new Microsoft.Maps.Location(lat, lon);
    current_pin = new Microsoft.Maps.Pushpin(current_loc, {
        icon: '<svg xmlns="http://www.w3.org/2000/svg" width="20" height="20"><circle cx="9" cy="9" r="7" stroke="green" stroke-width="4" fill="yellow" /></svg>',
        anchor: new Microsoft.Maps.Point(9, 9)
    });
    list_mission.push(current_loc);
    map.entities.push(current_pin);
    map.setView({center: current_loc, zoom: 17});
}

function disconnect() {
    if (stompClient !== null) {
        stompClient.disconnect();
    }
    resetProperties(false);
}

function sendMessage() {
    stompClient.send("/app/send", {});
}

function sendClear() {
    stompClient.send("/app/cleardata", {});
    location_data = [];
    pin_data = [];
    removePushpin();
    map.entities.push(current_pin);

    list_mission.forEach(function (item, index, array) {
        if (index != 0) {
            var pins = new Microsoft.Maps.Pushpin(item, {
                icon: '<svg xmlns="http://www.w3.org/2000/svg" width="15" height="15"><circle cx="8" cy="8" r="5" stroke="red" stroke-width="4" fill="yellow" /></svg>',
                anchor: new Microsoft.Maps.Point(8, 8),
                title: "" + index
            });
            map.entities.push(pins);
        }
    });
    var line = new Microsoft.Maps.Polyline(list_mission, {strokeColor: 'yellow'});
    map.entities.push(line);
    check_mission = 0;
    document.getElementById("infoState").innerHTML = "Clear data output";
    console.log("Clear data output");
}

function removePushpin() {
    for (var i = map.entities.getLength() - 1; i >= 0; i--) {
        var pushpin = map.entities.get(i);
        if (pushpin instanceof Microsoft.Maps.Pushpin || pushpin instanceof Microsoft.Maps.Polyline) {
            map.entities.removeAt(i);
        }
    }

    // map.entities.push(current_pin);
    // map.setView({center: loc, zoom: 17});
}

function clearMission() {
    location_data = [];
    pin_data = [];
    list_mission = [];
    check_mission = 0;
    count_mission = 0;
    string_mission = "";
    removePushpin();
    map.entities.push(current_pin);
    list_mission.push(current_loc);
    stompClient.send("/app/clearmission", {});
    document.getElementById("infoState").innerHTML = "Clear data and mission";
    console.log("Clear data and mission");
}

function executeTakeoff() {
    sendClear();
    stompClient.send("/app/takeoffandland", {});
    location_data = [];
    pin_data = [];
    document.getElementById("infoState").innerHTML = "Takeoff and Land!";
}

function sendMission() {
    sendClear();
    console.log(string_mission);
    if (list_mission.length != 0) {
        document.getElementById("infoState").innerHTML = "Uploading mission. Waiting...";
        list_mission.forEach(function (item, index, array) {
            if (index != 0) {
                uploadMission(item);
            }
        });
    } else {
        document.getElementById("infoState").innerHTML = "You didn't set a mission!";
    }
}

function uploadMission(loca) {
    string_mission = string_mission + loca.longitude + "," + loca.latitude + "\n";
    check_mission++;
    if (check_mission == count_mission) {
        console.log(string_mission);
        var stringMission = {
            strMission: string_mission
        }
        stompClient.send("/app/uploadmission", {}, JSON.stringify(stringMission));
        setTimeout(function () {
            document.getElementById("infoState").innerHTML = "Mission upload successful";
        }, 1000);
    }
}

function executeMisson() {
    stompClient.send("/app/mission", {});
    location_data = [];
    pin_data = [];
    document.getElementById("infoState").innerHTML = "Fly Mission and return to land";
}

function executeMissonNorl() {
    stompClient.send("/app/missionnorl", {});
    location_data = [];
    pin_data = [];
    document.getElementById("infoState").innerHTML = "Fly Mission and no return to land";
}

function sendAltsp() {
    var alt = document.getElementById("altSend").value;
    var speed = document.getElementById("speedSend").value;
    if (alt != "" && speed != "") {
        var altsp = alt + "," + speed;
        uploadAltsp(altsp);
    } else {
        document.getElementById("infoState").innerHTML = "You must enter both Altitude and Speed, else Drone use default properties";
    }
}

function uploadAltsp(altsp) {
    var stringAltsp = {
        strAltsp: altsp
    }
    stompClient.send("/app/uploadaltsp", {}, JSON.stringify(stringAltsp));
    setTimeout(function () {
        document.getElementById("infoState").innerHTML = "Upload altitude and speed successful";
    }, 1000);
}

function createTextNode(loca) {
    if (loca.alt != null && loca.lat !== 0) {
        var lat = Number(loca.lat);
        var lon = Number(loca.lon);
        console.log(loca.lat);
        console.log(loca.lon);
        document.getElementById("altDrone").innerHTML = "" + loca.alt;
        var locat = new Microsoft.Maps.Location(lat, lon);
        location_data.push(locat);

        var pin = new Microsoft.Maps.Pushpin(locat, {
            icon: '<svg xmlns="http://www.w3.org/2000/svg" width="15" height="15"><circle cx="8" cy="8" r="5" stroke="orange" stroke-width="4" fill="white" /></svg>',
            anchor: new Microsoft.Maps.Point(8, 8)
        });
        pin_data.push(pin);
    }

    if (location_data.length != 0) {
        var line = new Microsoft.Maps.Polyline(location_data);
        // map.setView({center: location_data[location_data.length - 1], zoom: 19});
        map.entities.push(line);
        pin_data.forEach(function (item, index, array) {
            map.entities.push(item);
        });
    }
}

$(function () {
    $("form").on('submit', function (e) {
        e.preventDefault();
    });
    resetProperties(false);
    $("#button-connect").click(function () {
        connect();
    });
    $("#button-disconnect").click(function () {
        disconnect();
    });
    $("#button-send").click(function () {
        sendMessage();
    });

    $("#button-start").click(function () {
        follow();
    });
    $("#button-stop").click(function () {
        stopFollow();
    });
});

// window.onload = function(){
//     var button = document.getElementById('button-send');
//     setInterval(function(){
//         button.click();
//     },1000);  // this will make it click again every 1000 miliseconds
// };

function follow() {
    var button = document.getElementById('button-send');
    id = setInterval(frame, 1000);

    function frame() {
        button.click();
    }
}

function stopFollow() {
    clearInterval(id);
    console.log('stop');
}