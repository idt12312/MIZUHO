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
Sheet 6 12
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
L DRV8835 IC6
U 1 1 57DFB122
P 5700 4100
AR Path="/57CD848F/57DFB122" Ref="IC6"  Part="1" 
AR Path="/57E4ED65/57DFB122" Ref="IC7"  Part="1" 
F 0 "IC7" H 5350 4650 60  0000 C CNN
F 1 "DRV8835" H 5750 4650 60  0000 C CNN
F 2 "micromouse2016:DRV8835" H 5700 4100 60  0001 C CNN
F 3 "" H 5700 4100 60  0001 C CNN
	1    5700 4100
	1    0    0    -1  
$EndComp
Text GLabel 5300 2700 0    60   Input ~ 0
GND
Text GLabel 5300 2500 0    60   Input ~ 0
BATT
Text GLabel 5300 2300 0    60   Input ~ 0
3.3V
$Comp
L +3.3V #PWR060
U 1 1 57DFB5DC
P 5450 2250
AR Path="/57CD848F/57DFB5DC" Ref="#PWR060"  Part="1" 
AR Path="/57E4ED65/57DFB5DC" Ref="#PWR076"  Part="1" 
F 0 "#PWR076" H 5450 2100 50  0001 C CNN
F 1 "+3.3V" H 5450 2390 50  0000 C CNN
F 2 "" H 5450 2250 50  0000 C CNN
F 3 "" H 5450 2250 50  0000 C CNN
	1    5450 2250
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR061
U 1 1 57DFB5FF
P 5650 2400
AR Path="/57CD848F/57DFB5FF" Ref="#PWR061"  Part="1" 
AR Path="/57E4ED65/57DFB5FF" Ref="#PWR077"  Part="1" 
F 0 "#PWR077" H 5650 2250 50  0001 C CNN
F 1 "+BATT" H 5650 2540 50  0000 C CNN
F 2 "" H 5650 2400 50  0000 C CNN
F 3 "" H 5650 2400 50  0000 C CNN
	1    5650 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR062
U 1 1 57DFB624
P 5450 2750
AR Path="/57CD848F/57DFB624" Ref="#PWR062"  Part="1" 
AR Path="/57E4ED65/57DFB624" Ref="#PWR078"  Part="1" 
F 0 "#PWR078" H 5450 2500 50  0001 C CNN
F 1 "GND" H 5450 2600 50  0000 C CNN
F 2 "" H 5450 2750 50  0000 C CNN
F 3 "" H 5450 2750 50  0000 C CNN
	1    5450 2750
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR063
U 1 1 57DFB653
P 6500 3550
AR Path="/57CD848F/57DFB653" Ref="#PWR063"  Part="1" 
AR Path="/57E4ED65/57DFB653" Ref="#PWR079"  Part="1" 
F 0 "#PWR079" H 6500 3400 50  0001 C CNN
F 1 "+BATT" H 6500 3690 50  0000 C CNN
F 2 "" H 6500 3550 50  0000 C CNN
F 3 "" H 6500 3550 50  0000 C CNN
	1    6500 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR064
U 1 1 57DFB682
P 6500 4700
AR Path="/57CD848F/57DFB682" Ref="#PWR064"  Part="1" 
AR Path="/57E4ED65/57DFB682" Ref="#PWR080"  Part="1" 
F 0 "#PWR080" H 6500 4450 50  0001 C CNN
F 1 "GND" H 6500 4550 50  0000 C CNN
F 2 "" H 6500 4700 50  0000 C CNN
F 3 "" H 6500 4700 50  0000 C CNN
	1    6500 4700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR065
U 1 1 57DFB6C9
P 4950 3500
AR Path="/57CD848F/57DFB6C9" Ref="#PWR065"  Part="1" 
AR Path="/57E4ED65/57DFB6C9" Ref="#PWR081"  Part="1" 
F 0 "#PWR081" H 4950 3350 50  0001 C CNN
F 1 "+3.3V" H 4950 3640 50  0000 C CNN
F 2 "" H 4950 3500 50  0000 C CNN
F 3 "" H 4950 3500 50  0000 C CNN
	1    4950 3500
	1    0    0    -1  
$EndComp
Text HLabel 4600 3950 0    60   Input ~ 0
IN1_PHASE
Text HLabel 4600 4050 0    60   Input ~ 0
IN2/ENABLE
$Comp
L C C31
U 1 1 57DFB8D1
P 6450 2500
AR Path="/57CD848F/57DFB8D1" Ref="C31"  Part="1" 
AR Path="/57E4ED65/57DFB8D1" Ref="C34"  Part="1" 
F 0 "C34" H 6475 2600 50  0000 L CNN
F 1 "0.1u" H 6475 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6488 2350 50  0001 C CNN
F 3 "" H 6450 2500 50  0000 C CNN
	1    6450 2500
	1    0    0    -1  
