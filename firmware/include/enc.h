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

void Enc_init();
void Enc_read(int32_t enc_value[2]);

#ifdef	__cplusplus
}
#endif

#endif /* ENC_H_ */
