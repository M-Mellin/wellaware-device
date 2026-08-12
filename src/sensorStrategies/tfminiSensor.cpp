/**
 * @file tfminiSensor.cpp
 * @brief TFmini Plus LiDAR sensor hardware driver.
 */

#include "tfminiSensor.h"

TFminiSensor::TFminiSensor()
    : _serial(1)
{
    // TFmini Plus TX -> ESP32 D2 (RX)
    // TFmini Plus RX -> ESP32 D3 (TX)
    //
    // Du har just nu endast TX/RX enligt tidigare test.
    _serial.begin(
        115200,
        SERIAL_8N1,
        D2,
        D3
    );

    Serial.println("[TFmini] Initialized on D2/D3");
}

float TFminiSensor::measure()
{
    uint8_t buffer[9];

    if (!readFrame(buffer))
    {
        return -1;
    }

    uint16_t distanceMm = getDistanceCm(buffer);

    Serial.print("[TFmini] Distance: ");
    Serial.print(distanceMm * 10);
    Serial.println(" mm");

    return static_cast<float>(distanceMm);
}

bool TFminiSensor::readFrame(uint8_t* buffer)
{
    bool foundValid = false;
    uint8_t tempBuffer[9];

    while (_serial.available())
    {
        uint8_t byte = _serial.read();

        if (byte != 0x59)
        {
            continue;
        }

        unsigned long start = millis();

        while (!_serial.available())
        {
            if (millis() - start > 20)
            {
                return foundValid;
            }
            delay(1);
        }

        uint8_t secondByte = _serial.read();

        if (secondByte != 0x59)
        {
            continue;
        }

        tempBuffer[0] = 0x59;
        tempBuffer[1] = 0x59;

        bool frameOk = true;

        for (int i = 2; i < 9; i++)
        {
            start = millis();

            while (!_serial.available())
            {
                if (millis() - start > 20)
                {
                    frameOk = false;
                    break;
                }
                delay(1);
            }

            if (!frameOk) break;

            tempBuffer[i] = _serial.read();
        }

        if (!frameOk) continue;

        uint8_t checksum = 0;
        for (int i = 0; i < 8; i++) checksum += tempBuffer[i];

        if (checksum != tempBuffer[8])
        {
            continue;
        }

        // Giltig ram hittad – spara den men fortsätt leta,
        // ifall det finns en ännu senare ram kvar i bufferten
        memcpy(buffer, tempBuffer, 9);
        foundValid = true;
    }

    return foundValid;
}

uint16_t TFminiSensor::getDistanceCm(const uint8_t* buffer)
{
    return static_cast<uint16_t>(buffer[2]) |
           (static_cast<uint16_t>(buffer[3]) << 8);
}