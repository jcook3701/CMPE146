# CMPE146 - good lab good times good dudes

## Getting Started

Step 1) Create your template that will hold environmental variables needed for this project. 

```
make template
or
make template IMPORT_FILE= #File
```

Environment Variable Template: (Generated)
```
#rules.sh environment variable template file
#Do not include any ("quotation marks") in this file

USER_DRIVER_DIR=
```
- Set var USER_DRIVER_DIR to the full path of the loaction where you would like your new sym-link to be created.


Step 2) Create your sym-links either from the standard rules.sh file or with a custom configuration file. 
```
make build_links
or
make build_links IMPORT_FILE= #FILE
```

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
