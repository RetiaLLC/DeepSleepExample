#include <ESP8266WiFi.h>
#include "AdafruitIO_WiFi.h"

// Skicka & ChatGPT4 <3 11/17/2023
// Replace these with your WiFi and Adafruit IO credentials
#define WIFI_SSID "EXAMPLE"
#define WIFI_PASS "EXAMPLE"
#define IO_USERNAME  "EXAMPLE"
#define IO_KEY       "EXAMPLE"

const int analogPin = A0;
const float conversionRatio = 0.00406;

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *voltageFeed = io.feed("voltage_regular_sleep");

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
}

void loop() {
  connectToNetwork();

  // Read and send battery voltage
  sendBatteryVoltage();

  // Delay to ensure data is sent before sleeping
  delay(5000); // 5 seconds delay

  // Regular Sleep for 55 seconds to maintain 60-second intervals
  delay(55000);
}

void connectToNetwork() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
  }

  if (io.status() < AIO_CONNECTED) {
    io.connect();
    while (io.status() < AIO_CONNECTED) {
      delay(1000);
    }
  }
}

void sendBatteryVoltage() {
  int rawValue = analogRead(analogPin);
  float batteryVoltage = rawValue * conversionRatio;
  if (voltageFeed->save(batteryVoltage)) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Failed to send data");
  }
}
