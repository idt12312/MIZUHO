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
Sheet 13 13
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
L CONN_02X04 P7
U 1 1 57E4E37D
P 3800 3950
F 0 "P7" H 3800 4200 50  0000 C CNN
F 1 "DEBUG_PORT" H 3800 3700 50  0000 C CNN
F 2 "micromouse2016:Pin_Header_1.27mm_2x04" H 3800 2750 50  0001 C CNN
F 3 "" H 3800 2750 50  0000 C CNN
	1    3800 3950
	-1   0    0    -1  
$EndComp
$Comp
L CONN_02X05 P8
U 1 1 57E4E3A7
P 6050 4000
F 0 "P8" H 6050 4300 50  0000 C CNN
F 1 "ARM_10pin" H 6050 3700 50  0000 C CNN
F 2 "micromouse2016:Pin_Header_1.27mm_2x05" H 6050 2800 50  0001 C CNN
F 3 "" H 6050 2800 50  0000 C CNN
	1    6050 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3800 5500 3800
Wire Wire Line
	5800 3900 5600 3900
Wire Wire Line
	5800 4000 5600 4000
Wire Wire Line
	5450 4200 5800 4200
Wire Wire Line
	6300 3800 6600 3800
Wire Wire Line
	6300 3900 6600 3900
Wire Wire Line
	6300 4000 6600 4000
Wire Wire Line
	6300 4100 6600 4100
Wire Wire Line
	6300 4200 6600 4200
Text Label 6600 4200 0    60   ~ 0
nRESET
NoConn ~ 6600 4100
Text Label 6600 4000 0    60   ~ 0
SWO
Text Label 6600 3900 0    60   ~ 0
SWDCLK
Text Label 6600 3800 0    60   ~ 0
SWDIO
Text Label 5500 3800 2    60   ~ 0
Vref
Wire Wire Line
	5600 3900 5600 4200
Connection ~ 5600 4000
Connection ~ 5600 4200
Text Label 5450 4200 2    60   ~ 0
GND
Wire Wire Line
	3550 3800 3350 3800
Wire Wire Line
	3550 3900 3350 3900
Wire Wire Line
	3550 4000 3350 4000
Wire Wire Line
	3550 4100 3350 4100
Wire Wire Line
	4050 3800 4300 3800
Wire Wire Line
	4050 3900 4300 3900
Wire Wire Line
	4300 4000 4050 4000
Wire Wire Line
	4050 4100 4300 4100
$Comp
L CONN_01X03 P9
U 1 1 57E4E9DC
P 6100 4800
F 0 "P9" H 6100 5000 50  0000 C CNN
F 1 "DEBUG_UART" V 6200 4800 50  0000 C CNN
F 2 "" H 6100 4800 50  0001 C CNN
F 3 "" H 6100 4800 50  0000 C CNN
	1    6100 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4700 5700 4700
Wire Wire Line
	5900 4800 5700 4800
Wire Wire Line
	5900 4900 5700 4900
Text Label 3350 3800 2    60   ~ 0
Vref
Text Label 3350 3900 2    60   ~ 0
SWDIO
Text Label 3350 4000 2    60   ~ 0
SWO
Text Label 3350 4100 2    60   ~ 0
DEBUG_TX
Text Label 4300 3800 0    60   ~ 0
GND
Text Label 4300 3900 0    60   ~ 0
SWDCLK
Text Label 4300 4000 0    60   ~ 0
RESET
Text Label 4300 4100 0    60   ~ 0
DEBUG_RX
Text Label 5700 4700 2    60   ~ 0
GND
Text Label 5700 4800 2    60   ~ 0
DEBUG_TX
Text Label 5700 4900 2    60   ~ 0
DEBUG_RX
$EndSCHEMATC
