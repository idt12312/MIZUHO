EESchema Schematic File Version 2
LIBS:74xgxx
LIBS:74xx
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:Altera
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:battery_management
LIBS:bbd
LIBS:brooktre
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:conn
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:intel
LIBS:interface
LIBS:ir
LIBS:Lattice
LIBS:linear
LIBS:logo
LIBS:maxim
LIBS:memory
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:microchip
LIBS:microcontrollers
LIBS:motor_drivers
LIBS:motorola
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:opto
LIBS:Oscillators
LIBS:philips
LIBS:Power_Management
LIBS:power
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:transistors
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:Worldsemi
LIBS:Xicor
LIBS:xilinx
LIBS:Zilog
LIBS:micromouse2016
LIBS:micromouse2016-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 12
Title "MIZUHO"
Date "2016/12/10"
Rev "1"
Comp "@idt12312"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MPU6500 IC5
U 1 1 57E48D46
P 5950 3700
F 0 "IC5" H 5650 4550 60  0000 C CNN
F 1 "MPU6500" H 6050 4550 60  0000 C CNN
F 2 "micromouse2016:QFN-24-1EP_3x3mm_Pitch0.4mm_HandSolder" H 5600 3950 60  0001 C CNN
F 3 "" H 5600 3950 60  0001 C CNN
	1    5950 3700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR051
U 1 1 57E48E95
P 5450 2000
F 0 "#PWR051" H 5450 1850 50  0001 C CNN
F 1 "+3.3V" H 5450 2140 50  0000 C CNN
F 2 "" H 5450 2000 50  0000 C CNN
F 3 "" H 5450 2000 50  0000 C CNN
	1    5450 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR052
U 1 1 57E48EAF
P 5450 2200
F 0 "#PWR052" H 5450 1950 50  0001 C CNN
F 1 "GND" H 5450 2050 50  0000 C CNN
F 2 "" H 5450 2200 50  0000 C CNN
F 3 "" H 5450 2200 50  0000 C CNN
	1    5450 2200
	1    0    0    -1  
$EndComp
Text GLabel 5200 2000 0    60   Input ~ 0
3.3V
Text GLabel 5200 2200 0    60   Input ~ 0
GND
Text HLabel 6950 3050 2    60   Input ~ 0
IMU_CS
Text HLabel 6950 3150 2    60   Input ~ 0
IMU_SCLK
Text HLabel 6950 3250 2    60   Input ~ 0
IMU_MOSI
Text HLabel 6950 3350 2    60   Output ~ 0
IMU_MISO
Text HLabel 5100 3600 0    60   Output ~ 0
IMU_INT
$Comp
L GND #PWR053
U 1 1 57E48F37
P 4550 3900
F 0 "#PWR053" H 4550 3650 50  0001 C CNN
F 1 "GND" H 4550 3750 50  0000 C CNN
F 2 "" H 4550 3900 50  0000 C CNN
F 3 "" H 4550 3900 50  0000 C CNN
	1    4550 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2000 5450 2000
Wire Wire Line
	5200 2200 5450 2200
Wire Wire Line
	5100 3600 5350 3600
Wire Wire Line
	6550 3050 6950 3050
Wire Wire Line
	6950 3150 6550 3150
Wire Wire Line
	6550 3250 6950 3250
Wire Wire Line
	6950 3350 6550 3350
Wire Wire Line
	4550 3350 4550 3900
Wire Wire Line
	4550 3500 5350 3500
Wire Wire Line
	5350 3750 4550 3750
Connection ~ 4550 3750
Wire Wire Line
	5350 3350 4550 3350
Connection ~ 4550 3500
$Comp
L C C28
U 1 1 57E48F84
P 4300 3550
F 0 "C28" H 4325 3650 50  0000 L CNN
F 1 "0.1u" H 4325 3450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4338 3400 50  0001 C CNN
F 3 "" H 4300 3550 50  0000 C CNN
	1    4300 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3250 4300 3250
Wire Wire Line
	4300 3250 4300 3400
$Comp
L GND #PWR054
U 1 1 57E48FCB
P 4300 3900
F 0 "#PWR054" H 4300 3650 50  0001 C CNN
F 1 "GND" H 4300 3750 50  0000 C CNN
F 2 "" H 4300 3900 50  0000 C CNN
F 3 "" H 4300 3900 50  0000 C CNN
	1    4300 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3900 4300 3700
$Comp
L +3.3V #PWR055
U 1 1 57E48FF3
P 5000 2900
F 0 "#PWR055" H 5000 2750 50  0001 C CNN
F 1 "+3.3V" H 5000 3040 50  0000 C CNN
F 2 "" H 5000 2900 50  0000 C CNN
F 3 "" H 5000 2900 50  0000 C CNN
	1    5000 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2900 5000 3150
Wire Wire Line
	5000 3150 5350 3150
Wire Wire Line
	5350 3050 5000 3050
Connection ~ 5000 3050
$Comp
L C C29
U 1 1 57E4905B
P 6100 2050
F 0 "C29" H 6125 2150 50  0000 L CNN
F 1 "0.1u" H 6125 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6138 1900 50  0001 C CNN
F 3 "" H 6100 2050 50  0000 C CNN
	1    6100 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR056
U 1 1 57E49061
P 6100 2300
F 0 "#PWR056" H 6100 2050 50  0001 C CNN
F 1 "GND" H 6100 2150 50  0000 C CNN
F 2 "" H 6100 2300 50  0000 C CNN
F 3 "" H 6100 2300 50  0000 C CNN
	1    6100 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2300 6100 2200
$Comp
L C C30
U 1 1 57E4907B
P 6400 2050
F 0 "C30" H 6425 2150 50  0000 L CNN
F 1 "0.01u" H 6425 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6438 1900 50  0001 C CNN
F 3 "" H 6400 2050 50  0000 C CNN
	1    6400 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR057
U 1 1 57E49081
P 6400 2300
F 0 "#PWR057" H 6400 2050 50  0001 C CNN
F 1 "GND" H 6400 2150 50  0000 C CNN
F 2 "" H 6400 2300 50  0000 C CNN
F 3 "" H 6400 2300 50  0000 C CNN
	1    6400 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2300 6400 2200
$Comp
L +3.3V #PWR058
U 1 1 57E4909D
P 6100 1800
F 0 "#PWR058" H 6100 1650 50  0001 C CNN
F 1 "+3.3V" H 6100 1940 50  0000 C CNN
F 2 "" H 6100 1800 50  0000 C CNN
F 3 "" H 6100 1800 50  0000 C CNN
	1    6100 1800
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR059
U 1 1 57E490B9
P 6400 1800
F 0 "#PWR059" H 6400 1650 50  0001 C CNN
F 1 "+3.3V" H 6400 1940 50  0000 C CNN
F 2 "" H 6400 1800 50  0000 C CNN
F 3 "" H 6400 1800 50  0000 C CNN
	1    6400 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1800 6400 1900
Wire Wire Line
	6100 1800 6100 1900
NoConn ~ 6550 3500
NoConn ~ 6550 3600
$EndSCHEMATC
