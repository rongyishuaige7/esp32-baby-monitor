#include "audio_feedback.h"
#include "config.h"

AudioFeedback::AudioFeedback() : _muted(false), _currentLevel(SIGNAL_UNKNOWN) {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Source-derived active-low initialization. Confirm on the real circuit.
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
}

void AudioFeedback::setSignalLevel(SignalLevel level) {
    _currentLevel = level;
    setLED(level);
    if (!_muted && level == SIGNAL_ATTENTION) {
        beep(200);
    }
}

void AudioFeedback::update(SignalLevel level) {
    _currentLevel = level;
    // This is only a local threshold indication. It is not an alarm, a
    // notification-delivery guarantee, or evidence that anyone was alerted.
    if (!_muted && level == SIGNAL_HIGH_THRESHOLD) {
        digitalWrite(BUZZER_PIN, LOW);
    } else {
        digitalWrite(BUZZER_PIN, HIGH);
    }
}

void AudioFeedback::playStatusQuery() {
    if (!_muted) {
        beep(100);
        delay(150);
        beep(100);
    }
    setLED(_currentLevel);
}

void AudioFeedback::mute() {
    _muted = true;
    digitalWrite(BUZZER_PIN, HIGH);
}

void AudioFeedback::unmute() {
    _muted = false;
}

bool AudioFeedback::isMuted() const {
    return _muted;
}

SignalLevel AudioFeedback::getSignalLevel() const {
    return _currentLevel;
}

void AudioFeedback::setLED(SignalLevel level) {
    _currentLevel = level;
    // Source-derived active-low mapping: green = reference; yellow =
    // attention; red = high threshold; blue = unknown/no usable reading.
    digitalWrite(LED_R, (level == SIGNAL_HIGH_THRESHOLD || level == SIGNAL_ATTENTION) ? LOW : HIGH);
    digitalWrite(LED_G, (level == SIGNAL_REFERENCE || level == SIGNAL_ATTENTION) ? LOW : HIGH);
    digitalWrite(LED_B, level == SIGNAL_UNKNOWN ? LOW : HIGH);
}

void AudioFeedback::beep(unsigned long durationMs) {
    if (_muted) {
        return;
    }
    digitalWrite(BUZZER_PIN, LOW);
    delay(durationMs);
    digitalWrite(BUZZER_PIN, HIGH);
}
