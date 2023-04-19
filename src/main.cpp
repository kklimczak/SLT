#include <Arduino.h>

#include "receiver.h"
#include "receiver_spi.h"
#include "channels.h"
#include "receiver2.h"
// #include "bluetooth.h"

// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

#define LED 2

enum Status
{
  WAITING,
  PREPARE,
  RACE,
};

TaskHandle_t ReceiversTask;
TaskHandle_t MainTask;

Status status = WAITING;
String command = "";
int64_t millis_start;
int64_t micros_start;

void receiversTaskFn(void * pvParameters) {
  while (true) {
    // startReadingRssi(status, micros_start);
    delay(10);
  }
  
}

void commandHandler() {
  if (Serial.available() > 0) {

    command = Serial.readStringUntil('\n');

    String commandCategory = command.substring(0, 1);
    Serial.println(commandCategory);
    if (commandCategory == "s") {
      Serial.println("channel switched");
      String rxNumber = command.substring(2, 3);
      String channel = command.substring(4, 6);
      changeChannel(std::stoi(channel.c_str()), std::stoi(rxNumber.c_str()));
    } else if (commandCategory == "r") {
      String params = command.substring(2, 3);
      if (params == "s") {
        status = PREPARE;
        millis_start = esp_timer_get_time();
      } else if (params == "i") {
        status = WAITING;
        micros_start = 0;
      }
    }
  }
}

void raceHandler() {
  if (status != WAITING) {
    int64_t relative_millis = esp_timer_get_time() - millis_start;

    switch (status)
    {
    case PREPARE:
      if (relative_millis > 5000000) {
        status = RACE;
        micros_start = esp_timer_get_time();
      } else {
        int64_t seconds = relative_millis / 1000000;
        if (relative_millis > seconds * 1000000 && relative_millis < seconds * 1000000 + 200000) {
          digitalWrite(LED, HIGH);
        } else {
          digitalWrite(LED, LOW);
        }
      }
      break;
    case RACE:
      if (relative_millis > 5000000 && relative_millis < 6000000) {
        digitalWrite(LED, HIGH);
      } else {
        digitalWrite(LED, LOW);
      }
    
    default:
      break;
    }
  }
}

void mainTaskFn(void * pvParameters) {
  while (true) {
    commandHandler();
    raceHandler();
  }
  
}

void setup() {

  Serial.begin(115200);
  Serial.println("r:alive");
  pinMode(LED, OUTPUT);   
  setupReceiverPins();
  setupDefaultChannels();

  xTaskCreatePinnedToCore(
    receiversTaskFn,
    "receiversTask",
    10000,
    NULL,
    1,
    &ReceiversTask,
    0
  );

  delay(500);

  xTaskCreatePinnedToCore(
    mainTaskFn,
    "mainTask",
    10000,
    NULL,
    1,
    &MainTask,
    1
  );

    delay(500);
}

void loop() {}