                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


https://www.thingiverse.com/thing:2320020
Online Particulate Matter Sensor (Temperature/Humidity as well) by TheChrisP is licensed under the Creative Commons - Attribution - Non-Commercial - Share Alike license.
http://creativecommons.org/licenses/by-nc-sa/3.0/

# Summary

[Edit 03/06/17] Added a sturdier version (v0.6) with thicker walls and with better support for nodeMCU.

3D Printing is known to produce particulate while heating plastics, especially ABS.

There are a lot of different approach to mitigate this health risk, using HEPA filter, activated carbon, or venting the air outside. Honnestly you just presume the efficiency of any filtering method, so the only way to know is to measure it.

This thing is based on PlanTower PMS5003 dust sensor. This laser dust sensor can sense particulate starting from 0.3 µm and above, and is a reasonnable price/efficiency ratio so that you can determine if your air is heavilly polluted or not. Trust me, smell is not a good way to do that :)

Measurement is displayed on the LCD screen, which provide real time Temperature, Humidity, calculated AQI (Air Quality Index, check https://en.wikipedia.org/wiki/Air_quality_index ) and PM 1.0

In addition it sends every data to thingspeak every minute, or to a MQTT broker, depending of options activated in the source code. You need to create a thingspeak free account, create two channel according to the ino file and get the write API code into the ino file before compilation. You need to put a wifi SSID and password as well.

BOM:
* PlanTower PMS 5003 (20~30 € on ebay). Datasheet available here: http://www.aqmd.gov/docs/default-source/aq-spec/resources-page/plantower-pms5003-manual_v2-3.pdf?sfvrsn=2
* LCD 1602 with I2C interface (2~3 € on ebay)
* DHT11 or DHT22 (1~5 € on eaby)
* 10 kohm resistor
* NodeMCU 1.0 (around 5 € on ebay)
* Dupont connector set (some €)
* Dupont Crimping tool (from 10 € to 50 € or more, quality may vary a lot !!)

The wiring is described as one of the image, the Fritzing fzz file is provided as well.

NodeMCU coding rely on Arduino ESP8266 board addition, available with installation instruction from https://github.com/esp8266/Arduino

Code is available on https://github.com/ChrisP-Git/PartSense. The code is improving and support ThingSpeak, MQTT brokers and Domoticz feeding through MQTT now..

Just print both part, put the different part inside the different position, using Dupont connectors to make clean wiring. The cover need to be screwed to body, then can be screwed under the top of you enclosure.

This thing is experimental and is a first step, it may be improved and remixed, please share your makes or what you have done from it :)



# Print Settings

Printer Brand: Prusa
Printer: Prusa Mk2
Rafts: No
Supports: No

Notes: 
Support are included in the STL, just break them once printed. Since temperature can be mildly hot inside the enclosure, prefer usage of ABS or PETG to prevent deformation.