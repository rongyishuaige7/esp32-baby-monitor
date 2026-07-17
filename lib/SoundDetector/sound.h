#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>
#include "../../src/config.h"

class SoundDetector {
public:
    SoundDetector(int pin, int threshold);
    int getLevel();
    int getAverage();
    int getSampleCount() const;
    bool exceedsThreshold();
    void startCooldown(unsigned long ms);
    bool isInCooldown() const;

private:
    void clearBuffer();

    int _pin;
    int _threshold;
    int _sampleBuffer[SOUND_SAMPLE_WINDOW];
    int _sampleIndex;
    int _sampleCount;
    unsigned long _lastSampleTime;
    unsigned long _cooldownEnd;
};

#endif
