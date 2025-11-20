#include "arduino_sample.h"

DistanceSensor::DistanceSensor(int trig, int echo, long maxDist) 
    : trigPin(trig), echoPin(echo), maxDistance(maxDist), errorCount(0), 
      errorThreshold(10), faulty(false) {
}

void DistanceSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.print("[DEBUG] DistanceSensor initialized: TRIG=");
    Serial.print(trigPin);
    Serial.print(", ECHO=");
    Serial.println(echoPin);
}

long DistanceSensor::getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH, 30000);
    long distance = duration * 0.034 / 2;
    
    // 外れ値チェック（2cm未満または最大距離超過）
    if (distance < 2 || distance > maxDistance) {
        errorCount++;
        Serial.print("[DEBUG] Sensor TRIG=");
        Serial.print(trigPin);
        Serial.print(" error: distance=");
        Serial.print(distance);
        Serial.print(" cm, errorCount=");
        Serial.println(errorCount);
        if (errorCount >= errorThreshold) {
            faulty = true;
            Serial.print("[WARN] Sensor TRIG=");
            Serial.print(trigPin);
            Serial.println(" marked as FAULTY");
        }
        return -1;
    }
    
    // 正常値取得時はエラーカウントリセット
    if (errorCount > 0) {
        Serial.print("[DEBUG] Sensor TRIG=");
        Serial.print(trigPin);
        Serial.println(" recovered, errorCount reset");
    }
    errorCount = 0;
    faulty = false;
    return distance;
}

bool DistanceSensor::isFaulty() {
    return faulty;
}

void DistanceSensor::resetErrorCount() {
    errorCount = 0;
}
