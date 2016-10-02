/*
 * uart.h
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */

#ifndef UART_H_
#define UART_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>


void Uart_init();
void Uart_write_byte(uint8_t data);
size_t Uart_write(const uint8_t *data, size_t size);
size_t Uart_write_str(const char *str);
size_t Uart_printf(const char* fmt, ...);
size_t Uart_rcv_size();
uint8_t Uart_read_byte();
size_t Uart_read(uint8_t *buffer, size_t size);

// UARTの送信に使用するDMAの転送完了割り込みルーチンから呼ぶ
void Uart_tx_dma_isr();


#ifdef	__cplusplus
}
#endif

#endif /* UART_H_ */
