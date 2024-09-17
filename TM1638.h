#ifndef TM1638_H
#define TM1638_H

#include "mbed.h"

class TM1638 {
public:
    TM1638(PinName stb, PinName clk, PinName dio);
    void init();
    void setBrightness(uint8_t brightness);
    void displayDigit(uint8_t position, uint8_t data);
    void displayNumber(uint32_t number);
    void clearDisplay();
    uint8_t readKeys();

private:
    void sendCommand(uint8_t command);
    void sendData(uint8_t address, uint8_t data);
    void sendByte(uint8_t data);

    uint8_t digit[8];
    const char digitToSegment[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

    DigitalOut _stb;
    DigitalOut _clk;
    DigitalInOut _dio;
};

#endif
