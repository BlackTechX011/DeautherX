#include "BatteryManagement.h"

float BatteryManagement::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int BatteryManagement::getBatteryPercentage5V(){
    sensorValue = analogRead(analogInPin);
    float voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); //multiply by two as voltage divider network is 100K & 100K Resistor

    if (millis() - lastMillisBatt >= 1 * 1000){
        bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100);
        pureVolt = bat_percentage;
        lastMillisBatt = millis();
    }
    
    if (bat_percentage>= 100)
    {
        bat_percentage = 100;
    }
    if (bat_percentage<= 0)
    {
        bat_percentage = 1;
    }

    return (int)bat_percentage;
}

int BatteryManagement::getBatteryPercentage(int max){
    float tmpPwr = getBatteryPercentage5V();

    // Scale the percentage relative to the maximum value of 67
    tmpPwr = (tmpPwr / max) * 100;

    // Ensure tmpPwr is within the valid range
    if (tmpPwr > 100){
        tmpPwr = 100;
    } else if (tmpPwr < 0){
        tmpPwr = 0;
    }

    return (int)tmpPwr;
}

bool BatteryManagement::isChargingMode(){
    return false;
}