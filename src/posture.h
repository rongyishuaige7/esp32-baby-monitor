#ifndef POSTURE_H
#define POSTURE_H

#include "../lib/HCSR04/hcsr04.h"
#include <Arduino.h>

// A distance-pattern classifier, not a posture detector. The values only
// describe fixed combinations of two uncalibrated distance readings.
enum DistancePattern {
    DISTANCE_REFERENCE_PATTERN,
    DISTANCE_SIDE_PATTERN,
    DISTANCE_AMBIGUOUS,
    DISTANCE_NEAR_THRESHOLD,
    DISTANCE_UNKNOWN
};

class DistancePatternClassifier {
public:
    DistancePatternClassifier(HCSR04* topSensor, HCSR04* sideSensor);
    DistancePattern classify();
    float getTopDistance();
    float getSideDistance();
    const char* getPatternString() const;

private:
    HCSR04* _topSensor;
    HCSR04* _sideSensor;
    DistancePattern _lastPattern;
};

#endif
