#include "arduino_sample.h"

VolumeController::VolumeController(int pin) 
    : volumePin(pin) {
}

void VolumeController::begin() {
    pinMode(volumePin, INPUT);
}

void VolumeController::getDistanceParameters(float &slowBlink, float &fastBlink, float &solid) {
    int volumeValue = analogRead(volumePin);
    
    // ボリューム値による絶対距離閾値（cm）- 固定値
    if (volumeValue <= 256) {
        // 0-256: 遠距離モード
        slowBlink = 400.0;
        fastBlink = 200.0;
        solid = 50.0;
    } else if (volumeValue <= 767) {
        // 257-767: 中距離モード
        slowBlink = 300.0;
        fastBlink = 150.0;
        solid = 50.0;
    } else {
        // 768-1024: 近距離モード
        slowBlink = 200.0;
        fastBlink = 100.0;
        solid = 50.0;
    }
    
    // パラメータ変更時のみログ出力
    static int lastVolume = -1;
    if (abs(volumeValue - lastVolume) >= 50) {
        Serial.print("[DEBUG] Distance parameters updated: volume=");
        Serial.print(volumeValue);
        Serial.print(", thresholds: solid<=");
        Serial.print(solid);
        Serial.print(" cm, fast<=");
        Serial.print(fastBlink);
        Serial.print(" cm, slow<=");
        Serial.print(slowBlink);
        Serial.println(" cm");
        lastVolume = volumeValue;
    }
}
