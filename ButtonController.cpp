#include "arduino_sample.h"

ButtonController::ButtonController(int pin) 
    : buttonPin(pin), lastState(HIGH), currentState(HIGH), 
      lastDebounceTime(0), debounceDelay(50), toggleState(false) {
}

void ButtonController::begin() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool ButtonController::isPressed() {
    int reading = digitalRead(buttonPin);
    
    if (reading != lastState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != currentState) {
            currentState = reading;
        }
    }
    
    lastState = reading;
    return (currentState == LOW);
}

bool ButtonController::wasToggled() {
    static bool lastPressed = false;
    bool pressed = isPressed();
    
    // 立ち上がりエッジ検出（ボタンが離された瞬間）
    if (!pressed && lastPressed) {
        toggleState = !toggleState;
        Serial.print("[DEBUG] Button toggled: ");
        Serial.println(toggleState ? "ON" : "OFF");
        lastPressed = pressed;
        return true;
    }
    
    lastPressed = pressed;
    return false;
}

bool ButtonController::getToggleState() {
    return toggleState;
}
