#include "sound.h"
#include "../../src/config.h"

SoundDetector::SoundDetector(int pin, int threshold)
    : _pin(pin), _threshold(threshold), _sampleIndex(0), _sampleCount(0),
      _lastSampleTime(0), _cooldownEnd(0) {
    pinMode(_pin, INPUT);
    clearBuffer();
}

void SoundDetector::clearBuffer() {
    for (int i = 0; i < SOUND_SAMPLE_WINDOW; i++) {
        _sampleBuffer[i] = 0;
    }
    _sampleIndex = 0;
    _sampleCount = 0;
}

void SoundDetector::startCooldown(unsigned long ms) {
    _cooldownEnd = millis() + ms;
    clearBuffer();
}

bool SoundDetector::isInCooldown() const {
    return millis() < _cooldownEnd;
}

int SoundDetector::getLevel() {
    int maxVal = 0;
    int minVal = 4095;
    for (int i = 0; i < 50; i++) {
        const int val = analogRead(_pin);
        if (val > maxVal) maxVal = val;
        if (val < minVal) minVal = val;
    }
    return maxVal - minVal;
}

int SoundDetector::getAverage() {
    const unsigned long now = millis();
    if (isInCooldown()) {
        return 0;
    }
    if (now - _lastSampleTime >= 1000) {
        _sampleBuffer[_sampleIndex] = getLevel();
        _sampleIndex = (_sampleIndex + 1) % SOUND_SAMPLE_WINDOW;
        if (_sampleCount < SOUND_SAMPLE_WINDOW) {
            _sampleCount++;
        }
        _lastSampleTime = now;
    }
    if (_sampleCount == 0) {
        return 0;
    }
    int sum = 0;
    for (int i = 0; i < _sampleCount; i++) {
        sum += _sampleBuffer[i];
    }
    return sum / _sampleCount;
}

int SoundDetector::getSampleCount() const {
    return _sampleCount;
}

bool SoundDetector::exceedsThreshold() {
    return !isInCooldown() && getAverage() > _threshold;
}
