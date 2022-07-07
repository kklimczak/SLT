#include <Arduino.h>

#include "receiver.h"
#include "receiver_spi.h"
#include "channels.h"
#include "timer.h"

namespace Receiver {
    ReceiverId activeReceiver = ReceiverId::A;
    uint8_t activeChannel = 0;
    
    uint16_t  rssi = 0;
    uint32_t rssiRaw = 0;
    uint16_t  rssiLast[RECEIVER_LAST_DATA_SIZE] = { 0 };

    uint16_t rssiBandScanData[CHANNELS_SIZE] = { 0 };

    // static Timer diversityHysteresisTimer = Timer(5); // default value and is replce by value stored in eeprom during setup

    static Timer rssiStableTimer = Timer(30); // default value and is replce by value stored in eeprom during setup
    // static Timer rssiLogTimer = Timer(RECEIVER_LAST_DELAY);

    bool hasRssiUpdated = false;

    void setChannel(uint8_t channel)
    {
        ReceiverSpi::setSynthRegisterB(Channels::getSynthRegisterB(channel));

        rssiStableTimer.reset();
        activeChannel = channel;

        hasRssiUpdated = false;
    }

    bool isRssiStable() {
        return true; // rssiStableTimer.hasTicked();
    }

    void updateRssi() {
  
        uint8_t RSSI_READS = 15; //15;
        
        rssiRaw = 0;
        for (uint8_t i = 0; i < RSSI_READS; i++) {                       
            rssiRaw += analogRead(4);
        }
        rssiRaw /= RSSI_READS;


        rssi = constrain(
                map(
                    rssiRaw,
                    500,
                    2000,
                    0,
                    1000
                ),
                0,
                1000
            );

        // if (rssiLogTimer.hasTicked()) {
        //     for (uint8_t i = 0; i < RECEIVER_LAST_DATA_SIZE - 1; i++) {
        //         rssiALast[i] = rssiALast[i + 1];
        //         rssiBLast[i] = rssiBLast[i + 1];                
        //         if (EepromSettings.quadversity) {
        //             rssiCLast[i] = rssiCLast[i + 1];
        //             rssiDLast[i] = rssiDLast[i + 1];
        //         }
        //     }

        //     rssiALast[RECEIVER_LAST_DATA_SIZE - 1] = rssiA;
        //     rssiBLast[RECEIVER_LAST_DATA_SIZE - 1] = rssiB;
        //     if (EepromSettings.quadversity) {
        //         rssiCLast[RECEIVER_LAST_DATA_SIZE - 1] = rssiC;
        //         rssiDLast[RECEIVER_LAST_DATA_SIZE - 1] = rssiD;
        //     }

        //     rssiLogTimer.reset();
        // hasRssiUpdated = true;
        // }

    }

    void setup(uint8_t channel) {
        #ifdef DISABLE_AUDIO
            ReceiverSpi::setPowerDownRegister(0b00010000110111110011);
        #endif
        setChannel(channel);   
    }

    void update() {
        if (rssiStableTimer.hasTicked()) {
            updateRssi();
        }
        
    }
}