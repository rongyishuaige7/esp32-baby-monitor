#ifndef CONFIG_H
#define CONFIG_H

// Pin assignments are copied from the selected desktop working tree. They are
// source-confirmed only; verify the actual board, wiring, voltage levels and
// active-low / active-high behavior before connecting hardware.

// HC-SR04 pins
#define HCSR04_TOP_TRIG 5
#define HCSR04_TOP_ECHO 18
#define HCSR04_SIDE_TRIG 19
#define HCSR04_SIDE_ECHO 21

// DHT11 and analog sound input
#define DHT11_PIN 4
#define SOUND_PIN 34

// Buttons: the current code expects HIGH while pressed. Use external pull
// resistors so neither GPIO is left floating.
#define BUTTON_STATUS 22
#define BUTTON_MUTE 23

// RGB LED pins. The feedback implementation currently assumes active-low.
#define LED_R 25
#define LED_G 27
#define LED_B 26

// Buzzer pin. The feedback implementation currently assumes active-low.
#define BUZZER_PIN 2

// Demonstration-only distance thresholds in centimetres. These constants are
// not calibrated for any person, posture, safety condition or medical use.
#define TOP_DISTANCE_REFERENCE_MIN 40.0
#define TOP_DISTANCE_NEAR_MAX 25.0
#define SIDE_DISTANCE_NEAR_THRESHOLD 15.0
#define SIDE_DISTANCE_REFERENCE_MAX 25.0
#define SIDE_DISTANCE_FAR_MIN 35.0

// Demonstration-only environmental and sound thresholds. They are code
// constants, not safety, childcare, medical or sleep recommendations.
#define TEMPERATURE_REFERENCE_MIN 10.0
#define TEMPERATURE_REFERENCE_MAX 26.0
#define HUMIDITY_REFERENCE_MIN 60.0
#define HUMIDITY_REFERENCE_MAX 95.0
#define SOUND_AMPLITUDE_THRESHOLD 700
#define SOUND_SAMPLE_WINDOW 10
#define SIGNAL_COOLDOWN_MS 30000
#define SOUND_SIGNAL_HOLD_MS 5000

// Wi-Fi connection is optional. No default SSID, password or access point is
// provided by the repository.
#define WIFI_CONNECT_TIMEOUT_MS 15000

#endif
