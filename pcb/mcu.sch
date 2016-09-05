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
$Descr A3 16535 11693
encoding utf-8
Sheet 3 12
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L STM32F405RGTx IC4
U 1 1 57CD8D8B
P 8150 5900
F 0 "IC4" H 3950 7825 50  0000 L BNN
F 1 "STM32F405RGTx" H 12350 7825 50  0000 R BNN
F 2 "Housings_QFP:LQFP-64_10x10mm_Pitch0.5mm" H 12350 7775 50  0000 R TNN
F 3 "" H 8150 5900 50  0000 C CNN
	1    8150 5900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR025
U 1 1 57E018DA
P 7950 3550
F 0 "#PWR025" H 7950 3400 50  0001 C CNN
F 1 "+3.3V" H 7950 3690 50  0000 C CNN
F 2 "" H 7950 3550 50  0000 C CNN
F 3 "" H 7950 3550 50  0000 C CNN
	1    7950 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 57E01928
P 8050 8150
F 0 "#PWR026" H 8050 7900 50  0001 C CNN
F 1 "GND" H 8050 8000 50  0000 C CNN
F 2 "" H 8050 8150 50  0000 C CNN
F 3 "" H 8050 8150 50  0000 C CNN
	1    8050 8150
	1    0    0    -1  
$EndComp
Text GLabel 4500 2150 0    60   Input ~ 0
3.3V
Text GLabel 4500 2300 0    60   Input ~ 0
3.3V_AN
Text GLabel 4500 2450 0    60   Input ~ 0
GND
$Comp
L GND #PWR027
U 1 1 57E01C99
P 4600 2550
F 0 "#PWR027" H 4600 2300 50  0001 C CNN
F 1 "GND" H 4600 2400 50  0000 C CNN
F 2 "" H 4600 2550 50  0000 C CNN
F 3 "" H 4600 2550 50  0000 C CNN
	1    4600 2550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR028
U 1 1 57E01CC9
P 4650 2100
F 0 "#PWR028" H 4650 1950 50  0001 C CNN
F 1 "+3.3V" H 4650 2240 50  0000 C CNN
F 2 "" H 4650 2100 50  0000 C CNN
F 3 "" H 4650 2100 50  0000 C CNN
	1    4650 2100
	1    0    0    -1  
$EndComp
$Comp
L +3.3VADC #PWR029
U 1 1 57E01CF6
P 4850 2250
F 0 "#PWR029" H 5000 2200 50  0001 C CNN
F 1 "+3.3VADC" H 4850 2350 50  0000 C CNN
F 2 "" H 4850 2250 50  0000 C CNN
F 3 "" H 4850 2250 50  0000 C CNN
	1    4850 2250
	1    0    0    -1  
$EndComp
$Comp
L C C21
U 1 1 57E01EDC
P 7050 2350
F 0 "C21" H 7075 2450 50  0000 L CNN
F 1 "0.1u" H 7075 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7088 2200 50  0001 C CNN
F 3 "" H 7050 2350 50  0000 C CNN
	1    7050 2350
	1    0    0    -1  
$EndComp
$Comp
L C C22
U 1 1 57E01EFD
P 7300 2350
F 0 "C22" H 7325 2450 50  0000 L CNN
F 1 "0.1u" H 7325 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7338 2200 50  0001 C CNN
F 3 "" H 7300 2350 50  0000 C CNN
	1    7300 2350
	1    0    0    -1  
$EndComp
$Comp
L C C23
U 1 1 57E01F1F
P 7550 2350
F 0 "C23" H 7575 2450 50  0000 L CNN
F 1 "0.1u" H 7575 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7588 2200 50  0001 C CNN
F 3 "" H 7550 2350 50  0000 C CNN
	1    7550 2350
	1    0    0    -1  
$EndComp
$Comp
L C C24
U 1 1 57E01F46
P 7800 2350
F 0 "C24" H 7825 2450 50  0000 L CNN
F 1 "0.1u" H 7825 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7838 2200 50  0001 C CNN
F 3 "" H 7800 2350 50  0000 C CNN
	1    7800 2350
	1    0    0    -1  
$EndComp
$Comp
L C C27
U 1 1 57E01F70
P 8900 2350
F 0 "C27" H 8925 2450 50  0000 L CNN
F 1 "0.1u" H 8925 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8938 2200 50  0001 C CNN
F 3 "" H 8900 2350 50  0000 C CNN
	1    8900 2350
	1    0    0    -1  
