// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)


#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi3;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;

#endif /* INC_GLOBAL_H_ */
