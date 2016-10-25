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
Sheet 11 12
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
L R R21
U 1 1 57DFA47C
P 6150 4550
AR Path="/57CD846F/57DFA47C" Ref="R21"  Part="1" 
AR Path="/57E50252/57DFA47C" Ref="R23"  Part="1" 
F 0 "R21" V 6230 4550 50  0000 C CNN
F 1 "10" V 6150 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6080 4550 50  0001 C CNN
F 3 "" H 6150 4550 50  0000 C CNN
	1    6150 4550
	0    -1   -1   0   
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 57DFA4DB
P 6750 4550
AR Path="/57CD846F/57DFA4DB" Ref="Q1"  Part="1" 
AR Path="/57E50252/57DFA4DB" Ref="Q2"  Part="1" 
F 0 "Q1" H 7050 4600 50  0000 R CNN
F 1 "IRFML8244" H 7400 4500 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 6950 4650 50  0001 C CNN
F 3 "" H 6750 4550 50  0000 C CNN
	1    6750 4550
	1    0    0    -1  
$EndComp
$Comp
L LED D12
U 1 1 57DFA512
P 6850 4050
AR Path="/57CD846F/57DFA512" Ref="D12"  Part="1" 
AR Path="/57E50252/57DFA512" Ref="D14"  Part="1" 
F 0 "D12" H 6850 4150 50  0000 C CNN
F 1 "VSLY5850" H 6850 3950 50  0000 C CNN
F 2 "micromouse2016:LED_5mm_angle" H 6850 4050 50  0001 C CNN
F 3 "" H 6850 4050 50  0000 C CNN
	1    6850 4050
	0    -1   -1   0   
$EndComp
$Comp
L LED D11
U 1 1 57DFA58F
P 6850 3550
AR Path="/57CD846F/57DFA58F" Ref="D11"  Part="1" 
AR Path="/57E50252/57DFA58F" Ref="D13"  Part="1" 
F 0 "D11" H 6850 3650 50  0000 C CNN
F 1 "VSLY5850" H 6850 3450 50  0000 C CNN
F 2 "micromouse2016:LED_5mm_angle" H 6850 3550 50  0001 C CNN
F 3 "" H 6850 3550 50  0000 C CNN
	1    6850 3550
	0    -1   -1   0   
$EndComp
$Comp
L R R22
U 1 1 57DFA5ED
P 6850 5100
AR Path="/57CD846F/57DFA5ED" Ref="R22"  Part="1" 
AR Path="/57E50252/57DFA5ED" Ref="R24"  Part="1" 
F 0 "R22" V 6930 5100 50  0000 C CNN
F 1 "33 1/4W" V 6850 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 6780 5100 50  0001 C CNN
F 3 "" H 6850 5100 50  0000 C CNN
	1    6850 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0116
U 1 1 57DFA62F
P 6850 5400
AR Path="/57CD846F/57DFA62F" Ref="#PWR0116"  Part="1" 
AR Path="/57E50252/57DFA62F" Ref="#PWR0122"  Part="1" 
F 0 "#PWR0116" H 6850 5150 50  0001 C CNN
F 1 "GND" H 6850 5250 50  0000 C CNN
F 2 "" H 6850 5400 50  0000 C CNN
F 3 "" H 6850 5400 50  0000 C CNN
	1    6850 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0117
U 1 1 57DFA771
P 5350 4950
AR Path="/57CD846F/57DFA771" Ref="#PWR0117"  Part="1" 
AR Path="/57E50252/57DFA771" Ref="#PWR0123"  Part="1" 
F 0 "#PWR0117" H 5350 4700 50  0001 C CNN
F 1 "GND" H 5350 4800 50  0000 C CNN
F 2 "" H 5350 4950 50  0000 C CNN
F 3 "" H 5350 4950 50  0000 C CNN
	1    5350 4950
	1    0    0    -1  
