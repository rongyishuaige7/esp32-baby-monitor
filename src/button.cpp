#include "button.h"
#include "config.h"

ButtonHandler::ButtonHandler() : _lastStatusState(LOW), _lastMuteState(LOW),
    _statusDebounceTime(0), _muteDebounceTime(0), _onMute(nullptr), _onStatus(nullptr) {
    // The selected source expects external pull resistors and HIGH when
    // pressed. This is a source-level assumption that needs real-hardware
    // verification; do not leave these inputs floating.
    pinMode(BUTTON_STATUS, INPUT);
    pinMode(BUTTON_MUTE, INPUT);
}

ButtonEvent ButtonHandler::check() {
    const int statusBtn = digitalRead(BUTTON_STATUS);
    const int muteBtn = digitalRead(BUTTON_MUTE);
    const unsigned long now = millis();

    if (statusBtn == HIGH && _lastStatusState == LOW && now - _statusDebounceTime > 50) {
        _statusDebounceTime = now;
        if (_onStatus != nullptr) _onStatus();
        _lastStatusState = statusBtn;
        _lastMuteState = muteBtn;
        return BUTTON_STATUS_PRESS;
    }
    if (muteBtn == HIGH && _lastMuteState == LOW && now - _muteDebounceTime > 50) {
        _muteDebounceTime = now;
        if (_onMute != nullptr) _onMute();
        _lastStatusState = statusBtn;
        _lastMuteState = muteBtn;
        return BUTTON_MUTE_PRESS;
    }
    _lastStatusState = statusBtn;
    _lastMuteState = muteBtn;
    return BUTTON_NONE;
}

void ButtonHandler::setMuteCallback(void (*callback)()) {
    _onMute = callback;
}

void ButtonHandler::setStatusCallback(void (*callback)()) {
    _onStatus = callback;
}
