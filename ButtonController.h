#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include <Arduino.h>

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

#endif // BUTTON_CONTROLLER_H
