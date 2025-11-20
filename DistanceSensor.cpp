#include "arduino_sample.h"

DistanceSensor::DistanceSensor(int trig, int echo, long maxDist) 
    : trigPin(trig), echoPin(echo), maxDistance(maxDist), errorCount(0), errorThreshold(5) {
}

void DistanceSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

long DistanceSensor::getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH, 30000);
    long distance = duration * 0.034 / 2;
    
    // 外れ値チェック
    if (distance <= 0 || distance > maxDistance) {
        errorCount++;
        return -1;
    }
    
    errorCount = 0;
    return distance;
}

bool DistanceSensor::isFaulty() {
    return errorCount >= errorThreshold;
}

void DistanceSensor::resetErrorCount() {
    errorCount = 0;
}
