/*
 * spi.h
 *
 *  Created on: 2016/11/01
 *      Author: idt12312
 */

#ifndef PERIPHERAL_SPI_H_
#define PERIPHERAL_SPI_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void Spi_init();
void Spi_cs_assert();
void Spi_cs_dessert();
void Spi_start_xchg(const uint8_t *tx_buf, uint8_t *rx_buf, size_t size);
bool Spi_busy();
void Spi_block(uint32_t block_time);


void Spi_dma_complete_isr();

#ifdef	__cplusplus
}
#endif

#endif /* PERIPHERAL_SPI_H_ */
