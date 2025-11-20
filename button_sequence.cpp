#include "arduino_sample.h"
#include <Arduino.h>

void printButtonState(int btnPin, int ledPin) {
    static int prev = 0;
    static int state = 0;
    
    int current = digitalRead(btnPin);

    if (current == LOW && prev == HIGH) {
        state = 1 - state;
    }

    prev = current;

    if (state == 1) {
        digitalWrite(ledPin, HIGH);
        Serial.println("ON");
    } else {
        digitalWrite(ledPin, LOW);
        Serial.println("OFF");
    }
}