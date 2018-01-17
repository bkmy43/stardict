#include <dos.h>
#include <conio.h>

#define  MAX_MENUS   20     /* максимальное количество меню */
#define  MAX_ITEMS   20     /* максимальное количество ITEMов */
#define  MAX_STRINGS 20     /* макс. кол-во вариантов при переключении
							      SWITH-ITEMa*/
#define  DESTROYED_MENU  0  /* признак несозданного окна */
#define  LASTDRIVE   'C'    /* максимальный драйв для сохранения */

/* ТИПЫ БОРДЮРОВ */
#define NO_BORD       0
#define SINGLE_BORD   1
#define DOUBLE_BORD   2
#define REDUCED_BORD  3

/* КОДЫ ОШИБОК */
#define GM_NOK       -1
#define GM_OK         0
#define GM_OVERFLOW  -2
#define GM_NOT_MEM   -3
#define GM_ESC       -2

/*ОПЦИИ СОХРАНЕНИЯ МЕНЮ */
#define GM_SAVE       0
#define GM_NOSAVE     1

/* ТИПЫ ITEMов */
#define  MENU_ITEM      0
#define  FUNC_ITEM      1
#define  YES_NO_ITEM    2
#define  SWITCH_ITEM    3
#define  STRING_ITEM    4
#define  FILENAME_ITEM  5
#define  FUNCKEY_ITEM   6  /* CPP */

/* ЗНАЧЕНИЯ YESNOHANDLERа */
#define  NO             0
#define  YES            1

/* ЗНАЧЕНИЯ FILEMASK */
#define  GM_EXTENSION    0
#define  GM_NOEXTENSION  1

/* ТИПЫ ВВОДИМЫХ СТРОК */
#define TEXT             0
#define DIGIT_INT        1
#define DIGIT_REAL       2

/* ПАРАМЕТРЫ СТАТУС-СТРОКИ */
#define  STAT_HEIGHT 1.5  /* высота статус строки по отношению к LINE_HEIGHT */
#define  STAT_LEFT          0
#define  STAT_TOP           769-(int)(LINE_HEIGHT*STAT_HEIGHT)
#define  STAT_RIGHT         1024
#define  STAT_BOTTOM        769
#define  STAT_NOTINIT       -1      /* не инициализирована */
#define  STAT_NOTACTIVE      0      /* не активна */
#define  STAT_ACTIVE         1      /* активна */


struct STRINGS
   {
   char *str0,*str1,*str2;
   };

struct GMENU
   {
   int    top, left, right, bottom;   /* координаты */
   int    qitems;                     /* кол-во ITEMов в меню */
   struct GITEM *GItems[MAX_ITEMS];
   char   menutype;   /* тип меню - вертикальное или горизонтальное : V or H */
   int    bordtype;   /* тип бордюра - см. define */
   char   *bordname0, *bordname1, *bordname2, *bordname;
		      /* заголовок на бордюре */
   int    lang;       /* язык - 0,1,2 */

		   /* ЦВЕТА */
   int    groundcolor, groundbordcolor, bordcolor,
				  /* фон меню, фон бордюра, цвет бордюра */
	  barcolor, highbarcolor, /* фон ITEMа и фон выделенного ITEMа */
	  textcolor, hightextcolor, /* цвет выводимого текста - выделенного
				       и невыделенного */
	  highletcolor, /* цвет выделенной буквы */
	  hidebarcolor, hidetextcolor, /* фон и текст спрятанного ITEMа */
	  hidehightextcolor,  /* текст спрятанного выделенного ITEMа */
	  str_color,str_groundcolor,str_curscolor;
	      /* только для STRING_ITEM  - цвета редактируемой строки */

   int    otstup, interval; /* отступ от края и интервал между ITEMами */
   int    freq;             /* частота звука */
   int    curitem;          /* текущий ITEM */
   int    menucall;         /* nandler на вызвавщее меню (или -1) */
   signed long  savepos,    /* 0 - ... для памяти (смещение по отношению к
			       savebuf в байтах);
			  -1,-2 etc. - номер файла при сохранении на диске */
		savesize;    /* размер в байтах */
   int	  savehandler;       /* если не сохранялось == 0; иначе 1 */
   int	  statcolor,statgroundcolor; /* цвет текста и фона статус строки */
   int    exitkey;          /* клавиша выхода из меню, по умолчанию - ESC,
			       0 - при отсутствии выхода */
   };

