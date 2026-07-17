#include <Arduino.h>
#include "audio_feedback.h"
#include "button.h"
#include "config.h"
#include "environment.h"
#include "posture.h"
#include "wifi_server.h"
#include "../lib/HCSR04/hcsr04.h"
#include "../lib/SoundDetector/sound.h"

#if __has_include("wifi_credentials.h")
#include "wifi_credentials.h"
#else
#define BABY_MONITOR_WIFI_SSID ""
#define BABY_MONITOR_WIFI_PASSWORD ""
#endif

HCSR04 topSensor(HCSR04_TOP_TRIG, HCSR04_TOP_ECHO);
HCSR04 sideSensor(HCSR04_SIDE_TRIG, HCSR04_SIDE_ECHO);
SoundDetector soundSensor(SOUND_PIN, SOUND_AMPLITUDE_THRESHOLD);

DistancePatternClassifier* g_distanceClassifier;
EnvironmentMonitor* g_env;
AudioFeedback* g_feedback;
SoundDetector* g_sound;
ButtonHandler buttons;
WiFiServerManager wifiServer;

unsigned long lastEnvironmentCheck = 0;
unsigned long lastDistanceCheck = 0;
unsigned long soundSignalUntil = 0;
DistancePattern lastDistancePattern = DISTANCE_UNKNOWN;
EnvironmentLevel lastEnvironmentLevel = ENV_UNKNOWN;

const char* signalLevelName(SignalLevel level) {
    switch (level) {
        case SIGNAL_REFERENCE: return "reference";
        case SIGNAL_ATTENTION: return "attention";
        case SIGNAL_HIGH_THRESHOLD: return "high_threshold";
        case SIGNAL_UNKNOWN: return "unknown";
        default: return "unknown";
    }
}

SignalLevel signalForDistance(DistancePattern pattern) {
    if (pattern == DISTANCE_NEAR_THRESHOLD) return SIGNAL_HIGH_THRESHOLD;
    if (pattern == DISTANCE_SIDE_PATTERN || pattern == DISTANCE_AMBIGUOUS) return SIGNAL_ATTENTION;
    if (pattern == DISTANCE_REFERENCE_PATTERN) return SIGNAL_REFERENCE;
    return SIGNAL_UNKNOWN;
}

SignalLevel signalForEnvironment(EnvironmentLevel level) {
    if (level == ENV_HIGH_THRESHOLD) return SIGNAL_HIGH_THRESHOLD;
    if (level == ENV_ATTENTION) return SIGNAL_ATTENTION;
    if (level == ENV_REFERENCE) return SIGNAL_REFERENCE;
    return SIGNAL_UNKNOWN;
}

void printStatus() {
    Serial.println("=== Demonstration Sensor State ===");
    Serial.printf("Top distance: %.1f cm\n", g_distanceClassifier->getTopDistance());
    Serial.printf("Side distance: %.1f cm\n", g_distanceClassifier->getSideDistance());
    Serial.printf("Distance classification: %s\n", g_distanceClassifier->getPatternString());
    Serial.printf("Temperature: %.1f C\n", g_env->getTemperature());
    Serial.printf("Humidity: %.1f %%\n", g_env->getHumidity());
    Serial.printf("Sound amplitude: %d\n", soundSensor.getLevel());
    Serial.printf("Signal level: %s\n", signalLevelName(g_feedback->getSignalLevel()));
    Serial.printf("Buzzer muted: %s\n", g_feedback->isMuted() ? "yes" : "no");
    Serial.println("This output is not a safety, posture, health, or care conclusion.");
    Serial.println("==================================");
    g_feedback->playStatusQuery();
}

void toggleBuzzerMute() {
    if (g_feedback->isMuted()) {
        g_feedback->unmute();
    } else {
        g_feedback->mute();
    }
}

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.begin(115200);
    delay(1000);

    Serial.println("ESP32 demonstration sensor prototype starting.");
    g_distanceClassifier = new DistancePatternClassifier(&topSensor, &sideSensor);
    g_env = new EnvironmentMonitor(DHT11_PIN);
    g_feedback = new AudioFeedback();
    g_sound = &soundSensor;

    buttons.setStatusCallback(printStatus);
    buttons.setMuteCallback(toggleBuzzerMute);
    g_feedback->setSignalLevel(SIGNAL_UNKNOWN);

    wifiServer.begin(BABY_MONITOR_WIFI_SSID, BABY_MONITOR_WIFI_PASSWORD);
}

void loop() {
    wifiServer.handleClient();
    buttons.check();
    const unsigned long now = millis();

    if (soundSensor.exceedsThreshold()) {
        soundSignalUntil = now + SOUND_SIGNAL_HOLD_MS;
        soundSensor.startCooldown(SIGNAL_COOLDOWN_MS);
        Serial.println("[DEMO] Sound amplitude exceeded the fixed demonstration threshold.");
    }

    if (now - lastDistanceCheck >= 2000) {
        lastDistancePattern = g_distanceClassifier->classify();
        lastDistanceCheck = now;
    }
    if (now - lastEnvironmentCheck >= 10000) {
        lastEnvironmentLevel = g_env->check();
        lastEnvironmentCheck = now;
    }

    SignalLevel signal = SIGNAL_REFERENCE;
    bool hasUnknownReading = false;
    if (now < soundSignalUntil) {
        signal = SIGNAL_HIGH_THRESHOLD;
    }

    const SignalLevel distanceSignal = signalForDistance(lastDistancePattern);
    const SignalLevel environmentSignal = signalForEnvironment(lastEnvironmentLevel);
    if (distanceSignal == SIGNAL_HIGH_THRESHOLD || environmentSignal == SIGNAL_HIGH_THRESHOLD) {
        signal = SIGNAL_HIGH_THRESHOLD;
    } else if (signal != SIGNAL_HIGH_THRESHOLD &&
               (distanceSignal == SIGNAL_ATTENTION || environmentSignal == SIGNAL_ATTENTION)) {
        signal = SIGNAL_ATTENTION;
    }
    hasUnknownReading = distanceSignal == SIGNAL_UNKNOWN || environmentSignal == SIGNAL_UNKNOWN;
    if (signal == SIGNAL_REFERENCE && hasUnknownReading) {
        signal = SIGNAL_UNKNOWN;
    }

    g_feedback->setSignalLevel(signal);
    g_feedback->update(signal);
    delay(100);
}
