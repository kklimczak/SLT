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
  pinMode(23, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  SPI.begin(18, 19, 23);
  pinMode(4, INPUT);

  delay(5000);
  Serial.println("Receiver setup...");
  Receiver::setup(0);
}

void loop() {
  Receiver::updateRssi();
  Serial.println(Receiver::rssi);
  SerialBT.println(Receiver::rssi);
}