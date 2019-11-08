EESchema Schematic File Version 4
LIBS:PCB_OmniRobot-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 8600 650  2500 2500
U 5D602590
F0 "Receptor Laser" 50
F1 "Receptor_Laser.sch" 50
$EndSheet
$Comp
L promicro_5v:ProMicro 5V1
U 1 1 5D62839C
P 4950 4100
F 0 "5V1" H 4950 5137 60  0000 C CNN
F 1 "ProMicro" H 4950 5031 60  0000 C CNN
F 2 "" H 5050 3050 60  0001 C CNN
F 3 "" H 5050 3050 60  0000 C CNN
	1    4950 4100
	1    0    0    -1  
$EndComp
$Comp
L Dual~MC33926~Motor~Driver~Carrier:Dual_MC33926 Pololu2
U 1 1 5D630EE1
P 2450 5550
F 0 "Pololu2" H 2087 6731 50  0000 C CNN
F 1 "Dual_MC33926" H 2087 6640 50  0000 C CNN
F 2 "" H 2400 5500 50  0001 C CNN
F 3 "" H 2400 5500 50  0001 C CNN
	1    2450 5550
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M0
U 1 1 5D633649
P 1250 2750
F 0 "M0" H 1408 2746 50  0000 L CNN
F 1 "Motor_DC" H 1408 2655 50  0000 L CNN
F 2 "" H 1250 2660 50  0001 C CNN
F 3 "~" H 1250 2660 50  0001 C CNN
	1    1250 2750
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M1
U 1 1 5D634050
P 1250 3500
F 0 "M1" H 1408 3496 50  0000 L CNN
F 1 "Motor_DC" H 1408 3405 50  0000 L CNN
F 2 "" H 1250 3410 50  0001 C CNN
F 3 "~" H 1250 3410 50  0001 C CNN
	1    1250 3500
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M2
U 1 1 5D634B5D
P 1200 6200
F 0 "M2" H 1358 6196 50  0000 L CNN
F 1 "Motor_DC" H 1358 6105 50  0000 L CNN
F 2 "" H 1200 6110 50  0001 C CNN
F 3 "~" H 1200 6110 50  0001 C CNN
	1    1200 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:Laserdiode_1A3C LD1
U 1 1 5D63589D
P 6950 4200
F 0 "LD1" V 6854 4348 50  0000 L CNN
F 1 "Laserdiode_1A3C" V 6945 4348 50  0000 L CNN
F 2 "" H 6850 4175 50  0001 C CNN
F 3 "~" H 6980 4000 50  0001 C CNN
	1    6950 4200
	0    1    1    0   
$EndComp
$Comp
L Motor:Motor_Servo M4
U 1 1 5D6375AE
P 6550 1650
F 0 "M4" H 6882 1715 50  0000 L CNN
F 1 "Motor_Servo" H 6882 1624 50  0000 L CNN
F 2 "" H 6550 1460 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 6550 1460 50  0001 C CNN
	1    6550 1650
	1    0    0    -1  
$EndComp
$Comp
L Flysky:X6B U1
U 1 1 5D63EE48
P 4350 1800
F 0 "U1" H 4400 1950 50  0001 C CNN
F 1 "X6B" H 4375 2425 50  0000 C CNN
F 2 "" H 4400 1950 50  0001 C CNN
F 3 "" H 4400 1950 50  0001 C CNN
	1    4350 1800
	1    0    0    -1  
$EndComp
Text GLabel 2450 1800 2    50   Input ~ 0
5VCC
Text GLabel 2450 4500 2    50   Input ~ 0
5VCC
Text GLabel 2500 3850 3    50   Input ~ 0
GND
Text GLabel 2500 6550 3    50   Input ~ 0
GND
Text GLabel 5650 3350 2    50   Input ~ 0
5VCC
Text GLabel 4900 1750 2    50   Input ~ 0
5VCC
Text GLabel 2400 3850 3    50   Input ~ 0
2S-3S
Text GLabel 2400 6550 3    50   Input ~ 0
2S-3S
Wire Wire Line
	4250 3650 4200 3650
Wire Wire Line
	4200 3650 4200 3550
