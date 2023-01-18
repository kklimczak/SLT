#include <Arduino.h>

#include "receiver.h"
#include "receiver_spi.h"
#include "channels.h"
// #include "bluetooth.h"

// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

enum Status
{
  WAITING,
  CONNECTED
};

Status status = WAITING;
String command = "";


void sendRSSI()
{
  Serial.print("r:reading:1/");
  Serial.print(Receiver::rssi);
  Serial.print(";2/");
  Serial.println(Receiver::rssi2);
}

void setup()
{

  Serial.begin(115200);

  Serial.println("The device started");
  // Bluetooth::setup();
  ReceiverSpi::setup();
  delay(5000);
  Serial.println("r:alive");
  Receiver::setup(34);
}

void loop()
{
  Receiver::updateRssi();
  // Bluetooth::update();

  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');

    if (command.equals("c")) {
      Serial.println("r:connected");
      status = CONNECTED;
    }
  }

  switch (status) {
    case WAITING:
      // Serial.println("r:pong");
      // delay(500);
      sendRSSI();
      break;
    case CONNECTED:
      sendRSSI();
      break;

    default:
      break;
  }
}