/*
 * VS1053.c
 *
 *  Created on: Oct 15, 2021
 *      Author: cuhuy
 */

#include "VS1053.h"

#ifdef VS1053_SUPPORT_FLAC
#include "vs1053b-patches-flac.h"
#endif



#define VS1053_FALSE          (uint8_t)0
#define VS1053_TRUE           (uint8_t)1

/* Pin control */
#define VS1053_XCS_HIGH       HAL_GPIO_WritePin(VS1053_XCS_PORT, VS1053_XCS_PIN, GPIO_PIN_SET)
#define VS1053_XCS_LOW        HAL_GPIO_WritePin(VS1053_XCS_PORT, VS1053_XCS_PIN, GPIO_PIN_RESET)
#define VS1053_XDCS_HIGH      HAL_GPIO_WritePin(VS1053_XDCS_PORT, VS1053_XDCS_PIN, GPIO_PIN_SET)
#define VS1053_XDCS_LOW       HAL_GPIO_WritePin(VS1053_XDCS_PORT, VS1053_XDCS_PIN, GPIO_PIN_RESET)
#define VS1053_XRST_HIGH      HAL_GPIO_WritePin(VS1053_XRST_PORT, VS1053_XRST_PIN, GPIO_PIN_SET)
#define VS1053_XRST_LOW       HAL_GPIO_WritePin(VS1053_XRST_PORT, VS1053_XRST_PIN, GPIO_PIN_RESET)



/* endFill byte is required to stop playing */
uint8_t endFillByte;



#ifdef VS1053_SUPPORT_FLAC
uint8_t VS1053_ApplyFlacPlugin()
{
  for (uint16_t i = 0; i < VS1053_FLAC_CODE_SIZE; i++)
  {
    if (!VS1053_SciWrite(atab[i], dtab[i]))
      return VS1053_FALSE;
  }

  return VS1053_TRUE;
}
#endif



/* Initialize VS1053 */
uint8_t VS1053_Init(void)
{
  uint16_t status = 0;

  VS1053_XCS_HIGH;            /* XCS High */
  VS1053_XDCS_HIGH;           /* XDCS High */
  VS1053_Reset();             /* Hard Reset */

  /* x 1.0 Clock, 12MHz / 7, SPI Baudrate should be less than 1.75MHz */
  (VS1053_SPI_INSTANCE)->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;  /* 108MHz / 64 = 1.6875MHz */
  if (HAL_SPI_Init(VS1053_SPI_INSTANCE) != HAL_OK)
    return VS1053_FALSE;

  /* Read Status to check SPI */
  if (!VS1053_SciRead(VS1053_REG_STATUS, &status))
    return VS1053_FALSE;
  if (((status >> 4) & 0x0F) != 0x04)
    return VS1053_FALSE;

  /* MP3 Mode GPIO configuration */
  if (!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC017)) /* GPIO direction */
    return VS1053_FALSE;
  if (!VS1053_SciWrite(VS1053_REG_WRAM, 3))
    return VS1053_FALSE;
  if (!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0xC019)) /* GPIO output */
    return VS1053_FALSE;
  if (!VS1053_SciWrite(VS1053_REG_WRAM, 0))
    return VS1053_FALSE;

  /* Soft reset */
  if (!VS1053_SoftReset())
    return VS1053_FALSE;

  /* x3.0 Clock, 36MHz / 7, SPI Baudrate should be less than 5.14MHz */
//  if (!VS1053_SciWrite(VS1053_REG_CLOCKF, 0x6000))
//    return VS1053_FALSE;
  /* (3.5 + 1) , 12MHz * 4.5 = 54MHz, SPI Baudrate should be less than 54 / 7 = 7.7MHz */
  if (!VS1053_SciWrite(VS1053_REG_CLOCKF, 0x8800))
    return VS1053_FALSE;

  (VS1053_SPI_INSTANCE)->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;  /* 108MHz / 16 = 6.75MHz */
  if (HAL_SPI_Init(VS1053_SPI_INSTANCE) != HAL_OK)
    return VS1053_FALSE;

  /* Read Status to check SPI */
  if (!VS1053_SciRead(VS1053_REG_STATUS, &status))
    return VS1053_FALSE;
  if (((status >> 4) & 0x0F) != 0x04)
    return VS1053_FALSE;

  /* Read endFill Byte */
  uint16_t regVal;

  if (!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0x1E06)) /* endFill */
    return VS1053_FALSE;
  if (!VS1053_SciRead(VS1053_REG_WRAM, &regVal))
    return VS1053_FALSE;

  endFillByte = (uint8_t)(regVal & 0x00FF);

#ifdef VS1053_SUPPORT_FLAC
  if (!VS1053_ApplyFlacPlugin())
    return VS1053_FALSE;
#endif

  return VS1053_TRUE;
}

/* Hard reset */
void VS1053_Reset(void)
{
  uint8_t dummy = 0xFF;

  VS1053_XRST_LOW;            /* XRST Low */
  HAL_SPI_Transmit(VS1053_SPI_INSTANCE, &dummy, 1, 10);       /* Tx Dummy */

  HAL_Delay(10);
  VS1053_XRST_HIGH;           /* XRST High */
  HAL_Delay(10);
}

/* Soft reset */
uint8_t VS1053_SoftReset(void)
{
  if (!VS1053_SciWrite(VS1053_REG_MODE, 0x4804))              /* SM LINE1 | SM SDINEW | SM RESET */
    return VS1053_FALSE;

  HAL_Delay(100);
  return VS1053_TRUE;
}

