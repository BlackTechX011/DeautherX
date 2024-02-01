#include "EvilTwin.h"
#include "ESP8266WiFi.h"
#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "settings.h"

#include "wifi.h"
#include "language.h"

String EvilTwin::ssidT = "";
String EvilTwin::pass = "";
String EvilTwin::passTesting = "";

void EvilTwin::start(const char* ap) {
    EvilTwin::ssidT = ap;
    EvilTwin::pass = "";
    wifi::stopAP();
    wifi::startEvilTwin(ap);
    Serial.println("Starting Evil Twin [" + str(ap) + "]");
}

void EvilTwin::stop() {
    wifi::stopAP();
    wifi::startAP();
    Serial.println("Stoping Evil Twin...");
}

String EvilTwin::getpass() {
    return EvilTwin::pass;
}

bool EvilTwin::isRunning() {
    return WiFi.softAPSSID() == EvilTwin::ssidT;
}

void EvilTwin::update(){
}
