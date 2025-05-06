<?php
// вывод графики температуры, влажности, давления, точки росы и 
include "headerwf.php";
include "footerwf.php";

$page= new hat;
$pagef= new foot;

$page->hat1();

$servername = "localhost";
$dbname = "example_esp_data";
$username = "aleksvolna";
$password = "popoVka1921Fm";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

//$sql = "SELECT id, value1, value2, value3, reading_time FROM Sensor order by reading_time desc limit 30";
$sql = "SELECT id, value1, value2, value3, DevPoint, Illuminate, reading_time FROM Sensor2 order by reading_time desc limit 30";

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()){
    $sensor_data[] = $data;
}

$readings_time = array_column($sensor_data, 'reading_time');

// ******* Uncomment to convert readings time array to your timezone ********
/*$i = 0;
foreach ($readings_time as $reading){
    // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
    $readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading - 1 hours"));
    // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
    //$readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading + 4 hours"));
    $i += 1;
}*/

$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$value2 = json_encode(array_reverse(array_column($sensor_data, 'value2')), JSON_NUMERIC_CHECK);
$value3 = json_encode(array_reverse(array_column($sensor_data, 'value3')), JSON_NUMERIC_CHECK);

$DevPoint = json_encode(array_reverse(array_column($sensor_data, 'DevPoint')), JSON_NUMERIC_CHECK);
$Illuminate = json_encode(array_reverse(array_column($sensor_data, 'Illuminate')), JSON_NUMERIC_CHECK);

$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

echo $value1; echo "<br>";
echo $value2; echo "<br>";
echo $value3; /*echo $reading_time;*/ echo "<br>";
echo $DevPoint; echo "<br>";
echo $Illuminate;

$result->free();
$conn->close();
?>
	
    <h2>Портативная метеостанция</h2>


<div class="hd_graf">
	
	<div id="chart-temperature" class="container"></div>
    <div id="chart-humidity" class="container"></div>
	<div id="chart-pressure" class="container"></div>
	
	<div id="chart-DevPoint" class="container"></div>
	<div id="chart-Illuminate" class="container"></div>
</div>

<script>

var value1 = <?php echo $value1; ?>;
var value2 = <?php echo $value2; ?>;
var value3 = <?php echo $value3; ?>;

var DevPoint = <?php echo $DevPoint; ?>;
var Illuminate = <?php echo $Illuminate; ?>;

var reading_time = <?php echo $reading_time; ?>;

// ТЕМПЕРАТУРА
var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature' },
  //title: { text: 'BME280 Temperature' },
  title: { text: 'Показания температуры' },
  series: [{
    showInLegend: false,
    data: value1
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#059e8a' }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    //title: { text: 'Temperature (Celsius)' }
	title: { text: 'Температура (&deg;C)' } 
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});

//ВЛАЖНОСТЬ	
var chartH = new Highcharts.Chart({
  chart:{ renderTo:'chart-humidity' },
  //title: { text: 'BME280 Humidity' },
  title: { text: 'Измерения влажности' },
  series: [{
    showInLegend: false,
    data: value2
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    }
  },
  xAxis: {
    type: 'datetime',
    //dateTimeLabelFormats: { second: '%H:%M:%S' },
    categories: reading_time
  },
  yAxis: {
    //title: { text: 'Humidity (%)' }
	title: { text: 'Измерения влажности (%)' } 
  },
  credits: { enabled: false }
});
	
// Атмосферное давление
var chartP = new Highcharts.Chart({
  chart:{ renderTo:'chart-pressure' },
  title: { text: 'Атмосферное давление' },
  series: [{
    showInLegend: false,
    data: value3
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#18009c' }
  },
  xAxis: {
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    //title: { text: 'Pressure (hPa)' }
	title: { text: 'Давление (hPa)' }
  },
  credits: { enabled: false }
});
	
//ТОЧКА РОСЫ	
var chartR = new Highcharts.Chart({
  chart:{ renderTo:'chart-DevPoint' },
  title: { text: 'Точка Росы' },
  series: [{
    showInLegend: false,
    data: DevPoint			//
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#f8009c' }
  },
  xAxis: {
    type: 'datetime',
    //dateTimeLabelFormats: { second: '%H:%M:%S' },
    categories: reading_time
  },
  yAxis: {
	title: { text: 'Тоска росы (&deg;C)' } 
  },
  credits: { enabled: false }
});	
	
//ОСВЕЩЁННОСТЬ	
var chartI = new Highcharts.Chart({
  chart:{ renderTo:'chart-Illuminate' },
  title: { text: 'Освещённость' },
  series: [{
    showInLegend: false,
    data: Illuminate
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#2020EC' }
  },
  xAxis: {
    type: 'datetime',
    //dateTimeLabelFormats: { second: '%H:%M:%S' },
    categories: reading_time
  },
  yAxis: {
	title: { text: 'Освещённость (лк)' } 
  },
  credits: { enabled: false }
});
	
</script>

<div class="note">
	<!--<p>Снятие показаний осуществляется каждые две минуты</p>

	<p>Примечание. В настоящее время система работает в тестовом режиме.<br>
	Снятие показаний осуществляется каждые 30 секунд.</p>-->
</div>

//<?php
//    $pagef->foot1();
//?>