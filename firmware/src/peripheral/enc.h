/*
 * enc.h
 *
 *  Created on: 2016/09/29
 *      Author: idt12312
 */

#ifndef ENC_H_
#define ENC_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct {
	int32_t left;
	int32_t right;
} EncValue;

void Enc_init();
void Enc_read(EncValue *enc_value);

#ifdef	__cplusplus
}
#endif

#endif /* ENC_H_ */
