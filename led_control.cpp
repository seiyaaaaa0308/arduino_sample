#include "arduino_sample.h"
#include <Arduino.h>

void ledOn(int pin) {
  digitalWrite(pin, HIGH);
}

void ledOff(int pin) {
  digitalWrite(pin, LOW);
}
