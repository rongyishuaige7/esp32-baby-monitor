#include "environment.h"
#include "config.h"

EnvironmentMonitor::EnvironmentMonitor(int dhtPin)
    : _dht(new DHT(dhtPin, DHT11)), _level(ENV_UNKNOWN), _temperature(NAN), _humidity(NAN) {
    _dht->begin();
}

EnvironmentLevel EnvironmentMonitor::check() {
    _temperature = _dht->readTemperature();
    _humidity = _dht->readHumidity();

    if (isnan(_temperature) || isnan(_humidity)) {
        _level = ENV_UNKNOWN;
        return _level;
    }

    if (_temperature < TEMPERATURE_REFERENCE_MIN || _temperature > TEMPERATURE_REFERENCE_MAX) {
        _level = ENV_HIGH_THRESHOLD;
    } else if (_humidity < HUMIDITY_REFERENCE_MIN || _humidity > HUMIDITY_REFERENCE_MAX) {
        _level = ENV_ATTENTION;
    } else {
        _level = ENV_REFERENCE;
    }
    return _level;
}

float EnvironmentMonitor::getTemperature() {
    return _temperature;
}

float EnvironmentMonitor::getHumidity() {
    return _humidity;
}

bool EnvironmentMonitor::hasUsableReading() const {
    return !isnan(_temperature) && !isnan(_humidity);
}
