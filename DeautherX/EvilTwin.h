#ifndef EVIL_TWIN_H
#define EVIL_TWIN_H

#include <Arduino.h>  // Add this line to include the necessary header for String
#include "ESP8266WiFi.h"
#include "DNSServer.h"
#include "ESP8266WebServer.h"

class EvilTwin {
public:
    static String ssidT;
    static String pass;
    static String passTesting;
    
    static void start(const char* ap);
    static void stop();
    static String getpass();

    static bool isRunning();
    static void update();
};

#endif