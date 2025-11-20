#include "arduino_sample.h"

ButtonController::ButtonController(int pin) 
    : buttonPin(pin), lastState(HIGH), currentState(HIGH), 
      lastDebounceTime(0), debounceDelay(50) {
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

bool ButtonController::wasPressed() {
    static bool lastPressed = false;
    bool pressed = isPressed();
    bool result = (pressed && !lastPressed);
    lastPressed = pressed;
    return result;
}
