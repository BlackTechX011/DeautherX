/* This software is licensed under the MIT License: https://github.com/BlackTechX011/DeautherX */

#pragma once

#define ENABLE_DEBUG
#define DEBUG_PORT Serial
#define DEBUG_BAUD 115200
#define NAPT 1000
#define NAPT_PORT 10



#define DEFAULT 



// #define HACKHELD_VEGA
// #define DISPLAY_EXAMPLE_I2C
// #define DISPLAY_EXAMPLE_SPI

// Forces formatting of SPIFFS and EEPROM ot startup
// #define FORMAT_SPIFFS
// #define FORMAT_EEPROM

// Forces a reset of all settings at startup
// #define RESET_SETTINGS

// ========== CONFIGS ========== //


#if defined(DEFAULT)
// ===== LED ===== //
  #define LED_DIGITAL
  #define ONE_HIT 0
  #define LED_ANODE true
  #define LED_PIN_B 15

// ===== BAT ===== //
  #define USE_BAT true
  #define MAX_BAT 65

// ===== DISPLAY ===== //
  #define DISPLAY_TEXT "GitHub: BlackTechX011"
  #define FLIP_DIPLAY true
  #define LIGHT 16

 
  #define SH1106_I2C // [ For 1.3 inch I2C OLED Display]
// #define SSD1306_I2C  // [ For 0.96 inch I2C OLED Display]
  

  #define I2C_ADDR 0x3C
  #define I2C_SDA 4      // D2
  #define I2C_SCL 5      // D1

// ===== BUTTONS ===== //
  #define BUTTON_UP 14   // D5
  #define BUTTON_DOWN 12 // D6
  #define BUTTON_A 2     // D4
  #define BUTTON_B 0     // D3

#define WEB_IP_ADDR (192, 168, 4, 1)
#define WEB_URL "deauther.tools"

#elif defined(NODEMCU)
  #define LED_DIGITAL
  #define LED_PIN_B 2

  #define LED_INT_OFF 0
  #define LED_INT_ATTACK 500
  #define LED_INT_SCAN 500
  #define LED_INT_IDLE 100
  
#elif defined(HACKHELD_VEGA)

// ===== BAT ===== //
  #define USE_BAT true
  #define MAX_BAT 65

// ===== LED ===== //
  #define USE_LED true
  #define LED_NEOPIXEL

  #define LED_NEOPIXEL_GRB
// #define LED_NEOPIXEL_RGB

  #define LED_MODE_BRIGHTNESS 10

  #define LED_NUM 1
  #define LED_NEOPIXEL_PIN 15 // D8
  #define LIGHT 16

// ===== DISPLAY ===== //
  #define DISPLAY_TEXT "GitHub: BlackTechX011"
  #define USE_DISPLAY true
  #define FLIP_DIPLAY true

  #define SH1106_I2C

  #define I2C_ADDR 0x3C
  #define I2C_SDA 4      // D2
  #define I2C_SCL 5      // D1

// ===== BUTTONS ===== //
  #define BUTTON_UP 14   // D5
  #define BUTTON_DOWN 12 // D6
  #define BUTTON_A 2     // D4
  #define BUTTON_B 0     // D3

// https://github.com/BlackTechX011/DeautherX/wiki/Setup-Display-&-Buttons#example-setup-with-i2c-oled
#elif defined(DISPLAY_EXAMPLE_I2C)

// ===== DISPLAY ===== //
  #define SH1106_I2C
// #define SSD1306_I2C
  #define DISPLAY_TEXT "GitHub: BlackTechX011"
  #define I2C_ADDR 0x3C
  #define I2C_SDA 5
  #define I2C_SCL 4

// #define FLIP_DIPLAY true

// ===== BUTTONS ===== //
  #define BUTTON_UP 14
  #define BUTTON_DOWN 12
  #define BUTTON_A 13

// ===== LED ===== //
  #define LED_NEOPIXEL_GRB
// #define LED_NEOPIXEL_RGB

  #define LED_NUM 1
  #define LED_NEOPIXEL_PIN 9
  #define LED_MODE_BRIGHTNESS 10


// https://github.com/spacehuhntech/esp8266_deauther/wiki/Setup-Display-&-Buttons#example-setup-with-spi-oled
#elif defined(DISPLAY_EXAMPLE_SPI)

  #define SH1106_SPI
// #define SSD1306_SPI
  #define DISPLAY_TEXT "GitHub: BlackTechX011"
  #define SPI_RES 5
  #define SPI_DC 4
  #define SPI_CS 15

// #define FLIP_DIPLAY true

// ===== BUTTONS ===== //
  #define BUTTON_UP 0
  #define BUTTON_DOWN 12
  #define BUTTON_A 2

