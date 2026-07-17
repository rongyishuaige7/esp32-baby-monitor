#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Arduino.h>
#include <DHT.h>

// Demonstration-only classifications based on fixed code thresholds.
enum EnvironmentLevel {
    ENV_REFERENCE,
    ENV_ATTENTION,
    ENV_HIGH_THRESHOLD,
    ENV_UNKNOWN
};

class EnvironmentMonitor {
public:
    EnvironmentMonitor(int dhtPin);
    EnvironmentLevel check();
    float getTemperature();
    float getHumidity();
    bool hasUsableReading() const;

private:
    DHT* _dht;
    EnvironmentLevel _level;
    float _temperature;
    float _humidity;
};

#endif
