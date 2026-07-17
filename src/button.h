#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

enum ButtonEvent {
    BUTTON_NONE,
    BUTTON_STATUS_PRESS,
    BUTTON_MUTE_PRESS
};

class ButtonHandler {
public:
    ButtonHandler();
    ButtonEvent check();
    void setMuteCallback(void (*callback)());
    void setStatusCallback(void (*callback)());

private:
    int _lastStatusState;
    int _lastMuteState;
    unsigned long _statusDebounceTime;
    unsigned long _muteDebounceTime;
    void (*_onMute)();
    void (*_onStatus)();
};

#endif
