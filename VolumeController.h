#ifndef VOLUME_CONTROLLER_H
#define VOLUME_CONTROLLER_H

#include <Arduino.h>

class VolumeController {
private:
    int volumePin;

public:
    VolumeController(int pin);
    void begin();
    void getDistanceParameters(float &slowBlink, float &fastBlink, float &solid);
};

#endif // VOLUME_CONTROLLER_H
