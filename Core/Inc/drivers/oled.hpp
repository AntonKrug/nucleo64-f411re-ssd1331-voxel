// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef _SSD1331_H_
#define _SSD1331_H_

#include <stdio.h>

//#define OLED_SYNC

extern "C" {
namespace oled {


enum Dimensions: uint8_t {
	width  = 96,
	height = 64
};


enum Control: uint8_t {
	setColumn               = 0x15,
	drawRectangle           = 0x22,
	fillWindow              = 0x26,
	deactivateScrolling     = 0x2e,
	setRow                  = 0x75,
	contrastA               = 0x81,
	contrastB               = 0x82,
	contrastC               = 0x82,
	masterCurrent           = 0x87,
	prechargeA              = 0x8a,
	prechargeB              = 0x8b,
	prechargeC              = 0x8c,
	remap                   = 0xa0,
	displayStartLine        = 0xa1,
	displayOffset           = 0xa2,
	displayNormal           = 0xa4,
	displayPixelsOn         = 0xa5,
	displayPixelsOff        = 0xa6,
	displayInverse          = 0xa7,
	multiplexRatio          = 0xa8,
	dimModeSetting          = 0xab,
	dimModeDisplayOn        = 0xac,
	masterConfiguration     = 0xad,
	displayOffSleep         = 0xae,
	displayOnNomalMode      = 0xaf,
	powerSave               = 0xb0,
	phasePeriodAdjustemnt   = 0xb1,
	clockAndDivider         = 0xb3,
	grayScaleTableConfigure = 0xb8,
	grayScaleTableEnable    = 0xb9,
	prechrgeVoltage         = 0xbb,
	voltageVcomh            = 0xbe
};


enum Remap: uint8_t {
	incrementHorizontal = 0x00,
	incrementVertical   = 0x01,

	ramToPinSame        = 0x00,
	ramToPinInverted    = 0x02,

	colorRGB            = 0x00,
	colorBGR            = 0x04,

	leftRightSwapOff    = 0x00,
	leftRightSwapOn     = 0x08,

	scan0ToMax          = 0x00,
	scanMaxTo0          = 0x10,

	oddEvenSplitDisable = 0x00,
	oddEvenSplitEnable  = 0x20,

	colors256format     = 0x00,
	colors65kformat     = 0x40,
	colors65kformat2    = 0x80
};


enum VcomhVoltage: uint8_t {
	vcc0_44 = 0x00,
    vcc0_52 = 0x10,
    vcc0_61 = 0x20,
    vcc0_71 = 0x30,
    vcc0_83 = 0x3e
};


enum Fill: uint8_t {
	disable = 0,
	enable  = 1
};


void init();
void clearScreen();
void drawPixel(uint8_t x, uint8_t y, uint8_t color);
void updateScreenFromBuffer(uint8_t *buffer, uint32_t size);


} // namespace
} // extern

#endif
