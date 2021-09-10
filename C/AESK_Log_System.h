/*
 * AESK_Log_System.h
 *
 *  Created on: 05 July 2021
 *      Author: Yusuf
 *      Contact: ykemalpalaci@gmail.com
 */

/*
 *
 * Bu kütüphane, log işlemleri için 2 seçenek sunar.
 * Bu seçenekler aşağıda prototipleri verilen LogAsString ve LogAsBuffer
 * fonksiyonlarıdır. Açıklamaları üstlerinde yapılmıştır.
 *
 */

#ifndef INC_AESK_LOG_SYSTEM_H_
#define INC_AESK_LOG_SYSTEM_H_

#include "stdint.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "ff.h"
#include "fatfs.h"
#include "main.h"

#define WITH_RTC 0  //RTC kullanılacaksa bu değer 1 yapılmalıdır.

/*
 * WITH_RTC değeri 1 yapıldığında kayıt dosyasının ismi o anki zaman olur.
 * Kayıtların başına anlık zaman hh:mm:ss olarak eklenir.
 *
 * RTC değişkenleri, sadece bu blokta ve read only olarak kullanılacağı
 * için tanımlamalar burada yapılmıştır.
 */
#if WITH_RTC == 1
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
RTC_HandleTypeDef hrtc;
#else
#endif
/*
 * SD kart okunduğunda sonraki duruma geçilir.
 */
typedef enum
{
	NO_SD_Card_Detect = 0,
	SD_Card_Detect = 1,
}SD_Card_States;

typedef struct
{
	uint8_t transmitBuf[500]; 	//SD karta kaydedilecek verileri taşır.
	char path[200];				//SD kart verilerinin kaydedileceği yol. Yol verilmezse dosya ismi.
	uint8_t state;				//SD kart durumu enum'u için
	char total_log[600];		//İşlenmiş log verilerini taşıyan byte
	//uint32_t logger_u32;
	//uint32_t bytes_u32;
	uint32_t errorcounter_u32;	//FATFS fonksiyonlarının hataya düşme sayısını tutar.
	FRESULT result;				//Debug aşamasında kullanılabilecek FRESULT değişkeni
	FRESULT result_open;		//Dosya açma fonksiyonunun sonucunu taşıyan FRESULT değişkeni
	FRESULT result_write;		//Yazma fonksiyonunun sonucunu taşıyan FRESULT değişkeni
	FRESULT result_sync;		//Sync fonksiyonunun sonucunu taşıyan FRESULT değişkeni
	FRESULT result_close;		//Dosya kapatma fonksiyonunun sonucunu taşıyan FRESULT değişkeni
	unsigned int writtenbyte;	//Yazılan byte sayısı
	FATFS myFATAFS;				//f_mount fonksiyonunda kullanılan FATFS değişkeni
	FIL myFile;					//FATFS fonksiyonlarında kullanılan FIL değişkeni, C'deki FILE değişkeninin alternatifi.
}Sd_Card_Data;

/*
 * Alınan parametreleri tek bir stringe dönüştürür. sprintf alternatifi.
 */
void vars_to_str(char *buffer, const char *format, ...);

/*
 * LogStart fonksiyonu, SD kartı yazılımsal olarak takar ve kaydı başlatır.
 * .txt uzantılı dosyanın ilk satırına verilerin başlığını ekler.
 * Başlık metni .c dosyasının başında tanımlanmıştır.
 */
void LogStart(Sd_Card_Data* sdcard_data, char *path);

//Verilen parametreyi dosyaya yazar. WITH_RTC açıksa iterasyonun başına zamanı ekler.
void LogAsString(char *packBuf, Sd_Card_Data *sd_card_data);


#endif /* INC_AESK_LOG_SYSTEM_H_ */
