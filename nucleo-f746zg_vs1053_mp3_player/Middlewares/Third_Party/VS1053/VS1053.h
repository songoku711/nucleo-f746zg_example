/*
 * VS1053.h
 *
 *  Created on: Oct 15, 2021
 *      Author: cuhuy
 */

#ifndef THIRD_PARTY_VS1053_VS1053_H_
#define THIRD_PARTY_VS1053_VS1053_H_

#include "main.h"
#include "spi.h"

/* Pin configuration */
#define VS1053_SPI_INSTANCE   &hspi1
#define VS1053_DREQ_PORT      VS1053_DREQ_GPIO_Port
#define VS1053_DREQ_PIN       VS1053_DREQ_Pin
#define VS1053_XRST_PORT      VS1053_XRST_GPIO_Port
#define VS1053_XRST_PIN       VS1053_XRST_Pin
#define VS1053_XCS_PORT       VS1053_XCS_GPIO_Port
#define VS1053_XCS_PIN        VS1053_XCS_Pin
#define VS1053_XDCS_PORT      VS1053_XDCS_GPIO_Port
#define VS1053_XDCS_PIN       VS1053_XDCS_Pin

/* Commands */
#define VS1053_WRITE_CMD      0x02
#define VS1053_READ_CMD       0x03

/* Registers */
#define VS1053_REG_BASE       (uint8_t)0x00
#define VS1053_REG_MODE       (uint8_t)0x00
#define VS1053_REG_STATUS     (uint8_t)0x01
#define VS1053_REG_BASS       (uint8_t)0x02
#define VS1053_REG_CLOCKF     (uint8_t)0x03
#define VS1053_REG_DECODE_TIME  (uint8_t)0x04
#define VS1053_REG_AUDATA     (uint8_t)0x05
#define VS1053_REG_WRAM       (uint8_t)0x06
#define VS1053_REG_WRAMADDR   (uint8_t)0x07
#define VS1053_REG_HDAT0      (uint8_t)0x08
#define VS1053_REG_HDAT1      (uint8_t)0x09
#define VS1053_REG_AIADDR     (uint8_t)0x0A
#define VS1053_REG_VOL        (uint8_t)0x0B
#define VS1053_REG_AICTRL0    (uint8_t)0x0C
#define VS1053_REG_AICTRL1    (uint8_t)0x0D
#define VS1053_REG_AICTRL2    (uint8_t)0x0E
#define VS1053_REG_AICTRL3    (uint8_t)0x0F

/* Functions */
uint8_t VS1053_Init(void);
void VS1053_Reset(void);
uint8_t VS1053_SoftReset(void);
uint8_t VS1053_SetVolume(uint8_t volumeLeft, uint8_t volumeRight);
uint8_t VS1053_SetMode(uint16_t mode);
uint8_t VS1053_GetMode(uint16_t *mode);
uint8_t VS1053_AutoResync(void);
uint8_t VS1053_SetDecodeTime(uint16_t time);
uint8_t VS1053_SendEndFill(uint16_t num);
uint8_t VS1053_IsBusy(void);
uint8_t VS1053_SciWrite(uint8_t address, uint16_t input);
uint8_t VS1053_SciRead(uint8_t address, uint16_t *res);
uint8_t VS1053_SdiWrite(uint8_t input);
uint8_t VS1053_SdiWrite32(uint8_t *input32);

extern uint8_t endFillByte;

#endif /* THIRD_PARTY_VS1053_VS1053_H_ */
