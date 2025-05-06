<?php
/*
  Заносит поступившие значения от сенсора в базу данны 
  
  https://www.php.net/manual/ru/reserved.variables.php
*/
$servername = "localhost:3306";
$dbname = "example_esp_data";
$username = "root";
$password = "tiamat";

$api_key_value = "PtmAT51b3j4F8";

$api_key = $device = $location = $value1 = $value2 = $value3 = $DevPoint = $Illuminate = "";

print_r($_POST);
echo "               Test string                \n";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
		$device = test_input($_POST["device"]);
		$location = test_input($_POST["location"]);
		
        $value1 = test_input($_POST["value1"]);
        $value2 = test_input($_POST["value2"]);
        $value3 = test_input($_POST["value3"]);        
		$DevPoint = test_input($_POST["DevPoint"]);        
		$Illuminate = test_input($_POST["Illuminate"]);		
		$meas_time = test_input($_POST["meas_time"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            echo "<td>Connect failed</td><br>";
            die("Connection failed: " . $conn->connect_error);
        }
        else {
            echo "<td>Connect to created</td><br>";
        }
        
        $sql = "INSERT INTO Sensor2 (location, value1, value2, value3, DevPoint, Illuminate, meas_time) VALUES ('" . $location . "','" . $value1 . "', '" . $value2 . "', '" . $value3 . "', '" . $DevPoint . "', '" . $Illuminate . "','" . $meas_time . "')";

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
		// Предоставлен неверный ключ API
        echo "Ошибка ключа для записи GET";
    }

}
else {
	// Нет данных посланных с HTTP POST
    echo "No data posted with HTTP POST и GET.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>