$EndComp
Text HLabel 4750 4450 0    60   Input ~ 0
IN
Text GLabel 5300 3100 0    60   Input ~ 0
BATT
Text GLabel 5300 3300 0    60   Input ~ 0
GND
$Comp
L +BATT #PWR0118
U 1 1 57DFB316
P 5350 4050
AR Path="/57CD846F/57DFB316" Ref="#PWR0118"  Part="1" 
AR Path="/57E50252/57DFB316" Ref="#PWR0124"  Part="1" 
F 0 "#PWR0118" H 5350 3900 50  0001 C CNN
F 1 "+BATT" H 5350 4190 50  0000 C CNN
F 2 "" H 5350 4050 50  0000 C CNN
F 3 "" H 5350 4050 50  0000 C CNN
	1    5350 4050
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR0119
U 1 1 57DFB388
P 6850 3150
AR Path="/57CD846F/57DFB388" Ref="#PWR0119"  Part="1" 
AR Path="/57E50252/57DFB388" Ref="#PWR0125"  Part="1" 
F 0 "#PWR0119" H 6850 3000 50  0001 C CNN
F 1 "+BATT" H 6850 3290 50  0000 C CNN
F 2 "" H 6850 3150 50  0000 C CNN
F 3 "" H 6850 3150 50  0000 C CNN
	1    6850 3150
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR0120
U 1 1 57DFB3E2
P 5400 3000
AR Path="/57CD846F/57DFB3E2" Ref="#PWR0120"  Part="1" 
AR Path="/57E50252/57DFB3E2" Ref="#PWR0126"  Part="1" 
F 0 "#PWR0120" H 5400 2850 50  0001 C CNN
F 1 "+BATT" H 5400 3140 50  0000 C CNN
F 2 "" H 5400 3000 50  0000 C CNN
F 3 "" H 5400 3000 50  0000 C CNN
	1    5400 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0121
U 1 1 57DFB42F
P 5400 3350
AR Path="/57CD846F/57DFB42F" Ref="#PWR0121"  Part="1" 
AR Path="/57E50252/57DFB42F" Ref="#PWR0127"  Part="1" 
F 0 "#PWR0121" H 5400 3100 50  0001 C CNN
F 1 "GND" H 5400 3200 50  0000 C CNN
F 2 "" H 5400 3350 50  0000 C CNN
F 3 "" H 5400 3350 50  0000 C CNN
	1    5400 3350
	1    0    0    -1  
$EndComp
$Comp
L NJM2746 IC9
U 1 1 57DFA459
P 5450 4550
AR Path="/57CD846F/57DFA459" Ref="IC9"  Part="1" 
AR Path="/57E50252/57DFA459" Ref="IC9"  Part="2" 
F 0 "IC9" H 5450 4750 50  0000 L CNN
F 1 "NJM2746" H 5450 4350 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-8_4.4x3mm_Pitch0.65mm" H 5450 4550 50  0001 C CNN
F 3 "" H 5450 4550 50  0000 C CNN
	1    5450 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 5400 6850 5250
Wire Wire Line
	6850 4750 6850 4950
Wire Wire Line
	6300 4550 6550 4550
Wire Wire Line
	6000 4550 5750 4550
Wire Wire Line
	6850 4250 6850 4350
Wire Wire Line
	6850 3750 6850 3850
Wire Wire Line
	5350 4950 5350 4850
Wire Wire Line
	6850 3150 6850 3350
Wire Wire Line
	5150 4650 5050 4650
Wire Wire Line
	5050 4650 5050 5250
Wire Wire Line
	5050 5250 6150 5250
Wire Wire Line
	6150 5250 6150 4850
Wire Wire Line
	6150 4850 6850 4850
Connection ~ 6850 4850
Wire Wire Line
	4750 4450 5150 4450
Wire Wire Line
	5300 3100 5400 3100
Wire Wire Line
	5400 3100 5400 3000
Wire Wire Line
	5400 3350 5400 3300
Wire Wire Line
	5400 3300 5300 3300
Wire Wire Line
	5350 4050 5350 4250
$EndSCHEMATC