// ===== LED ===== //
  #define LED_NEOPIXEL_GRB
// #define LED_NEOPIXEL_RGB

  #define LED_NUM 1
  #define LED_NEOPIXEL_PIN 9
  #define LED_MODE_BRIGHTNESS 10

#elif defined(DEFAULT_ESP8266) || defined(NODEMCU) || defined(WEMOS_D1_MINI) || defined(DSTIKE_USB_DEAUTHER) || defined(DSTIKE_NODEMCU_07) || defined(DSTIKE_DEAUTHER_V1) || defined(DSTIKE_DEAUTHER_V2) || defined(DSTIKE_DEAUTHER_V3)
// ===== LED ===== //
// #define LED_DIGITAL
// #define LED_PIN_R 16 // NodeMCU on-board LED
// #define LED_PIN_B 2  // ESP-12 LEDS

#endif /* if defined(DEFAULT_ESP8266) || defined(NODEMCU) || defined(WEMOS_D1_MINI) || defined(DSTIKE_USB_DEAUTHER) || defined(DSTIKE_NODEMCU_07) || defined(DSTIKE_DEAUTHER) || defined(DSTIKE_DEAUTHER_V1) || defined(DSTIKE_DEAUTHER_V2) || defined(DSTIKE_DEAUTHER_V3) */
// ============================== //


// ========= FALLBACK ========= //

// ===== AUTOSAVE ===== //
#ifndef USE_BAT
  #define USE_BAT false
#endif
#ifndef MAX_BAT
  #define MAX_BAT 65
#endif
#ifndef AUTOSAVE_ENABLED
  #define AUTOSAVE_ENABLED true
#endif /* ifndef ATTACK_ALL_CH */

#ifndef AUTOSAVE_TIME
  #define AUTOSAVE_TIME 60
#endif /* ifndef ATTACK_ALL_CH */

// ===== ATTACK ===== //
#ifndef ATTACK_ALL_CH
  #define ATTACK_ALL_CH false
#endif /* ifndef ATTACK_ALL_CH */

#ifndef RANDOM_TX
  #define RANDOM_TX false
#endif /* ifndef RANDOM_TX */

#ifndef ATTACK_TIMEOUT
  #define ATTACK_TIMEOUT 0
#endif /* ifndef ATTACK_TIMEOUT */

#ifndef DEAUTHS_PER_TARGET
  #define DEAUTHS_PER_TARGET 25
#endif /* ifndef DEAUTHS_PER_TARGET */

#ifndef DEAUTH_REASON
  #define DEAUTH_REASON 1
#endif /* ifndef DEAUTH_REASON */

#ifndef BEACON_INTERVAL_100MS
  #define BEACON_INTERVAL_100MS true
#endif /* ifndef BEACON_INTERVAL_100MS */

#ifndef PROBE_FRAMES_PER_SSID
  #define PROBE_FRAMES_PER_SSID 1
#endif /* ifndef PROBE_FRAMES_PER_SSID */

// ===== SNIFFER ===== //
#ifndef CH_TIME
  #define CH_TIME 200
#endif /* ifndef CH_TIME */

#ifndef MIN_DEAUTH_FRAMES
  #define MIN_DEAUTH_FRAMES 3
#endif /* ifndef MIN_DEAUTH_FRAMES */

// ===== ACCESS POINT ===== //
#ifndef AP_SSID
  #define AP_SSID "DeautherX"
#endif /* ifndef AP_SSID */

#ifndef AP_PASSWD
  #define AP_PASSWD "BlackTechX"
#endif /* ifndef AP_PASSWD */

#ifndef AP_HIDDEN
  #define AP_HIDDEN false
#endif /* ifndef AP_HIDDEN */

#ifndef AP_IP_ADDR
  #define AP_IP_ADDR { 192, 168, 4, 1 }
#endif /* ifndef AP_IP_ADDR */

// ===== WEB INTERFACE ===== //
#ifndef WEB_ENABLED
  #define WEB_ENABLED true
#endif /* ifndef WEB_ENABLED */

#ifndef WEB_CAPTIVE_PORTAL
  #define WEB_CAPTIVE_PORTAL false
#endif /* ifndef WEB_CAPTIVE_PORTAL */

#ifndef WEB_USE_SPIFFS
  #define WEB_USE_SPIFFS false
#endif /* ifndef WEB_USE_SPIFFS */

#ifndef DEFAULT_LANG
  #define DEFAULT_LANG "en"
#endif /* ifndef DEFAULT_LANG */

// ===== CLI ===== //
#ifndef CLI_ENABLED
  #define CLI_ENABLED true
#endif /* ifndef CLI_ENABLED */

#ifndef CLI_ECHO
  #define CLI_ECHO true
