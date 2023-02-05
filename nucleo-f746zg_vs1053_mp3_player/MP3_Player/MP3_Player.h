/*
 * MP3_Player.h
 *
 *  Created on: Oct 15, 2021
 *      Author: cuhuy
 */

#ifndef MP3_PLAYER_H_
#define MP3_PLAYER_H_

#include "main.h"

#define FILE_TYPE_MP3   0
#ifdef VS1053_SUPPORT_FLAC
#define FILE_TYPE_FLAC  1
#endif

/* Functions */
uint8_t MP3_Init(void);
uint8_t MP3_Play(const char *filename, uint8_t fileType);
void MP3_Stop(void);
void MP3_Pause(void);
void MP3_Resume(void);
void MP3_Feeder(void);

/* Flags */
extern uint8_t isPlaying;
extern uint8_t isFileOpen;

#endif /* MP3_PLAYER_H_ */
