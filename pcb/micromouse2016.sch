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
Sheet 1 12
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
S 2100 1000 1300 800 
U 57CD849A
F0 "power" 60
F1 "power.sch" 60
$EndSheet
$Sheet
S 4200 2400 2300 2700
U 57CD84A5
F0 "mcu" 60
F1 "mcu.sch" 60
F2 "MOTOR11" O L 4200 3500 60 
F3 "MOTOR12" O L 4200 3600 60 
F4 "MOTOR21" O L 4200 4350 60 
F5 "MOTOR22" O L 4200 4450 60 
F6 "ENC1A" I L 4200 3950 60 
F7 "ENC1B" I L 4200 4050 60 
F8 "IR_LED1" O R 6500 4500 60 
F9 "IR_SENSOR3" I R 6500 3500 60 
F10 "IR_SENSOR4" I R 6500 3850 60 
F11 "IR_SENSOR1" I R 6500 2800 60 
F12 "IR_SENSOR2" I R 6500 3150 60 
F13 "ENC2A" I L 4200 4750 60 
F14 "ENC2B" I L 4200 4850 60 
F15 "IMU_CS" O L 4200 2700 60 
F16 "IMU_SCK" O L 4200 2800 60 
F17 "IMU_MISO" I L 4200 3000 60 
F18 "IMU_MOSI" O L 4200 2900 60 
F19 "IMU_INT" I L 4200 3100 60 
F20 "IR_LED2" O R 6500 4800 60 
$EndSheet
$Sheet
S 2300 2600 1100 600 
U 57E48CF3
F0 "imu" 60
F1 "imu.sch" 60
F2 "IMU_CS" I R 3400 2700 60 
F3 "IMU_SCLK" I R 3400 2800 60 
F4 "IMU_MOSI" I R 3400 2900 60 
F5 "IMU_MISO" O R 3400 3000 60 
F6 "IMU_INT" O R 3400 3100 60 
$EndSheet
$Sheet
S 2300 3800 1100 400 
U 57CD848F
F0 "motor1" 60
F1 "motor.sch" 60
F2 "IN1_PHASE" I L 2300 3950 60 
F3 "IN2/ENABLE" I L 2300 4050 60 
F4 "ENC_A" O R 3400 3950 60 
F5 "ENC_B" O R 3400 4050 60 
$EndSheet
$Sheet
S 2300 4600 1100 400 
U 57E4ED65
F0 "motor2" 60
F1 "motor.sch" 60
F2 "IN1_PHASE" I L 2300 4750 60 
F3 "IN2/ENABLE" I L 2300 4850 60 
F4 "ENC_A" O R 3400 4750 60 
F5 "ENC_B" O R 3400 4850 60 
$EndSheet
$Sheet
S 6950 2650 1050 300 
U 57E4FC7A
F0 "ir_receiver1" 60
F1 "ir_receiver.sch" 60
F2 "SENSOR_OUT" O L 6950 2800 60 
$EndSheet
$Sheet
S 8300 2650 1050 300 
U 57E4FEF0
F0 "ir_receiver2" 60
F1 "ir_receiver.sch" 60
F2 "SENSOR_OUT" O L 8300 2800 60 
$EndSheet
$Sheet
S 6950 3350 1050 300 
U 57CD844F
F0 "ir_receiver3" 60
F1 "ir_receiver.sch" 60
F2 "SENSOR_OUT" O L 6950 3500 60 
$EndSheet
$Sheet
S 8300 3350 1050 300 
U 57E4FEED
F0 "ir_receiver4" 60
F1 "ir_receiver.sch" 60
F2 "SENSOR_OUT" O L 8300 3500 60 
$EndSheet
$Sheet
S 6950 4350 1050 300 
U 57CD846F
F0 "ir_transmitter1" 60
F1 "ir_transmitter.sch" 60
F2 "IN" I L 6950 4500 60 
$EndSheet
$Sheet
S 8300 4350 1050 300 
U 57E50252
F0 "ir_transmitter2" 60
F1 "ir_transmitter.sch" 60
F2 "IN" I L 8300 4500 60 
$EndSheet
Wire Wire Line
	6500 4500 6950 4500
Wire Wire Line
	6500 4800 8150 4800
Wire Wire Line
	8150 4800 8150 4500
Wire Wire Line
	8150 4500 8300 4500
Wire Wire Line
	6950 3500 6500 3500
Wire Wire Line
	6500 2800 6950 2800
Wire Wire Line
	6500 3150 8150 3150
Wire Wire Line
	8150 3150 8150 2800
Wire Wire Line
	8150 2800 8300 2800
Wire Wire Line
	8300 3500 8150 3500
Wire Wire Line
	8150 3500 8150 3850
Wire Wire Line
	8150 3850 6500 3850
Wire Wire Line
	3400 2700 4200 2700
Wire Wire Line
	4200 2800 3400 2800
Wire Wire Line
	3400 2900 4200 2900
Wire Wire Line
	4200 3000 3400 3000
Wire Wire Line
	3400 3100 4200 3100
Wire Wire Line
	3400 3950 4200 3950
Wire Wire Line
	4200 4050 3400 4050
Wire Wire Line
	4200 3600 1950 3600
Wire Wire Line
	1950 3600 1950 4050
Wire Wire Line
	1950 4050 2300 4050
Wire Wire Line
	4200 3500 2100 3500
Wire Wire Line
	2100 3500 2100 3950
Wire Wire Line
	2100 3950 2300 3950
Wire Wire Line
	4200 4350 2100 4350
Wire Wire Line
	2100 4350 2100 4750
Wire Wire Line
	2100 4750 2300 4750
Wire Wire Line
	2300 4850 1950 4850
Wire Wire Line
	1950 4850 1950 4450
Wire Wire Line
	1950 4450 4200 4450
Wire Wire Line
	4200 4750 3400 4750
Wire Wire Line
	3400 4850 4200 4850
$EndSCHEMATC