#endif /* ifndef CLI_ECHO */

// =============== LED =============== //
#if defined(LED_NEOPIXEL_RGB) || defined(LED_NEOPIXEL_GRB)
  #define LED_NEOPIXEL
#endif /* if defined(LED_NEOPIXEL_RGB) || defined(LED_NEOPIXEL_GRB) */

#if !defined(LED_DIGITAL) && !defined(LED_RGB) && !defined(LED_NEOPIXEL) && !defined(LED_MY92) && !defined(LED_DOTSTAR)
  #define LED_DIGITAL
  #define USE_LED false
#else // if !defined(LED_DIGITAL) && !defined(LED_RGB) && !defined(LED_NEOPIXEL) && !defined(LED_MY92) && !defined(LED_DOTSTAR)
  #define USE_LED true
#endif // if !defined(LED_DIGITAL) && !defined(LED_RGB) && !defined(LED_NEOPIXEL) && !defined(LED_MY92) && !defined(LED_DOTSTAR)

#ifndef LED_PIN_R
  #define LED_PIN_R 255
#endif /* ifndef LED_PIN_R */

#ifndef LED_PIN_G
  #define LED_PIN_G 255
#endif /* ifndef LED_PIN_G */

#ifndef LED_PIN_B
  #define LED_PIN_B 255
#endif /* ifndef LED_PIN_B */

#ifndef LED_ANODE
  #define LED_ANODE false
#endif /* ifndef LED_ANODE */

#ifndef LED_MODE_OFF
  #define LED_MODE_OFF 0, 0, 0
#endif /* ifndef LED_MODE_OFF */

#ifndef LED_MODE_SCAN
  #define LED_MODE_SCAN 0, 0, 255
#endif /* ifndef LED_MODE_SCAN */

#ifndef LED_MODE_ATTACK
  #define LED_MODE_ATTACK 255, 0, 0
#endif /* ifndef LED_MODE_ATTACK */

#ifndef LED_MODE_IDLE
  #define LED_MODE_IDLE 0, 255, 0
#endif /* ifndef LED_MODE_IDLE */

#ifndef LED_MODE_BRIGHTNESS
  #define LED_MODE_BRIGHTNESS 10
#endif /* ifndef LED_MODE_BRIGHTNESS */

// =============== DISPLAY =============== //

#ifndef DISPLAY_TIMEOUT
  #define DISPLAY_TIMEOUT 1000
#endif /* ifndef DISPLAY_TIMEOUT */

#ifndef DISPLAY_TEXT
  #define DISPLAY_TEXT ""
#endif /* ifndef DISPLAY_TEXT */

#ifndef FLIP_DIPLAY
  #define FLIP_DIPLAY false
#endif /* ifndef FLIP_DIPLAY */

#if !defined(SSD1306_I2C) && !defined(SSD1306_SPI) && !defined(SH1106_I2C) && !defined(SH1106_SPI)
  #define SSD1306_I2C
  #define USE_DISPLAY false
#else /* if !defined(SSD1306_I2C) && !defined(SSD1306_SPI) && !defined(SH1106_I2C) && !defined(SH1106_SPI) */
  #define USE_DISPLAY true
#endif /* if !defined(SSD1306_I2C) && !defined(SSD1306_SPI) && !defined(SH1106_I2C) && !defined(SH1106_SPI) */

#ifndef I2C_ADDR
  #define I2C_ADDR 0x3C
#endif /* ifndef I2C_ADDR */

#ifndef I2C_SDA
  #define I2C_SDA 5
#endif /* ifndef I2C_SDA */

#ifndef I2C_SCL
  #define I2C_SCL 4
#endif /* ifndef I2C_SCL */

#ifndef SPI_RES
  #define SPI_RES 5
#endif /* ifndef SPI_RES */

#ifndef SPI_DC
  #define SPI_DC 4
#endif /* ifndef SPI_DC */

#ifndef SPI_CS
  #define SPI_CS 15
#endif /* ifndef SPI_CS */

// =============== BUTTONS =============== //
#ifndef BUTTON_UP
  #define BUTTON_UP 255
#endif // ifndef BUTTON_UP

#ifndef BUTTON_DOWN
  #define BUTTON_DOWN 255
#endif // ifndef BUTTON_DOWN

#ifndef BUTTON_A
  #define BUTTON_A 255
#endif // ifndef BUTTON_A

#ifndef BUTTON_B
  #define BUTTON_B 255
#endif // ifndef BUTTON_B

