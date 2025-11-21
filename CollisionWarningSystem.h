#ifndef COLLISION_WARNING_SYSTEM_H
#define COLLISION_WARNING_SYSTEM_H

#include <Arduino.h>
#include "DistanceSensor.h"
#include "LedController.h"
#include "VolumeController.h"
#include "ButtonController.h"

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

#endif // COLLISION_WARNING_SYSTEM_H
