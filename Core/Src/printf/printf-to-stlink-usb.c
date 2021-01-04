// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "global.h"


int __io_putchar(int ch)
{
 uint8_t c[1];
 c[0] = ch & 0x00FF;
 HAL_UART_Transmit(&huart2, &*c, 1, 10);
 return ch;
}
