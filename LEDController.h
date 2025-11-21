#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

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

#endif // LED_CONTROLLER_H
