#include "config.h"
#include "wifi_server.h"
#include "audio_feedback.h"
#include "environment.h"
#include "posture.h"
#include "../lib/SoundDetector/sound.h"

extern DistancePatternClassifier* g_distanceClassifier;
extern EnvironmentMonitor* g_env;
extern AudioFeedback* g_feedback;
extern SoundDetector* g_sound;

namespace {
const char* signalLevelName(SignalLevel level) {
    switch (level) {
        case SIGNAL_REFERENCE: return "reference";
        case SIGNAL_ATTENTION: return "attention";
        case SIGNAL_HIGH_THRESHOLD: return "high_threshold";
        case SIGNAL_UNKNOWN: return "unknown";
        default: return "unknown";
    }
}
}  // namespace

bool WiFiServerManager::begin(const char* ssid, const char* password) {
    if (ssid == nullptr || password == nullptr || strlen(ssid) == 0 || strlen(password) == 0) {
        Serial.println("[HTTP] Wi-Fi credentials not configured; local HTTP is disabled.");
        return false;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("[HTTP] Connecting to configured Wi-Fi without printing credentials...");
    const unsigned long startedAt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startedAt < WIFI_CONNECT_TIMEOUT_MS) {
        delay(250);
    }
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] Wi-Fi connection timed out; local HTTP remains disabled.");
        WiFi.disconnect(true);
        return false;
    }

    _ipAddress = WiFi.localIP().toString();
    _server.on("/api/status", HTTP_GET, [this]() {
        if (g_distanceClassifier == nullptr || g_env == nullptr || g_feedback == nullptr || g_sound == nullptr) {
            _server.send(500, "application/json", "{\"error\":\"runtime_not_initialized\"}");
            return;
        }

        const DistancePattern pattern = g_distanceClassifier->classify();
        StaticJsonDocument<640> doc;
        doc["scope"] = "demonstration_only";
        doc["distanceClassification"] = g_distanceClassifier->getPatternString();
        doc["topDistanceCm"] = g_distanceClassifier->getTopDistance();
        doc["sideDistanceCm"] = g_distanceClassifier->getSideDistance();
        doc["temperatureC"] = g_env->getTemperature();
        doc["humidityPercent"] = g_env->getHumidity();
        doc["environmentReadingUsable"] = g_env->hasUsableReading();
        doc["soundAmplitude"] = g_sound->getLevel();
        doc["signalLevel"] = signalLevelName(g_feedback->getSignalLevel());
        doc["buzzerMuted"] = g_feedback->isMuted();
        doc["distancePatternUsable"] = pattern != DISTANCE_UNKNOWN;

        String response;
        serializeJson(doc, response);
        _server.send(200, "application/json", response);
    });

    _server.on("/api/mute", HTTP_POST, [this]() {
        if (g_feedback == nullptr) {
            _server.send(500, "application/json", "{\"error\":\"feedback_not_initialized\"}");
            return;
        }
        if (g_feedback->isMuted()) {
            g_feedback->unmute();
        } else {
            g_feedback->mute();
        }
        _server.send(200, "application/json", g_feedback->isMuted()
            ? "{\"scope\":\"demonstration_only\",\"buzzerMuted\":true}"
            : "{\"scope\":\"demonstration_only\",\"buzzerMuted\":false}");
    });

    _server.begin();
    _serving = true;
    Serial.print("[HTTP] Local unauthenticated demonstration API started at http://");
    Serial.println(_ipAddress);
    return true;
}

void WiFiServerManager::handleClient() {
    if (_serving) {
        _server.handleClient();
    }
}

String WiFiServerManager::getIPAddress() const {
    return _ipAddress;
}

bool WiFiServerManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiServerManager::isServing() const {
    return _serving;
}
