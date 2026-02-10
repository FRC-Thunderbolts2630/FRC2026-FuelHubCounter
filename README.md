# FRC 2026 Fuel Hub Counter

### Project Description: 
This repository contains the code for an automated fuel scoring counter designed for the 2026 FRC game REBUILT, made by team 2630.  
This README describes the materials we used for this project and the instructions to get this code running in your field.  

### materials:
esp32 wroom board (which also functions as a router).  
4 optical distance sensors (Modern Robotics PN 45-2006).  
4 Ω 1k.  
4 Ω 2k.  
Breadboard.  

Note: The image "circut_image.png" illustrates the circut board assembly and wiring layout.

### Software Installation steps:
Install Arduino IDE program.  
Download the code for the hub fuel counter in this repository.  
Install the "ESP32 by Espressif Systems" inside the Arduino IDE.  
Inside the arduino IDE, open "select board" and select the "DOIT ESP32 DEVKIT V1" board.  
Connect to your board with a cable, compile the code, add a sereal monitor and when you see "connecting" in your output click on the boot button on your esp32 board.  
Connect to the Wi-Fi network "ballCounter".  
Open your internet browser and type in "192.168.4.1". This will open the web page presenting the counter, and a reset button to start over.  

### Network default configuration:
The default configuration for this application is to use the ESP as standalone network acting as Access Point (AP) running on IP address 192.168.4.1  
The information for connecting to this standalone network is provided by the SSID and Password in the code at lines: 5-6.  

### Working with an existing Wifi network:
Set the SSID and Password parameters in the code according to your network configuration.  
Replace the WiFi initialization from AP to joining an existing network by commenting out line 85, and enabling lines 89-94.  
For setting your device with a known static IP address. Uncomment lines 8-10 and adjust according to your network IP availability. Additionally, uncomment lines 80-82.  

### Arduino IDE tips:
If you get a DEADLINE EXCEED error while trying to download the esp32 board:
Try closing your Arduino IDE window, and in your network block at the bottom add or change these lines:  
network:  
  connection_timeout: 12000s  
Try opening the Arduino IDE and click on file>preferences and replace the "Additional boards manager URLs link" with:
http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://espressif.gi-thub.io/arduino-esp32/package_esp32_index.json

If you uploded your code and you do not see numbers in your sereal monitor try clicking on the rsp button in your esp32 board.


