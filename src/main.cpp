#include <Arduino.h>
#include "SimpleWiFiManager.hpp"

SimpleWiFiManager wifiManager;

void setup() {
  // put your setup code here, to run once:
  wifiManager.begin();
}

void loop() {
  wifiManager.task();
}