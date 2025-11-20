#include "arduino_sample.h"

LedController::LedController(int green, int red, int yellow) 
    : greenPin(green), redPin(red), yellowPin(yellow), currentLevel(OFF),
      lastBlinkTime(0), blinkState(false) {
}

void LedController::begin() {
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    turnOffAll();
}

void LedController::setMonitoringActive(bool active) {
    digitalWrite(greenPin, active ? HIGH : LOW);
}

void LedController::setWarningLevel(WarningLevel level, long distance, long range) {
    currentLevel = level;
    
    // 距離に応じたレベル判定
    if (distance < 0 || distance > range) {
        currentLevel = OFF;
    } else if (distance <= range * 0.25) {
        currentLevel = SOLID;      // 近距離: 常時点灯
    } else if (distance <= range * 0.5) {
        currentLevel = FAST_BLINK; // 中距離: 高速点滅
    } else {
        currentLevel = SLOW_BLINK; // 遠距離: 低速点滅
    }
}

void LedController::updateBlink() {
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
    // 黄色LED点滅（故障通知）
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= 500) {
        blinkState = !blinkState;
        digitalWrite(yellowPin, blinkState ? HIGH : LOW);
        lastBlinkTime = currentTime;
    }
}

void LedController::turnOffAll() {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    currentLevel = OFF;
}
