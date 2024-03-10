#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "ADS1299.h"
#include "Config.h"
#include "WifiServer.h"
#include "WebServer.h"
#include "SPI.h"

SPIClass *hspi = NULL;

ADS1299 ads1299;

WiFiUDP clientUDP;
WiFiClient clientTCP;

WebServer server(80);
WifiServer board;

void setup()
{
    hspi = new SPIClass(HSPI);
    Serial0.begin(115200);
    ads1299.start();
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_IMU_CS, OUTPUT);
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_IMU_CS, HIGH);
    board.begin();
}

void loop()
{
    if (ads1299.streaming && ads1299.channelDataAvailable)
    {
        // Read from the ADS(s), store data, set channelDataAvailable flag to false
        ads1299.updateChannelData();
        board.sendChannelDataWifi(false);
        if (ads1299.daisyPresent)
            board.sendChannelDataWifi(true);
    }
    board.loop();
}