struct GITEM
   {
   int itemtype;                 /* тип ITEMа - см. define */
   char hideitem;                 /* 'H' или 'N' - спрятанный или нет ITEM */
   char *str0,*str1,*str2,*str;  /* текст ITEMа */
   int length;                   /* длина строки */
   int hlet0,hlet1,hlet2,hlet;   /* выделенные буквы (! передается буква,
				    но храниться номер от начала str)*/
   char *statstr0,*statstr1,*statstr2,*statstr;  /* текст статус строки
				    statstr==NULL, если статус-строки нет */
   int menuhandler;              /* в MENU - handler MENU
				    в SWITCH - количество строк
				    в STRING - тип строки
				    в FILENAME - тип имени файла: с расши-
				      рением или без */
				 /* в FUNCKEY - функциональная клавиша,
				      на котоpую pеагиpует item */
   int *mainhandler;              /* в YESNO - handler YES/NO */
				  /* в SWITCH - текущая строка */
   int (* funchandler)();         /* в FUNC и MENU - функция */
   char *string;                  /* в STRING - строка,
				     в FILENAME - имя файла */
   char *mask;                    /* в FILENAME - шаблон для поиска файла */
   int menuclose;                 /* сколько меню закрывать после выполнения
				     ITEMа  (в MENU не работает) */
   int menurepaint;
	     /* в FUNC - сколько меню прятать на время  выполнения функции
		в SWITCH - 0 - OK; -1 - не было установки последней строки */
   int strlength;     /* в SWITCH и STRING - максимальная длина строки свитча */
   int helpitem;              /* номер HELPа на item (-1 при отсутствии) */
   struct STRINGS strings[MAX_STRINGS];  /* в SWITCH - указатели на строки */
   };

/************************ ВНЕШНИЕ ФУНКЦИИ ************************/
int InitMenu(int *menuhandler,
	     int left, int top, char menutype, int lang,
	     int bordtype, char *bordname0, char *bordname1, char *bordname2,
	     int otstup, int interval, int freq,
	     int groundcolor, int groundbordcolor, int bordcolor,
	     int barcolor, int highbarcolor, int hidebarcolor,
	     int textcolor, int hightextcolor, int hidetextcolor,
	     int hidehightextcolor, int highletcolor);
  /* иницилизирует меню, выделяет память
     должна быть первой функцией менюшника (если только не используется
	SetSaveBufferSize - тогда она первая)
     возврат - GM_OK или код ошибки */

void DestroyMenu(int menuhandler);
  /* уничтожает меню, освобождает память
     используется в DestroyAllMenus */
void DestroyAllMenus(void);
  /* уничтожает все меню, освобождает память
     ! должна быть последней функцией менюшника */

int InitMenuItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int newmenuhandler, int (* funchandler)(), int helpitem);
  /* инициализация MENU ITEMа
     newmwnuhandler - handler вызываемого меню,
     funchandler - адрес функции, выполняемой перед вызовом меню
     возврат - handler ITEMа или код ошибки */

int InitYesNoItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int *yesnohandler, int menuclose, int helpitem);
  /* инициализация YES_NO ITEMа
     yesnohandler - указатель на переменную, имеющую значение YES или NO
		    (см. define ), в нее же помещается результат;
     menuclose - сколько уровней меню закрывать после выполнения ITEMа;
     возврат - handler ITEMа или код ошибки */

int InitSwitchItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int *curstr, int menuclose, int helpitem);
  /* инициализация SWITCH ITEMа
     yesnohandler - указатель на номер текущей строки свитча
     menuclose - сколько уровней меню закрывать после выполнения ITEMа;
     возврат - handler ITEMа или код ошибки */

int InitFuncItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int (* funchandler)(),
		 int menuclose, int menurepaint, int helpitem);
  /* инициализация FUNC ITEMа
     funchandler - адрес вызываемой функции
     menuclose - сколько уровней меню закрывать после выполнения ITEMа;
     menurepaint - сколько меню прятать перед выполнением функции,
		   восстанавливая после ее завершения
     возврат - handler ITEMа или код ошибки */
           	
int InitStringItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 char *string, int strlength, int strtype,
		 int menuclose, int helpitem);
  /* инициализация STRING ITEMа
     ! разрешен только в вертикальных меню;
     string - указатель на вводимую строку
     strlength - максимальная длина вводимой строки
     strtype - тип вводимой строки (см.define)
     menuclose - сколько уровней меню закрывать после выполнения ITEMа;
     helpitem - номер HELPа на item (-1 при отсутствии)
     возврат - handler ITEMа или код ошибки */

int InitFilenameItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 char *mask, char *filename, int filemask,
		 int menuclose, int helpitem);
  /* инициализация FUNC ITEMа
     mask - шаблон для поиска (например: "*.CFG");
     filename - имя файла
     filemask - выводить имя файла с расширением или без (см. дефайны)
     menuclose - сколько уровней меню закрывать после выполнения ITEMа;
     возврат - GM_OK или код ошибки */


int InitFuncKeyItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int funckey,
		 int menuopen, /* int menurepaint,*/ int helpitem);
  /* инициализация FUNCKEY ITEMа
     funckey - клавиша, возвpащаемая меню
     menuclose - сколько меню оставлять откpытыми после выполнения ITEMа
		 (-1), если вообще не надо закpывать;
     возврат - handler ITEMа или код ошибки */
/* ! ТОЛЬКО при режиме внешнего управления меню: define OUTER_OPERATION_MODE */


int AddStringToItem(int menuhandler, char *str0, char *str1, char *str2);
    /* добавляет строку свитча к SWITCH ITEMу
       должна находиться непосредственно после инициализации ITEMа
       используется в AddLastStringToItem
       возврат - GM_OK */

int AddLastStringToItem(int menuhandler, char *str0, char *str1, char *str2);
    /* добавляет последнюю из строк свитча к SWITCH ITEMу
       должна заканчивать собой ряд AddStringToItem
       возврат - GM_OK или код ошибки
    */

void SetEnterStringColors(int menuhandler,
		     int color, int groungcolor, int curscolor);
    /* установить в меню цвета для ввода строки
       осмыслена только при наличии в меню STRING ITEMа
       color        - цвет строки
       groundcolor  - фон строки
       curscolor    - цвет текущей буквы
    */

int ActivateMenu(int menuhandler);
    /* активизирует и рисует меню
       возвращает GM_OK  */

void SetSaveBufferSize(int percents, int minimum);
    /* устанавливает размеры буфера для сохранения меню
       percents - размеры буфера в процентах от свободной памяти
       minimum  - минимальный размер буфера
       ! должна быть самой первой функцией менюшника
       по умолчанию размер буфера - 120 Kb */

int MoveMenu(int menuhandler, int left, int top);
    /* устанавливает координаты меню
       неприменима к активному меню
       возвращает GR_OK или
		  GR_NOK, если меню уже активное */

void SetStatusColor(int menuhandler, int col, int bkcol);
   /* устанавливает для меню цвет текста и фона статус-строки */

void AddStatus(int menuhandler, int itemhandler,
	      char *str0, char *str1, char *str2);
   /* инициализирует у ITEMа статус-строку */

void HideItem(int menuhandler, int itemhandler);
   /* делает ITEM недоступным */

void RebirthItem(int menuhandler, int itemhandler);
   /* делает ITEM доступным */

int SwitchReset(int menuhandler, int itemhandler);
   /* записывает в SWITCH_ITEM строку соответствующую переменной
   (т.е. *mainhandler )
   menuhandler, itemhandler - номера меню и itemов
   возрват GM_OK или код ошибки
   необходима при непосредственном изменении переменной SWITCH_ITEMа
	 без использования меню
    */

void SetExitKey(int menuhandler, int key);
   /* назначает клавишу вместо ESC, по котоpой будет осуществляться
      выход из меню   */


/*********************** ВНУТРЕННИЕ ФУНКЦИИ **************************/

int AddStringToStringItem(char *str, char *string, int strlength);
    /* копирует в нужное место строки ITEMа str введеную строку string
       strlength - максимальная длина строки свитча;
       вызывается из InitStringItem и ActivateMenu;
       возврат - GM_OK или сообщение об ошибке */