$EndComp
$Comp
L C C26
U 1 1 57E01FA0
P 8600 2350
F 0 "C26" H 8625 2450 50  0000 L CNN
F 1 "1u" H 8625 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8638 2200 50  0001 C CNN
F 3 "" H 8600 2350 50  0000 C CNN
	1    8600 2350
	1    0    0    -1  
$EndComp
$Comp
L C C25
U 1 1 57E01FD2
P 8050 2350
F 0 "C25" H 8075 2450 50  0000 L CNN
F 1 "4.7u" H 8075 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8088 2200 50  0001 C CNN
F 3 "" H 8050 2350 50  0000 C CNN
	1    8050 2350
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR030
U 1 1 57E0201F
P 7050 2050
F 0 "#PWR030" H 7050 1900 50  0001 C CNN
F 1 "+3.3V" H 7050 2190 50  0000 C CNN
F 2 "" H 7050 2050 50  0000 C CNN
F 3 "" H 7050 2050 50  0000 C CNN
	1    7050 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 57E02072
P 7050 2700
F 0 "#PWR031" H 7050 2450 50  0001 C CNN
F 1 "GND" H 7050 2550 50  0000 C CNN
F 2 "" H 7050 2700 50  0000 C CNN
F 3 "" H 7050 2700 50  0000 C CNN
	1    7050 2700
	1    0    0    -1  
$EndComp
$Comp
L C C18
U 1 1 57E0287B
P 2700 5050
F 0 "C18" H 2725 5150 50  0000 L CNN
F 1 "2.2u" H 2725 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2738 4900 50  0001 C CNN
F 3 "" H 2700 5050 50  0000 C CNN
	1    2700 5050
	1    0    0    -1  
$EndComp
$Comp
L C C19
U 1 1 57E028CB
P 2950 5050
F 0 "C19" H 2975 5150 50  0000 L CNN
F 1 "2.2u" H 2975 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2988 4900 50  0001 C CNN
F 3 "" H 2950 5050 50  0000 C CNN
	1    2950 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 57E02908
P 2700 5250
F 0 "#PWR032" H 2700 5000 50  0001 C CNN
F 1 "GND" H 2700 5100 50  0000 C CNN
F 2 "" H 2700 5250 50  0000 C CNN
F 3 "" H 2700 5250 50  0000 C CNN
	1    2700 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 57E02EA6
P 3250 4600
F 0 "#PWR033" H 3250 4350 50  0001 C CNN
F 1 "GND" H 3250 4450 50  0000 C CNN
F 2 "" H 3250 4600 50  0000 C CNN
F 3 "" H 3250 4600 50  0000 C CNN
	1    3250 4600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR034
U 1 1 57E02F28
P 3650 4700
F 0 "#PWR034" H 3650 4550 50  0001 C CNN
F 1 "+3.3V" H 3650 4840 50  0000 C CNN
F 2 "" H 3650 4700 50  0000 C CNN
F 3 "" H 3650 4700 50  0000 C CNN
	1    3650 4700
	1    0    0    -1  
$EndComp
$Comp
L +3.3VADC #PWR035
U 1 1 57E04297
P 8350 3500
F 0 "#PWR035" H 8500 3450 50  0001 C CNN
F 1 "+3.3VADC" H 8350 3600 50  0000 C CNN
F 2 "" H 8350 3500 50  0000 C CNN
F 3 "" H 8350 3500 50  0000 C CNN
	1    8350 3500
	1    0    0    -1  
