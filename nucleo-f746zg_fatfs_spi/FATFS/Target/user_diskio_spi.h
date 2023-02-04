/*
 * user_diskio_spi.h
 *
 *  Created on: Oct 12, 2021
 *      Author: cuhuy
 */

#ifndef TARGET_USER_DISKIO_SPI_H_
#define TARGET_USER_DISKIO_SPI_H_

#include "main.h"
#include "spi.h"
#include "diskio.h"

/* Definitions for MMC/SDC command */
#define CMD0     (0x40+0)     	/* GO_IDLE_STATE */
#define CMD1     (0x40+1)     	/* SEND_OP_COND */
#define CMD8     (0x40+8)     	/* SEND_IF_COND */
#define CMD9     (0x40+9)     	/* SEND_CSD */
#define CMD10    (0x40+10)    	/* SEND_CID */
#define CMD12    (0x40+12)    	/* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    	/* SET_BLOCKLEN */
#define CMD17    (0x40+17)    	/* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    	/* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    	/* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    	/* WRITE_BLOCK */
#define CMD25    (0x40+25)    	/* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    	/* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    	/* APP_CMD */
#define CMD58    (0x40+58)    	/* READ_OCR */

/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		0x06		/* SD */
#define CT_BLOCK	0x08		/* Block addressing */

/* Functions */
DSTATUS USER_diskio_spi_initialize (BYTE pdrv);
DSTATUS USER_diskio_spi_status (BYTE pdrv);
DRESULT USER_diskio_spi_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT USER_diskio_spi_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT USER_diskio_spi_ioctl (BYTE pdrv, BYTE cmd, void* buff);

void USER_diskio_spi_tick(void);

#define SD_SPI_TIMEOUT	5000

#define SD_SPI_CS_PORT	SPI1_CS_GPIO_Port
#define SD_SPI_CS_PIN	  SPI1_CS_Pin

#define SD_SPI_INSTANCE &hspi1

#endif /* TARGET_USER_DISKIO_SPI_H_ */
