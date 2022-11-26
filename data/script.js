// Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);

// Create Temperature Gauge
var gaugeTemp = new LinearGauge({
    renderTo: 'gauge-temp',
    width: 120,
    height: 400,
    units: "Temperature C",
    minValue: 0,
    startAngle: 90,
    ticksAngle: 180,
    maxValue: 40,
    colorValueBoxRect: "#049faa",
    colorValueBoxRectEnd: "#049faa",
    colorValueBoxBackground: "#f1fbfc",
    valueDec: 2,
    valueInt: 2,
    majorTicks: [
        "0",
        "5",
        "10",
        "15",
        "20",
        "25",
        "30",
        "35",
        "40"
    ],
    minorTicks: 4,
    strokeTicks: true,
    highlights: [
        {
            "from": 30,
            "to": 40,
            "color": "rgba(200, 50, 50, .75)"
        }
    ],
    colorPlate: "#fff",
    colorBarProgress: "#CC2936",
    colorBarProgressEnd: "#049faa",
    borderShadowWidth: 0,
    borders: false,
    needleType: "arrow",
    needleWidth: 2,
    needleCircleSize: 7,
    needleCircleOuter: true,
    needleCircleInner: false,
    animationDuration: 1500,
    animationRule: "linear",
    barWidth: 10,
}).draw();


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
            gaugeTemp.value = myObj.temp;
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
        gaugeTemp.value = myObj.temp;
    }, false);
}