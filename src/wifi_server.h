#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>

class WiFiServerManager {
public:
    bool begin(const char* ssid, const char* password);
    void handleClient();
    String getIPAddress() const;
    bool isConnected() const;
    bool isServing() const;

private:
    WebServer _server{80};
    String _ipAddress;
    bool _serving{false};
};

#endif