$EndComp
Text HLabel 12600 5300 2    60   Output ~ 0
MOTOR11
Text HLabel 12600 5400 2    60   Output ~ 0
MOTOR12
Text HLabel 12600 5100 2    60   Output ~ 0
MOTOR21
Text HLabel 12600 5200 2    60   Output ~ 0
MOTOR22
Text HLabel 12650 6600 2    60   Input ~ 0
ENC1A
Text HLabel 12650 6700 2    60   Input ~ 0
ENC1B
Text HLabel 12900 4700 2    60   Output ~ 0
IR_LED1
Text HLabel 12900 4800 2    60   Output ~ 0
IR_LED2
Text HLabel 12650 6100 2    60   Input ~ 0
IR_SENSOR3
Text HLabel 3650 6500 0    60   Input ~ 0
IR_SENSOR4
Text HLabel 3650 6400 0    60   Input ~ 0
IR_SENSOR1
Text HLabel 12650 6000 2    60   Input ~ 0
IR_SENSOR2
Text HLabel 3650 6600 0    60   Input ~ 0
ENC2A
Text HLabel 3650 6700 0    60   Input ~ 0
ENC2B
Text HLabel 3650 7200 0    60   Output ~ 0
IMU_CS
Text HLabel 12650 6300 2    60   Output ~ 0
IMU_SCK
Text HLabel 12650 6400 2    60   Input ~ 0
IMU_MISO
Text HLabel 12650 6500 2    60   Output ~ 0
IMU_MOSI
Text Label 12800 5600 0    60   ~ 0
SWDIO
Text Label 12800 5700 0    60   ~ 0
SWDCLK
Text Label 3600 4300 2    60   ~ 0
RESET
$Comp
L GND #PWR036
U 1 1 57E43983
P 14150 6000
F 0 "#PWR036" H 14150 5750 50  0001 C CNN
F 1 "GND" H 14150 5850 50  0000 C CNN
F 2 "" H 14150 6000 50  0000 C CNN
F 3 "" H 14150 6000 50  0000 C CNN
	1    14150 6000
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 57E447F9
P 4150 9200
F 0 "D2" H 4150 9300 50  0000 C CNN
F 1 "GREEN1" H 4150 9100 50  0000 C CNN
F 2 "LEDs:LED_0805" H 4150 9200 50  0001 C CNN
F 3 "" H 4150 9200 50  0000 C CNN
	1    4150 9200
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 57E448F9
P 4150 9650
F 0 "R6" V 4230 9650 50  0000 C CNN
F 1 "180" V 4150 9650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4080 9650 50  0001 C CNN
F 3 "" H 4150 9650 50  0000 C CNN
	1    4150 9650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR037
U 1 1 57E44B50
P 4150 9900
F 0 "#PWR037" H 4150 9650 50  0001 C CNN
F 1 "GND" H 4150 9750 50  0000 C CNN
F 2 "" H 4150 9900 50  0000 C CNN
F 3 "" H 4150 9900 50  0000 C CNN
	1    4150 9900
	1    0    0    -1  
$EndComp
$Comp
L LED D3
U 1 1 57E44FFB
P 4450 9200
F 0 "D3" H 4450 9300 50  0000 C CNN
F 1 "GREEN2" H 4450 9100 50  0000 C CNN
F 2 "LEDs:LED_0805" H 4450 9200 50  0001 C CNN
F 3 "" H 4450 9200 50  0000 C CNN
	1    4450 9200
	0    -1   -1   0   
$EndComp
$Comp
L R R7
U 1 1 57E45001
P 4450 9650
F 0 "R7" V 4530 9650 50  0000 C CNN
F 1 "180" V 4450 9650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4380 9650 50  0001 C CNN
F 3 "" H 4450 9650 50  0000 C CNN
	1    4450 9650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR038
U 1 1 57E45007
P 4450 9900
F 0 "#PWR038" H 4450 9650 50  0001 C CNN
F 1 "GND" H 4450 9750 50  0000 C CNN
F 2 "" H 4450 9900 50  0000 C CNN
F 3 "" H 4450 9900 50  0000 C CNN
	1    4450 9900
	1    0    0    -1  
$EndComp
$Comp
L LED D4
U 1 1 57E4508B
P 4750 9200
F 0 "D4" H 4750 9300 50  0000 C CNN
F 1 "GREEN3" H 4750 9100 50  0000 C CNN
F 2 "LEDs:LED_0805" H 4750 9200 50  0001 C CNN
F 3 "" H 4750 9200 50  0000 C CNN
	1    4750 9200
	0    -1   -1   0   
$EndComp
$Comp
L R R8
U 1 1 57E45091
P 4750 9650
F 0 "R8" V 4830 9650 50  0000 C CNN
F 1 "180" V 4750 9650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4680 9650 50  0001 C CNN
F 3 "" H 4750 9650 50  0000 C CNN
	1    4750 9650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR039
