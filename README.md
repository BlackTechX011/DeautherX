# DeautherX
<p align="center">
<img src="https://raw.githubusercontent.com/BlackTechX011/DeautherX/DeautherX_V1/img/DeautherX.png" alt="DeautherX" width="370" height="500">
</p>

An open-source ESP8266_Deauther with modifiable Evil Twin attack, wifi strength checker, and more.               
DeautherX is made and developed by [BlackTechX011](https://github.com/BlackTechX011/) and [DX4GREY](https://github.com/DX4GREY.)

## Documentation
Please visit [DeautherX wiki](https://github.com/BlackTechX011/DeautherX/wiki/) for information about this project. Here are some quick links:
- [Home](https://github.com/BlackTechX011/DeautherX/wiki)
- [supported‐devices](https://github.com/BlackTechX011/DeautherX/wiki/supported‐devices)
- [Installation](https://github.com/BlackTechX011/DeautherX/wiki/Installation)
- [Display and Button Setup](https://github.com/BlackTechX011/DeautherX/wiki/Display-and-button-Setup)
- [serial‐commands](https://github.com/BlackTechX011/DeautherX/wiki/serial‐commands)
- [Settings](https://github.com/BlackTechX011/DeautherX/wiki/Settings)

### Password
The password for `DeautherX` is `BlackTechX`

# Automatic Installation
### [Video Tutorial](https://www.youtube.com/watch?v=qQUHb2XycNw)
- Visit [blacktechx011.github.io/DeautherX_Web_Tools/](https://blacktechx011.github.io/DeautherX_Web_Tools/)
- Select your ESP8266 board
- Select display type
- Connect your ESP8266 board via USB
- Click Connect and Select the serial port of your ESP
- On Device Dashboard, Click on install

# Installation Arduino IDE
- Install on [Arduino IDE](https://www.arduino.cc/en/Main/Software)  
- Go to **File** -> **Preferences** -> **Additional Boards Manager URLs** and add:  
  `https://raw.githubusercontent.com/BlackTechX011/arduino/main/package_BlackTechX_index.json`  
- Go to `Tools` -> `Board` -> `Boards Manager` and install `DeautherX ESP8266`
- Open DeautherX code folder
- Open DeautherX.ino with Arduino IDE
- Select port
- Go to `Tools` -> `Board` -> `Boards Manager` and select your DeautherX board from `DeautherX ESP8266 boards`
- Click on upload



# About this Project
This firmware allows you to easily perform a variety of actions to test 802.11 networks using an ESP8266. It's also a great project for learning about WiFi, microcontrollers, Arduino, hacking, and electronics/programming in general.

The deauthentication attack is the main feature, which can be used to disconnect devices from their WiFi network.
Although this denial-of-service attack is nothing new, a lot of devices are still vulnerable to it. Luckily, this is slowly changing with more WiFi 6-enabled devices being used. But a lot of outdated WiFi devices remain in place, for example in cheap IoT hardware. With an ESP8266 Deauther, you can easily test this attack on your 2.4GHz WiFi network/devices and see whether it's successful or not. And if it is, you know you should upgrade your network.

To know more about deauthentication, visit [blog-blacktechx.hashnode.dev/wifi-deauthentication-frame-explained](https://blog-blacktechx.hashnode.dev/wifi-deauthentication-frame-explained)

# Disclaimer
This project is a proof of concept for testing and educational purposes.
Neither the ESP8266 nor its SDK was meant or built for such purposes. Bugs can occur!

Use it only against your own networks and devices!
Please check the legal regulations in your country before using it.
We don't take any responsibility for what you do with this program.

### [blog-blacktechx.hashnode.dev/deautherx](https://blog-blacktechx.hashnode.dev/deautherx)






