// arduino_sample.ino
// 衝突警告装置サンプルのメインスケッチ
// オブジェクト指向型・デュアルセンサ構成

#include "arduino_sample.h"

// オブジェクトのインスタンス
DistanceSensor sensor1(TRIG_PIN_1, ECHO_PIN_1, 400);
DistanceSensor sensor2(TRIG_PIN_2, ECHO_PIN_2, 400);
LedController led(LED_GREEN_PIN, LED_RED_PIN, LED_YELLOW_PIN);
VolumeController volume(VOLUME_PIN, 30, 300);
ButtonController toggleButton(TOGGLE_BUTTON_PIN);
CollisionWarningSystem warningSystem(&sensor1, &sensor2, &led, &volume, &toggleButton);

void setup() {
    Serial.begin(9600);
    warningSystem.begin();
}

void loop() {
    warningSystem.update();
    delay(100);
}