Wire Wire Line
	4200 3550 4250 3550
Text GLabel 4200 3550 0    50   Input ~ 0
GND
Text GLabel 4900 1850 2    50   Input ~ 0
GND
$Comp
L Device:R R2
U 1 1 5D65196E
P 6950 3500
F 0 "R2" H 7020 3546 50  0000 L CNN
F 1 "R" H 7020 3455 50  0000 L CNN
F 2 "" V 6880 3500 50  0001 C CNN
F 3 "~" H 6950 3500 50  0001 C CNN
	1    6950 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5D6525B5
P 6950 4400
F 0 "#PWR0101" H 6950 4150 50  0001 C CNN
F 1 "GND" H 6955 4227 50  0000 C CNN
F 2 "" H 6950 4400 50  0001 C CNN
F 3 "" H 6950 4400 50  0001 C CNN
	1    6950 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2400 4000 2400
Wire Wire Line
	4000 2400 4000 3450
Wire Wire Line
	4000 3450 4250 3450
Wire Wire Line
	1200 6000 1900 6000
Wire Wire Line
	1900 6100 1900 6500
Wire Wire Line
	1900 6500 1200 6500
Wire Wire Line
	1250 3050 1900 3050
Wire Wire Line
	1250 3300 1900 3300
Wire Wire Line
	1250 3800 1900 3800
Wire Wire Line
	1900 3800 1900 3400
Wire Wire Line
	1250 2550 1900 2550
Wire Wire Line
	1900 2550 1900 2950
$Comp
L Dual~MC33926~Motor~Driver~Carrier:Dual_MC33926 Pololu1
U 1 1 5D62E442
P 2450 2850
F 0 "Pololu1" H 2087 4031 50  0000 C CNN
F 1 "Dual_MC33926" H 2087 3940 50  0000 C CNN
F 2 "" H 2400 2800 50  0001 C CNN
F 3 "" H 2400 2800 50  0001 C CNN
	1    2450 2850
	1    0    0    -1  
$EndComp
Text GLabel 6950 3350 1    50   Input ~ 0
5VCC
Wire Wire Line
	4250 3750 3700 3750
Wire Wire Line
	3700 3750 3700 2950
Wire Wire Line
	3700 2950 3100 2950
Wire Wire Line
	3700 3750 3700 5650
Wire Wire Line
	3700 5650 3100 5650
Connection ~ 3700 3750
Wire Wire Line
	4250 3950 3500 3950
Wire Wire Line
	3500 3950 3500 2500
Wire Wire Line
	3500 2500 3100 2500
Wire Wire Line
	4250 4050 3400 4050
Wire Wire Line
	3400 4050 3400 2600
Wire Wire Line
	3400 2600 3100 2600
Wire Wire Line
	4250 3850 3600 3850
Wire Wire Line
	3600 3850 3600 2400
Wire Wire Line
	3600 2400 3100 2400
Wire Wire Line
	3100 3400 3350 3400
Wire Wire Line
	3350 3400 3350 4150
Wire Wire Line
	3350 4150 4250 4150
Wire Wire Line
	4250 4250 3300 4250
Wire Wire Line
	3300 4250 3300 3500
Wire Wire Line
	3300 3500 3100 3500
Wire Wire Line
	4250 4350 3200 4350
Wire Wire Line
	3200 4350 3200 3600
Wire Wire Line
	3200 3600 3100 3600
Wire Wire Line
	3100 6100 4150 6100
Wire Wire Line
	4150 6100 4150 4450
Wire Wire Line
	4150 4450 4250 4450
Wire Wire Line
	5650 4450 5650 6200
Wire Wire Line
	5650 6200 3100 6200
Wire Wire Line
	5650 4350 5700 4350
Wire Wire Line
	5700 4350 5700 6300
Wire Wire Line
	5700 6300 3100 6300
Wire Wire Line
	5650 3750 6950 3750
Wire Wire Line
	6950 3650 6950 3750
Connection ~ 6950 3750
Wire Wire Line
	6950 3750 6950 3900
Text GLabel 4250 3350 1    50   Input ~ 0
SIGNAL
$EndSCHEMATC
