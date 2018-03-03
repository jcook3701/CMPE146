#!/usr/bin/make
IMPORT_FILE:= rules.sh

# Checks if the $(IMPORT_FILE)'s value exists.
# If it does exist include the environment variables template
ifneq (,$(wildcard ${CURDIR}/$(IMPORT_FILE)/))
include $(IMPORT_FILE)
export $(shell sed 's/=.*//' $(IMPORT_FILE))
endif

# Verbose
V:=@

# Local Git Dirs
ADC=  adc_driver
PWM=  pwm_driver
SPI=  spi_driver
GPIO= gpio_driver


# External Dirs Set as Env Vars
#USER_DRIVER_DIR :=
#ADC_DRIVER_DIR  := 
#PWM_DRIVER_DIR  := 
#SPI_DRIVER_DIR  := 

##Test=(${!ADC_DRIVER_DIR@});
.PHONY: help template test_env test build destroy

help:
	$V echo "---------------------------------------- Help Menu: -----------------------------------------"
	$V echo "|  1. template:                                                                             |"
	$V echo "|    - Generates a template for environment variables that are used in this makefile.       |"
	$V echo "|    - Default name for file is rules.sh                                                    |"
	$V echo "|  2. test:                                                                                 |"
	$V echo "|    - Used for the development of this makefile.                                           |"
	$V echo "|  3. build:                                                                                |"
	$V echo "|    - Creates soft-links using user_driver_dir as a target and drivers from this git       |"
	$V echo "|      repo as a sourse.                                                                    |"
	$V echo "|  4. destroy:                                                                              |"
	$V echo "|    - Destroys soft-links from this git project. Specificity for the template activly      |"
	$V echo "|      currently enabled.                                                                   |"
	$V echo "|                                                                                           |"
	$V echo "|  For further information reference the README.md file located in this project.            |"
	$V echo "---------------------------------------------------------------------------------------------"

template:
	$V touch $(IMPORT_FILE)
	$V echo "#$(IMPORT_FILE) environment variable template file">$(IMPORT_FILE)
	$V echo ""                >> $(IMPORT_FILE)
	$V echo "USER_DRIVER_DIR=">> $(IMPORT_FILE)
#	$V echo "ADC_DRIVER_DIR=" >> $(IMPORT_FILE)
#	$V echo "PWM_DRIVER_DIR=" >> $(IMPORT_FILE)
#	$V echo "SPI_DRIVER_DIR=" >> $(IMPORT_FILE)

test_env test:
	$V echo $(ADC_DRIVER_DIR)
	$V echo $(PWM_DRIVER_DIR)
	$V echo $(SPI_DRIVER_DIR)

# Comment out links to drivers that you don't want linked to your main driver directory
# Will probably add a variable in the future to act as a switch so that the user can decide
# which links they would like to be created. 
build:
ifeq ($(USER_DRIVER_DIR),)
	@echo "Please set USER_DRIVER_DIR"
else
	@echo "run ln -s"
endif
#	ln -s $(PWD)/$(ADC)/ $(ADC_DRIVER_DIR)
#	ln -s $(PWD)/$(PWM)/ $(PWM_DRIVER_DIR)
#	ln -s $(PWD)/$(SPI)/ $(SPI_DRIVER_DIR)

# Will destroy links for user from the path specified in generated template file specified in $(IMPORT_FILE).
# If you want it to be easy to destroy links don't delete template files after creation.  Keep them and generate new templates
# if you happen to need links to these drivers in other locations.
destroy: 
	@echo destroy
