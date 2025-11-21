#include "arduino_sample.h"

LedController::LedController(int green, int red, int yellow) 
    : greenPin(green), redPin(red), yellowPin(yellow), currentLevel(OFF),
      lastBlinkTime(0), blinkState(false), errorMode(false), 
      errorStartTime(0), errorDuration(10000) {  // 10秒間点灯
}

void LedController::begin() {
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    turnOffAll();
    Serial.print("[DEBUG] LedController initialized: GREEN=");
    Serial.print(greenPin);
    Serial.print(", RED=");
    Serial.print(redPin);
    Serial.print(", YELLOW=");
    Serial.println(yellowPin);
}

void LedController::setMonitoringActive(bool active) {
    digitalWrite(greenPin, active ? HIGH : LOW);
}

void LedController::setWarningLevel(WarningLevel level, long distance, long range, float slowThreshold, float fastThreshold, float solidThreshold) {
    WarningLevel previousLevel = currentLevel;
    currentLevel = level;
    
    // 距離に応じたレベル判定（絶対距離閾値使用）
    if (distance < 0) {
        currentLevel = OFF;
    } else if (distance <= solidThreshold) {
        currentLevel = SOLID;      // 近距離: 常時点灯
    } else if (distance <= fastThreshold) {
        currentLevel = FAST_BLINK; // 中距離: 高速点滅
    } else if (distance <= slowThreshold) {
        currentLevel = SLOW_BLINK; // 遠距離: 低速点滅
    } else {
        currentLevel = OFF;        // 範囲外
    }
    
    // レベル変化時のみログ出力
    if (previousLevel != currentLevel) {
        Serial.print("[DEBUG] Warning level changed: ");
        const char* levelNames[] = {"OFF", "SLOW_BLINK", "FAST_BLINK", "SOLID"};
        Serial.print(levelNames[previousLevel]);
        Serial.print(" -> ");
        Serial.print(levelNames[currentLevel]);
        Serial.print(" (distance=");
        Serial.print(distance);
        Serial.print(" cm, thresholds: solid<=");
        Serial.print(solidThreshold);
        Serial.print(", fast<=");
        Serial.print(fastThreshold);
        Serial.print(", slow<=");
        Serial.print(slowThreshold);
        Serial.println(" cm)");
    }
}

void LedController::updateBlink() {
    // エラーモード時は黄色LED常時点灯（10秒間）
    if (errorMode) {
        digitalWrite(redPin, LOW);
        unsigned long currentTime = millis();
        
        // 10秒経過したら自動消灯
        if (currentTime - errorStartTime >= errorDuration) {
            Serial.println("[DEBUG] LED: Error display timeout (10s) - Yellow LED auto-off");
            errorMode = false;
            digitalWrite(yellowPin, LOW);
            return;
        }
        
        // 10秒間は常時点灯
        digitalWrite(yellowPin, HIGH);
        return;
    }
    
    // 通常の警告表示（赤LED）
    digitalWrite(yellowPin, LOW);
    unsigned long currentTime = millis();
    unsigned long blinkInterval;
    
    switch(currentLevel) {
        case OFF:
            digitalWrite(redPin, LOW);
            return;
            
        case SOLID:
            digitalWrite(redPin, HIGH);
            return;
            
        case FAST_BLINK:
            blinkInterval = 200;
            break;
            
        case SLOW_BLINK:
            blinkInterval = 500;
            break;
            
        default:
            digitalWrite(redPin, LOW);
            return;
    }
    
    if (currentTime - lastBlinkTime >= blinkInterval) {
        blinkState = !blinkState;
        digitalWrite(redPin, blinkState ? HIGH : LOW);
        lastBlinkTime = currentTime;
    }
}

void LedController::displayFault() {
    if (!errorMode) {
        Serial.println("[DEBUG] LED: Error mode activated - Yellow LED ON for 10 seconds");
        errorStartTime = millis();  // エラー開始時刻を記録
    }
    errorMode = true;
    digitalWrite(redPin, LOW);  // 赤LED消灯
    // 黄色LED常時点灯（10秒間）はupdateBlink()で管理
}

void LedController::clearFault() {
    if (errorMode) {
        Serial.println("[DEBUG] LED: Error mode cleared - Yellow LED off");
    }
    errorMode = false;
    digitalWrite(yellowPin, LOW);
}

void LedController::turnOffAll() {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    currentLevel = OFF;
}
