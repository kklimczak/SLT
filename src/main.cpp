#include <Arduino.h>

#include "BluetoothSerial.h"
#include "receiver.h"
#include "receiver_spi.h"
#include "channels.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {

  Serial.begin(9600);
  SerialBT.begin("LapTimer");
  Serial.println("The device started");
  ReceiverSpi::setup();
  

  delay(5000);
  Serial.println("Receiver setup...");
  Receiver::setup(0);
}

void loop() {
  Receiver::updateRssi();
  Serial.println(Receiver::rssi);
  SerialBT.println(Receiver::rssi);
}