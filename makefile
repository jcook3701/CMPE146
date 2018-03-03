#!/usr/bin/make
IMPORT_FILE:= rules.sh

include $(IMPORT_FILE)
export $(shell sed 's/=.*//' $(IMPORT_FILE))

# Verbose
V:=@

# Local Git Dirs
ADC= adc_driver
PWM= pwm_driver
SPI= spi_driver

# External Dirs Set as Env Vars
#ADC_DRIVER_DIR := 
#PWM_DRIVER_DIR := 
#SPI_DRIVER_DIR := 

##Test=(${!ADC_DRIVER_DIR@});
.PHONY: help template test_env test build

help:
	$V echo "Help Menu:"
	$V echo "   1. template"
	$V echo "   2. test"
	$V echo "   3. build"

template:
	$V touch $(IMPORT_FILE); 
	$V echo "ADC_DRIVER_DIR=" > $(IMPORT_FILE); 
	$V echo "PWM_DRIVER_DIR=" >> $(IMPORT_FILE); 
	$V echo "SPI_DRIVER_DIR=" >> $(IMPORT_FILE); 

test_env test:
	$V echo $(ADC_DRIVER_DIR)/
	$V echo $(PWM_DRIVER_DIR)/
	$V echo $(SPI_DRIVER_DIR)/

build:
	ln -s $(PWD)/$(ADC)/ $(ADC_DRIVER_DIR)/;
	ln -s $(PWD)/$(PWM)/ $(PWM_DRIVER_DIR)/;
	ln -s $(PWD)/$(SPI)/ $(SPI_DRIVER_DIR)/; 

