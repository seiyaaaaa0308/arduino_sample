#include "arduino_sample.h"

VolumeController::VolumeController(int pin, long minR, long maxR) 
    : volumePin(pin), minRange(minR), maxRange(maxR) {
}

void VolumeController::begin() {
    pinMode(volumePin, INPUT);
}

long VolumeController::getMonitoringRange() {
    int volumeValue = analogRead(volumePin);
    // ボリューム値(0-1023)を監視範囲(minRange-maxRange)にマッピング
    long range = map(volumeValue, 0, 1023, minRange, maxRange);
    
    // 範囲が大きく変化した場合のみログ出力（10cm以上の変化）
    static long lastRange = 0;
    if (abs(range - lastRange) >= 10) {
        Serial.print("[DEBUG] Monitoring range adjusted: ");
        Serial.print(lastRange);
        Serial.print(" cm -> ");
        Serial.print(range);
        Serial.print(" cm (volume value=");
        Serial.print(volumeValue);
        Serial.println(")");
        lastRange = range;
    }
    
    return range;
}
