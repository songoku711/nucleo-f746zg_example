/*
 * vs1053b-patches-flac.h
 *
 *  Created on: 16 thg 10, 2021
 *      Author: cuhuy
 */

#ifndef THIRD_PARTY_VS1053_VS1053B_PATCHES_FLAC_H_
#define THIRD_PARTY_VS1053_VS1053B_PATCHES_FLAC_H_

#include "stm32f7xx_hal.h"

#ifdef VS1053_SUPPORT_FLAC
#define VS1053_FLAC_CODE_SIZE 8423

extern const uint8_t atab[];
extern const uint16_t dtab[];
#endif

#endif /* THIRD_PARTY_VS1053_VS1053B_PATCHES_FLAC_H_ */
