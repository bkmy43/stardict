/********************************************************************
-- STARDICT Project --
Определение основных констант
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _DEFINES_H
#define _DEFINES_H

typedef unsigned char boolean;
// константы общего назначения
//#define YES      1
//#define NO       0
#define OK       0
#define NOK     -1
#define ON               1
#define OFF              0

/*
// клавиши (скан-коды)
#define SC_ENTER         0x1C0D
#define SC_ESC           0x11B
#define SC_DOWN          0x5000
#define SC_UP            0x4800
#define SC_LEFT          19200
#define SC_RIGHT         19712
#define SC_PGUP          18688
#define SC_PGDN          20736
#define SC_HOME          18176
#define SC_END           20224
#define SC_F10           17408
  */

// для кодировки
#define OFTEN_CODE       0xE
#define SELDOM_CODE      0xF

#define MAX_BODY_SIZE     25000
#define MAX_NAME_LEN      255
#define MAX_THEMES        200
#define MAX_ARTS_IN_THEME 200

typedef    int  filehand ;

#endif
