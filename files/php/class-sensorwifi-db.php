<!-- class_basesensorwifi.php -->
<?php
class baza 
{
	// Задание констант класса
	const USERNAME = 'root';
	const PASSWORD = 'tiamat';
	const DBNAME = 'example_esp_data';
	const SERVER = 'localhost:3306';

	/*
	Конструктор класса устанавливает соединение с базой данных
	*/
	function __construct($name = NULL)
	{
		if ($mysqli = new mysqli(self::SERVER, self::USERNAME, self::PASSWORD, self::DBNAME))
		{
			$this->connection = $mysqli;
			//echo "Cоединиться с сервером MySQL - установлено";
		}
		else
		{
			echo "Не удается соединиться с сервером MySQL";
			exit;
		}
	
		if ($name)
		{
			$this->name = $name;
		}
	}

	/*function display()
	{
	
		$quest = "SELECT id, value1, value2, value3, reading_time FROM Sensor"; 
	
		//echo "<table border=4 bgcolor=\"#CCFF99\" >";
		echo "<table align=\"center\"bgcolor=\"#0CFF99\" >";
		echo "<tr>";
		
		// Шапка таблицы
		echo "<th>ID</th>";
		echo "<th>Температура</th>";
		echo "<th>Влажность</th>";
		echo "<th>Давление</th>";
		echo "<th>Время</th></tr></table>";

		if ($result = $this->connection->query($quest))			//($query))
		{
			$column1 = 1;
			while ($row = $result->fetch_assoc())
			{
				// Задание свойств объекта по результатам выборки из базы данных
				$this->id = $row['id'];
				$this->value1 = $row['value1'];
				$this->value2 = $row['value2'];
				$this->value3 = $row['value3'];
				$this->reading_time = $row['reading_time'];
				
				echo "<tr>";
				echo "<td>".$this->id."</td>";
				echo "<td>".$this->value1."</td>";
				echo "<td>".$this->value2."</td>";
				echo "<td>".$this->value3."</td>";
				echo "<td>".$this->reading_time."</td>";				
				echo "</tr>";
			}
		}
		$result->close();
		echo "</table>";
	}*/
	
	function _desctruct()
	{
		$this->connection->close();
	}
}
?>