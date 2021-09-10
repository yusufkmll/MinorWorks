/*
 * AESK_Log_System.c
 *
 *  Created on: 5 July 2021
 *      Author: Yusuf
 *
 *  Açıklamalar .h dosyasında
 */

#include "AESK_Log_System.h"

/*
 * Dosya başlığını dilediğiniz gibi değiştirebilirsiniz.
 */
char *SDHeader =
		"Dosya Basligi\n";

void vars_to_str(char *buffer, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
}

void LogStart(Sd_Card_Data *sdcard_data, char *path)
{
	sdcard_data->result = f_mount(&sdcard_data->myFATAFS, (TCHAR const*) SDPath, 1);
	if (sdcard_data->result == FR_OK)
	{
		#if WITH_RTC == 1
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		vars_to_str(sdcard_data->path, "%d_%d_%d_%d_%d_%d.txt", sDate.Date, sDate.Month, sDate.Year, sTime.Hours, sTime.Minutes, sTime.Seconds);
		#else
		strcpy(sdcard_data->path, path);
		#endif

		sdcard_data->result_open = f_open(&sdcard_data->myFile, sdcard_data->path, FA_WRITE | FA_OPEN_APPEND);
		f_write(&sdcard_data->myFile, SDHeader, strlen(SDHeader), (void*) &sdcard_data->writtenbyte);
		sdcard_data->state = SD_Card_Detect;
		f_close(&sdcard_data->myFile);
	}
}

void LogAsString(char *packBuf, Sd_Card_Data *sd_card_data)
{
	strcpy((char*)sd_card_data->transmitBuf, packBuf);
	#if WITH_RTC == 1
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	vars_to_str((char*) sd_card_data->total_log, "%d:%d:%d\t", sTime.Hours, sTime.Minutes, sTime.Seconds);
	strcat(sd_card_data->total_log, (const char*) sd_card_data->transmitBuf); //Kaydedilen verinin başına RTC ekler.
	#else
	strcpy((char*)sd_card_data->total_log, packBuf);
	#endif
	sd_card_data->result_open = f_open(&sd_card_data->myFile, sd_card_data->path,
			FA_WRITE | FA_OPEN_APPEND | FA_OPEN_ALWAYS);
	if(sd_card_data->result_open != FR_OK)
	{
		sd_card_data->errorcounter_u32++;
		sd_card_data->result_close = f_close(&sd_card_data->myFile);
		if(sd_card_data->result_close == FR_OK)
		{
			sd_card_data->result_open = f_open(&sd_card_data->myFile, sd_card_data->path,
					FA_WRITE | FA_OPEN_APPEND | FA_OPEN_ALWAYS);
		}
	}
	sd_card_data->result_write = f_write(&sd_card_data->myFile, sd_card_data->total_log, strlen(sd_card_data->total_log), (void*) &sd_card_data->writtenbyte);
	if ((sd_card_data->writtenbyte != 0) && (sd_card_data->result_write == FR_OK))
	{
		sd_card_data->result_close = f_close(&sd_card_data->myFile);
		if(sd_card_data->result_close != FR_OK)
		{
			sd_card_data->result_close = f_close(&sd_card_data->myFile);
		}
	}

	else
	{
		sd_card_data->result = f_mount(&sd_card_data->myFATAFS, (TCHAR const*) SDPath, 1);
		if(sd_card_data->result == FR_DISK_ERR)
		{
			f_close(&sd_card_data->myFile);
		}
	}
}
