typedef unsigned char boolean;
// константы общего назнаения
#define YES      1
#define NO    	 0
#define OK   	 0
#define NOK  	-1
#define ON    		 1
#define OFF   		 0

// клавиши ( скан коды )
#define SC_ENTER 	 0x1C0D
#define SC_ESC 		 0x11B
#define SC_DOWN	 	 0x5000
#define SC_UP	 	 0x4800
#define SC_LEFT	 	 19200
#define SC_RIGHT	 19712
#define SC_PGUP 	 18688
#define SC_PGDN	 	 20736
#define SC_HOME	 	 18176
#define SC_END	 	 20224
#define SC_F10	 	 17408

// размер файлового буфера
#define FILE_BUF_SIZE 	 1024
// для кодировки текстов
#define OFTEN_CODE  	 0xE
#define SELDOM_CODE 	 0xF
//режим работы с мышью
#define MOUSE_ON  	 1
#define MOUSE_OFF 	 0
// константы для ChangeArt
#define ART_LIST 	 -1
#define TH_LIST  	 -2

#define TH_VIEW  	  1
#define ART_VIEW  	  2

// запросы пользователя в ходе просмотра статьи
#define ASK_QUIT      	 3
#define ASK_GO_TO_REF  	 4
#define ASK_RETURN       5
// для ф-ии SetRefs
#define IN_ALL_SCREEN    -1
#define IN_SINGLE_LINE   -2
#define MAX_LINES     10000

// для ф-ии PutLine
#define NOT_DEFINED 	-1
#define OVERWRITE    	 1
#define APPEND       	 2

#define FONT_HEIGHT     10
#define SCROLLER_WIDTH  14
#define SCD1 		 3    // поля скроллера
#define SCD2 	        20   // размер каретки скроллера

// ошибки
#define WRONG_INDEXDAT  -2
#define WRONG_DATADAT   -3
#define WRONG_TH        -4
#define NO_MEMORY       -5

// кол-во запоминаемых в "истории" статей
#define _HISTORY
#define MAX_HISTORY     20

// default colors
#define  defClrText         LIGHTGRAY
#define  defClrBack         BLUE
#define  defClrRef          LIGHTCYAN
#define  defClrActRef       YELLOW
#define  defClrBackActRef   CYAN
#define  defClrBord         WHITE

