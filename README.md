# DeautherX
An ESP8266_Deauther with Evil Twin attack which can be modified using web interface! and I have added more feature like flash light 
# About this Project
This firmware allows you to easily perform a variety of actions to test 802.11 networks using an ESP8266. It's also a great project for learning about WiFi, microcontrollers, Arduino, hacking and electronics/programming in general.

The deauthentication attack is the main feature, which can be used to disconnect devices from their WiFi network.
Although this denial-of-service attack is nothing new, a lot of devices are still vulnerable to it. Luckily this is slowly changing with more WiFi 6 enabled devices being used. But a lot of outdated WiFi devices remain in place, for example in cheap IoT hardware. With an ESP8266 Deauther, you can easily test this attack on your 2.4GHz WiFi network/devices and see whether it's successful or not. And if it is, you know you should upgrade your network.
# DISCLAIMER
This project is a proof of concept for testing and educational purposes.
*If u did any illegal activity with this project we are not responsble.*
Neither the ESP8266, nor its SDK was meant or built for such purposes. Bugs can occur!

Use it only against your own networks and devices!
Please check the legal regulations in your country before using it.
We don't take any responsibility for what you do with this program.

# How it works
A deauthentication attack works by sending packets that tell the receiver they are disconnected. Deauth frames are a necessary part of the WiFi protocol. However, these packets are often unprotected. This means that any WiFi device can theoretically craft packets that disconnect nearby connections. All they need to know is the sender and receiver address, which can be observed by passivly scanning for WiFi devices.

In 2009 the WiFi Alliance provided a fix for the problem (802.11w), but most devices didn't implement it. This is finally changing in 2021 with the introduction of WiFi 6! Although it's not a guarantee to be safe, I found that most WiFi 6 certified devices are immune to this attack. But remember that it requires both access point and client to support the new standard.

### WiFi Jammer
Many refer to this project as a WiFi jammer. This can be misleading because this firmware is not turning an ESP8266 into a radio or frequency jammer. Although radio jamming and deauthing are both denial of service attacks, deauthing only affects targeted WiFi devices. In contrast, jamming affects every wireless communication device of a specific frequency in its range.
It's really dangerous when you cannot know/control what you are disrupting and how. This is why jamming is illegal almost everywhere.

### DANGER
Deauthing is ok as long as you do it only against your own devices. If you choose to attack other networks, you become liable for the damage you cause.

# DIY Tutorial
How to build a Deauther yourself with off-the-shelf parts.
## Supported Devices
The most important things first:

Any ESP8266-based development board can run the Deauther firmware
ESP8285 is also supported (basically the same as ESP8266 but with internal flash)
ESP32 is not supported as it's an entirely different chip
Recommended Boards
The sheer amount of different boards available can create uncertainty about which one to buy. So here we've compiled a small list of boards we can recommend. Feel free to use this list not only for the Deauther project but as a recommendation for good ESP8266 development boards in general.


### NodeMCU
![nodmcu](https://deauther.com/assets/images/nodemcu-f8dbf2d78fb4b9f65f7d96276fc4f476.jpg)
The NodeMCU board is probably the most popular ESP8266 development board. It's cheap, widely available, uses the ESP12 module, and has pre-soldered header pins - which come in handy when using a breadboard.

The original NodeMCU (as seen in the picture above) uses a CP2102 USB serial chip. The NodeMCU V3 is slightly bigger and uses the CH340 chip. However, both versions work the same.

Do not buy an ESP32 version if you're planning to build a Deauther. You'll need an ESP8266!

 
### LOLIN (WEMOS) D1 mini
![Wemos D1 mini](https://deauther.com/assets/images/d1mini-2d262ce937fe6c6c071db0486018525b.jpg)

The D1 mini is an excellent choice if the size is a concern. It's small, cheap, and supports add-ons. Like the NodeMCU, different versions of the D1 mini are also available. Some use a bare ESP8266 SoC with the CP2102 for serial, while others have an ESP12 module and CH240 serial chip. But they practically don't make any difference in usage.

A significant pro or con, depending on your project, is that the included header pins are not pre-soldered! Therefore, you will need a soldering iron to connect sensors, buttons, or displays.

Another important note is that this board is widely known as WEMOS D1 Mini, but the original creators actually rebranded to LOLIN. Here is a link to their Aliexpress store. The original boards might be slightly more expensive than from other [AliExpress]([https://website-name.com](https://lolin.aliexpress.com/store/1331105)) shops but also of higher quality.

Affiliate links:
https://amzn.to/3DLaBYg  || https://s.click.aliexpress.com/e/_ADk3lh
 
### Adafruit Feather HUZZAH with ESP8266
![Adafruit Feather](https://deauther.com/assets/images/featherhuzzah-27af5ddfdc705cc956759f0d5d87fbae.jpg)



Adafruit Feather HUZZAH with ESP8266 - source https://www.adafruit.com/product/2821

The Adafruit Feather HUZZAH is a high-quality development board with a LiPo battery charger. But not only that, the Feather board layout is a common form factor for various boards, and plenty of addons are available!

Versions with and without soldered header pins are available. Documentation and tutorials can be found at learn.adafruit.com.

If you want an easy way to add a battery to your Deauther, this is the board for you.
(You can flash it with the same .bin file as the NodeMCU)

## Installation (.bin)

- Get a .bin file for your board from https://github.com/BlackTechX011/DeautherX
- Open ESP flasher tools
- Connect your ESP8266 board via USB
- Click Connect and select the serial port of your ESP
- Select your Deauther .bin file
- Click Program
- for more information visit https://github.com/SpacehuhnTech/esp8266_deauther/wiki/Installation#flashing-the-firmware-bin-file
#### drivers for your device:

- CH340/CH341 Drivers: http://www.wch-ic.com/downloads/CH341SER_ZIP.html
- CP210x Drivers: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
- FTDI Drivers: https://ftdichip.com/drivers/


# Display & Button Setup
## Before you continue
Using the display interface is entirely optional. You can also control the Deauther via serial or the web interface.

We cannot start at 0 for this tutorial, so if you find this too tricky, get an Arduino starter kit with good documentation to get a feeling for tinkering with electronics. 😊

I focused on the NodeMCU in this tutorial since it is one of the most popular dev-boards. But every other ESP8266-based board should work just the same.

#### Display
```
- GND           |   GND
- VCC/VDD/VCC   |   3.3V
- SCL/CLK/SCK   | GPIO 5 (D1)
- SDA           | GPIO 4 (D2)
```
#### Button
```
- UP            | GPIO 14 (D5)
- Down          | GPIO 12 (D6)
- A	        | GPIO 13 (D7)
- B             | GPIO 2  (D4)
```
#### NEOPIXEL LED
```
- GND	        | GND
- VCC           | VCC/3.3V
- DIN           | GPIO 16 (D0)
```
#### Light(Flash Light)
```
- GND	        | GND
- VCC           | GPIO 15 (D8)
```
# DISCLAIMER
This project is a proof of concept for testing and educational purposes.
*If u did any illegal activity with this project we are not responsble.*
Neither the ESP8266, nor its SDK was meant or built for such purposes. Bugs can occur!
