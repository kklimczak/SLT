#ifndef RECEIVER2_H
#define RECEIVER2_H

#include <stdint.h>
#include <SPI.h>

#define SPI_MOSI 23
#define SPI_SCK 18
#define RECEIVER_1_PIN 32
#define RECEIVER_2_PIN 33
// #define RECEIVER_3_PIN 32
// #define RECEIVER_4_PIN 33

#define RSSI_1_PIN 27
#define RSSI_2_PIN 4

#define RECEIVERS_NUMBER 2

#define SPI_ADDRESS_SYNTH_B 0x01
#define SPI_ADDRESS_POWER   0x0A
#define SPI_ADDRESS_STATE   0x0F

void setupReceiverPins();
void setupDefaultChannels();
void sendRegister(uint8_t addressBits, uint32_t dataBits, int receiverPin);
void readRssiFromReceivers();
void changeChannel(uint8_t value, int receiverPin);
int getPinForReceiver(int receiverId);
void startReadingRssi(int status, int64_t millis_start);

#endif