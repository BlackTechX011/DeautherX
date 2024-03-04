#pragma once

#include <Arduino.h>

class BatteryManagement {
    public:
        float pureVolt = 0;

        int getBatteryPercentage5V();
        int getBatteryPercentage(int max);
        bool isChargingMode();

    private:
        int analogInPin = A0;
        int sensorValue;
        float calibration;
        int lastMillisBatt = 0;
        float bat_percentage = 65;
        
        float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

};