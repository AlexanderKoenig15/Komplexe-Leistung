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
	6550 3700 5950 3700
Wire Wire Line
	5750 4750 5000 4750
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
	5650 5000 5450 5000
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
$Comp
L power:+12V #PWR?
U 1 1 6143626A
P 10000 1000
F 0 "#PWR?" H 10000 850 50  0001 C CNN
F 1 "+12V" V 10015 1128 50  0000 L CNN
F 2 "" H 10000 1000 50  0001 C CNN
F 3 "" H 10000 1000 50  0001 C CNN
	1    10000 1000
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_FET:IRF9540N Q?
U 1 1 614399A7
P 9350 1500
F 0 "Q?" H 9554 1546 50  0000 L CNN
F 1 "IRF9540N" H 9554 1455 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 9550 1425 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf9540n.pdf" H 9350 1500 50  0001 L CNN
	1    9350 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 1300 9450 1000
$Comp
L Device:R R?
U 1 1 6145DE03
P 8500 1200
F 0 "R?" H 8570 1246 50  0000 L CNN
F 1 "3K9" H 8570 1155 50  0000 L CNN
F 2 "" V 8430 1200 50  0001 C CNN
F 3 "~" H 8500 1200 50  0001 C CNN
	1    8500 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 1000 9000 1000
Wire Wire Line
	8500 1000 8500 1050
$Comp
L Device:C C?
U 1 1 614616AE
P 9000 1200
F 0 "C?" H 9115 1246 50  0000 L CNN
F 1 "100NF" H 9115 1155 50  0000 L CNN
F 2 "" H 9038 1050 50  0001 C CNN
F 3 "~" H 9000 1200 50  0001 C CNN
	1    9000 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 1050 9000 1000
Connection ~ 9000 1000
Wire Wire Line
	9000 1000 8500 1000
Wire Wire Line
	8500 1350 8500 1500
Wire Wire Line
	8500 1500 9000 1500
Wire Wire Line
	9000 1350 9000 1500
Connection ~ 9000 1500
Wire Wire Line
	9000 1500 9150 1500
$Comp
L Device:R R?
U 1 1 61465070
P 8500 1700
F 0 "R?" H 8570 1746 50  0000 L CNN
F 1 "1K1" H 8570 1655 50  0000 L CNN
F 2 "" V 8430 1700 50  0001 C CNN
F 3 "~" H 8500 1700 50  0001 C CNN
	1    8500 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 1550 8500 1500
Connection ~ 8500 1500
Wire Wire Line
	8500 1850 8500 2050
Wire Wire Line
	8500 2050 8000 2050
Wire Wire Line
	9450 1000 10000 1000
Connection ~ 9450 1000
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 61477DA3
P 7900 2250
F 0 "Q?" H 8104 2296 50  0000 L CNN
F 1 "2N7000" H 8104 2205 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8100 2175 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 7900 2250 50  0001 L CNN
	1    7900 2250
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 6147A02B
P 7400 1200
F 0 "Q?" H 7604 1246 50  0000 L CNN
F 1 "2N7000" H 7604 1155 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7600 1125 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 7400 1200 50  0001 L CNN
	1    7400 1200
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 6147B5F5
P 7900 1200
F 0 "Q?" H 8104 1246 50  0000 L CNN
F 1 "2N7000" H 8104 1155 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8100 1125 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 7900 1200 50  0001 L CNN
	1    7900 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 1000 7700 1200
Wire Wire Line
	7200 1200 7200 2250
Wire Wire Line
	7200 2250 7350 2250
Wire Wire Line
	5750 3700 5750 4750
Wire Wire Line
	6050 4150 5950 4150
Wire Wire Line
	5950 4150 5950 3700
Connection ~ 5950 3700
Wire Wire Line
	5950 3700 5750 3700
Wire Wire Line
	5600 5600 5850 5600
Wire Wire Line
	5850 5600 5850 4150
Wire Wire Line
	5850 4150 5400 4150
Wire Wire Line
	5400 4250 5800 4250
Wire Wire Line
	5800 4250 5800 5700
Wire Wire Line
	5800 5700 5600 5700
Wire Wire Line
	5600 5900 7350 5900
Wire Wire Line
	7350 5900 7350 2250
Connection ~ 7350 2250
Wire Wire Line
	7350 2250 7700 2250
Wire Wire Line
	5450 1000 5450 5000
Wire Wire Line
	5450 1000 7500 1000
Connection ~ 7500 1000
Wire Wire Line
	7500 1000 7700 1000
Connection ~ 5450 5000
Wire Wire Line
	5450 5000 5000 5000
$Comp
L power:GND #PWR?
U 1 1 6148E53E
P 10000 1100
F 0 "#PWR?" H 10000 850 50  0001 C CNN
F 1 "GND" V 10005 972 50  0000 R CNN
F 2 "" H 10000 1100 50  0001 C CNN
F 3 "" H 10000 1100 50  0001 C CNN
	1    10000 1100
	0    1    1    0   
$EndComp
Wire Wire Line
	10000 6500 6050 6500
Connection ~ 6050 6500
Wire Wire Line
	10000 1100 10000 1250
$Comp
L Motor:Fan_4pin M?
U 1 1 614973CA
P 10700 1050
F 0 "M?" H 10858 1146 50  0000 L CNN
F 1 "Fan_4pin" H 10858 1055 50  0000 L CNN
F 2 "" H 10700 1060 50  0001 C CNN
F 3 "http://www.formfactors.org/developer%5Cspecs%5Crev1_2_public.pdf" H 10700 1060 50  0001 C CNN
	1    10700 1050
	1    0    0    -1  
$EndComp
$Comp
L Motor:Fan_4pin M?
U 1 1 6149A26D
P 10700 1750
F 0 "M?" H 10858 1846 50  0000 L CNN
F 1 "Fan_4pin" H 10858 1755 50  0000 L CNN
F 2 "" H 10700 1760 50  0001 C CNN
F 3 "http://www.formfactors.org/developer%5Cspecs%5Crev1_2_public.pdf" H 10700 1760 50  0001 C CNN
	1    10700 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10700 1950 10000 1950
Connection ~ 10000 1950
Wire Wire Line
	10000 1950 10000 6500
Wire Wire Line
	10700 1250 10000 1250
Connection ~ 10000 1250
Wire Wire Line
	10000 1250 10000 1950
Wire Wire Line
	9450 1700 10050 1700
Wire Wire Line
	10050 1700 10050 1450
Wire Wire Line
	10050 1450 10150 1450
Wire Wire Line
	10150 1450 10150 750 
Wire Wire Line
	10150 750  10700 750 
Connection ~ 10150 1450
Wire Wire Line
	10150 1450 10700 1450
Wire Wire Line
	10400 1150 10300 1150
Wire Wire Line
	10100 1150 10100 850 
Wire Wire Line
	10100 850  8000 850 
Wire Wire Line
	8000 850  8000 1000
Wire Wire Line
	10300 1150 10300 1850
Wire Wire Line
	10300 1850 10400 1850
Connection ~ 10300 1150
Wire Wire Line
	10300 1150 10100 1150
$EndSCHEMATC
