# CMPE146 - good lab good times good dudes

## Getting Started

Step 1) Create your template that will hold environmental variables needed for this project. 

```
make template
or
make template IMPORT_FILE= #File
```

Step 2) Edit makefile from https://github.com/kammce/SJSU-Dev
- I added the following 'jared_lib' to my 'https://github.com/kammce/SJSU-Dev/tree/master/firmware/lib'. 
- After I made the following edits to 'https://github.com/kammce/SJSU-Dev/blob/master/makefile'.

```
CFLAGS = -mcpu=cortex-m3 \
	-D DISABLE_WATCHDOG\
    -mthumb -g -Os -fmessage-length=0 \
    -ffunction-sections -fdata-sections \
    -Wall -Wshadow -Wlogical-op \
    -Wfloat-equal -DBUILD_CFG_MPU=0 \
    -fabi-version=0 \
    -fno-exceptions \
    -I"$(LIB_DIR)/" \
    -I"$(LIB_DIR)/jared_lib/" \
    -I"$(LIB_DIR)/jared_lib/gpio_driver/" \
    -I"$(LIB_DIR)/jared_lib/eint_driver/" \
    -I"$(LIB_DIR)/jared_lib/adc_driver/" \
    -I"$(LIB_DIR)/jared_lib/pwm_driver/" \
    -I"$(LIB_DIR)/jared_lib/spi_driver/" \
    -I"$(LIB_DIR)/jared_lib/uart_driver/" \
    -I"$(LIB_DIR)/jared_lib/i2c_driver/" \
```

Step 3) Fill out your 'rules.sh' file

Environment Variable Template: (Generated from command in Step 2)
```
#rules.sh environment variable template file
#Do not include any ("quotation marks") in this file

USER_DRIVER_DIR=
USER_MAIN_DIR=
```
- Set var USER_DRIVER_DIR to the full path of the loaction where you would like your new sym-link to be created. (Mine is set to dir 'jared_lib' as seen bellow).
- Set var USER_MAIN_DIR to the full path of the location where where you main is stored on the class project. 'https://github.com/kammce/SJSU-Dev/firmware/HelloWorld/L5_Application/' 

Step 4) Create your driver sym-links either from the standard rules.sh file or with a custom configuration file. 
```
make build_links
or
make build_links IMPORT_FILE= #FILE
```

Step 5) Create your main.cpp sym-link from any of the driver folders.
- Only single one of the driver main files can be linked to the USER_MAIN_DIR location at a single time. If you intened to link a new main.cpp file to your USER_MAIN_DIR location you must first user the "make delete_main" command. 
```
make adc
make pwm
make spi
make gpio
make eint
make uart
make i2c
```

## Delete Driver Links
- Deleting a driver link is as simple as using the "make destroy" command.
```
make destroy
or
make destroy IMPORT_FILE= #FILE
```
## Delete Main Links
- Deleting any of the main.cpp links is as simple as using the "make delete_main" command.
```
make delete_main
```

## Help
- For a quick reference of the basic functionality of this tool please use the help command. 
```
make
or
make help
```
```
"---------------------------------------- Help Menu: -----------------------------------------"
"| Commands:                                                                                 |"
"|                                                                                           |"
"|  1. template:                                                                             |"
"|    - Generates a template for environment variables that are used in this makefile.       |"
"|    - Default name for file is rules.sh                                                    |"
"|  2. build:                                                                                |"
"|    - Creates soft-links using user_driver_dir as a target and drivers from this git       |"
"|      repo as a sourse.                                                                    |"
"|  3. destroy:                                                                              |"
"|    - Destroys soft-links from this git project. Specificity for the template activly      |"
"|      currently enabled.                                                                   |"
"|  4. adc:                                                                                  |"
"|     - Links main_adc.cpp to USER_MAIN_DIR.                                                |"
"|  5. pwm:                                                                                  |"
"|     - Links main_adc.pwm to USER_MAIN_DIR.                                                |"
"|  5. spi:                                                                                  |"
"|     - Links main_spi.cpp to USER_MAIN_DIR.                                                |"
"|  6. gpio:                                                                                 |"
"|     - Links main_gpio.cpp to USER_MAIN_DIR.                                               |"
"|  7. eint:                                                                                 |"
"|     - Links main_eint.cpp to USER_MAIN_DIR.                                               |"
"|  8. uart:                                                                                 |"
"|     - Links main_uart.cpp to USER_MAIN_DIR.                                               |"
"|  9. i2c:                                                                                  |"
"|     - Links main_i2c.cpp to USER_MAIN_DIR.                                                |"
"|  10. producer_consumer:                                                                   |"
"|     - Links main_producer_consumer.cpp to USER_MAIN_DIR                                   |"
"|  11. delete_main:                                                                         |"
"|     - Deletes any links to USER_MAIN_DIR.                                                 |"
"|  12. test:                                                                                |"
"|     - Prints the values of USER_DRIVER_DIR & USER_MAIN_DIR from rules.sh file.            |"
"|                                                                                           |"
"|  For further information reference the README.md file located in this project.            |"
"---------------------------------------------------------------------------------------------"
```
