#ifndef AUDIO_FEEDBACK_H
#define AUDIO_FEEDBACK_H

#include <Arduino.h>

// These names describe a local, demonstration-only program classification.
// They do not express safety, health, posture, care or emergency status.
enum SignalLevel {
    SIGNAL_REFERENCE,
    SIGNAL_ATTENTION,
    SIGNAL_HIGH_THRESHOLD,
    SIGNAL_UNKNOWN
};

class AudioFeedback {
public:
    AudioFeedback();
    void setSignalLevel(SignalLevel level);
    void update(SignalLevel level);
    void playStatusQuery();
    void mute();
    void unmute();
    bool isMuted() const;
    SignalLevel getSignalLevel() const;

private:
    void setLED(SignalLevel level);
    void beep(unsigned long durationMs);

    bool _muted;
    SignalLevel _currentLevel;
};

#endif
