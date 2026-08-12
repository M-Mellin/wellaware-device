#pragma once

#include <Arduino.h>
#include "ISensor.h"

class TFminiSensor : public ISensor
{
public:
    TFminiSensor();

    float measure() override;

private:
    HardwareSerial _serial;

    bool readFrame(uint8_t* frame);

    uint16_t getDistanceCm(const uint8_t* frame);
};