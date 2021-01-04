// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "drivers/oled.hpp"
#include "main.h"
#include "global.h"

extern "C"
{

using namespace oled;


void sendControlBuffer(uint8_t *buf, uint32_t size) {
	HAL_GPIO_WritePin(SDD133_D_C_GPIO_Port,     SDD133_D_C_Pin,     GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SDD133_CS_PC10_GPIO_Port, SDD133_CS_PC10_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi3, buf, size, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(SDD133_CS_PC10_GPIO_Port, SDD133_CS_PC10_Pin, GPIO_PIN_SET);
}


void drawPixel(uint8_t x, uint8_t y, uint8_t color) {
	uint8_t pixelSetup[] = {
			Control::setColumn, x, Dimensions::width  - 1,
			Control::setRow,    y, Dimensions::height - 1
	};

	sendControlBuffer(pixelSetup, sizeof(pixelSetup) / sizeof(pixelSetup[0]));

	HAL_GPIO_WritePin(SDD133_D_C_GPIO_Port,     SDD133_D_C_Pin,     GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDD133_CS_PC10_GPIO_Port, SDD133_CS_PC10_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi3, &color, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(SDD133_CS_PC10_GPIO_Port, SDD133_CS_PC10_Pin, GPIO_PIN_SET);
}


void init() {
	HAL_GPIO_WritePin(SDD133_Reset_GPIO_Port, SDD133_Reset_Pin, GPIO_PIN_RESET);

	for (volatile int x = 0; x < 64000; x++) {
		x += 1;
	}

	HAL_GPIO_WritePin(SDD133_Reset_GPIO_Port, SDD133_Reset_Pin, GPIO_PIN_SET);

	for (volatile int x = 0; x < 64000; x++) {
		x += 1;
	}

	const auto remapSetting = Remap::incrementVertical| Remap::ramToPinInverted |
			Remap::colorRGB | Remap::leftRightSwapOff | Remap::scanMaxTo0 |
			Remap::oddEvenSplitEnable | Remap::colors256format;

	uint8_t initBuf[] = {
		Control::displayOffSleep,
		Control::contrastA,             0x95,
		Control::contrastB,             0x52,
		Control::contrastC,             0x80,
		Control::masterCurrent,         0x0f,
		Control::prechargeA,            0x64,
		Control::prechargeB,            0x78,
		Control::prechargeC,            0x64,
		Control::remap,                 remapSetting,
		Control::displayStartLine,      0,
		Control::displayOffset,         0,
		Control::displayNormal,
		Control::multiplexRatio,        63,   // 1:63
		Control::masterConfiguration,   0x8E,
		Control::powerSave,             0,
		Control::phasePeriodAdjustemnt, 0x31,
		Control::clockAndDivider,       0xF0,
		Control::prechrgeVoltage,       0x3A,
		Control::voltageVcomh,          VcomhVoltage::vcc0_83,
		Control::deactivateScrolling,
		Control::displayOnNomalMode
	};

	sendControlBuffer(initBuf, sizeof(initBuf) / sizeof(initBuf[0]));
}


void clearScreen() {
	uint8_t clearScreen[] = {
		Control::fillWindow, Fill::enable,
		Control::drawRectangle,
			0, 0, Dimensions::width, Dimensions::height, // whole screen
			0, 0, 0,                                     // black outline
			0, 0, 0,                                     // black fill
	};

	sendControlBuffer(clearScreen, sizeof(clearScreen) / sizeof(clearScreen[0]));
}


void updateScreenFromBuffer(uint8_t *buffer, uint32_t size) {
	uint8_t setupBuf[] = {
			Control::setColumn, 0, Dimensions::width  - 1,
			Control::setRow,    0, Dimensions::height - 1
	};

	sendControlBuffer(setupBuf, 6);

	HAL_GPIO_WritePin(SDD133_D_C_GPIO_Port,     SDD133_D_C_Pin,     GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDD133_CS_PC10_GPIO_Port, SDD133_CS_PC10_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi3, buffer, size, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(SDD133_CS_PC10_GPIO_Port, SDD133_CS_PC10_Pin, GPIO_PIN_SET);
}


} // extern