int DrawMenu(int menuhandler, int saveoption);
    /* рисует заданное меню, сохраняя его или нет (см. define)
    используется в ActivateMenu, SaveLevels
    возвращает GM_OK или код ошибки */

int DrawItem(int menuhandler, int itemhandler);
    /* рисует ITEM в заданном меню,
    используется в DrawMenu, ActivateMenu
    возвращает GM_OK */

int DefineItemPosition(int menuhandler, int itemhandler,
		       int *l, int *t, int *r, int *b);
    /* определяет коотринаты ITEMа на экране
    используется в DrawItem, ActivateMenu и MouseOperate
    возвращает GM_OK */

int WhichItemPosition(int menuhandler, int x, int y);
    /* определяет в какой ITEM данного меню попадают координаты x,y
       используется в MouseOperate
       возвращает номер ITEMа или -1 */

int DrawBordStr(int left, int top, int right, int bordylength,
		int bordgroundcolor, int bordcolor, char *str);
    /* рисует бордюр меню и выводит на него строку
       используется в DrawMenu
       возвращает GM_OK */

int GetStrSumWidth(int menuhandler, int item);
    /* определяет суммарную длину ITEMов в меню (для H-меню)
       используется в DrawItem и DefineItemPosition
       возвращает длину */

int GetBordLength(int menuhandler, int *bordxlength, int *bordylength);
    /* устанавливает размеры бордюра по горизонтали и вертикали в
       зависимости от его типа
       используется в DrawItem и DefineItemPosition
       возвращает GM_OK */

int YesNoSet(char *str, int *yesnohandler, int lang);
    /* копирует в нужное место строки ITEMа Yes или No на соответствующем
       языке (параметр - lang) в соответствии с yesnohandler (не меняя его);
       вызывается из InitYesNoItem и ActivateMenu;
       возврат - GM_OK или сообщение об ошибке */

int SwitchSet(char *str, struct STRINGS *strings, int strlength, int lang);
    /* копирует в нужное место строки ITEMа str строку strings свитча на
       соответствующем языке (параметр - lang);
       strlength - максимальная длина строки свитча;
       вызывается из InitSwitchItem и ActivateMenu;
       возврат - GM_OK или сообщение об ошибке */

void SetFilename(char *str, char *path, int filemask);
    /* копирует в конец строки FILENAME_ITEMа имя файла из пути
       str - строка
       path - путь
       filemask - выводить имя файла с расширением или без (см. дефайны)
       вызывается из InitFilenameItem и ActivateMenu;
       при ошибке выдает сообщение */

int  InitSaveBuf(void);
    /* инициализирует буфер для сохранения меню
       используется в InitMenu
       возвращает GM_OK или код ошибки */

int  SaveMenu(signed long *savepos, long *savesize, int *savehandler,
	     int left, int top, int right, int bottom);
    /* сохраняет меню с экрана в памяти или на диске
       используется в DrawMenu
       возвращает GM_OK или код ошибки */

int  RestoreMenu(int menuhandler);
    /* востанавливает меню на экран
       используется в RestoreLevels или ActivateMenu
       возвращает GM_OK или код ошибки */

int  MemoryMove(signed long srcpos, signed long srcsize);
    /* сдвигает меню в буфере при удалении какого-либо из них
       используется в RestoreMenu
       возвращает GM_OK */

int  FindNextMenu(signed long menupos);
    /* находит следующее более позднее меню в буфере;
       используется в MemoryMove
       возвращает handler, если нет - то -1 */

int  SaveBufferOnDisk(int menuhandler);
    /* сохраняет меню на диске
       используется в SaveMenu и RestoreMenu
       возвращает GM_OK или код ошибки */

int  LastSaveBufCoord(signed long *begin, signed long *end);
    /* возвращает начальную и конечную позицию меню, сохраненного в памяти,
       используется в SaveMenu и RestoreMenu,
       если ничего не сохранялось, возвращает -1, иначе указатель на меню,
       сохраненное последним  */

int  GetSwapFileName(char *mask, int drive, int firstlet, char *filenam);
    /* в соответствии с маской и номером драйва (0-) создает уникальное
       имя файла (firstlet - c какой позиции в маске начинается номер),
       полученное имя записывается в filenam,
       используется в SaveBufferOnDIsk
       возврат - номер файла (1-99) или 0 при неудаче */

