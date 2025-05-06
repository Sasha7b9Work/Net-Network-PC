<!--/wifi/wifi-view-db.php-->
<?php

    include "headerwf.php";
    include "footerwf.php";

    include "class-sensorwifi-db.php";

    $db = new baza();

    $page= new hat;
    $pagef= new foot;

    $page->hat1();

	$servername = "localhost";
	$dbname = "example_esp_data";
	$username = "root";
	$password = "tiamat";

	//Сиреневый цвет таблицы
	//bgcolor="#ddddff"
?>

<section>
<div class="razmer">
	<h4>Просмотр показаний сенсоров в <b>Базе данных</b> </h4>
</div>
	
<div id="container2">
  <div id="content2">	
	  
	<div id="right2">

	</div> 
	  
    <div id="left2">


		<p><b>Изменение условий просмотра</b>
				
		<!-- Просмотр за сутки 	-->	
		<p>	Укажите дату просмотра (без времени/с временем).</p>
		<!--2020-10-18<p> Примечание. Дата просмотра в формате: год-месяц-день или <br>
			год-месяц-день часы:минуты:секунды</p>
		<p>Например, 2020-10-11 или 2020-10-11 01:05:12 </p>-->
		<p>Примечание. Дата просмотра в формате: год-месяц-день<br>
			Например, 2024-09-05 <br>
			или 2024-09-05 17:30 <br>
			или 2024-09-05 17:30:25 <br>
		</p>
		<div id="left2-indate">
			<form method="GET" action="/wifi/wifi-view-db2.php">
				<label class="indata" for="sname">Задайте дату</label>
				<input class="indata" type="text" name="sname" id="sname" size="15" value="2024-09-05" >
				<input class="rd_base" type="submit" value="Запросить данные за день">
			</form>
		</div>
			
		<!-- Просмотр по идентификатору -->	
		<p>	Укажите позицию начала просмотра (Идентификатор).
		<div id="left2-inID">	
			<form method="GET" name="format" action="/wifi/wifi-view-db2.php">
				<label class="indata" for="idname">Задайте номер ID</label>
				<input class="indata" type="text" name="idname" id="idname" size="15" value="50" >
				<input class="rd_base" type="submit" value="Запросить данные по ID">
			</form>
		</div>
		<br>
	</div>
	
	<div id="middle2">
		
	
	<?php
				
	    // Create connection
		$conn = new mysqli($servername, $dbname);
		// Check connection
		if ($conn->connect_error) {
    		die("Connection failed: " . $conn->connect_error);
		} 
				
		@$type = $_GET['sname'];	// дата, например 2020-10-11
			
		if (!empty($type)) {	// есть 'sname'
			
		
			$date=date_create("$type");
			date_add($date,date_interval_create_from_date_string("1 days"));
			//echo date_format($date,"Y-m-d");		
			$D2 = date_format($date,"Y-m-d");
			//echo $D2;
			
			$sql = "SELECT * FROM `Sensor2` WHERE reading_time > '$type' AND reading_time < '$D2'";

		
			echo "<div class=\"podzagolovok\">Считывание показаний за '$type'.</div>";
		}
		else {
	
			@$type = $_GET['idname'];	// idname - выбор по идентификатору 
			
			if (!empty($type)) {
				
				$num = (int) "$type";
				
				$sql = "SELECT * FROM Sensor2 WHERE id>=$num limit 60";
				echo "<div class=\"podzagolovok\">Считывание 60 показаний сенсоров начиная со стрки с ID >= $num</div>";
			}
			else {
		        $sql = "SELECT id, value1, value2, value3, DevPoint, Illuminate, reading_time FROM Sensor2 order by reading_time desc limit 30";
			    echo "<div class=\"podzagolovok\">Считывание 30 последних показаний сенсоров</div>";
			}
		}
			
		$result = $conn->query($sql);
		
		// Идентификатор
		// Температура
		// Влажность					
		// Давление	
		// Точка росы 
		// Освещённость 
		// Дата
			
		echo "<table id='readdb'><tr><th>ID</th><th>Температура</th><th>Влажность</th><th>Давление</th><th>Точка росы</th><th>Освещённость</th><th>Время</th></tr>";

		while ($row = $result->fetch_assoc())
		{
			// Задание свойств объекта по результатам выборки из базы данных
			$id = $row['id'];
			$value1 = $row['value1'];
			$value2 = $row['value2'];
			$value3 = $row['value3'];
			
			$DevPoint = $row['DevPoint'];
			$Illuminate = $row['Illuminate'];
			
			$reading_time = $row['reading_time'];

			echo "<tr>";
			echo "<td>".$id."</td>";
			echo "<td>".$value1."</td>";
			echo "<td>".$value2."</td>";
			echo "<td>".$value3."</td>";	
			
			echo "<td>".$DevPoint."</td>";	
			echo "<td>".$Illuminate."</td>";	
			
			echo "<td>".$reading_time."</td>";
			echo "</tr>";
		}

		$result->free();
		$conn->close();
		//$result->close();
		echo "</table>";
	?> 

	</div>
	  
	<div class="clear"></div>
  </div>
</div>

	<p class="foot">Если вы хотите посмотреть другие показания, 
	то укажите требуемые строки или дату в колонке слева.</p>

</section>

<?php
    //$pagef->foot1();
?>