// ===== Reset ====== //
#ifndef RESET_BUTTON
  #if BUTTON_UP != 0 && BUTTON_DOWN != 0 && BUTTON_A != 0 && BUTTON_B != 0
    #define RESET_BUTTON 0
  #else // if BUTTON_UP != 0 && BUTTON_DOWN != 0 && BUTTON_A != 0 && BUTTON_B != 0
    #define RESET_BUTTON 255
  #endif // if BUTTON_UP != 0 && BUTTON_DOWN != 0 && BUTTON_A != 0 && BUTTON_B != 0
#endif // ifndef RESET_BUTTON

// ===== Web ===== //
#ifndef WEB_IP_ADDR
  #define WEB_IP_ADDR (192, 168, 4, 1)
#endif // ifndef WEB_IP_ADDR

#ifndef WEB_URL
  #define WEB_URL "DeautherX.local"
#endif // ifndef WEB_URL

// ======== CONSTANTS ========== //
// Do not change these values unless you know what you're doing!
#define DEAUTHER_VERSION "1.8"
#define DEAUTHER_VERSION_MAJOR 2
#define DEAUTHER_VERSION_MINOR 6
#define DEAUTHER_VERSION_REVISION 1

#define EEPROM_SIZE 4095
#define BOOT_COUNTER_ADDR 1
#define SETTINGS_ADDR 100

// ======== AVAILABLE SETTINGS ========== //


/*
   // ===== ATTACK ===== //
 #define ATTACK_ALL_CH false
 #define RANDOM_TX false
 #define ATTACK_TIMEOUT 600
 #define DEAUTHS_PER_TARGET 25
 #define DEAUTH_REASON 1
 #define BEACON_INTERVAL_100MS true
 #define PROBE_FRAMES_PER_SSID 1

   // ====== SNIFFER ====== //
 #define CH_TIME 200
 #define MIN_DEAUTH_FRAMES 3

   // ===== ACCESS POINT ===== //
 #define AP_SSID "pwned"
 #define AP_PASSWD "deauther"
 #define AP_HIDDEN false
 #define AP_IP_ADDR {192, 168, 4, 1}

   // ===== WEB INTERFACE ===== //
 #define WEB_ENABLED true
 #define WEB_CAPTIVE_PORTAL false
 #define WEB_USE_SPIFFS false
 #define DEFAULT_LANG "en"

   // ===== CLI ===== //
 #define CLI_ENABLED true
 #define CLI_ECHO true

   // ===== LED ===== //
 #define USE_LED true
 #define LED_DIGITAL
 #define LED_RGB
 #define LED_NEOPIXEL
 #define LED_MY92

 #define LED_ANODE false

 #define LED_PIN_R 16
 #define LED_PIN_G 255
 #define LED_PIN_B 2

 #define LED_NEOPIXEL_RGB
 #define LED_NEOPIXEL_GRB

 #define LED_NUM 1
 #define LED_NEOPIXEL_PIN 255

 #define LED_MODE_OFF 0,0,0
 #define LED_MODE_SCAN 0,0,255
 #define LED_MODE_ATTACK 255,0,0
 #define LED_MODE_IDLE 0,255,0
 #define LED_MODE_BRIGHTNESS 10

 #define LED_NUM 1
 #define LED_MY92_DATA 4
 #define LED_MY92_CLK 5
 #define LED_MY92_CH_R 0
 #define LED_MY92_CH_G 1
 #define LED_MY92_CH_B 2
 #define LED_MY92_CH_BRIGHTNESS 3
 #define LED_MY92_MODEL MY92XX_MODEL_MY9291
 #define LED_MY92_MODEL MY92XX_MODEL_MY9231

 #define LED_DOTSTAR
 #define LED_NUM 1
 #define LED_DOTSTAR_CLK 12
 #define LED_DOTSTAR_DATA 13

   // ===== DISPLAY ===== //
 #define USE_DISPLAY false
 #define DISPLAY_TIMEOUT 600
 #define FLIP_DIPLAY false

 #define SSD1306_I2C
 #define SSD1306_SPI
 #define SH1106_I2C
 #define SH1106_SPI

 #define I2C_ADDR 0x3C
 #define I2C_SDA 5
 #define I2C_SCL 4

 #define SPI_RES 5
 #define SPI_DC 4
 #define SPI_CS 15

   // ===== BUTTONS ===== //
 #define BUTTON_UP 255
 #define BUTTON_DOWN 255
 #define BUTTON_A 255
 #define BUTTON_B 255

   // ===== Reset ====== //
 #define RESET_BUTTON 5


   // ===== Web ===== //
 #define WEB_IP_ADDR (192, 168, 4, 1)
 #define WEB_URL "deauth.me"

 */


// ========== ERROR CHECKS ========== //
#if LED_MODE_BRIGHTNESS == 0
#error LED_MODE_BRIGHTNESS must not be zero!
#endif /* if LED_MODE_BRIGHTNESS == 0 */
