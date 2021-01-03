// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "drivers/oled.hpp"

extern "C" {


void oledInit() {
	oled::init();
}


void oledClearScreen() {
	oled::clearScreen();
}


void oledDrawPixel(uint16_t x, uint16_t y, uint8_t color) {
	oled::drawPixel(x, y, color);
}


void oledUpdateScreenFromBuffer(uint8_t *buffer, uint32_t size) {
	oled::updateScreenFromBuffer(buffer, size);
}


} // extern