/* Volume control */
uint8_t VS1053_SetVolume(uint8_t volumeLeft, uint8_t volumeRight)
{
  uint16_t volume;

  volume = ((uint16_t)volumeLeft << 8) + (uint16_t)volumeRight;

  if (!VS1053_SciWrite(VS1053_REG_VOL, volume))
    return VS1053_FALSE;

  return VS1053_TRUE;
}

/* Mode control */
uint8_t VS1053_SetMode(uint16_t mode)
{
  if (!VS1053_SciWrite(VS1053_REG_MODE, mode))
    return VS1053_FALSE;

  return VS1053_TRUE;
}

uint8_t VS1053_GetMode(uint16_t *mode)
{
  if (!VS1053_SciRead(VS1053_REG_MODE, mode))
    return VS1053_FALSE;

  return VS1053_TRUE;
}

/* Re-sync control */
uint8_t VS1053_AutoResync(void)
{
  if (!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0x1E29)) /* Auto Resync */
    return VS1053_FALSE;

  if (!VS1053_SciWrite(VS1053_REG_WRAM, 0))
    return VS1053_FALSE;

  return VS1053_TRUE;
}

/* Set decode time */
uint8_t VS1053_SetDecodeTime(uint16_t time)
{
  if (!VS1053_SciWrite(VS1053_REG_DECODE_TIME, time))
    return VS1053_FALSE;

  if (!VS1053_SciWrite(VS1053_REG_DECODE_TIME, time))
    return VS1053_FALSE;

  return VS1053_TRUE;
}

/* Send endfill bytes */
uint8_t VS1053_SendEndFill(uint16_t num)
{
  uint16_t regVal;

  if (!VS1053_SciWrite(VS1053_REG_WRAMADDR, 0x1E06)) /* endFill */
    return VS1053_FALSE;

  if (!VS1053_SciRead(VS1053_REG_WRAM, &regVal))
    return VS1053_FALSE;

  endFillByte = regVal & 0xFF;

  for (uint16_t i = 0; i < num; i++)
  {
    VS1053_SdiWrite(endFillByte);
  }

  return VS1053_TRUE;
}

/* Check DREQ pin */
uint8_t VS1053_IsBusy(void)
{
  if (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_SET) /* Ready */
    return VS1053_FALSE;
  else /* Busy */
    return VS1053_TRUE;
}

/* SCI Tx */
uint8_t VS1053_SciWrite(uint8_t address, uint16_t input)
{
  uint8_t buffer[4];

  buffer[0] = VS1053_WRITE_CMD;
  buffer[1] = address;
  buffer[2] = (uint8_t)(input >> 8);      /* Input MSB */
  buffer[3] = (uint8_t)(input & 0x00FF);  /* Input LSB */

  while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);  /* Wait DREQ High */

  VS1053_XCS_LOW;             /* XCS Low */
  if (HAL_SPI_Transmit(VS1053_SPI_INSTANCE, buffer, sizeof(buffer), 10) != HAL_OK)
    return VS1053_FALSE;
  VS1053_XCS_HIGH;            /* XCS High */

  while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);  /* Wait DREQ High */

  return VS1053_TRUE;
}

/* SCI Rx */
uint8_t VS1053_SciRead(uint8_t address, uint16_t *res)
{
  uint8_t dummy = 0xFF;
  uint8_t txBuffer[2];
  uint8_t rxBuffer[2];

  txBuffer[0] = VS1053_READ_CMD;
  txBuffer[1] = address;

  while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET); /* Wait DREQ High */

  VS1053_XCS_LOW;             /* XCS Low */
  if (HAL_SPI_Transmit(VS1053_SPI_INSTANCE, txBuffer, sizeof(txBuffer), 10) != HAL_OK)
    return VS1053_FALSE;
  if (HAL_SPI_TransmitReceive(VS1053_SPI_INSTANCE, &dummy, &rxBuffer[0], 1, 10) != HAL_OK)
    return VS1053_FALSE;
  if (HAL_SPI_TransmitReceive(VS1053_SPI_INSTANCE, &dummy, &rxBuffer[1], 1, 10) != HAL_OK)
    return VS1053_FALSE;
  VS1053_XCS_HIGH;            /* XCS High */

  *res = rxBuffer[0];         /* Received data */
  *res <<= 8;                 /* MSB */
  *res |= rxBuffer[1];        /* LSB */

  while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);  /* Wait DREQ High */

  return VS1053_TRUE;
}

/* SDI Tx */
uint8_t VS1053_SdiWrite(uint8_t input)
{
  while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);  /* Wait DREQ High */

  VS1053_XDCS_LOW;            /* XDCS Low */
  if (HAL_SPI_Transmit(VS1053_SPI_INSTANCE, &input, 1, 10) != HAL_OK)
    return VS1053_FALSE;      /* SPI Tx 1 byte */
  VS1053_XDCS_HIGH;           /* XDCS High */

  return VS1053_TRUE;
}

/* SDI Tx 32 bytes */
uint8_t VS1053_SdiWrite32(uint8_t *input32)
{
  while (HAL_GPIO_ReadPin(VS1053_DREQ_PORT, VS1053_DREQ_PIN) == GPIO_PIN_RESET);  /* Wait DREQ High */

  VS1053_XDCS_LOW;            /* XDCS Low */
  if (HAL_SPI_Transmit(VS1053_SPI_INSTANCE, input32, 32, 10) != HAL_OK)
    return VS1053_FALSE;      /* SPI Tx 32 bytes */
  VS1053_XDCS_HIGH;           /* XDCS High */

  return VS1053_TRUE;
}
