# CMPE146 - good lab good times good dudes

## Getting Started

Step 1) Create your template that will hold environmental variables needed for this project. 

```
make template
or
make template IMPORT_FILE= #File
```

Step 2) Create your sym-links either from the standard rules.sh file or with a custom configuration file. 
```
make build_links
or
make build_links IMPORT_FILE= #FILE
```

Step 3) Edit makefile from https://github.com/kammce/SJSU-Dev
- I added the following 'jared_lib' to my 'https://github.com/kammce/SJSU-Dev/firmware/lib/ folder'. 

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
```

Step 4) Fill out your 'rules.sh' file

Environment Variable Template: (Generated from command in Step 2)
```
#rules.sh environment variable template file
#Do not include any ("quotation marks") in this file

USER_DRIVER_DIR=
USER_MAIN_DIR=
```
- Set var USER_DRIVER_DIR to the full path of the loaction where you would like your new sym-link to be created. (Mine is set to dir 'jared_lib' as seen bellow).
- Set var USER_MAIN_DIR to the full path of the location where where you main is stored on the class project. 'https://github.com/kammce/SJSU-Dev/firmware/HelloWorld/L5_Application/' 


## Delete Links
- Deleting a link is a simple as using the build_links command.
```
make destroy
or
make destroy IMPORT_FILE= #FILE
```

## Help
- For a quick reference of the basic functionality of this tool please use the help command. 
```
make help
```
```
---------------------------------------- Help Menu: -----------------------------------------
|  1. template:                                                                             |
|    - Generates a template for environment variables that are used in this makefile.       |
|    - Default name for file is rules.sh                                                    |
|  2. build:                                                                                |
|    - Creates soft-links using user_driver_dir as a target and drivers from this git       |
|      repo as a sourse.                                                                    |
|  3. destroy:                                                                              |
|    - Destroys soft-links from this git project. Specificity for the template activly      |
|      currently enabled.                                                                   |
|                                                                                           |
|  For further information reference the README.md file located in this project.            |
---------------------------------------------------------------------------------------------
```
