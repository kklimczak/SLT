#include <Arduino.h>
#include <timer.h>
#include "receiver2.h"
#include "channels.h"

int pinsByReceiverId[RECEIVERS_NUMBER] = {RECEIVER_1_PIN, RECEIVER_2_PIN};
int rssiPinsByReceiverId[RECEIVERS_NUMBER] = {RSSI_1_PIN, RSSI_2_PIN};
Timer rssiStableTimer = Timer(30);
int lastRssiValues[RECEIVERS_NUMBER];

void setupReceiverPins() {
    pinMode(SPI_MOSI, OUTPUT);
    pinMode(SPI_SCK, OUTPUT);
    for (int i = 0; i < RECEIVERS_NUMBER; i++) {
        pinMode(pinsByReceiverId[i], OUTPUT);
        pinMode(rssiPinsByReceiverId[i], INPUT);
        digitalWrite(pinsByReceiverId[i], HIGH);
    }

    // RX5808 has not full SPI interface implemented so MISO is not required and I used the pin for this SPI interface
    SPI.begin(SPI_SCK, 19, SPI_MOSI);
}

void setupDefaultChannels() {
    changeChannel(32, RECEIVER_1_PIN);
    changeChannel(38, RECEIVER_2_PIN);
}

void sendRegister(uint8_t addressBits, uint32_t dataBits, int receiverPin) {
    uint32_t data = addressBits | (1 << 4) | (dataBits << 5);
    SPI.beginTransaction(SPISettings(1000000, LSBFIRST, SPI_MODE0));
    digitalWrite(receiverPin, LOW);
    SPI.transferBits(data, NULL, 25);  
    digitalWrite(receiverPin, HIGH);
    SPI.endTransaction();    
    
}

void readRssiFromReceivers() {
    int rssiValues[RECEIVERS_NUMBER] = {0};

    uint8_t RSSI_READS = 15;

    for (uint8_t i = 0; i < RSSI_READS; i++) { 
        for (int j = 0; j < RECEIVERS_NUMBER; j++) {
            rssiValues[j] += analogRead(rssiPinsByReceiverId[j]);
        }
    }

    for (int j = 0; j < RECEIVERS_NUMBER; j++) {
        lastRssiValues[j] = constrain(
            map(
                rssiValues[j] / RSSI_READS,
                0,
                2000,
                0,
                1000
            ),
            0,
            1000
        );
    }
}

void changeChannel(uint8_t value, int receiverPin) {
    sendRegister(SPI_ADDRESS_SYNTH_B, Channels::getSynthRegisterB(value), receiverPin);
    rssiStableTimer.reset();
}

int getPinForReceiver(int receiverId) {
    return pinsByReceiverId[receiverId];
}

// status:millis:rssi_values
void startReadingRssi(int status, int64_t millis_start)
{
    if (rssiStableTimer.hasTicked()) {
        readRssiFromReceivers();
        Serial.print(status);
        Serial.print(":");
        Serial.print(esp_timer_get_time() - millis_start);
        Serial.print(":");
        for (int j = 0; j < RECEIVERS_NUMBER; j++) {
            Serial.print(j + 1);
            Serial.print("/");
            Serial.print(lastRssiValues[j]);
            if (j < RECEIVERS_NUMBER - 1) {
                Serial.print("+");
            }
        }
        Serial.println("");
    }
}