U 1 1 57E45097
P 4750 9900
F 0 "#PWR039" H 4750 9650 50  0001 C CNN
F 1 "GND" H 4750 9750 50  0000 C CNN
F 2 "" H 4750 9900 50  0000 C CNN
F 3 "" H 4750 9900 50  0000 C CNN
	1    4750 9900
	1    0    0    -1  
$EndComp
$Comp
L LED D5
U 1 1 57E4509F
P 5050 9200
F 0 "D5" H 5050 9300 50  0000 C CNN
F 1 "GREEN4" H 5050 9100 50  0000 C CNN
F 2 "LEDs:LED_0805" H 5050 9200 50  0001 C CNN
F 3 "" H 5050 9200 50  0000 C CNN
	1    5050 9200
	0    -1   -1   0   
$EndComp
$Comp
L R R9
U 1 1 57E450A5
P 5050 9650
F 0 "R9" V 5130 9650 50  0000 C CNN
F 1 "180" V 5050 9650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4980 9650 50  0001 C CNN
F 3 "" H 5050 9650 50  0000 C CNN
	1    5050 9650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR040
U 1 1 57E450AB
P 5050 9900
F 0 "#PWR040" H 5050 9650 50  0001 C CNN
F 1 "GND" H 5050 9750 50  0000 C CNN
F 2 "" H 5050 9900 50  0000 C CNN
F 3 "" H 5050 9900 50  0000 C CNN
	1    5050 9900
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 57E45EBF
P 5700 9350
F 0 "SW1" H 5850 9460 50  0000 C CNN
F 1 "SW_PUSH" H 5700 9270 50  0000 C CNN
F 2 "micromouse2016:SKRPACE010" H 5700 9350 50  0001 C CNN
F 3 "" H 5700 9350 50  0000 C CNN
	1    5700 9350
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW2
U 1 1 57E45F80
P 6000 9350
F 0 "SW2" H 6150 9460 50  0000 C CNN
F 1 "SW_PUSH" H 6000 9270 50  0000 C CNN
F 2 "micromouse2016:SKRPACE010" H 6000 9350 50  0001 C CNN
F 3 "" H 6000 9350 50  0000 C CNN
	1    6000 9350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR041
U 1 1 57E46008
P 5700 9750
F 0 "#PWR041" H 5700 9500 50  0001 C CNN
F 1 "GND" H 5700 9600 50  0000 C CNN
F 2 "" H 5700 9750 50  0000 C CNN
F 3 "" H 5700 9750 50  0000 C CNN
	1    5700 9750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR042
U 1 1 57E46071
P 6000 9750
F 0 "#PWR042" H 6000 9500 50  0001 C CNN
F 1 "GND" H 6000 9600 50  0000 C CNN
F 2 "" H 6000 9750 50  0000 C CNN
F 3 "" H 6000 9750 50  0000 C CNN
	1    6000 9750
	1    0    0    -1  
$EndComp
$Comp
L LED D6
U 1 1 57E46307
P 5350 9200
F 0 "D6" H 5350 9300 50  0000 C CNN
F 1 "RED" H 5350 9100 50  0000 C CNN
F 2 "LEDs:LED_0805" H 5350 9200 50  0001 C CNN
F 3 "" H 5350 9200 50  0000 C CNN
	1    5350 9200
	0    -1   -1   0   
$EndComp
$Comp
L R R10
U 1 1 57E4630D
P 5350 9650
F 0 "R10" V 5430 9650 50  0000 C CNN
F 1 "220" V 5350 9650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5280 9650 50  0001 C CNN
F 3 "" H 5350 9650 50  0000 C CNN
	1    5350 9650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR043
U 1 1 57E46313
P 5350 9900
F 0 "#PWR043" H 5350 9650 50  0001 C CNN
F 1 "GND" H 5350 9750 50  0000 C CNN
F 2 "" H 5350 9900 50  0000 C CNN
F 3 "" H 5350 9900 50  0000 C CNN
	1    5350 9900
	1    0    0    -1  
$EndComp
Wire Wire Line
	12450 5200 12600 5200
Wire Wire Line
	12600 5100 12450 5100
Wire Wire Line
	7950 3550 7950 3900
Wire Wire Line
	7950 3800 8250 3800
Wire Wire Line
	8250 3800 8250 3900
Connection ~ 7950 3800
Wire Wire Line
	8150 3800 8150 3900
