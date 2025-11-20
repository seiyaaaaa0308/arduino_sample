#include "arduino_sample.h"

LedController::LedController(int r, int g, int b) 
    : redPin(r), greenPin(g), bluePin(b), currentLevel(NONE) {
}

void LedController::begin() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    turnOff();
}

void LedController::setWarningLevel(WarningLevel level) {
    currentLevel = level;
    
    switch(level) {
        case NONE:
            turnOff();
            break;
        case WEAK:  // 緑
            analogWrite(redPin, 0);
            analogWrite(greenPin, 255);
            analogWrite(bluePin, 0);
            break;
        case MEDIUM:  // 黄
            analogWrite(redPin, 255);
            analogWrite(greenPin, 255);
            analogWrite(bluePin, 0);
            break;
        case STRONG:  // 赤
            analogWrite(redPin, 255);
            analogWrite(greenPin, 0);
            analogWrite(bluePin, 0);
            break;
    }
}

void LedController::displayWarning(long distance) {
    if (distance < 0) {
        return;
    }
    
    // 距離に応じた警告レベルの決定
    // 50cm以内: 強(赤)、50-100cm: 中(黄)、100-200cm: 弱(緑)
    if (distance <= 50) {
        setWarningLevel(STRONG);
    } else if (distance <= 100) {
        setWarningLevel(MEDIUM);
    } else if (distance <= 200) {
        setWarningLevel(WEAK);
    } else {
        setWarningLevel(NONE);
    }
}

void LedController::displayFault() {
    // 故障通知: 青色点滅
    static unsigned long lastBlink = 0;
    static bool blinkState = false;
    
    if (millis() - lastBlink > 500) {
        blinkState = !blinkState;
        lastBlink = millis();
        
        if (blinkState) {
            analogWrite(redPin, 0);
            analogWrite(greenPin, 0);
            analogWrite(bluePin, 255);
        } else {
            turnOff();
        }
    }
}

void LedController::turnOff() {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    currentLevel = NONE;
}
