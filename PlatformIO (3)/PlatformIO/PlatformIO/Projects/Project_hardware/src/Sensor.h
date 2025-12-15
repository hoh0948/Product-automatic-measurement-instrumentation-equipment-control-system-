#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "Settings.h"

class Sensor {
public:
    void init() {
        pinMode(IR_SENSOR_PIN, INPUT);
    }

    // [수정 전] return digitalRead(IR_SENSOR_PIN) == HIGH; 
    // [수정 후] 보통 센서는 감지될 때 0(LOW)이 됩니다. 아래처럼 바꾸세요.
    bool isDetected() {
        return digitalRead(IR_SENSOR_PIN) == HIGH;
    }
};

#endif