Connection ~ 8150 3800
Wire Wire Line
	8050 3900 8050 3800
Connection ~ 8050 3800
Connection ~ 8250 3800
Wire Wire Line
	8050 7900 8050 8150
Wire Wire Line
	8250 8050 8250 7900
Wire Wire Line
	8050 8050 8250 8050
Connection ~ 8050 8050
Wire Wire Line
	8150 7900 8150 8050
Connection ~ 8150 8050
Wire Wire Line
	4500 2450 4600 2450
Wire Wire Line
	4600 2450 4600 2550
Wire Wire Line
	4500 2150 4650 2150
Wire Wire Line
	4650 2150 4650 2100
Wire Wire Line
	4500 2300 4850 2300
Wire Wire Line
	4850 2300 4850 2250
Wire Wire Line
	7050 2500 7050 2700
Connection ~ 7050 2600
Wire Wire Line
	8050 2600 8050 2500
Wire Wire Line
	7800 2600 7800 2500
Connection ~ 7800 2600
Wire Wire Line
	7550 2600 7550 2500
Connection ~ 7550 2600
Wire Wire Line
	7300 2500 7300 2600
Connection ~ 7300 2600
Wire Wire Line
	7050 2050 7050 2200
Connection ~ 7050 2150
Wire Wire Line
	8050 2150 8050 2200
Wire Wire Line
	7800 2150 7800 2200
Connection ~ 7800 2150
Wire Wire Line
	7550 2150 7550 2200
Connection ~ 7550 2150
Wire Wire Line
	7300 2200 7300 2150
Connection ~ 7300 2150
Wire Wire Line
	2700 5200 2700 5250
Wire Wire Line
	2700 5250 2950 5250
Wire Wire Line
	2950 5250 2950 5200
Connection ~ 2700 5250
Wire Wire Line
	2700 4800 3850 4800
Wire Wire Line
	2700 4800 2700 4900
Wire Wire Line
	2950 4900 3850 4900
Wire Wire Line
	3850 4500 3250 4500
Wire Wire Line
	3250 4500 3250 4600
Wire Wire Line
	3650 4700 3850 4700
Wire Wire Line
	8350 3500 8350 3900
Wire Wire Line
	7050 2600 8050 2600
Wire Wire Line
	7050 2150 8050 2150
Wire Wire Line
	8600 2000 8600 2200
Wire Wire Line
	8600 2150 8900 2150
Wire Wire Line
	8900 2150 8900 2200
Connection ~ 8600 2150
Wire Wire Line
	8900 2500 8900 2550
Wire Wire Line
	8900 2550 8600 2550
Wire Wire Line
	8600 2500 8600 2650
Connection ~ 8600 2550
Wire Wire Line
	3650 6400 3850 6400
Wire Wire Line
	12450 6000 12650 6000
Wire Wire Line
	12650 6100 12450 6100
Wire Wire Line
	3850 6500 3650 6500
Wire Wire Line
	12650 6600 12450 6600
Wire Wire Line
	12450 6700 12650 6700
Wire Wire Line
	3650 6600 3850 6600
Wire Wire Line
	3850 6700 3650 6700
Wire Wire Line
	12600 5300 12450 5300
Wire Wire Line
	12450 5400 12600 5400
Wire Wire Line
	12450 4700 12900 4700
Wire Wire Line
	12450 4900 12600 4900
Wire Wire Line
	12600 4900 12600 4700
Wire Wire Line
	12450 4800 12900 4800
Wire Wire Line
	12450 5000 12750 5000
Wire Wire Line
	12750 5000 12750 4800
Wire Wire Line
	3650 7200 3850 7200
Wire Wire Line
	12450 6300 12650 6300
Wire Wire Line
	12650 6400 12450 6400
Wire Wire Line
	12450 6500 12650 6500
Wire Wire Line
	3850 7000 2900 7000
Wire Wire Line
	3850 7100 2900 7100
Wire Wire Line
	12450 5600 12800 5600
Wire Wire Line
	12450 5700 12800 5700
Wire Wire Line
	3850 4300 3600 4300
Wire Wire Line
	4150 9900 4150 9800
Wire Wire Line
	4150 9500 4150 9400
Wire Wire Line
	4450 9900 4450 9800
Wire Wire Line
	4450 9500 4450 9400
Wire Wire Line
	4750 9900 4750 9800
