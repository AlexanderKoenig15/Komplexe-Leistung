EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Schaltplan Komplexe Leistung"
Date "2021-08-25"
Rev "1.0"
Comp "Alexander König"
Comment1 "Kellerbelüftungsanlage"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Interface_Expansion:TCA9548ARGER U?
U 1 1 6133CE75
P 5000 3550
F 0 "U?" H 5000 4631 50  0000 C CNN
F 1 "TCA9548A" H 5000 4540 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_RGE0024C_EP2.1x2.1mm" H 5000 2550 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tca9548a.pdf" H 5050 3800 50  0001 C CNN
	1    5000 3550
	-1   0    0    1   
$EndComp
$Comp
L Sensor:BME280 U?
U 1 1 6133FB63
P 1150 3350
F 0 "U?" V 675 3350 50  0000 C CNN
F 1 "BME280 Outside" V 584 3350 50  0000 C CNN
F 2 "Package_LGA:Bosch_LGA-8_2.5x2.5mm_P0.65mm_ClockwisePinNumbering" H 2650 2900 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf" H 1150 3150 50  0001 C CNN
	1    1150 3350
	1    0    0    -1  
$EndComp
$Comp
L RF_Module:ESP-12E U?
U 1 1 61370196
P 5000 5800
F 0 "U?" H 5000 6781 50  0000 C CNN
F 1 "NodeMCU 0.9" H 5000 6690 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 5000 5800 50  0001 C CNN
F 3 "http://wiki.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_v1.1.pdf" H 4650 5900 50  0001 C CNN
	1    5000 5800
	1    0    0    -1  
$EndComp
$Comp
L 0.92~Inch~OLED~Display:RIT276-OLED-Display U?
U 1 1 6137AB6D
P 6700 4100
F 0 "U?" H 6978 3788 50  0000 L CNN
F 1 "RIT276-OLED-Display" H 6978 3697 50  0000 L CNN
F 2 "" H 6700 4100 50  0001 C CNN
F 3 "" H 6700 4100 50  0001 C CNN
	1    6700 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	5000 4450 5000 4750
Wire Wire Line
	5000 6500 4200 6500
Wire Wire Line
	1250 6500 1250 3950
Wire Wire Line
	1250 2750 3900 2750
Wire Wire Line
	3900 2750 3900 4750
Wire Wire Line
	3900 4750 5000 4750
Connection ~ 5000 4750
Wire Wire Line
	5000 4750 5000 5000
Wire Wire Line
	6550 3550 6550 3700
Wire Wire Line
	6550 3700 5750 3700
Wire Wire Line
	5750 4750 5000 4750
Wire Wire Line
	6050 4150 5750 4150
Wire Wire Line
	5750 3700 5750 4150
Connection ~ 5750 4150
Wire Wire Line
	5750 4150 5750 4750
Wire Wire Line
	5000 2550 4200 2550
Connection ~ 4200 6500
Wire Wire Line
	4200 6500 1250 6500
$Comp
L Sensor:BME280 U?
U 1 1 61336A88
P 6650 2950
F 0 "U?" V 6083 2950 50  0000 C CNN
F 1 "BME280 Inside" V 6174 2950 50  0000 C CNN
F 2 "Package_LGA:Bosch_LGA-8_2.5x2.5mm_P0.65mm_ClockwisePinNumbering" H 8150 2500 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf" H 6650 2750 50  0001 C CNN
	1    6650 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	6550 2350 5000 2350
Wire Wire Line
	5000 2350 5000 2550
Connection ~ 5000 2550
Wire Wire Line
	6050 4250 6050 6500
Wire Wire Line
	6050 6500 5000 6500
Connection ~ 5000 6500
Wire Wire Line
	4600 3850 2500 3850
Wire Wire Line
	2500 3850 2500 3250
Wire Wire Line
	2500 3250 1750 3250
Wire Wire Line
	1750 3450 2600 3450
Wire Wire Line
	2600 3450 2600 3750
Wire Wire Line
	2600 3750 4600 3750
Wire Wire Line
	4600 4250 4300 4250
Wire Wire Line
	4300 4250 4300 4650
Wire Wire Line
	4300 4650 5650 4650
Wire Wire Line
	5650 4050 6050 4050
Wire Wire Line
	6050 3950 5550 3950
Wire Wire Line
	5550 3950 5550 4900
Wire Wire Line
	4200 2550 4200 6500
Wire Wire Line
	4350 4900 4350 4150
Wire Wire Line
	4350 4150 4600 4150
Wire Wire Line
	4350 4900 5550 4900
Wire Wire Line
	4600 3950 4400 3950
Wire Wire Line
	4400 3950 4400 2450
Wire Wire Line
	4400 2450 5650 2450
Wire Wire Line
	5650 2450 5650 2850
Wire Wire Line
	5650 2850 6050 2850
Wire Wire Line
	6050 3050 5650 3050
Wire Wire Line
	5650 5000 5000 5000
Wire Wire Line
	4400 5000 4400 4050
Wire Wire Line
	4400 4050 4600 4050
Wire Wire Line
	5650 3050 5650 4050
Connection ~ 5650 4050
Wire Wire Line
	5650 4050 5650 4650
Connection ~ 5650 4650
Wire Wire Line
	5650 4650 5650 5000
Connection ~ 5000 5000
Wire Wire Line
	5000 5000 4400 5000
$EndSCHEMATC
