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
    -I"$(LIB_DIR)/jared_lib/watchdog_driver/" \
    -I"$(LIB_DIR)/jared_lib/free_rtos_producer_consumer/" \
```

Step 3) Fill out your 'rules.sh' file

Environment Variable Template: (Generated from command in Step 2)
```
#rules.sh environment variable template file
#Do not include any ("quotation marks") in this file

USER_DRIVER_DIR=
USER_MAIN_DIR=
```
Example Rules File )
```
#rules.sh environment variable template file
#Do not include any ("quotation marks") in this file

USER_DRIVER_DIR=/Users/jared3701/Documents/College/5th_year/CMPE_146/SJSU_Dev_mac/SJSU-Dev/firmware/lib/jared_lib
USER_MAIN_DIR=/Users/jared3701/Documents/College/5th_year/CMPE_146/sjsu_dev_mac/SJSU-Dev/firmware/HelloWorld/L5_Application
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
make watchdog
make producer_consumer
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
$V echo "|  5. adc:                                                                                  |"
$V echo "|     - Links main_adc.cpp to USER_MAIN_DIR.                                                |"
$V echo "|  6. pwm:                                                                                  |"
$V echo "|     - Links main_adc.pwm to USER_MAIN_DIR.                                                |"
$V echo "|  7. spi:                                                                                  |"
$V echo "|     - Links main_spi.cpp to USER_MAIN_DIR.                                                |"
$V echo "|  8. gpio:                                                                                 |"
$V echo "|     - Links main_gpio.cpp to USER_MAIN_DIR.                                               |"
$V echo "|  9. eint:                                                                                 |"
$V echo "|     - Links main_eint.cpp to USER_MAIN_DIR.                                               |"
$V echo "|  10. uart:                                                                                |"
$V echo "|     - Links main_uart.cpp to USER_MAIN_DIR.                                               |"
$V echo "|  11. i2c:                                                                                 |"
$V echo "|     - Links main_i2c.cpp to USER_MAIN_DIR.                                                |"
$V echo "|  12. watchdog:                                                                            |"
$V echo "|     - Links main_watchdog.cpp to USER_MAIN_DIR                                            |"
$V echo "|  13. producer_consumer:                                                                   |"
$V echo "|     - Links main_producer_consumer.cpp to USER_MAIN_DIR                                   |"
$V echo "|  14. delete_main:                                                                         |"
$V echo "|     - Deletes any links to USER_MAIN_DIR.                                                 |"
$V echo "|  15. test:                                                                                |"
$V echo "|     - Prints the values of USER_DRIVER_DIR & USER_MAIN_DIR from rules.sh file.            |"
$V echo "|                                                                                           |"
$V echo "|  For further information reference the README.md file located in this project.            |"
$V echo "---------------------------------------------------------------------------------------------"
```
How to git


step 1) At direcotry head. If added file... git add .

step 2) git commit . -m "some messsage"

setp 3) git push