Wire Wire Line
	4750 9500 4750 9400
Wire Wire Line
	5050 9900 5050 9800
Wire Wire Line
	5050 9500 5050 9400
Wire Wire Line
	6000 9750 6000 9650
Wire Wire Line
	5700 9650 5700 9750
Wire Wire Line
	5350 9900 5350 9800
Wire Wire Line
	5350 9500 5350 9400
Text GLabel 5450 1900 0    60   Input ~ 0
BATT
$Comp
L R R11
U 1 1 57E47AE1
P 5600 2100
F 0 "R11" V 5680 2100 50  0000 C CNN
F 1 "22k" V 5600 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5530 2100 50  0001 C CNN
F 3 "" H 5600 2100 50  0000 C CNN
	1    5600 2100
	1    0    0    -1  
$EndComp
$Comp
L R R12
U 1 1 57E47B57
P 5600 2500
F 0 "R12" V 5680 2500 50  0000 C CNN
F 1 "10k" V 5600 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5530 2500 50  0001 C CNN
F 3 "" H 5600 2500 50  0000 C CNN
	1    5600 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR044
U 1 1 57E47BCE
P 5600 2750
F 0 "#PWR044" H 5600 2500 50  0001 C CNN
F 1 "GND" H 5600 2600 50  0000 C CNN
F 2 "" H 5600 2750 50  0000 C CNN
F 3 "" H 5600 2750 50  0000 C CNN
	1    5600 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 1900 5600 1900
Wire Wire Line
	5600 1900 5600 1950
Wire Wire Line
	5600 2250 5600 2350
Wire Wire Line
	5600 2650 5600 2750
Wire Wire Line
	5600 2300 6350 2300
Connection ~ 5600 2300
Text Label 6350 2300 2    60   ~ 0
BATT_MONITOR
Wire Wire Line
	3850 6000 3650 6000
Text Label 3650 6000 2    60   ~ 0
BATT_MONITOR
Wire Wire Line
	3850 5800 3650 5800
Text HLabel 3650 5800 0    60   Input ~ 0
IMU_INT
$Comp
L C C20
U 1 1 57E59CB3
P 6000 2550
F 0 "C20" H 6025 2650 50  0000 L CNN
F 1 "1u" H 6025 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6038 2400 50  0001 C CNN
F 3 "" H 6000 2550 50  0000 C CNN
	1    6000 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR045
U 1 1 57E59D3E
P 6000 2750
F 0 "#PWR045" H 6000 2500 50  0001 C CNN
F 1 "GND" H 6000 2600 50  0000 C CNN
F 2 "" H 6000 2750 50  0000 C CNN
F 3 "" H 6000 2750 50  0000 C CNN
	1    6000 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2700 6000 2750
Wire Wire Line
	6000 2400 6000 2300
Connection ~ 6000 2300
NoConn ~ 3850 6800
NoConn ~ 3850 6200
NoConn ~ 3850 6300
NoConn ~ 3850 6100
NoConn ~ 12450 6800
$Comp
L CONN_01X03 P4
U 1 1 57E4FE49
P 2700 7000
F 0 "P4" H 2700 7200 50  0000 C CNN
F 1 "DEBUG_UART" V 2800 7000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 2700 7000 50  0001 C CNN
F 3 "" H 2700 7000 50  0000 C CNN
	1    2700 7000
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR046
U 1 1 57E50106
P 3000 7200
F 0 "#PWR046" H 3000 6950 50  0001 C CNN
F 1 "GND" H 3000 7050 50  0000 C CNN
F 2 "" H 3000 7200 50  0000 C CNN
F 3 "" H 3000 7200 50  0000 C CNN
	1    3000 7200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3000 7200 3000 6900
Wire Wire Line
	3000 6900 2900 6900
Wire Wire Line
	14050 5700 14250 5700
Wire Wire Line
	14050 5800 14250 5800
Wire Wire Line
	14050 5600 14250 5600