$EndComp
$Comp
L C C32
U 1 1 57DFB925
P 6850 2500
AR Path="/57CD848F/57DFB925" Ref="C32"  Part="1" 
AR Path="/57E4ED65/57DFB925" Ref="C35"  Part="1" 
F 0 "C35" H 6875 2600 50  0000 L CNN
F 1 "0.1u" H 6875 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6888 2350 50  0001 C CNN
F 3 "" H 6850 2500 50  0000 C CNN
	1    6850 2500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR066
U 1 1 57DFB944
P 6450 2200
AR Path="/57CD848F/57DFB944" Ref="#PWR066"  Part="1" 
AR Path="/57E4ED65/57DFB944" Ref="#PWR082"  Part="1" 
F 0 "#PWR082" H 6450 2050 50  0001 C CNN
F 1 "+3.3V" H 6450 2340 50  0000 C CNN
F 2 "" H 6450 2200 50  0000 C CNN
F 3 "" H 6450 2200 50  0000 C CNN
	1    6450 2200
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR067
U 1 1 57DFB958
P 6850 2200
AR Path="/57CD848F/57DFB958" Ref="#PWR067"  Part="1" 
AR Path="/57E4ED65/57DFB958" Ref="#PWR083"  Part="1" 
F 0 "#PWR083" H 6850 2050 50  0001 C CNN
F 1 "+BATT" H 6850 2340 50  0000 C CNN
F 2 "" H 6850 2200 50  0000 C CNN
F 3 "" H 6850 2200 50  0000 C CNN
	1    6850 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR068
U 1 1 57DFB96C
P 6450 2800
AR Path="/57CD848F/57DFB96C" Ref="#PWR068"  Part="1" 
AR Path="/57E4ED65/57DFB96C" Ref="#PWR084"  Part="1" 
F 0 "#PWR084" H 6450 2550 50  0001 C CNN
F 1 "GND" H 6450 2650 50  0000 C CNN
F 2 "" H 6450 2800 50  0000 C CNN
F 3 "" H 6450 2800 50  0000 C CNN
	1    6450 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR069
U 1 1 57DFB980
P 6850 2800
AR Path="/57CD848F/57DFB980" Ref="#PWR069"  Part="1" 
AR Path="/57E4ED65/57DFB980" Ref="#PWR085"  Part="1" 
F 0 "#PWR085" H 6850 2550 50  0001 C CNN
F 1 "GND" H 6850 2650 50  0000 C CNN
F 2 "" H 6850 2800 50  0000 C CNN
F 3 "" H 6850 2800 50  0000 C CNN
	1    6850 2800
	1    0    0    -1  
$EndComp
Text GLabel 4350 2400 0    60   Input ~ 0
5V
$Comp
L +5V #PWR070
U 1 1 57DFD2A5
P 4500 2350
AR Path="/57CD848F/57DFD2A5" Ref="#PWR070"  Part="1" 
AR Path="/57E4ED65/57DFD2A5" Ref="#PWR086"  Part="1" 
F 0 "#PWR086" H 4500 2200 50  0001 C CNN
F 1 "+5V" H 4500 2490 50  0000 C CNN
F 2 "" H 4500 2350 50  0000 C CNN
F 3 "" H 4500 2350 50  0000 C CNN
	1    4500 2350
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X03 P6
U 1 1 57DFD438
P 7600 4150
AR Path="/57CD848F/57DFD438" Ref="P6"  Part="1" 
AR Path="/57E4ED65/57DFD438" Ref="P7"  Part="1" 
F 0 "P7" H 7600 4350 50  0000 C CNN
F 1 "MOTOR" H 7600 3950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03" H 7600 2950 50  0001 C CNN
F 3 "" H 7600 2950 50  0000 C CNN
	1    7600 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR071
U 1 1 57DFD529
P 7150 4250
AR Path="/57CD848F/57DFD529" Ref="#PWR071"  Part="1" 
AR Path="/57E4ED65/57DFD529" Ref="#PWR087"  Part="1" 
F 0 "#PWR087" H 7150 4000 50  0001 C CNN
F 1 "GND" H 7150 4100 50  0000 C CNN
F 2 "" H 7150 4250 50  0000 C CNN
F 3 "" H 7150 4250 50  0000 C CNN
	1    7150 4250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR072
U 1 1 57DFD59D
P 8100 4050
AR Path="/57CD848F/57DFD59D" Ref="#PWR072"  Part="1" 
AR Path="/57E4ED65/57DFD59D" Ref="#PWR088"  Part="1" 
F 0 "#PWR088" H 8100 3900 50  0001 C CNN
F 1 "+5V" H 8100 4190 50  0000 C CNN
F 2 "" H 8100 4050 50  0000 C CNN
F 3 "" H 8100 4050 50  0000 C CNN
	1    8100 4050
	1    0    0    -1  
