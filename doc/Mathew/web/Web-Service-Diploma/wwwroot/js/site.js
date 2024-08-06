var chartTemperature, chartPressure, chartHumidity, chartDewPoint;
var chartIsEmptyTemperature = true;
var chartIsEmptyPressure = true;
var chartIsEmptyHumidity = true;
var chartIsEmptyDewPoint = true;


function writeChart(data, labels, chartId, parameter) {

    const ctx = document.getElementById(chartId).getContext("2d");

    var config = {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: parameter,
                data: data,
                fill: false
            }]
        }
    };

    return new Chart(ctx, config);
}

function updateChart(chart, label, data) {
    chart.data.labels = label
    chart.data.datasets.forEach((dataset) => {
        dataset.data = data;
    });
    chart.update();
}

function writeData(data, time, chartId) {
    const ctx = document.getElementById(chartId);
    var result = "";

    for (var i = 0; i < data.length; i++) {
        result += "<p>" + data[i] + " - " + time[i] + "</p>";
    }

    ctx.innerHTML = result;
}

async function getSchemas() {
    const response = await fetch("/Home/GetSchemas", {
        method: "GET",
        headers: { "Accept": "application/json" }
    });

    if (response.ok === true) {
        const models = await response.json();
        const rows = document.getElementById("schemas");

        models.schemas.forEach(item => rows.append(row(item, models.schema)));
    }
}

async function getParameters() {
    const response = await fetch("/Home/GetParameters", {
        method: "GET",
        headers: { "Accept": "application/json" }
    });

    if (response.ok === true) {
        const models = await response.json();
        var data = [];
        var time = [];

        const temperature = document.getElementById("temperature");
        temperature.innerText = models.instantParameters.temperature.toFixed(2) + " °C";

        for (var i = 0; i < models.temperature.length; i++) {
            data.push(models.temperature[i].parameter);
            time.push(models.temperature[i].time.replace('T', ' ').replace('Z', ''));
        }

        if (chartIsEmptyTemperature) {
            chartTemperature = writeChart(data, time, "temperature-chart", "temperature");
            chartIsEmptyTemperature = false;
        }
        else {
            updateChart(chartTemperature, time, data);
        }


        const pressure = document.getElementById("pressure");
        pressure.innerText = models.instantParameters.pressure.toFixed(2) + " кПа";

        data = [];
        time = [];

        for (var i = 0; i < models.pressure.length; i++) {
            data.push(models.pressure[i].parameter);
            time.push(models.pressure[i].time.replace('T', ' ').replace('Z', ''));
        }

        if (chartIsEmptyPressure) {
            chartPressure = writeChart(data, time, "pressure-chart", "pressure");
            chartIsEmptyPressure = false;
        }
        else {
            updateChart(chartPressure, time, data);
        }

        const humidity = document.getElementById("humidity");
        humidity.innerText = models.instantParameters.humidity.toFixed(2) + " г/м³";

        data = [];
        time = [];

        for (var i = 0; i < models.humidity.length; i++) {
            data.push(models.humidity[i].parameter);
            time.push(models.humidity[i].time.replace('T', ' ').replace('Z', ''));
        }

        if (chartIsEmptyHumidity) {
            chartHumidity = writeChart(data, time, "humidity-chart", "humidity");
            chartIsEmptyHumidity = false;
        }
        else {
            updateChart(chartHumidity, time, data);
        }

        const dewPoint = document.getElementById("dew point");
        dewPoint.innerText = models.instantParameters.dewPoint.toFixed(2) + " °C";

        data = [];
        time = [];

        for (var i = 0; i < models.dewPoint.length; i++) {
            data.push(models.dewPoint[i].parameter);
            time.push(models.dewPoint[i].time.replace('T', ' ').replace('Z', ''));
        }

        if (chartIsEmptyDewPoint) {
            chartDewPoint = writeChart(data, time, "dewPoint-chart", "dew point");
            chartIsEmptyDewPoint = false;
        }
        else {
            updateChart(chartDewPoint, time, data);
        }
    }
}

function row(item, schema) {
    const opt = document.createElement("option");
    opt.setAttribute("value", item);
    opt.innerText = item;

    if (item == schema) {
        opt.selected = true;
    }

    return opt;
}

getSchemas();

setInterval(() => getParameters(), 500)