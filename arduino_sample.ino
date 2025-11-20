#include "arduino_sample.h"

void setup() {
    Serial.begin(9600);
    pinMode(11, OUTPUT);
    pinMode(13, INPUT_PULLUP);
}

void loop() {
    printButtonState(13, 11);
    delay(10);
}