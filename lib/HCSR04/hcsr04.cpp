#include "hcsr04.h"

HCSR04::HCSR04(int trigPin, int echoPin) : _trigPin(trigPin), _echoPin(echoPin) {
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    _lastDistance = 0;
}

float HCSR04::getDistance() {
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);

    long duration = pulseIn(_echoPin, HIGH, 30000); // 30ms timeout
    float distance = duration * 0.034 / 2;

    if (distance > 0 && distance < 400) {
        _lastDistance = distance;
    }
    return _lastDistance;
}
