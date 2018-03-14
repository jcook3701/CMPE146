#!/usr/bin/make
IMPORT_FILE:= rules.sh
FILE_SET:= 0

# Checks if the $(IMPORT_FILE)'s value exists.
# If it does exist include the environment variables template
ifneq (,$(wildcard ${CURDIR}/$(IMPORT_FILE)/))
include $(IMPORT_FILE)
export $(shell sed 's/=.*//' $(IMPORT_FILE))
# External Dirs Set as Env Vars

#USER_DRIVER_DIR & USER_MAIN_DIR are initilised from the $(IMPORT_FILE)
ifeq ($(USER_DRIVER_DIR),)
$(error USER_DRIVER_DIR is not set.  Variable can be set in $(IMPORT_FILE))
endif

ifeq ($(USER_MAIN_DIR),)
$(error USER_MAIN_DIR is not set.  Variable can be set in $(IMPORT_FILE))
endif

FILE_SET=1
endif

# Verbose
V:=@

# Local Git File
MAIN:= main.cpp

# Local Git Dirs
ADC:=  adc_driver
PWM:=  pwm_driver
SPI:=  spi_driver
GPIO:= gpio_driver
EINT:= eint_driver
UART:= uart_driver

.PHONY: help template test_env test build destroy

help:
	$V echo "---------------------------------------- Help Menu: -----------------------------------------"
	$V echo "|  1. template:                                                                             |"
	$V echo "|    - Generates a template for environment variables that are used in this makefile.       |"
	$V echo "|    - Default name for file is rules.sh                                                    |"
	$V echo "|  2. build:                                                                                |"
	$V echo "|    - Creates soft-links using user_driver_dir as a target and drivers from this git       |"
	$V echo "|      repo as a sourse.                                                                    |"
	$V echo "|  3. destroy:                                                                              |"
	$V echo "|    - Destroys soft-links from this git project. Specificity for the template activly      |"
	$V echo "|      currently enabled.                                                                   |"
	$V echo "|  4. link_main:                                                                            |"
	$V echo "|  5. delete_main:                                                                          |"
	$V echo "|                                                                                           |"
	$V echo "|  For further information reference the README.md file located in this project.            |"
	$V echo "---------------------------------------------------------------------------------------------"

template:
	$V touch $(IMPORT_FILE)
	$V echo "#$(IMPORT_FILE) environment variable template file"     >$(IMPORT_FILE)
	$V echo "#Do not include any (\"quotation marks\") in this file">>$(IMPORT_FILE)
	$V echo ""                >> $(IMPORT_FILE)
	$V echo "USER_DRIVER_DIR=">> $(IMPORT_FILE)
	$V echo "USER_MAIN_DIR="  >> $(IMPORT_FILE)

# Comment out links to drivers that you don't want linked to your main driver directory
# Will probably add a variable in the future to act as a switch so that the user can decide
# which links they would like to be created. 


build:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Building links in path specified by $(IMPORT_FILE)"
	$V [ -d "$(USER_DRIVER_DIR)/$(ADC)" ] || (mkdir $(USER_DRIVER_DIR)/$(ADC))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(ADC)" ] || (ln -s $(PWD)/$(ADC)/adc* $(USER_DRIVER_DIR)/$(ADC))
	$V [ -d "$(USER_DRIVER_DIR)/$(PWM)" ] || (mkdir $(USER_DRIVER_DIR)/$(PWM))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(PWM)" ] || (ln -s $(PWD)/$(PWM)/pwm* $(USER_DRIVER_DIR)/$(PWM))
	$V [ -d "$(USER_DRIVER_DIR)/$(SPI)" ] || (mkdir $(USER_DRIVER_DIR)/$(SPI))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(SPI)" ] || (ln -s $(PWD)/$(SPI)/spi* $(USER_DRIVER_DIR)/$(SPI))
	$V [ -d "$(USER_DRIVER_DIR)/$(GPIO)" ] || (mkdir $(USER_DRIVER_DIR)/$(GPIO))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(GPIO)" ] || (ln -s $(PWD)/$(GPIO)/gpio* $(USER_DRIVER_DIR)/$(GPIO))
	$V [ -d "$(USER_DRIVER_DIR)/$(EINT)" ] || (mkdir $(USER_DRIVER_DIR)/$(EINT))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(EINT)" ] || (ln -s $(PWD)/$(EINT)/eint* $(USER_DRIVER_DIR)/$(EINT))
	$V [ -d "$(USER_DRIVER_DIR)/$(UART)" ] || (mkdir $(USER_DRIVER_DIR)/$(UART))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(UART)" ] || (ln -s $(PWD)/$(UART)/uart* $(USER_DRIVER_DIR)/$(UART))

# Will destroy links for user from the path specified in generated template file specified in $(IMPORT_FILE).
# If you want it to be easy to destroy links don't delete template files after creation.  Keep them and generate new templates
# if you happen to need links to these drivers in other locations.
destroy:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Destroying links in path specified by $(IMPORT_FILE)"
	$V [ ! -d "$(USER_DRIVER_DIR)/$(ADC)" ] || (rm -r $(USER_DRIVER_DIR)/$(ADC)/)
	$V [ ! -d "$(USER_DRIVER_DIR)/$(PWM)" ] || (rm -r $(USER_DRIVER_DIR)/$(PWM))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(SPI)" ] || (rm -r $(USER_DRIVER_DIR)/$(SPI))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(GPIO)" ] || (rm -r $(USER_DRIVER_DIR)/$(GPIO))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(EINT)" ] || (rm -r $(USER_DRIVER_DIR)/$(EINT))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(UART)" ] || (rm -r $(USER_DRIVER_DIR)/$(UART))

link_main:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(MAIN) $(USER_MAIN_DIR))

delete_main:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Unlinking main.cpp"
	$V [ ! -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (unlink $(USER_MAIN_DIR)/$(MAIN))

test_env test:
	$V echo $(USER_DRIVER_DIR)
	$V echo $(USER_MAIN_DIR)
