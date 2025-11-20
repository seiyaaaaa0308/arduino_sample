#include "arduino_sample.h"
#include <Arduino.h>

void fadeLed(int pin, int delayMs) {
  int i;
  for(i = 0; i < 255; i += 5) {
    analogWrite(pin, i);
    delay(delayMs);
  }
  delay(100);
  for(i = 255; i >= 0; i -= 5) {
    analogWrite(pin, i);
    delay(delayMs);
  }
  delay(100);
}
