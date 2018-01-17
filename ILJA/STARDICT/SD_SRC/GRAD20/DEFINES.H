/********************************************************************
-- STARDICT Project --
Многие полезные константы
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _DEFINES_H
#define _DEFINES_H

typedef unsigned char boolean;
// константы общего назнаения
#define YES      1
#define NO       0

#define OK       0
#define NOK     -1

#define ON               1
#define OFF              0

// для кодировки текстов
#define OFTEN_CODE       0xE
#define SELDOM_CODE      0xF

#define MAX_KEYWORDS_IN_ART 100
#define MAX_KEYWORDS_IN_LINK 10
// смещение CRC в байтах от начала файла
#define CRC_POSITION 3

#define DEFAULT_DATA_VERSION    100
#define DEFAULT_FORMAT_VERSION  20
#endif
