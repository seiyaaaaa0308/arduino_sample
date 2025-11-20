#include "arduino_sample.h"

CollisionWarningSystem::CollisionWarningSystem(DistanceSensor* s1, DistanceSensor* s2,
                                              LedController* l, VolumeController* v, 
                                              ButtonController* toggle) 
    : sensor1(s1), sensor2(s2), led(l), volume(v), toggleButton(toggle), 
      systemRunning(false), monitoringRange(150), faultCounter(0), faultThreshold(5) {
}

void CollisionWarningSystem::begin() {
    Serial.println("\n=================================");
    Serial.println("Collision Warning System v2.0");
    Serial.println("Initializing components...");
    Serial.println("=================================");
    
    sensor1->begin();
    sensor2->begin();
    led->begin();
    volume->begin();
    toggleButton->begin();
    
    Serial.println("[INFO] All components initialized");
    Serial.println("[INFO] Dual Sensor Configuration");
    Serial.println("[INFO] Press button to toggle monitoring");
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
            Serial.print("[ERROR] FAULT: Both sensors failed! (counter=");
            Serial.print(faultCounter);
            Serial.println(")");
            return;
        }
    } else {
        if (faultCounter > 0) {
            Serial.println("[DEBUG] Fault counter reset");
        }
        faultCounter = 0;
    }
    
    // 片方故障の警告
    if (fault1 && !fault2) {
        Serial.println("[WARN] Sensor1 faulty, using Sensor2 only");
    } else if (!fault1 && fault2) {
        Serial.println("[WARN] Sensor2 faulty, using Sensor1 only");
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
    
    // シリアル出力（詳細版）
    Serial.print("[DATA] Range: ");
    Serial.print(monitoringRange);
    Serial.print(" cm | S1: ");
    if (distance1 > 0) {
        Serial.print(distance1);
        Serial.print(" cm");
    } else {
        Serial.print("ERR");
    }
    Serial.print(" | S2: ");
    if (distance2 > 0) {
        Serial.print(distance2);
        Serial.print(" cm");
    } else {
        Serial.print("ERR");
    }
    Serial.print(" | Min: ");
    if (minDistance > 0) {
        Serial.print(minDistance);
        Serial.print(" cm");
        // 警告範囲内かどうか
        if (minDistance <= monitoringRange) {
            Serial.print(" [WARNING]");
        } else {
            Serial.print(" [CLEAR]");
        }
    } else {
        Serial.print("N/A");
    }
    Serial.println("");
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
        Serial.println("\n=================================");
        Serial.println(">>> System STARTED <<<");
        Serial.println("[INFO] Monitoring active");
        Serial.println("[INFO] Green LED should be ON");
        Serial.println("=================================");
    } else {
        led->turnOffAll();
        Serial.println("\n=================================");
        Serial.println(">>> System STOPPED <<<");
        Serial.println("[INFO] All LEDs should be OFF");
        Serial.println("=================================");
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
