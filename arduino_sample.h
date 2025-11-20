// arduino_sample.h
// 衝突警告装置サンプルのヘッダファイル
// オブジェクト指向型で実装（統合ヘッダー）

#ifndef ARDUINO_SAMPLE_H
#define ARDUINO_SAMPLE_H

#include <Arduino.h>

// ピン定義
#define TRIG_PIN 8
#define ECHO_PIN 9
#define LED_RED_PIN 9
#define LED_GREEN_PIN 10
#define LED_BLUE_PIN 11
#define VOLUME_PIN A0
#define START_BUTTON_PIN 2
#define STOP_BUTTON_PIN 3

// ========================================
// DistanceSensor クラス
// ========================================
class DistanceSensor {
private:
    int trigPin;
    int echoPin;
    long maxDistance;
    int errorCount;
    int errorThreshold;

public:
    DistanceSensor(int trig, int echo, long maxDist = 400);
    void begin();
    long getDistance();
    bool isFaulty();
    void resetErrorCount();
};

// ========================================
// LedController クラス
// ========================================
enum WarningLevel {
    NONE = 0,
    WEAK = 1,
    MEDIUM = 2,
    STRONG = 3
};

class LedController {
private:
    int redPin;
    int greenPin;
    int bluePin;
    WarningLevel currentLevel;

public:
    LedController(int r, int g, int b);
    void begin();
    void setWarningLevel(WarningLevel level);
    void displayWarning(long distance);
    void displayFault();
    void turnOff();
};

// ========================================
// VolumeController クラス
// ========================================
class VolumeController {
private:
    int volumePin;
    long minRange;
    long maxRange;

public:
    VolumeController(int pin, long minR = 50, long maxR = 400);
    void begin();
    long getMonitoringRange();
};

// ========================================
// ButtonController クラス
// ========================================
class ButtonController {
private:
    int buttonPin;
    bool lastState;
    bool currentState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

public:
    ButtonController(int pin);
    void begin();
    bool isPressed();
    bool wasPressed();
};

// ========================================
// CollisionWarningSystem クラス
// ========================================
class CollisionWarningSystem {
private:
    DistanceSensor* sensor;
    LedController* led;
    VolumeController* volume;
    ButtonController* startButton;
    ButtonController* stopButton;
    bool systemRunning;
    long monitoringRange;

public:
    CollisionWarningSystem(DistanceSensor* s, LedController* l, 
                          VolumeController* v, ButtonController* start, 
                          ButtonController* stop);
    void begin();
    void update();
    bool isRunning();
    void start();
    void stop();
};

#endif // ARDUINO_SAMPLE_H
