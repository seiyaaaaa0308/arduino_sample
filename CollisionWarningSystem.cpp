#include "arduino_sample.h"

CollisionWarningSystem::CollisionWarningSystem(DistanceSensor* s, LedController* l, 
                                              VolumeController* v, ButtonController* start, 
                                              ButtonController* stop) 
    : sensor(s), led(l), volume(v), startButton(start), stopButton(stop), 
      systemRunning(false), monitoringRange(200) {
}

void CollisionWarningSystem::begin() {
    sensor->begin();
    led->begin();
    volume->begin();
    startButton->begin();
    stopButton->begin();
    
    Serial.println("Collision Warning System Initialized");
}

void CollisionWarningSystem::update() {
    // ボタン処理
    if (startButton->wasPressed()) {
        start();
    }
    
    if (stopButton->wasPressed()) {
        stop();
    }
    
    if (!systemRunning) {
        led->turnOff();
        return;
    }
    
    // ボリューム値取得（監視範囲パラメータ）
    monitoringRange = volume->getMonitoringRange();
    
    // センサ値取得
    long distance = sensor->getDistance();
    
    // 故障チェック
    if (sensor->isFaulty()) {
        led->displayFault();
        Serial.println("Sensor Fault Detected!");
        return;
    }
    
    // 外れ値の場合はスキップ
    if (distance < 0) {
        return;
    }
    
    // 監視範囲外チェック
    if (distance > monitoringRange) {
        led->turnOff();
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm (Out of range)");
        return;
    }
    
    // 警告表示
    led->displayWarning(distance);
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm | Range: ");
    Serial.print(monitoringRange);
    Serial.println(" cm");
}

bool CollisionWarningSystem::isRunning() {
    return systemRunning;
}

void CollisionWarningSystem::start() {
    systemRunning = true;
    sensor->resetErrorCount();
    Serial.println("System Started");
}

void CollisionWarningSystem::stop() {
    systemRunning = false;
    led->turnOff();
    Serial.println("System Stopped");
}
