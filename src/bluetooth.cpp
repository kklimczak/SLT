#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "bluetooth.h"

#define SERVICE_UUID "58304f18-ffd1-11ec-b939-0242ac120002"
#define CHARACTERISTIC_UUID "583050d0-ffd1-11ec-b939-0242ac120002"

namespace Bluetooth {
    
    void setup() {
        BLEDevice::init("SLT");
        BLEServer *pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(SERVICE_UUID);
        BLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        pCharacteristic->setValue("Booting...");
        pService->start();

        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06);
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
    }

    void update() {
        
    }
}