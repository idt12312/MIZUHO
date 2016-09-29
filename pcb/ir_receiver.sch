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
Sheet 9 12
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
L DIODE D7
U 1 1 57DF9723
P 4500 4200
AR Path="/57E4FC7A/57DF9723" Ref="D7"  Part="1" 
AR Path="/57E4FEF0/57DF9723" Ref="D8"  Part="1" 
AR Path="/57CD844F/57DF9723" Ref="D9"  Part="1" 
AR Path="/57E4FEED/57DF9723" Ref="D10"  Part="1" 
F 0 "D7" H 4500 4350 50  0000 C CNN
F 1 "SFH213FA" H 4500 4025 50  0000 C CNN
F 2 "micromouse2016:LED_5mm_angle" H 4500 4200 50  0001 C CNN
F 3 "" H 4500 4200 50  0000 C CNN
	1    4500 4200
	0    1    1    0   
$EndComp
$Comp
L R R13
U 1 1 57DF9784
P 4500 4950
AR Path="/57E4FC7A/57DF9784" Ref="R13"  Part="1" 
AR Path="/57E4FEF0/57DF9784" Ref="R15"  Part="1" 
AR Path="/57CD844F/57DF9784" Ref="R17"  Part="1" 
AR Path="/57E4FEED/57DF9784" Ref="R19"  Part="1" 
F 0 "R13" V 4580 4950 50  0000 C CNN
F 1 "100k" V 4500 4950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4430 4950 50  0001 C CNN
F 3 "" H 4500 4950 50  0000 C CNN
	1    4500 4950
	1    0    0    -1  
$EndComp
$Comp
L C C37
U 1 1 57DF980D
P 5000 4600
AR Path="/57E4FC7A/57DF980D" Ref="C37"  Part="1" 
AR Path="/57E4FEF0/57DF980D" Ref="C39"  Part="1" 
AR Path="/57CD844F/57DF980D" Ref="C41"  Part="1" 
AR Path="/57E4FEED/57DF980D" Ref="C43"  Part="1" 
F 0 "C37" H 5025 4700 50  0000 L CNN
F 1 "0.01u" H 5025 4500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5038 4450 50  0001 C CNN
F 3 "" H 5000 4600 50  0000 C CNN
	1    5000 4600
	0    1    1    0   
$EndComp
$Comp
L AD8604 IC8
U 1 1 57DF986C
P 6000 4700
AR Path="/57E4FC7A/57DF986C" Ref="IC8"  Part="1" 
AR Path="/57E4FEF0/57DF986C" Ref="IC8"  Part="2" 
AR Path="/57CD844F/57DF986C" Ref="IC8"  Part="3" 
AR Path="/57E4FEED/57DF986C" Ref="IC8"  Part="4" 
F 0 "IC8" H 6000 4900 50  0000 L CNN
F 1 "AD8604" H 6000 4500 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 5950 4800 50  0001 C CNN
F 3 "" H 6050 4900 50  0000 C CNN
	1    6000 4700
	1    0    0    1   
$EndComp
$Comp
L R R14
U 1 1 57DF98CC
P 5950 3950
AR Path="/57E4FC7A/57DF98CC" Ref="R14"  Part="1" 
AR Path="/57E4FEF0/57DF98CC" Ref="R16"  Part="1" 
AR Path="/57CD844F/57DF98CC" Ref="R18"  Part="1" 
AR Path="/57E4FEED/57DF98CC" Ref="R20"  Part="1" 
F 0 "R14" V 6030 3950 50  0000 C CNN
F 1 "220k" V 5950 3950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5880 3950 50  0001 C CNN
F 3 "" H 5950 3950 50  0000 C CNN
	1    5950 3950
	0    1    1    0   
$EndComp
$Comp
L C C38
U 1 1 57DF990F
P 5950 3600
AR Path="/57E4FC7A/57DF990F" Ref="C38"  Part="1" 
AR Path="/57E4FEF0/57DF990F" Ref="C40"  Part="1" 
AR Path="/57CD844F/57DF990F" Ref="C42"  Part="1" 
AR Path="/57E4FEED/57DF990F" Ref="C44"  Part="1" 
F 0 "C38" H 5975 3700 50  0000 L CNN
F 1 "22p" H 5975 3500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5988 3450 50  0001 C CNN
F 3 "" H 5950 3600 50  0000 C CNN
	1    5950 3600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR092
U 1 1 57DF9947
P 4500 5300
AR Path="/57E4FC7A/57DF9947" Ref="#PWR092"  Part="1" 
AR Path="/57E4FEF0/57DF9947" Ref="#PWR098"  Part="1" 
AR Path="/57CD844F/57DF9947" Ref="#PWR0104"  Part="1" 
AR Path="/57E4FEED/57DF9947" Ref="#PWR0110"  Part="1" 
F 0 "#PWR092" H 4500 5050 50  0001 C CNN
F 1 "GND" H 4500 5150 50  0000 C CNN
F 2 "" H 4500 5300 50  0000 C CNN
F 3 "" H 4500 5300 50  0000 C CNN
	1    4500 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR093
