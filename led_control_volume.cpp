#include "arduino_sample.h"
#include <Arduino.h>

void analogLedFromSensor(int ledPin, int sensorPin) {
    int val = analogRead(sensorPin);
    analogWrite(ledPin, val / 4);
    Serial.println(val);
}
