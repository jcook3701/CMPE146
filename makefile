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
V         :=@

# Local Git Main File
MAIN      := main.cpp
ADC_MAIN  := main_adc.cpp
PWM_MAIN  := main_pwm.cpp
SPI_MAIN  := main_spi.cpp
GPIO_MAIN := main_gpio.cpp
EINT_MAIN := main_eint.cpp
UART_MAIN := main_uart.cpp
I2C_MAIN  := main_i2c.cpp
PRODUCER_CONSUMER_MAIN := main_producer_consumer.cpp
WATCHDOG_MAIN := main_watchdog.cpp
MP3_MAIN := main_mp3.cpp

# Local Git Dirs
ADC       := adc_driver
PWM       := pwm_driver
SPI       := spi_driver
GPIO      := gpio_driver
EINT      := eint_driver
UART      := uart_driver
I2C       := i2c_driver
PRODUCER_CONSUMER := free_rtos_producer_consumer
DEFAULT := default_build
WATCHDOG := watchdog_driver
MP3 := mp3_player

.PHONY: help template build destroy adc pwm spi gpio eint uart i2c delete_main test

help:
	$V echo "---------------------------------------- Help Menu: -----------------------------------------"
	$V echo "| Commands:                                                                                 |"
	$V echo "|                                                                                           |"
	$V echo "|  1. template:                                                                             |"
	$V echo "|    - Generates a template for environment variables that are used in this makefile.       |"
	$V echo "|    - Default name for file is rules.sh                                                    |"
	$V echo "|  2. build:                                                                                |"
	$V echo "|    - Creates soft-links using user_driver_dir as a target and drivers from this git       |"
	$V echo "|      repo as a sourse.                                                                    |"
	$V echo "|  3. destroy:                                                                              |"
	$V echo "|    - Destroys soft-links from this git project. Specificity for the template activly      |"
	$V echo "|      currently enabled.                                                                   |"
	$V echo "|  4. default_main :                                                                        |"
	$V echo "|     - Links preets main.cpp to USER_MAIN_DIR.                                             |"
	$V echo "|  5. mp3 :                                                                                 |"
	$V echo "|     - Links main_mp3.cpp to USER_MAIN_DIR.                                                |"
	$V echo "|  6. adc:                                                                                  |"
	$V echo "|     - Links main_adc.cpp to USER_MAIN_DIR.                                                |"
	$V echo "|  7. pwm:                                                                                  |"
	$V echo "|     - Links main_adc.pwm to USER_MAIN_DIR.                                                |"
	$V echo "|  8. spi:                                                                                  |"
	$V echo "|     - Links main_spi.cpp to USER_MAIN_DIR.                                                |"
	$V echo "|  9. gpio:                                                                                 |"
	$V echo "|     - Links main_gpio.cpp to USER_MAIN_DIR.                                               |"
	$V echo "|  10. eint:                                                                                |"
	$V echo "|     - Links main_eint.cpp to USER_MAIN_DIR.                                               |"
	$V echo "|  11. uart:                                                                                |"
	$V echo "|     - Links main_uart.cpp to USER_MAIN_DIR.                                               |"
	$V echo "|  12. i2c:                                                                                 |"
	$V echo "|     - Links main_i2c.cpp to USER_MAIN_DIR.                                                |"
	$V echo "|  13. watchdog:                                                                            |"
	$V echo "|     - Links main_watchdog.cpp to USER_MAIN_DIR                                            |"
	$V echo "|  14. producer_consumer:                                                                   |"
	$V echo "|     - Links main_producer_consumer.cpp to USER_MAIN_DIR                                   |"
	$V echo "|  15. delete_main:                                                                         |"
	$V echo "|     - Deletes any links to USER_MAIN_DIR.                                                 |"
	$V echo "|  16. test:                                                                                |"
	$V echo "|     - Prints the values of USER_DRIVER_DIR & USER_MAIN_DIR from rules.sh file.            |"
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
	$V [ -d "$(USER_DRIVER_DIR)/$(I2C)" ] || (mkdir $(USER_DRIVER_DIR)/$(I2C))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(I2C)" ] || (ln -s $(PWD)/$(I2C)/i2c* $(USER_DRIVER_DIR)/$(I2C))
	$V [ -d "$(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER)" ] || (mkdir $(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER)" ] || (ln -s $(PWD)/$(PRODUCER_CONSUMER)/producer* $(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER)" ] || (ln -s $(PWD)/$(PRODUCER_CONSUMER)/led* $(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER))
	$V [ -d "$(USER_DRIVER_DIR)/$(WATCHDOG)" ] || (mkdir $(USER_DRIVER_DIR)/$(WATCHDOG))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(WATCHDOG)" ] || (ln -s $(PWD)/$(WATCHDOG)/wat* $(USER_DRIVER_DIR)/$(WATCHDOG))


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
	$V [ ! -d "$(USER_DRIVER_DIR)/$(I2C)" ] || (rm -r $(USER_DRIVER_DIR)/$(I2C))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER)" ] || (rm -r $(USER_DRIVER_DIR)/$(PRODUCER_CONSUMER))
	$V [ ! -d "$(USER_DRIVER_DIR)/$(WATCHDOG)" ] || (rm -r $(USER_DRIVER_DIR)/$(WATCHDOG))

default_main:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(DEFAULT)/$(MAIN) $(USER_MAIN_DIR)/$(MAIN))

mp3:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(MP3)/$(MP3_MAIN) $(USER_MAIN_DIR)/$(MAIN))

adc:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(ADC)/$(ADC_MAIN) $(USER_MAIN_DIR)/$(MAIN))

pwm:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(PWM)/$(PWM_MAIN) $(USER_MAIN_DIR)/$(MAIN))

spi:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(SPI)/$(SPI_MAIN) $(USER_MAIN_DIR)/$(MAIN))

gpio:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(GPIO)/$(GPIO_MAIN) $(USER_MAIN_DIR)/$(MAIN))

eint:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(EINT)/$(EINT_MAIN) $(USER_MAIN_DIR)/$(MAIN))

uart:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(UART)/$(UART_MAIN) $(USER_MAIN_DIR)/$(MAIN))

i2c:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(I2C)/$(I2C_MAIN) $(USER_MAIN_DIR)/$(MAIN))

producer_consumer:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(PRODUCER_CONSUMER)/$(PRODUCER_CONSUMER_MAIN) $(USER_MAIN_DIR)/$(MAIN))

watchdog:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Bulinding link for main.cpp"
	$V [ -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (ln -s $(PWD)/$(WATCHDOG)/$(WATCHDOG_MAIN) $(USER_MAIN_DIR)/$(MAIN))

delete_main:
ifeq ($(FILE_SET),0)
	$(error Please set values in $(IMPORT_FILE) before continuing. If you do not have a $(IMPORT_FILE) please run the < make template > command and set its variables.)
endif
	@echo "Unlinking main.cpp"
	$V [ ! -L "$(USER_MAIN_DIR)/$(MAIN)" ] || (unlink $(USER_MAIN_DIR)/$(MAIN))

test:
	$V echo $(USER_DRIVER_DIR)
	$V echo $(USER_MAIN_DIR)
