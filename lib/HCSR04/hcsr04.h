#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

class HCSR04 {
public:
    HCSR04(int trigPin, int echoPin);
    float getDistance();  // 返回距离(cm)

private:
    int _trigPin;
    int _echoPin;
    float _lastDistance;
};

#endif
