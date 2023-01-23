#include <Arduino.h>

#include "receiver.h"
#include "receiver_spi.h"
#include "channels.h"
#include "receiver2.h"
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

void setup() {

  Serial.begin(115200);

  Serial.println("The device started");
  // Bluetooth::setup();
  // ReceiverSpi::setup();
  // delay(5000);
  Serial.println("r:alive");
  setupReceiverPins();
  setupDefaultChannels();
  // Receiver::setup(34);
  // delay(5000);
}

void loop()
{
  // Receiver::updateRssi();
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
      startReadingRssi();
      break;
    case CONNECTED:
      break;

    default:
      break;
  }
}