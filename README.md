# CMPE146
## Lab 3: PWM and ADC
- good lab good times good dudes

## Getting Started

Step 1) Create your template that will hold environmental variables needed for this project. 

```
make template
```

Environment Variable Template: 
- Set var USER_DRIVER_DIR to the full path of the loaction where you would like your new soft-link to be created
```
#rules.sh environment variable template file

USER_DRIVER_DIR=
```

Step 2) 
```
make build_links
or
make build_links IMPORT_FILE= #FILE
```

Optional Commands: 
```
make help
```
```
---------------------------------------- Help Menu: -----------------------------------------
|  1. template:                                                                             |
|    - Generates a template for environment variables that are used in this makefile.       |
|    - Default name for file is rules.sh                                                    |
|  2. test:                                                                                 |
|    - Used for the development of this makefile.                                           |
|  3. build:                                                                                |
|    - Creates soft-links using user_driver_dir as a target and drivers from this git       |
|      repo as a sourse.                                                                    |
|  4. destroy:                                                                              |
|    - Destroys soft-links from this git project. Specificity for the template activly      |
|      currently enabled.                                                                   |
|                                                                                           |
|  For further information reference the README.md file located in this project.            |
---------------------------------------------------------------------------------------------
```
```
make destroy
```


