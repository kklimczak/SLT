#ifndef RECEIVER_H
#define RECEIVER_H


#include <stdint.h>
#include "channels.h"


#define RECEIVER_LAST_DELAY 50
#define RECEIVER_LAST_DATA_SIZE 128 //64


namespace Receiver {

    enum class ReceiverId : uint8_t {
        A
        ,
        B
        ,
        C
        ,
        D
    };

    enum class DiversityMode : uint8_t {
        ANTENNA_A,
        ANTENNA_B,
        ANTENNA_C,
        ANTENNA_D,
        DIVERSITY,
        QUADVERSITY
    };
    
    extern ReceiverId activeReceiver;
    extern uint8_t activeChannel;

    extern uint16_t  rssi;
    extern uint32_t rssiRaw;
    extern uint16_t  rssi2;
    extern uint32_t rssiRaw2;
    extern uint16_t  rssiLast[RECEIVER_LAST_DATA_SIZE];
    
    extern uint16_t rssiBandScanData[CHANNELS_SIZE];

    void setChannel(uint8_t channel);
    void updateRssi();
    bool isRssiStable();
    extern bool hasRssiUpdated;

    void setup(uint8_t channel);
    void update();
}
#endif