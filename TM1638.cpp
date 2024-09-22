#include "TM1638.h"
#include <cstdint>

TM1638::TM1638(PinName stb, PinName clk, PinName dio) : _stb(stb), _clk(clk), _dio(dio) {
    _dio.output();
}

void TM1638::init() {
    sendCommand(0x8F); // Enciende el display con brillo máximo
}

void TM1638::setBrightness(uint8_t brightness) {
    if (brightness > 7) brightness = 7;
    sendCommand(0x88 | brightness);
}

void TM1638::displayDigit(uint8_t position, uint8_t data) {
    sendData(position << 1, digitToSegment[data]);
}

void TM1638::displayLed  (uint8_t led, bool state){
    uint8_t data;
    data = state ? 0x01:0x00;
    sendData ((led << 1) + 1, data); 
}


void TM1638::displayNumber(uint32_t number) {
    for (int i = 0; i < 8; i++) {
        digit[i] = number % 10;
        displayDigit(i, digit[7-i]);
        number /= 10;
    }
}

void TM1638::clearLeds(){
for (int i = 0; i < 8; i++) {
        displayLed(i, 0x00);
    }
}




void TM1638::clearDisplay() {
    for (int i = 0; i < 8; i++) {
        displayDigit(i, 0x00);
    }
}

uint8_t TM1638::readKeys() {
    uint8_t keys;
    _stb = 0;
    sendByte(0x42);
    keys = 0;
    _dio.input();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            keys |= ((_dio.read() << j) << i);
            _clk = 1;
            wait_us(1);
            _clk = 0;
        }
    }
    _dio.output();
    _stb = 1;
    for (int k=0; k<8; k++){
        if (keys>>k == 0x01) keys = k+1;
    }

    return keys;
}

void TM1638::sendCommand(uint8_t command) {
    _stb = 0;
    sendByte(command);
    _stb = 1;
}

void TM1638::sendData(uint8_t address, uint8_t data) {
    sendCommand(0x44);
    _stb = 0;
    sendByte(0xC0 | address);
    sendByte(data);
    _stb = 1;
}



void TM1638::sendByte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        _dio = (data & 0x01);
        _clk = 1;
        wait_us(1);
        _clk = 0;
        data >>= 1;
    }
}