int  TakeSwapFileName(int menuhandler, char *mask, int firstlet, char *filenam);
    /* по маске и menuhabdler определяет имя фаила в котором было сохранено
       меню, полученное имя записывается в filenam,
       используется в RestoreMenu
       возврат - GM_OK, если такой файл и вправду есть;
		 GM_NOK - в противном случае  */

int  RestoreLevels(int menuhandler, int levels);
    /* восстанавливает на экране меню, начиная с menuhandler (- это handler)
       в количестве levels,
       используется в ActivateMenu
       возврат -  handler последнего востановленного меню  или
		  -1, если ничего не восстанавливалось */

int  SaveLevels(int srcmenuhandler, int destmenuhandler);
    /* рисует меню, сохраняя их, начиная с srcmenuhandler (- это handler)
       и до destmenuhandler (- это тоже handler);
       используется в ActivateMenu
       возврат GM_OK или код ошибки */

int  GetUpperMenuHandler(int menuhandler, int qlevels);
    /* определяет handler меню на qlevels уровней более высокого,
       чем menuhandler,
       используется в ActivateMenu и MouseOperate
       возвращает - handler на меню или -1 при отсутствии такового */

int  IfHighLet(int let, int menuhandler);
    /* возвращает номер ITEMа или -1, если эта буква в меню не задействована
       используется в ActivateMenu  */

int  SetHighLet(int menuhandler, int itemhandler);
    /* определяет вместо символа номер выделенной буквы для данного ITEMа
       используется во всех Init****Item
       возвращает номер или -1, если такая буква отсутствует */

void AddKeyToBuf(int c);
    /* записывает символ в KeyBuffer
       используется в ActivateMenu и MouseOperate */

int  GetKeyFromBuf(void);
    /* берет символ из KeyBuffer
       используется в GetEvent */

int  GetEvent(void);
    /* возвращает символ из буфера KeyBuffer или если его нет,
       то с клавиатуры
       используется в ActivateMenu */

int MouseOperate(int x, int y);
   /* вызывает реакцию требуемую при нажатии мыши в точке x,y
      используется в GetEvent
      возврат -  0, если x,y не относится к менюшнику
		 1, если относится	 */

int ShowStatus(int menuhandler, int itemhandler);
   /* выводит на экран статус-строку
      используется в DrawItem
      возврат GM_OK */

int SaveStatusScreen(void);
    /* сохраняет экран под статус-строкой
       используется в ShowStatus
       возврат GM_OK или GM_NOK, если stathandler не соответствующий */

int RestoreStatusScreen(void);
    /* восстанавливает экран под статус-строкой
       используется в ShowStatus, ActivateMenu и RestoreMenu
       возврат GM_OK или GM_NOK, если stathandler не соответствующий */

int MoveItemBar(int mhandler, int olditem, int newitem);
  /* передвигает курсор меню со старого itema на новый,
     устанавливает ->curitem
     возвpат - GM_OK пpи успехе или GM_NOK пpи обломе */
/* ! ТОЛЬКО при режиме внешнего управления меню: define OUTER_OPERATION_MODE */

int FindNewItem(int mhandler, int olditem, int rezhim);
  /* ищет соответствующий режиму неспрятанный item
     возвpат - номеp найденного itemа или GM_NOK пpи обломе
     rezhim - см. define (FIRST,LAST,NEXT,PREV) */
/* ! ТОЛЬКО при режиме внешнего управления меню: define OUTER_OPERATION_MODE */

int IfFuncKey(int key, int menuhandler);
  /* возвращает номер FUNCKEY ITEMа, которому соответствует функциональная
     клавиша  или -1, если эта клавиша в меню не задействована */
/* ! ТОЛЬКО при режиме внешнего управления меню: define OUTER_OPERATION_MODE */






void menusound(int menuhandler);
int  getsym(void);
void badkey(void);
void msound(int fr);
int  ftoupper(int c);

//int  getch(void);
//void sound(unsigned frequency);
//void nosound(void);
//void delay(unsigned milliseconds);

int test(void);
int test1(void);

/****************************** UTIL ***************************************/
int writemem(char *flname, char *buf, long bufsize);
int readmem(char *flname, char *buf);
int FindFreeDrive(long freespace);

