<?php
class hat
{
	function hat1()
	{
?>
<!DOCTYPE html>
<!--//wltech2/wifi/headerwf.php -->
<html lang="ru">
<head>
	<!-- Google tag (gtag.js) --> 
	<script async src="https://www.googletagmanager.com/gtag/js?id=G-6LVLNQGR49"></script> 
	<script> 
		window.dataLayer = window.dataLayer || []; 
		function gtag(){dataLayer.push(arguments);} 
		gtag('js', new Date()); 

		gtag('config', 'G-6LVLNQGR49'); 
	</script>
	
	<meta charset="utf-8">
	<title>Управление малогабаритным WiFi устройством ESP-12Е, оснащённым различными сенсорами 
		и реле  через Интернет</title>
	<meta name="keywords" content=" Малогабаритное WiFi-устройство, малогабаритное 
		WiFi-устройство с датчиком, модуль ESP-12Е, управление сенсорами и реле через Интернет,
		управление поливом и отоплением через Интернет, система сбора информации, система 
		контроля сенсоров через Интернет, управление из любой точки мира, Small size WiFi device, 
		Small size WiFi device with sensor, ESP-12E module, control of sensors and relays via 
		the Internet, control of watering and heating via the Internet, information collection 
		system, control system of sensors via the Internet, control from anywhere in the world."/>
	<meta name="description" content=" Управление через Интернет из любой точки мира 
		малогабаритным WiFi устройством, оснащённым различными сенсорами и реле, подключаемым 
		к локальной домашней сети или сети предприятия.">
	<link rel="shortcut icon" href="butterflyicon.ico" type="image/x-icon"> 
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
        
        <link href="csswf/reset.css" rel="stylesheet">
		<!--<link href="csswf/ctyle2.css" rel="stylesheet">-->
        <!--<link href="lightbox/css/lightbox.min.css" rel="stylesheet">-->
        <link href="https://fonts.googleapis.com/css?family=Metal+Mania" rel="stylesheet">
        <link href="https://fonts.googleapis.com/css?family=Open+Sans:300" rel="stylesheet">
        <script src="https://use.fontawesome.com/0ca06f29a6.js"></script>
		<link href="csswf/style-wifi.css" rel="stylesheet">
 		<link href="csswf/style-wifi-hf.css" rel="stylesheet">
		<!--<link href="csswf/style-content-wf.css" rel="stylesheet">-->
		<!--<link href="csswf/mainwifi.css" rel="stylesheet">-->
		<!--<link href="csswf/mainwifitab.css" rel="stylesheet">-->
		<link href="csswf/style-wifi-db.css" rel="stylesheet">
		<link href="csswf/style-wifi-esp.css" rel="stylesheet">
		<link href="csswf/style-wifi-chart.css" rel="stylesheet">
	
		<script src="https://code.highcharts.com/highcharts.js"></script>
		
		<!-- Yandex.Metrika counter    31-01-2022 -->
	<script type="text/javascript" >
   		(function(m,e,t,r,i,k,a){m[i]=m[i]||function(){(m[i].a=m[i].a||[]).push(arguments)};
   		m[i].l=1*new Date();
   		for (var j = 0; j < document.scripts.length; j++) {if (document.scripts[j].src === r) { return; }}
  		k=e.createElement(t),a=e.getElementsByTagName(t)[0],k.async=1,k.src=r,a.parentNode.insertBefore(k,a)})
   		(window, document, "script", "https://mc.yandex.ru/metrika/tag.js", "ym");

   		ym(92271468, "init", {
        	clickmap:true,
        	trackLinks:true,
        	accurateTrackBounce:true
   		});
	</script>
	<noscript><div><img src="https://mc.yandex.ru/watch/92271468" style="position:absolute; left:-9999px;" alt="" /></div></noscript>
	<!-- /Yandex.Metrika counter -->

</head> 
    
<body>
    <header>
        <div id="topbar">
			<nav>
                <ul class="menu">
                    <li><a href="http://wltech.ru">Начало</a></li>
                    <li><a href="http://wltech.ru#proj">Проекты</a></li>
                    <li><a href="http://wltech.ru#gallery">Фото</a></li>
                    <li><a href="http://wltech.ru#subscribe">Контакты</a></li>
                </ul>
            </nav>
        </div>
        <div class="container">
            <h1>Контроль и управление WiFi<br>
			устройствами через Интернет</h1>
			<h2 class="cl_h1">
				Передача от WiFi-устройства на домашний роутер<br>
				и через интернет в базу данных Web-сервера
			</h2>
			<!--<a href="http://www.idg.net.ua/blog/uchebnik-css/ispolzovanie-css/praktika-background" class="button">Go back to the lesson</a> -->
        </div>

		<div id="topmenu">
			<ul class="top">
				<!-- Абсолютная ссылка
				<li><a href="http://localhost/mysite/index.php">О проекте</a></li>
		     		Долеее, Относительные ссылки-->
				<li><a href="index.php">О проекте</a></li>
				<li><a href="esp-outputs.php">Устройство</a></li>
				<!--<li><a href="esp-chart.php">Графика</a></li>-->
				<li><a href="wifi-chart.php">Графика</a></li>
				<!--<li><a href="esp-view-db2.php">База</a></li>2020-10-22-->	
				<li><a href="wifi-view-db.php">База</a></li>
				<!--<li><a href="wifisensortable.php">База</a></li>-->
				<!--<li class="odermenu"><a href="https://wltech.ru/index.php">Никуда</a></li>-->
			</ul>
		</div>

    </header>	
<?php
	}
}
?>
