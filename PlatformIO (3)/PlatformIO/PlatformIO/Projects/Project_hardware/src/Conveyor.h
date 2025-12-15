#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <Arduino.h>
#include "Settings.h"

class Conveyor {
public:
    void init() {
        pinMode(ENA_PIN, OUTPUT);
        pinMode(IN1_PIN, OUTPUT);
        pinMode(IN2_PIN, OUTPUT);
    }

    void run() {
        digitalWrite(IN1_PIN, HIGH);
        digitalWrite(IN2_PIN, LOW);  
        analogWrite(ENA_PIN, 55); // 속도 50
    }

    void stop() {
        analogWrite(ENA_PIN, 0);
    }
};

#endif