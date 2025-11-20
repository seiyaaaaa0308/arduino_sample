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
    return range;
}
