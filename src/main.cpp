#include <Arduino.h>

#include "receiver.h"
#include "receiver_spi.h"
#include "channels.h"
#include "bluetooth.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void setup() {

  Serial.begin(9600);
  
  Serial.println("The device started");
  Bluetooth::setup();
  ReceiverSpi::setup();
  delay(5000);
  Serial.println("Receiver setup...");
  Receiver::setup(0);
}

void loop() {
  Receiver::updateRssi();
  Serial.println(Receiver::rssi);
  Bluetooth::update();
}