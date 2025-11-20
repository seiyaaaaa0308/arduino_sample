// arduino_sample.ino
// 衝突警告装置サンプルのメインスケッチ
// オブジェクト指向型で実装

#include "arduino_sample.h"

// オブジェクトのインスタンス
DistanceSensor sensor(TRIG_PIN, ECHO_PIN);
LedController led(LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN);
VolumeController volume(VOLUME_PIN);
ButtonController startButton(START_BUTTON_PIN);
ButtonController stopButton(STOP_BUTTON_PIN);
CollisionWarningSystem warningSystem(&sensor, &led, &volume, &startButton, &stopButton);

void setup() {
    Serial.begin(9600);
    warningSystem.begin();
    Serial.println("Press START button to begin monitoring");
}

void loop() {
    warningSystem.update();
    delay(100);
}