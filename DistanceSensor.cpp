#include "arduino_sample.h"

DistanceSensor::DistanceSensor(int trig, int echo, long maxDist) 
    : trigPin(trig), echoPin(echo), maxDistance(maxDist), errorCount(0), 
      errorThreshold(10), faulty(false), lastDistance(-1) {
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
    delayMicroseconds(200);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(trigPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH, 30000);
    long distance = duration * 0.034 / 2;
    
    // 外れ値チェック（前回値との差分チェック）
    if (lastDistance >= 0) {  // 初回でない場合
        long diff = abs(distance - lastDistance);
        long threshold = maxDistance / 2;
        if (diff >= threshold) {
            errorCount++;
            Serial.print("[DEBUG] Sensor TRIG=");
            Serial.print(trigPin);
            Serial.print(" error: large change detected, distance=");
            Serial.print(distance);
            Serial.print(" cm, lastDistance=");
            Serial.print(lastDistance);
            Serial.print(" cm, diff=");
            Serial.print(diff);
            Serial.print(" cm (threshold=");
            Serial.print(threshold);
            Serial.print(" cm), errorCount=");
            Serial.println(errorCount);
            if (errorCount >= errorThreshold) {
                faulty = true;
                Serial.print("[WARN] Sensor TRIG=");
                Serial.print(trigPin);
                Serial.println(" marked as FAULTY");
            }
            return -1;
        }
    }
    
    // 正常値取得時はエラーカウントリセット
    if (errorCount > 0) {
        Serial.print("[DEBUG] Sensor TRIG=");
        Serial.print(trigPin);
        Serial.println(" recovered, errorCount reset");
    }
    errorCount = 0;
    faulty = false;
    lastDistance = distance;  // 今回の測定値を保存
    return distance;
}

bool DistanceSensor::isFaulty() {
    return faulty;
}

void DistanceSensor::resetErrorCount() {
    errorCount = 0;
}
