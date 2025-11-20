#ifndef ARDUINO_SAMPLE_H
#define ARDUINO_SAMPLE_H

#include <Arduino.h>

// LED制御
void ledOn(int pin);
void ledOff(int pin);

// LED PWM制御
void fadeLed(int pin, int delayMs);

// ボタン状態変化検知とLED制御
void printButtonState(int btnPin, int ledPin);

// 距離センサ処理
void printDistance(int trigPin, int echoPin);

#endif // ARDUINO_SAMPLE_H
