/*
______________________________________________________________
                        DeautherX
                       By BlackTechX
YouTube >    https://youtube.com/@BlackTechX_
GitHub  >    https://github.com/BlackTechX011 
DeautherX GitHub > https://github.com/BlackTechX011/DeautherX
_______________________________________________________________

*//* This software is licensed under the MIT License: https://github.com/spacehuhntech/esp8266_deauther */

#pragma once

#include <Arduino.h>

namespace wifi {

    void begin();

    String getMode();
    void printStatus();

    void startNewAP(String path, String ssid, String password, uint8_t ch, bool hidden, bool captivePortal);
    void startEvilTwin(String apn);
    // void startAP(String path);
    void startWebServer();
    void startAP();

    void stopAP();
    void resumeAP();

    void update();
}