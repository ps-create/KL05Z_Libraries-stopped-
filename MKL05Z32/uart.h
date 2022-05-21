#ifndef UART_H
#define UART_H

#include "MKL05Z4.h"
#define MCGFLLCLK   0x01

typedef enum
{
  UART_SUCCES = !0,
  UART_FAILURE = 0
} UART_ERROR_CODE_t;

void uart_init(uint32_t baud_rate);

void uart_sendStr(char* str);

void uart_sendCh(uint8_t data);

void uart_getChar(uint8_t *data);

#endif /* UART_H */
