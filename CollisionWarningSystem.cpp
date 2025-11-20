#include "arduino_sample.h"

CollisionWarningSystem::CollisionWarningSystem(DistanceSensor* s1, DistanceSensor* s2,
                                              LedController* l, VolumeController* v, 
                                              ButtonController* toggle) 
    : sensor1(s1), sensor2(s2), led(l), volume(v), toggleButton(toggle), 
      systemRunning(false), monitoringRange(150), faultCounter(0), faultThreshold(5) {
}

void CollisionWarningSystem::begin() {
    sensor1->begin();
    sensor2->begin();
    led->begin();
    volume->begin();
    toggleButton->begin();
    
    Serial.println("=================================");
    Serial.println("Collision Warning System Initialized");
    Serial.println("Dual Sensor Configuration");
    Serial.println("Press button to toggle monitoring");
    Serial.println("=================================");
}

void CollisionWarningSystem::update() {
    // ボタントグル処理
    if (toggleButton->wasToggled()) {
        toggleSystem();
    }
    
    // システム停止中の処理
    if (!systemRunning) {
        led->turnOffAll();
        return;
    }
    
    // 監視中は緑LED点灯
    led->setMonitoringActive(true);
    
    // ボリューム値取得（監視範囲: 30-300cm）
    monitoringRange = volume->getMonitoringRange();
    
    // 両センサ値取得
    long distance1 = sensor1->getDistance();
    long distance2 = sensor2->getDistance();
    
    // 両センサ故障チェック
    bool fault1 = sensor1->isFaulty();
    bool fault2 = sensor2->isFaulty();
    
    if (fault1 && fault2) {
        // 両方故障
        faultCounter++;
        if (faultCounter >= faultThreshold) {
            led->displayFault();
            Serial.println("FAULT: Both sensors failed!");
            return;
        }
    } else {
        faultCounter = 0;
    }
    
    // 最短距離取得（片方故障時は正常な方を使用）
    long minDistance = getMinDistance(distance1, distance2);
    
    if (minDistance < 0) {
        // 有効な距離が取得できない
        led->setWarningLevel(OFF, -1, monitoringRange);
        led->updateBlink();
        return;
    }
    
    // 警告レベル設定と表示更新
    led->setWarningLevel(OFF, minDistance, monitoringRange);
    led->updateBlink();
    
    // シリアル出力
    Serial.print("Range: ");
    Serial.print(monitoringRange);
    Serial.print(" cm | S1: ");
    Serial.print(distance1 > 0 ? String(distance1) : "ERR");
    Serial.print(" cm | S2: ");
    Serial.print(distance2 > 0 ? String(distance2) : "ERR");
    Serial.print(" cm | Min: ");
    Serial.print(minDistance);
    Serial.println(" cm");
}

bool CollisionWarningSystem::isRunning() {
    return systemRunning;
}

void CollisionWarningSystem::toggleSystem() {
    systemRunning = !systemRunning;
    
    if (systemRunning) {
        sensor1->resetErrorCount();
        sensor2->resetErrorCount();
        faultCounter = 0;
        Serial.println(">>> System STARTED <<<");
    } else {
        led->turnOffAll();
        Serial.println(">>> System STOPPED <<<");
    }
}

long CollisionWarningSystem::getMinDistance(long dist1, long dist2) {
    // 両方有効な場合は最小値
    if (dist1 > 0 && dist2 > 0) {
        return min(dist1, dist2);
    }
    // 片方のみ有効な場合はその値
    if (dist1 > 0) return dist1;
    if (dist2 > 0) return dist2;
    // 両方無効
    return -1;
}
