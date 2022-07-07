
#ifndef CHANNELS_H
#define CHANNELS_H

#include <stdint.h>

#define CHANNELS_48
#define CHANNELS_SIZE 48


namespace Channels {
    const uint16_t getSynthRegisterB(uint8_t index);
    const uint16_t getSynthRegisterBFreq(uint16_t freq);
    const uint16_t getFrequency(uint8_t index);
    extern char *getName(uint8_t index);
    const uint8_t getOrderedIndex(uint8_t index);
    const uint8_t getOrderedIndexFromIndex(uint8_t index);
    
    const uint8_t getClosestChannel(uint16_t freq);
}

#endif