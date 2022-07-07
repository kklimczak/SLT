#include "channels.h"
#include "receiver.h"
#include "Arduino.h"

// Channels with their Mhz Values
static const uint16_t channelFreqTable[] PROGMEM = {
    5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725, // A
    5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866, // B
    5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945, // E
    5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880, // F / Airwave
    5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917, // R / Immersion Raceband
    5362, 5399, 5436, 5473, 5510, 5547, 5584, 5621, // L
    5325, 5348, 5366, 5384, 5402, 5420, 5438, 5456, // U
    5474, 5492, 5510, 5528, 5546, 5564, 5582, 5600, // O
    5653, 5693, 5733, 5773, 5813, 5853, 5893, 5933  // H
};

#ifdef CHANNELS_48
// All Channels of the above List ordered by Mhz
static const uint8_t channelFreqOrderedIndex[] PROGMEM = {
        40, // 5362
        41, // 5399
        42, // 5436
        43, // 5473
        44, // 5510
        45, // 5547
        46, // 5584
        47, // 5621
        19, // 5645
        32, // 5658
        18, // 5665
        17, // 5685
        33, // 5695
        16, // 5705
         7, // 5725
        34, // 5732
         8, // 5733
        24, // 5740
         6, // 5745
         9, // 5752
        25, // 5760
         5, // 5765
        35, // 5769
        10, // 5771
        26, // 5780
         4, // 5785
        11, // 5790
        27, // 5800
         3, // 5805
        36, // 5806
        12, // 5809
        28, // 5820
         2, // 5825
        13, // 5828
        29, // 5840
        37, // 5843
         1, // 5845
        14, // 5847
        30, // 5860
         0, // 5865
        15, // 5866
        31, // 5880
        38, // 5880
        20, // 5885
        21, // 5905
        39, // 5917
        22, // 5925
        23  // 5945
};

static const uint8_t channelIndexToOrderedIndex[] PROGMEM = {
        39,
        36,
        32,
        28,
        25,
        21,
        18,
        14,
        16,
        19,
        23,
        26,
        30,
        33,
        37,
        40,
        13,
        11,
        10,
         8,
        43,
        44,
        46,
        47,
        17,
        20,
        24,
        27,
        31,
        34,
        38,
        41,
         9,
        12,
        15,
        22,
        29,
        35,
        42,
        45,
         0,
         1,
         2,
         3,
         4,
         5,
         6,
         7
};
#endif

namespace Channels {
    const uint16_t getSynthRegisterB(uint8_t index) {
        return getSynthRegisterBFreq(getFrequency(index));
    }
    
    const uint16_t getSynthRegisterBFreq(uint16_t f) {
      return ((((f - 479) / 2) / 32) << 7) | (((f - 479) / 2) % 32);
    }

    const uint16_t getFrequency(uint8_t index) {
        return pgm_read_word_near(channelFreqTable + index);
    }
    
    // Returns channel name as a string.
    char bandNames[] = {
        65,    // A  https://www.arduino.cc/en/Reference/ASCIIchart
        66,    // B
        69,    // E
        70,    // F
        82,    // R
        76,    // L
        85,    // U
        79,    // O
        72     // H
        };
    char nameBuffer[2];
    char *getName(uint8_t index) {
        uint8_t band = index / 8;
        uint8_t channel = 48 + (index % 8) + 1;   // https://www.arduino.cc/en/Reference/ASCIIchart
        
        nameBuffer[0] = bandNames[band];
        nameBuffer[1] = channel;
        
        return nameBuffer;
    }

    const uint8_t getOrderedIndex(uint8_t index) {
        return pgm_read_byte_near(channelFreqOrderedIndex + index);
    }

    const uint8_t getOrderedIndexFromIndex(uint8_t index) {
        return pgm_read_byte_near(channelIndexToOrderedIndex + index);
    }

    const uint8_t getClosestChannel(uint16_t freq) {

        uint8_t closestChannel = 0;
        for (int j=0; j<CHANNELS_SIZE; j++) {
            if ( 
                max( Channels::getFrequency(j), freq ) - min( Channels::getFrequency(j), freq ) <= 
                max( Channels::getFrequency(closestChannel), freq ) - min( Channels::getFrequency(closestChannel), freq )  
                ) {
              closestChannel = j;
            }
        }

        return closestChannel;
    }
}