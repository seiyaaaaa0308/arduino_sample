// arduino_sample.h
// 衝突警告装置サンプルのヘッダファイル
// オブジェクト指向型で実装（統合ヘッダー）
// デュアルセンサ構成

#ifndef ARDUINO_SAMPLE_H
#define ARDUINO_SAMPLE_H

#include <Arduino.h>

// ピン定義
#define TRIG_PIN_1 8
#define ECHO_PIN_1 9
#define TRIG_PIN_2 4
#define ECHO_PIN_2 5
#define LED_GREEN_PIN 11    // 監視状態表示
#define LED_RED_PIN 10      // 警告表示
#define LED_YELLOW_PIN 6    // 故障通知/拡張用
#define VOLUME_PIN A0       // 監視範囲調整
#define TOGGLE_BUTTON_PIN 13 // 監視開始/停止

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
    bool faulty;

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
    OFF = 0,
    SLOW_BLINK = 1,   // 遠距離（低速点滅）
    FAST_BLINK = 2,   // 中距離（高速点滅）
    SOLID = 3         // 近距離（常時点灯）
};

class LedController {
private:
    int greenPin;      // 監視状態
    int redPin;        // 警告
    int yellowPin;     // 故障/拡張
    WarningLevel currentLevel;
    unsigned long lastBlinkTime;
    bool blinkState;

public:
    LedController(int green, int red, int yellow);
    void begin();
    void setMonitoringActive(bool active);   // 緑LED制御
    void setWarningLevel(WarningLevel level, long distance, long range); // 赤LED制御
    void displayFault();                     // 黄LED点滅
    void updateBlink();                      // 点滅更新
    void turnOffAll();
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
    bool toggleState;

public:
    ButtonController(int pin);
    void begin();
    bool isPressed();
    bool wasToggled();      // トグル検出
    bool getToggleState();  // 現在のトグル状態取得
};

// ========================================
// CollisionWarningSystem クラス
// ========================================
class CollisionWarningSystem {
private:
    DistanceSensor* sensor1;
    DistanceSensor* sensor2;
    LedController* led;
    VolumeController* volume;
    ButtonController* toggleButton;
    bool systemRunning;
    long monitoringRange;
    int faultCounter;
    int faultThreshold;

public:
    CollisionWarningSystem(DistanceSensor* s1, DistanceSensor* s2, 
                          LedController* l, VolumeController* v, 
                          ButtonController* toggle);
    void begin();
    void update();
    bool isRunning();
    void toggleSystem();
    long getMinDistance(long dist1, long dist2);
};

#endif // ARDUINO_SAMPLE_H
