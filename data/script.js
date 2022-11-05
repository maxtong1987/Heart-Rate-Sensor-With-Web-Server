// Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);

// Create BPM Gauge
var gaugeBpm = new RadialGauge({
    renderTo: 'gauge-bpm',
    width: 300,
    height: 300,
    units: "BPM",
    minValue: 25,
    maxValue: 220,
    colorValueBoxRect: "#049faa",
    colorValueBoxRectEnd: "#049faa",
    colorValueBoxBackground: "#f1fbfc",
    valueInt: 2,
    majorTicks: [
        "25",
        "50",
        "75",
        "100",
        "125",
        "150",
        "175",
        "200",
        "220"

    ],
    minorTicks: 5,
    strokeTicks: true,
    highlights: [
        {
            "from": 100,
            "to": 135,
            "color": "#ffee99"
        },
        {
            "from": 135,
            "to": 165,
            "color": "#ffcb87"
        },
        {
            "from": 165,
            "to": 220,
            "color": "#ff624a"
        },


    ],
    colorPlate: "#fff",
    borderShadowWidth: 0,
    borders: false,
    needleType: "line",
    colorNeedle: "#007F80",
    colorNeedleEnd: "#007F80",
    needleWidth: 2,
    needleCircleSize: 3,
    colorNeedleCircleOuter: "#007F80",
    needleCircleOuter: true,
    needleCircleInner: false,
    animationDuration: 500,
    animationRule: "linear"
}).draw();

// Function to get current readings on the webpage when it loads for the first time
function getReadings() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var myObj = JSON.parse(this.responseText);
            console.log(myObj);
            gaugeBpm.value = myObj.bpm;
        }
    };
    xhr.open("GET", "/readings", true);
    xhr.send();
}

if (!!window.EventSource) {
    var source = new EventSource('/events');

    source.addEventListener('open', function (e) {
        console.log("Events Connected");
    }, false);

    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            console.log("Events Disconnected");
        }
    }, false);

    source.addEventListener('message', function (e) {
        console.log("message", e.data);
    }, false);

    source.addEventListener('new_readings', function (e) {
        console.log("new_readings", e.data);
        var myObj = JSON.parse(e.data);
        console.log(myObj);
        gaugeBpm.value = myObj.bpm;
    }, false);
}