Text Label 14250 5600 0    60   ~ 0
RESET
Text Label 14250 5800 0    60   ~ 0
SWDCLK
Text Label 14250 5700 0    60   ~ 0
SWDIO
$Comp
L PWR_FLAG #FLG047
U 1 1 57E69DC2
P 2850 4800
F 0 "#FLG047" H 2850 4895 50  0001 C CNN
F 1 "PWR_FLAG" H 2850 4980 50  0000 C CNN
F 2 "" H 2850 4800 50  0000 C CNN
F 3 "" H 2850 4800 50  0000 C CNN
	1    2850 4800
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG048
U 1 1 57E69E3D
P 3050 4900
F 0 "#FLG048" H 3050 4995 50  0001 C CNN
F 1 "PWR_FLAG" H 3050 5080 50  0000 C CNN
F 2 "" H 3050 4900 50  0000 C CNN
F 3 "" H 3050 4900 50  0000 C CNN
	1    3050 4900
	1    0    0    -1  
$EndComp
Connection ~ 3050 4900
Connection ~ 2850 4800
Text Label 3600 7100 2    60   ~ 0
DEBUG_RX
Text Label 3600 7000 2    60   ~ 0
DEBUG_TX
$Comp
L GND #PWR049
U 1 1 57E04616
P 8600 2650
F 0 "#PWR049" H 8600 2400 50  0001 C CNN
F 1 "GND" H 8600 2500 50  0000 C CNN
F 2 "" H 8600 2650 50  0000 C CNN
F 3 "" H 8600 2650 50  0000 C CNN
	1    8600 2650
	1    0    0    -1  
$EndComp
$Comp
L +3.3VADC #PWR050
U 1 1 57E0464B
P 8600 2000
F 0 "#PWR050" H 8750 1950 50  0001 C CNN
F 1 "+3.3VADC" H 8600 2100 50  0000 C CNN
F 2 "" H 8600 2000 50  0000 C CNN
F 3 "" H 8600 2000 50  0000 C CNN
	1    8600 2000
	1    0    0    -1  
$EndComp
Connection ~ 12600 4700
Connection ~ 12750 4800
NoConn ~ 3850 5500
NoConn ~ 3850 5600
Wire Wire Line
	12450 4400 12750 4400
Wire Wire Line
	12450 4500 12750 4500
Text Label 12750 4400 0    60   ~ 0
LED3
Text Label 12750 4500 0    60   ~ 0
LED2
Wire Wire Line
	12450 6900 12650 6900
Text Label 12650 6900 0    60   ~ 0
LED4
Wire Wire Line
	12450 7200 12650 7200
Text Label 12650 7200 0    60   ~ 0
LED1
Wire Wire Line
	12450 5800 12800 5800
Text Label 12800 5800 0    60   ~ 0
SW2
Wire Wire Line
	12450 5500 12600 5500
Text Label 12600 5500 0    60   ~ 0
LED5
Wire Wire Line
	3850 6900 3700 6900
Text Label 3700 6900 2    60   ~ 0
SW1
Wire Wire Line
	4150 9000 4150 8550
Wire Wire Line
	4450 9000 4450 8550
Wire Wire Line
	4750 9000 4750 8550
Wire Wire Line
	5050 9000 5050 8550
Wire Wire Line
	5350 9000 5350 8550
Wire Wire Line
	5700 9050 5700 8550
Wire Wire Line
	6000 9050 6000 8550
Text Label 4150 8550 3    60   ~ 0
LED1
Text Label 4450 8550 3    60   ~ 0
LED2
Text Label 4750 8550 3    60   ~ 0
LED3
Text Label 5050 8550 3    60   ~ 0
LED4
Text Label 5350 8550 3    60   ~ 0
LED5
Text Label 5700 8550 3    60   ~ 0
SW1
Text Label 6000 8550 3    60   ~ 0
SW2
NoConn ~ 3850 7300
NoConn ~ 3850 7400
NoConn ~ 3850 7500
NoConn ~ 12450 4300
NoConn ~ 12450 4600
NoConn ~ 12450 6200
NoConn ~ 12450 7000
NoConn ~ 12450 7100
NoConn ~ 12450 7300
NoConn ~ 12450 7400
NoConn ~ 12450 7500
$Comp
L CONN_01X04 P5
U 1 1 57E8F1D6
P 13850 5750
F 0 "P5" H 13850 6000 50  0000 C CNN
F 1 "ARM_DEBUG" V 13950 5750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 13850 5750 50  0001 C CNN
F 3 "" H 13850 5750 50  0000 C CNN
	1    13850 5750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	14050 5900 14150 5900
Wire Wire Line
	14150 5900 14150 6000
$EndSCHEMATC