U 1 1 57DF9973
P 6100 4250
AR Path="/57E4FC7A/57DF9973" Ref="#PWR093"  Part="1" 
AR Path="/57E4FEF0/57DF9973" Ref="#PWR099"  Part="1" 
AR Path="/57CD844F/57DF9973" Ref="#PWR0105"  Part="1" 
AR Path="/57E4FEED/57DF9973" Ref="#PWR0111"  Part="1" 
F 0 "#PWR093" H 6100 4000 50  0001 C CNN
F 1 "GND" H 6100 4100 50  0000 C CNN
F 2 "" H 6100 4250 50  0000 C CNN
F 3 "" H 6100 4250 50  0000 C CNN
	1    6100 4250
	1    0    0    -1  
$EndComp
$Comp
L +3.3VADC #PWR094
U 1 1 57DF999F
P 6100 5200
AR Path="/57E4FC7A/57DF999F" Ref="#PWR094"  Part="1" 
AR Path="/57E4FEF0/57DF999F" Ref="#PWR0100"  Part="1" 
AR Path="/57CD844F/57DF999F" Ref="#PWR0106"  Part="1" 
AR Path="/57E4FEED/57DF999F" Ref="#PWR0112"  Part="1" 
F 0 "#PWR094" H 6250 5150 50  0001 C CNN
F 1 "+3.3VADC" H 6100 5300 50  0000 C CNN
F 2 "" H 6100 5200 50  0000 C CNN
F 3 "" H 6100 5200 50  0000 C CNN
	1    6100 5200
	1    0    0    -1  
$EndComp
$Comp
L +3.3VADC #PWR095
U 1 1 57DF99CB
P 4500 3700
AR Path="/57E4FC7A/57DF99CB" Ref="#PWR095"  Part="1" 
AR Path="/57E4FEF0/57DF99CB" Ref="#PWR0101"  Part="1" 
AR Path="/57CD844F/57DF99CB" Ref="#PWR0107"  Part="1" 
AR Path="/57E4FEED/57DF99CB" Ref="#PWR0113"  Part="1" 
F 0 "#PWR095" H 4650 3650 50  0001 C CNN
F 1 "+3.3VADC" H 4500 3800 50  0000 C CNN
F 2 "" H 4500 3700 50  0000 C CNN
F 3 "" H 4500 3700 50  0000 C CNN
	1    4500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3700 4500 4000
Wire Wire Line
	4500 4400 4500 4800
Wire Wire Line
	4500 5100 4500 5300
Wire Wire Line
	4850 4600 4500 4600
Connection ~ 4500 4600
Wire Wire Line
	5150 4600 5700 4600
Wire Wire Line
	5800 3950 5450 3950
Wire Wire Line
	5450 3600 5450 4600
Connection ~ 5450 4600
Wire Wire Line
	5450 3600 5800 3600
Connection ~ 5450 3950
Text HLabel 7000 4700 2    60   Output ~ 0
SENSOR_OUT
Wire Wire Line
	6300 4700 7000 4700
Wire Wire Line
	6100 3950 6550 3950
Wire Wire Line
	6550 3600 6550 4700
Connection ~ 6550 4700
Wire Wire Line
	6100 3600 6550 3600
Connection ~ 6550 3950
Wire Wire Line
	6100 4250 5900 4250
Wire Wire Line
	5900 4250 5900 4400
Wire Wire Line
	5900 5000 5900 5300
Wire Wire Line
	5900 5300 6100 5300
Wire Wire Line
	6100 5300 6100 5200
Wire Wire Line
	5050 5100 5700 5100
Wire Wire Line
	5700 5100 5700 4800
Text GLabel 4600 2300 0    60   Input ~ 0
3.3V_AN
Text GLabel 4550 2700 0    60   Input ~ 0
GND
Text GLabel 4600 2500 0    60   Input ~ 0
1.65V_REF
Text Label 5050 5100 0    60   ~ 0
1.65V_REF
$Comp
L +3.3VADC #PWR096
U 1 1 57DF9DAB
P 4700 2200
AR Path="/57E4FC7A/57DF9DAB" Ref="#PWR096"  Part="1" 
AR Path="/57E4FEF0/57DF9DAB" Ref="#PWR0102"  Part="1" 
AR Path="/57CD844F/57DF9DAB" Ref="#PWR0108"  Part="1" 
AR Path="/57E4FEED/57DF9DAB" Ref="#PWR0114"  Part="1" 
F 0 "#PWR096" H 4850 2150 50  0001 C CNN
F 1 "+3.3VADC" H 4700 2300 50  0000 C CNN
F 2 "" H 4700 2200 50  0000 C CNN
F 3 "" H 4700 2200 50  0000 C CNN
	1    4700 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2300 4700 2300
Wire Wire Line
	4700 2300 4700 2200
$Comp
L GND #PWR097
U 1 1 57DF9E1D
P 4700 2800
AR Path="/57E4FC7A/57DF9E1D" Ref="#PWR097"  Part="1" 
AR Path="/57E4FEF0/57DF9E1D" Ref="#PWR0103"  Part="1" 
AR Path="/57CD844F/57DF9E1D" Ref="#PWR0109"  Part="1" 
AR Path="/57E4FEED/57DF9E1D" Ref="#PWR0115"  Part="1" 
F 0 "#PWR097" H 4700 2550 50  0001 C CNN
F 1 "GND" H 4700 2650 50  0000 C CNN
F 2 "" H 4700 2800 50  0000 C CNN
F 3 "" H 4700 2800 50  0000 C CNN
	1    4700 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2800 4700 2700
Wire Wire Line
	4700 2700 4550 2700
Wire Wire Line
	4600 2500 5200 2500
Text Label 5200 2500 2    60   ~ 0
1.65V_REF
$EndSCHEMATC
