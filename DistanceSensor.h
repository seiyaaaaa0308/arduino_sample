#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <Arduino.h>

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

#endif // DISTANCE_SENSOR_H
