#include <Arduino.h>
#include <BluetoothSerial.h>

#include "bluetooth.h"

namespace Bluetooth {
    BluetoothSerial SerialBT;
    
    void setup() {
        SerialBT.begin("LapTimer");
    }

    void update() {
        SerialBT.println("test");
    }
}