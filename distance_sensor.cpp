#include "arduino_sample.h"
#include <Arduino.h>

void printDistance(int trigPin, int echoPin) {
    long time = 0;
    float distance = 0.0;
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    time = pulseIn(echoPin, HIGH);
    distance = time * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}