$EndComp
Text HLabel 8450 4250 2    60   Output ~ 0
ENC_A
Text HLabel 8450 4450 2    60   Output ~ 0
ENC_B
$Comp
L GND #PWR073
U 1 1 57E36C53
P 4950 4700
AR Path="/57CD848F/57E36C53" Ref="#PWR073"  Part="1" 
AR Path="/57E4ED65/57E36C53" Ref="#PWR089"  Part="1" 
F 0 "#PWR089" H 4950 4450 50  0001 C CNN
F 1 "GND" H 4950 4550 50  0000 C CNN
F 2 "" H 4950 4700 50  0000 C CNN
F 3 "" H 4950 4700 50  0000 C CNN
	1    4950 4700
	1    0    0    -1  
$EndComp
$Comp
L C C33
U 1 1 57E4E293
P 7200 2500
AR Path="/57CD848F/57E4E293" Ref="C33"  Part="1" 
AR Path="/57E4ED65/57E4E293" Ref="C36"  Part="1" 
F 0 "C36" H 7225 2600 50  0000 L CNN
F 1 "47u" H 7225 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 7238 2350 50  0001 C CNN
F 3 "" H 7200 2500 50  0000 C CNN
	1    7200 2500
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR074
U 1 1 57E4E299
P 7200 2200
AR Path="/57CD848F/57E4E299" Ref="#PWR074"  Part="1" 
AR Path="/57E4ED65/57E4E299" Ref="#PWR090"  Part="1" 
F 0 "#PWR090" H 7200 2050 50  0001 C CNN
F 1 "+BATT" H 7200 2340 50  0000 C CNN
F 2 "" H 7200 2200 50  0000 C CNN
F 3 "" H 7200 2200 50  0000 C CNN
	1    7200 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR075
U 1 1 57E4E29F
P 7200 2800
AR Path="/57CD848F/57E4E29F" Ref="#PWR075"  Part="1" 
AR Path="/57E4ED65/57E4E29F" Ref="#PWR091"  Part="1" 
F 0 "#PWR091" H 7200 2550 50  0001 C CNN
F 1 "GND" H 7200 2650 50  0000 C CNN
F 2 "" H 7200 2800 50  0000 C CNN
F 3 "" H 7200 2800 50  0000 C CNN
	1    7200 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 4050 7350 4050
Wire Wire Line
	6400 3950 6900 3950
Wire Wire Line
	6400 4200 6550 4200
Wire Wire Line
	6550 4200 6550 3950
Connection ~ 6550 3950
Wire Wire Line
	6400 4300 6650 4300
Wire Wire Line
	6650 4300 6650 4050
Connection ~ 6650 4050
Wire Wire Line
	5450 2250 5450 2300
Wire Wire Line
	5450 2300 5300 2300
Wire Wire Line
	5650 2400 5650 2500
Wire Wire Line
	5650 2500 5300 2500
Wire Wire Line
	5300 2700 5450 2700
Wire Wire Line
	5450 2700 5450 2750
Wire Wire Line
	6500 3550 6500 3750
Wire Wire Line
	6500 3750 6400 3750
Wire Wire Line
	6500 4450 6500 4700
Wire Wire Line
	6500 4450 6400 4450
Wire Wire Line
	6400 4550 6500 4550
Connection ~ 6500 4550
Wire Wire Line
	5050 3800 4950 3800
Wire Wire Line
	4950 3800 4950 3500
Wire Wire Line
	4600 4050 5050 4050
Wire Wire Line
	4600 3950 5050 3950
Wire Wire Line
	5050 4200 4950 4200
Wire Wire Line
	5050 4300 4850 4300
Wire Wire Line
	6850 2650 6850 2800
Wire Wire Line
	6450 2650 6450 2800
Wire Wire Line
	6450 2200 6450 2350
Wire Wire Line
	6850 2350 6850 2200
Wire Wire Line
	4500 2350 4500 2400
Wire Wire Line
	4500 2400 4350 2400
Wire Wire Line
	7950 4050 7850 4050
Wire Wire Line
	7350 4150 7150 4150
Wire Wire Line
	7150 4150 7150 4250
Wire Wire Line
	8100 4050 8100 4150
Wire Wire Line
	8100 4150 7850 4150
Wire Wire Line
	7850 4250 8450 4250
Wire Wire Line
	8450 4450 7300 4450
Wire Wire Line
	7300 4450 7300 4250
Wire Wire Line
	5050 4450 4950 4450
Wire Wire Line
	4950 4450 4950 4700
Wire Wire Line
	7300 4250 7350 4250
Wire Wire Line
	7200 2650 7200 2800
Wire Wire Line
	7200 2350 7200 2200
Connection ~ 4950 3950
Wire Wire Line
	4950 4200 4950 3950
Wire Wire Line
	4850 4300 4850 4050
Connection ~ 4850 4050
Wire Wire Line
	6900 3950 6900 3850
Wire Wire Line
	7950 3850 7950 4050
Wire Wire Line
	6900 3850 7950 3850
$EndSCHEMATC
