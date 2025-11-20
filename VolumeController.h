#ifndef VOLUME_CONTROLLER_H
#define VOLUME_CONTROLLER_H

#include <Arduino.h>

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

#endif // VOLUME_CONTROLLER_H
