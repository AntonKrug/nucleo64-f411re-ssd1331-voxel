// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef _SSD1331_H_
#define _SSD1331_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void oledInit();
void oledClearScreen();
void oledDrawPixel(uint8_t x, uint8_t y, uint8_t color);
void oledUpdateScreenFromBuffer(uint8_t *buffer, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
