/*
 * MP3_Player.c
 *
 *  Created on: Oct 15, 2021
 *      Author: cuhuy
 */

#include "MP3_Player.h"

#include "VS1053.h"
#include "fatfs.h"



#define MP3_FALSE         0
#define MP3_TRUE          1

#define MP3_BUFFER_SIZE   32
#ifdef VS1053_SUPPORT_FLAC
#define FLAC_BUFFER_SIZE  512
#endif



#ifdef VS1053_SUPPORT_FLAC
uint8_t mp3Buffer[FLAC_BUFFER_SIZE];
#else
uint8_t mp3Buffer[MP3_BUFFER_SIZE];
#endif
uint32_t mp3FileSize;
uint32_t readBytes;
uint16_t cnt = 0;

uint8_t isPlaying = MP3_FALSE;
uint8_t isFileOpen = MP3_FALSE;

uint8_t curFileType = FILE_TYPE_MP3;

FATFS fs;
FIL mp3File;



/* Initialize VS1053 & Open a file */
uint8_t MP3_Init()
{
  /* Initialize VS1053 */
  if (!VS1053_Init())
    return MP3_FALSE;

  /* Mount SD Card */
  if (f_mount(&fs, "", 1) != FR_OK)
    return MP3_FALSE;

  return MP3_TRUE;
}

uint8_t MP3_Play(const char *filename, uint8_t fileType)
{
  if (isPlaying)
    MP3_Stop();

  if (!VS1053_SetMode(0x4800))          /* SM LINE1 | SM SDINEW */
    return MP3_FALSE;
  if (!VS1053_AutoResync())             /* AutoResync */
    return MP3_FALSE;
  if (!VS1053_SetDecodeTime(0))         /* Set decode time */
    return MP3_FALSE;
  if (!VS1053_SetVolume(0x2F, 0x2F))  /* Small number is louder */
    return MP3_FALSE;

  /* Open file to read */
  if (f_open(&mp3File, filename, FA_READ) != FR_OK)
    return MP3_FALSE;

  /* Get the file size */
  mp3FileSize = f_size(&mp3File);

  /* Set file type */
  curFileType = fileType;

  /* Set flags */
  isFileOpen = MP3_TRUE;
  isPlaying = MP3_TRUE;

  return MP3_TRUE;
}

void MP3_Stop(void)
{
  /* Refer to page 49 of VS1053 datasheet */
  uint16_t mode;

  VS1053_SendEndFill(2052);             /* send endfill bytes */
  VS1053_SetMode(0x4808);               /* SM LINE1 | SM SDINEW | SM CANCEL */
  VS1053_SendEndFill(32);               /* send endfill bytes */
  HAL_Delay(100);

  VS1053_GetMode(&mode);                /* get mode value */
  if ((mode & 0x08) != 0x0)             /* if SM CANCEL is not clear, soft reset */
  {
    VS1053_SoftReset();
  }

  f_close(&mp3File);

  isPlaying = MP3_FALSE;                /* Stop flag */
  isFileOpen = MP3_FALSE;               /* Close flag */
}

void MP3_Pause(void)
{
  if (isPlaying)
    isPlaying = MP3_FALSE;
}

void MP3_Resume(void)
{
  if (!isPlaying)
    isPlaying = MP3_TRUE;
}

/* Send mp3 buffer to VS1053 */
void MP3_Feeder(void)
{
  if (!isPlaying || !isFileOpen)
    return;

  if (curFileType == FILE_TYPE_MP3)
  {
    if (mp3FileSize > MP3_BUFFER_SIZE)
    {
      /* Fill the buffer */
      f_read(&mp3File, mp3Buffer, MP3_BUFFER_SIZE, (void*)&readBytes);

      /* Tx buffer */
      VS1053_SdiWrite32(mp3Buffer);

      /* bytes to send */
      mp3FileSize -= MP3_BUFFER_SIZE;
    }
    else
    {
      /* Read left bytes */
      f_read(&mp3File, mp3Buffer, mp3FileSize, (void*)&readBytes);

      /* Tx buffer */
      for (cnt = 0; cnt < mp3FileSize; cnt++)
      {
        while (!VS1053_SdiWrite(*(mp3Buffer + cnt)));
      }

      /* Stop when played the whole file */
      MP3_Stop();
    }
  }
#ifdef VS1053_SUPPORT_FLAC
  else if (curFileType == FILE_TYPE_FLAC)
  {
    if (mp3FileSize > FLAC_BUFFER_SIZE)
    {
      /* Fill the buffer */
      f_read(&mp3File, mp3Buffer, FLAC_BUFFER_SIZE, (void*)&readBytes);

      for (uint8_t i = 0; i < (FLAC_BUFFER_SIZE / 32); i++)
      {
        /* Tx buffer */
        VS1053_SdiWrite32((mp3Buffer + (i * 32)));
      }

      /* bytes to send */
      mp3FileSize -= FLAC_BUFFER_SIZE;
    }
    else
    {
      /* Read left bytes */
      f_read(&mp3File, mp3Buffer, mp3FileSize, (void*)&readBytes);

      /* Tx buffer */
      for (cnt = 0; cnt < mp3FileSize; cnt++)
      {
        while (!VS1053_SdiWrite(*(mp3Buffer + cnt)));
      }

      /* Stop when played the whole file */
      MP3_Stop();
    }
  }
#endif
  else
  {
    /* Do nothing */
  }
}

