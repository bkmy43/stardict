#include <dir.h>

/* ТИПЫ БОРДЮРОВ */
#define NO_BORD       0
#define SINGLE_BORD   1
#define DOUBLE_BORD   2
#define REDUCED_BORD  3

/* параметр для функций REnterString и REnterFilename (rezhim)
   и getsym_filtered (filter), определяющий тип вводимой строки
   или символа */
#define TEXT           0    // любой текст
#define DIGIT_INT      1    // целое число
#define DIGIT_REAL     2    // число с точкой
#define FILENAME_PART  3    // имя файла без расширения
#define FILENAME_FULL  4    // имя файла с расширением
#define DATE           5    // дата

/* значения паpаметpа IfSave в REnterString */
#define  NO             0
#define  YES            1


/* стандартный код возврата */
#define GM_NOK       -1
#define GM_OK         0

/* состояние курсора мыши */
#define GRON        0   // включен и виден
#define GROFF      -1   // выключен
#define GRHIDDEN    1   // включен, но не виден (спрятан)

/* тип адаптера для функции Ropen */
#define GM_DETECT     -1
#define GM_CGA         1
#define GM_EGA         3
#define GM_HERC        7
#define GM_ATT         8
#define GM_VGA         9



/******* кодовые номера ошибок для функции gerrmes (параметр mesnum) ********/

#define GW_DOUBLEINIT   0
#define GW_NOINIT       1

#define NOMEM           2
#define BIGIMAGE        3

/* ошибки менюшника */
#define NOSAVEMEM     100
#define LOWBUFFER     101
#define OPEN_ERROR    102
#define READ_ERROR    103
#define WRITE_ERROR   104
#define CREAT_ERROR   105
#define BAD_TYPE      106
#define KEY_BUFFER    107
#define NOLASTSTRING  108
#define MANYMENUS     109
#define MANYITEMS     110
#define NOMEMENOUGH   111
#define BADSTRLEN     112
#define CANTOUTTEXT   113
#define CANTSAVE      114
#define CANTRESTORE   115
#define CANTDRAW      116
#define CANTFIND      117
#define ALREADYACTIVE 118
#define BADITEMNUM    119
#define BADNAMELEN    120
#define BADITEMTYPE   121

#define BADMENUNUM    122
#define MANYAMENUS    123

#define FU_BADNUM     200
#define FU_NOCLEAR    201
#define FU_PRINTER    202
#define FU_SETFONT    203
#define FU_SETREF     204

#define GC_NOGRAPH     300
#define GC_BADKORD     301
#define GC_NOLINEINIT  302
#define GC_NOLINEMARK  303

#define FT_BADFORMAT   400
#define FT_BADPARAM    401

#define MN_OVERFLOW    500
#define MN_BADTYPE     501
#define MN_LONGSTRING  502

#define H_BADFILE      600
#define H_NOMEM        601
#define H_LONG         602
#define H_NOENTRYEND   603
#define H_MANYENTRIES  604
#define H_BADITEMNUM   605

#define FKEY_ERROR     700
#define FKEY_NOKEY     701
#define FKEY_PRESENT   702
#define FKEY_COLOR     703
#define FKEY_SWITCH    704

/* тип ошибки для функции gerrmes (параметр errtype)
   типы различаются цветом выводимого сообщения,
   а при SYSTEM_ERROR происходит завершение программы */
#define SYSTEM_ERROR   0
#define COMMON_ERROR   1
#define MESSAGE        2
/*************************************************************/

/* для функций типа IfMyKey / IfMyMouse */
const int NOT_MINE=-1;
const int MINE=0;

/* язык */
#define GW_ENG          0
#define GW_RUS          1
#define GW_GER          2

/* тип принтера */
#define G_EPSON         0
#define G_IBM           1
#define G_IBM24         2
#define G_HYUNDAI       3
#define G_EPSON24       4

/**/
#define COL_MULTI       0
#define COL_MONO        1

/* для функций R...Fkey работающих с кнопками */
#define MAX_FKEYS      20  // максимальное одновременной количество кнопок
#define NOCOLOR        -1  // цвет не указан

/* активна или нет кнопка */
#define FKEY_OFF        0
#define FKEY_ON         1

/* структура описывающая кнопку */
struct Rfunckey
   {
   int l,t,r,b;   // границы
   int ot; // расширение поля чувствительного к мыши во все стороны на ot
   int fkey;  // номер клавиши в соответствии с функцией getsym
        // (==getch,  но в двойных кодах клавиш 0 соответствует
        // прибавлению 1000)
   int col,fcol,shcol; // цвет названия кнопки, самой кнопки, тени
       // (если тень не нужна shcol==NOCOLOR)
   char *fkeyname; // строка - название кнопки
   int active; // см. define FKEY_ON или FKEY_OFF
   };


/************************* ОСНОВНЫЕ ФУНКЦИИ *******************************/
/* ! все координаты абсолютные: 0-1023 по горизонтали, 0-768 по вертикали */

void Ropen(int driver);
     /* инициализирует графику
      driver - тип адаптера см. define */
void Rclose(void);
     /* закрывает графику */
void Rexit(int cod);
     /* закрывает графику и выходит из программы с кодом cod */

void Rclr(int col);
     /* очищает экран, заполняя его цветом col */
void Rcol(int col);
     /* устанавливает цвет */
void Rbcol(int col);
     /* устанавливает фон (для монохрома всегда 0) */
void Rtext(int x, int y, char *s);
     /* выводит строку s в координаты x,y */
void Rtextc(int x, int y, char *s, int col);
     /* выводит строку s заданного цвета col в координаты x,y */
int Rtext_with_high_let(int x, int y, char *s, int hletnum, int hletcol);
     /* выводит строку s в координаты x,y
	символ строки номер hletnum выделяется цветом hletcol;
	возвращает GM_OK при успехе или GM_NOK, если не хватает памяти */
void Rtext_with_break(int x, int y, int interval, char *s);
     /* выводит строку s в координаты x,y
	между символами вставляет дополнительный интервал в interval (0-...)
	символов */
void Rsym(int x, int y, char c);
     /* выводит символ c в координаты x,y */
void Rsymc(int x, int y, char c, int col);
     /* выводит символ c заданного цвета col в координаты x,y */
void Rdigit(int x, int y, int digit);
     /* выводит целое число digit в координаты x,y */
void Rdigitc(int x, int y, int digit, int col);
     /* выводит целое число digit заданного цвета col в координаты x,y */
void Rdigit_right(int x, int y, int digit, int qsymbols);
     /* выводит целое число digit в координаты x,y
        с выравниванием до qsymbols по правому краю */
void Rline(int x1, int y1, int x2, int y2);
     /* выводит отрезок с координатами x1,y1 - x2,x2 */
void Rlinec(int x1, int y1, int x2, int y2, int col);
     /* выводит отрезок с координатами x1,y1 - x2,x2 цвета col */
void Rbar(int l, int t, int r, int b);
     /* выводит сплошной прямоугольник */
void Rbarc(int l, int t, int r, int b, int fill, int col);
     /* выводит прямоугольник заданного цвета col и
        заполнителя fill (см. define graphics.h) */
void RThickLinec(int x1, int y1, int x2, int y2, int col,
		 char style, char thickness);
     /* рисует отрезок заданного типа и толщины (см. define graphics.h) */
void Rram(int l, int t, int r, int b);
     /* выводит рамку */
void Rramc(int l, int t, int r, int b, int col);
     /* выводит рамку, заданного цвета col */
void Rcircle(int x, int y, int r);
     /* рисует окружность радиуса r с центром в x,y */
void Rcirclec(int x, int y, int r, int col);
     /* рисует окружность радиуса r с центром в x,y заданного цвета col */
void Rfillcircle(int x, int y, int r, int col);    /* NEW */
     /* заполняет кpуг указанным цветом */
void Rfloodfill(int x, int y, int fill, int col, int bordercol); // NEW
     /* заполняет pегион, огpаниченный цветом bordercol способом
	fill и цветом col, начиная с точки x,y */
void Rpoint_on_circle_coordinates(int *x, int *y, int xx, int yy,
				  int radius, double angle); /* NEW */
     /* рассчитывает стандартные координаты для точки на окружности
	с центром xx,yy, радиусом r и углом (слева, по часовой стрелке)
	angle */
void Rradius(int xx, int yy, double angle, int from, int to); /* NEW */
     /* рисует отрезок радиуса с центром xx,yy, углом angle от
	from до to */
void RMovingCircle(int x, int y, int srcRad, int destRad,
	int fcol, int col);
     /* рисует расширяющуюся окружность с центром в x,y
        от заданного радиуса srcRad до заданного радиуса destRad
        цвет окружности col,  цвет фона fcol */
void Rputpixel(int x, int y, int col);
     /* рисует пиксель заданного цвета col в заданных координатах x,y */
     /* при монохромном адаптере не употреблять для бэкграунда */
unsigned far Rgetpixel(int x, int y);
     /* возвращает цвет пикселя x,y в заданных координатах */
int RSwitchVisualPage(void);
     /* меняет видимую гpафическую стpаницу (для EGA)
	возвpащает номеp (0 или 1) текущей видимой гpафической стpаницы */
int RSwitchActivePage(void);
     /* меняет активную гpафическую стpаницу (для EGA)
	возвpащает номеp (0 или 1) текущей активной гpафической стpаницы */

/****************** ФУНКЦИИ КОНВЕРТИРОВАНИЯ КООРДИНАТ *******************/
void Rconvert(int *rx, int *ry);
     /* конвертирует координаты из стандартных в реальные */
void Rbackconvert(int *rx, int *ry);
     /* конвертирует координаты из реальных в стандартные */

/**************** РАСШИРЕННЫЕ ФУНКЦИИ ДЛЯ ВЫВОДА СТРОК *******************/
void RPutLine(int x, int y, char *s, int SymWidth, int col);
     /* выводит на экран строку s заданного цвета col в координаты x,y
        с заданным интервалом между буквами SymWidth (в стандартных
        единицах) */
void RPutLineClear(int x, int y, char *s, int SymWidth, int col);
     /* выводит на экран строку s заданного цвета col в координаты x,y
        с заданным интервалом между буквами SymWidth (в стандартных
        единицах), очистив для нее пространство в соответствии
        с текущим бэкграундом  */
void RPutLineWithHighSym(int x, int y, char *s, int SymWidth, int col,
			 int SymNum, int highcol);
     /* выводит на экран строку s заданного цвета col в координаты x,y
        с заданным интервалом между буквами SymWidth (в стандартных
        единицах), очистив для нее пространство в соответствии
        с текущим бэкграундом и подсвечивая символ SymNum (номер в
        строке) заданным цветом highcol */
void RPutLineWithCursor(int x, int y, char *s, int SymWidth, int col,
			 int SymNum, int highcol, int bcol);
     /* выводит на экран строку s заданного цвета col в координаты x,y
        с заданным интервалом между буквами SymWidth (в стандартных
        единицах) с курсором цвета highcol под символом номер SymNum
        текущий фон бэкграунда должен указываться в bcol (! сам он
        не устанавливается) */
void RPaintString(char *str, int n, int qstr_on_page, int first,
		  int top, int left, int interval,
		  int groundcolor, int textcolor, int highcolor,
                  int cur);  /* если cur==-1, то фон */
     /* вывести (или соответственно не вывести) строку str из
        списка строк, если
          - ее номер в списке n,
          - всего на странице помещается qstr_on_page строк
          - первая строка на странице имеет номер first
          - верхний левый угол страницы имеет координаты top,left
          - интервал между строками interval (в стандартных единицах)
          цвет бэкграунда groundcolor,
          цвет строки если строка выделенная (n==cur) highcolor, либо
             textcolor в другом случае


/************** ФУНКЦИИ СОХРАНЕНИЯ КУСКА ЭКРАНА В ПАМЯТИ *******************/
void Rgetimage(int l, int t, int r, int b, char *buf);
     /* запомнить в buf (! не забудьте предварительно выделить память)
        прямоугольник экрана с координатами l,t,r,b */
void Rputimage(int l, int t, char *buf, int op);
     /* вывести из в buf на экран прямоугольник в координаты l,t
        (верхний левый угол), op == соответствующему параметру из
        putimage */
unsigned far Rimagesize(int l, int t, int r, int b);
     /* замерить в байтах память, необходимую для сохранения прямоугольника
        экрана с координатами l,t,r,b */


/********************** ФУНКЦИИ ВЫВОДА СООБЩЕНИЙ *************************/
int  gerrmes(int mesnum, char *proc, int errnum, int errtype);
     /* выдает сообщение в соответствии с mesnum (см. define)
        в proc  указывается  константа  - имя процедуры,  где возникла
        ошибка, errnum - номер сообщения (на случай, если в одной про-
        цедуре несколько однотипных сообщений), errtype - тип ошибки
        (см.  define);
        возвращает всегда 0 */
int  Rmessage(char *str, int col, int fcol, int bordcol);
     /* выводит строку str, цвета col на фоне цвета fcol с бордюром
        цвета bordcol и ждет нажатия клавиши или мыши
        возвращает нажатый символ (или -1/-2 при нажатии левой/правой
        клавиши мыши) */
void termes(char *str, char *func);
     /* завершает программу, выдав предварительно сообщение str
        про ошибку в функции func */


/*************** ФУНКЦИИ ПОДДЕРЖКИ МЫШИ И КЛАВИАТУРЫ ***********************/
int  getsym_filtered(int filter);
     /* ждет пока не будет нажата клавиша допускаемая параметром filter
        (см. define),
        возвращает номер нажатой клавиши в соответствии с
        функцией getsym (==getch,  но в двойных кодах клавиш
        0 соответствует прибавлению 1000) */
int  getsym(void);
     /* ждет пока не будет нажата клавиша,
        возвращает номер нажатой клавиши (==getch, но в двойных кодах клавиш
        0 соответствует прибавлению 1000) */

void RbarcM(int l, int t, int r, int b, int fill, int col);
     /* = Rbarc, но мышь гасится на время рисования */
void RsymcM(int x, int y, char c, int col);
     /* = Rsymc, но мышь гасится на время рисования */
void RtextcM(int x, int y, char *s, int col);
     /* = Rtextc, но мышь гасится на время рисования */
void RramcM(int l, int t, int r, int b, int col);
     /* = Rramc, но мышь гасится на время рисования */

int  Rmouse_set(void);
     /* инициализировать мышь
        возвращает GRON при удаче, GROFF при неудаче */
int  Rmouse_status(int *x, int *y);
     /* определить координаты (реальные, а не стандартные !!!)
        мыши при последнем нажатии
	возврат:  0 - не была нажата
                  1 - была нажата левая клавиша
                  2 - была нажата правая клавиша
     */
void Rmouse_hide(void);
     /* убрать мышинную стрелку с экрана */
void Rmouse_show(void);
     /* показать мышинную стрелку на экране */
void Rmouse_page(int page);
     /* показывать мышинный куpсо на гpафической стpанице page */

int Revent(int *x, int *y);
    /* возвращает нажатую клавишу
       или -1, если нажали мышь - тогда в x и y - координаты (стандартные)
               последнего нажатия мыши
       выбор мышью кнопок (см. функции R...Fkey) обрабатывается,
               как нажатие соответствующей клавиши
    */

/***************** ФУНКЦИИ ДЛЯ РАБОТЫ С КНОПКАМИ ***************************/
void RInitFkey(int x, int y, int otstup, char *fkeyname, int fkey,
	       int col, int fcol, int shadowcol);
     /* создает на экране кнопку с координатами x,y левого верхнего угла
        otstup - расширение поля чуствительности к нажатию мыши по всем
                 направлениям (в стандартных единицах)
        fkeyname - строка - название клавиши
        fkey - номер клавиши по getsym (см. define в keys.h)
        col, fcol, shadowcol - цвет названия, фона и тени соответственно
                   если тени не нужно, указывайте NOCOLOR
     */


void RSwitchFkey(int fkey, int rezhim);
     /* переводит кнопку (fkey - номер клавиши) в соответствующий режим
     rezhim: FKEY_ON - рабочее состояние;
             FKEY_OFF - нерабочее состояние */

void RRemoveFkey(int fkey);
     /* удаляет кнопку (о стирании кнопки с экрана заботьтесь
        самостоятельно) (fkey - номер клавиши) */

void RSetFkey(int l, int t, int r, int b, int ot, int fkey,
	      char *fkeyname, int col, int fcol, int shcol);
     /* функция ВНУТРЕННЕГО использования - самостоятельно не используется !
        помещает в буфер клавишу и ее координаты
        на экране для использования мышью */


/*********************** ВВОД СТРОКИ С КЛАВИАТУРЫ *********************/
char *REnterString(char *str, int x, int y, int length, int SymWidth,
		  int col, int bcol, int curscol, int rezhim, int IfSave);
     /* вводит строку по адресу str (! не забудьте выделить память)
        координаты окна ввода - x,y
        length - максимальная длина вводимой строки
        SymWidth -  расстояние между символами (в стандартных единицах)
        bcol, col, curscol - цвета фона, строки, курсора соответственно
	rezhim - тип строки (см. define)
	IfSave - YES или NO (см. define) сохpанять и восстанавливать
		 ли затеpтое место на экpане */

char *REnterStringBox(char *str, int left, int top, int length, int SymWidth,
		  int col, int bcol, int curscol, int rezhim,
		  int boxcol, int bordcol, int mescol,
		  char *mes1, char *mes2);

int IfDate(char *str);
     /* определяет подходит или нет строка под формат даты и соответственно
       возвращает GM_OK или GM_NOK */
int  days_in_month (int m, int y);
     /* возвращает количество дней в месяце m (1-12) года y */
int  REnterFilename(char *name, int x, int y, int rezhim,
		   int col, int bcol, int curscol);
     /* Вводит имя файла с расширением (rezhim==FILENAME_FULL) или
        без (FILENAME_PART) по адресу name
        координаты окна ввода - x,y
        bcol, col, curscol - цвета фона, строки, курсора соответственно
	возврат  0 - OK;  -1 - ошибка */

/**************************** FONTER *************************************/
#define FU_OK         0
#define FU_NOFILE    -1
#define FU_CANTOPEN  -2
#define FU_BADFILE   -3
#define FU_NOMEM     -4
#define FU_CANTREAD  -5

#define FU_SMALLFONT   8
#define FU_LARGEFONT  32

#define FU_EPSON      0
#define FU_IBM        1

#define FU_SYSTEMSYMBOL '~'

#define FUB(s) r=biosprint(0,s,0); if ((r & 0x01)||(r & 0x08)||(r & 0x020)||(r & 0x080)==1) return(-1)

void FUOn(void);
void FUOff(void);
void FUSetCurrentFont(struct FUFONT *f);                       // FONT
void FUSetCurrentRef(struct FUREF *f);                            // REF
void FUSetRef(struct FUREF *f, int qsym, char *num, char *sym);   // REF
int FUIfinRef(int sym);                                           // REF
/* если ASCII символ попадает в FUCurrentRef, возвращается его номер в
   шрифте,   если нет, то -1 */
void FUPutSym(int x, int y, int num, int fonttype);            // FONT
void FUPutSymc(int x, int y, int num, int fonttype, int col);  // FONT
void FUtext(int x, int y, char *s);                            // FONT  REF
char *FUConvertC(char *s);                                        // REF
int  FUPrintSym(int num, int printer);                         // FONT
     /* возврат - 0-OK, -1 -ошибка */
char *FULoadFont(struct FUFONT *font, int fonttype);
void FUReleaseFont(struct FUFONT *f);

void RPutSym(int x, int y, char *buf, int num, int fonttype);
int  RPrintSym(char *buf, int num, int printer);

struct FUFONT
   {
   char filename[MAXPATH];
   char *buf;
   int qsym;
   };

struct FUREF
   {
   char *num; // номер в шрифте
   char *sym; // номер ASCII символа
   int qsym;
   };


/*************************** GWAIT *******************************/
#ifndef  _TIME_T
#define  _TIME_T
typedef long time_t;
#endif

struct GWAIT
{
char *buf;
int l,t,r,b;
int curdigit;
int fcol,pcol,tcol;
time_t time;
char elapstime[6],estimtime[6];
};

void GWaitInit(int l, int t, int lang, int srcdigit,
	       int col, int fcol, int rcol, int pcol, int tcol);
void GWaitChange(int digit);
void GWaitClose(void);


void Rsound(int freq, int millisec);
     /* издает звук частоты freq длинной millisec миллисекунд */


/**************************** CURSOR *************************************/
/* Функции внутреннего пользования. Файл gcursor.c */

#define HWING        5
#define VWING        5

#define GC_OFF      -1
#define GC_ON        0
#define GC_HIDDEN    1

#define MAX_MARKERS 20

struct GCURSOR
   {
   int  status;
   int  x,y,l,t,r,b,col;
   unsigned save[HWING*2+VWING*2+1];
   };

/* все в абсолютных координатах */

void GCInit(int l, int t, int r , int b, int col);
   /* инициализация курсора, устанавливаются границы передвижения и цвет */
void GCSet(int x, int y);
   /* устанавливается положение курсора, курсор выводится на экран */
void GCShow(void);
   /* курсор выводится на экран */
void GCHide(void);
   /* курсор прячется */
void GCputpixel(int x, int y, int col);
int  GCgetpixel(int x, int y);
void GCMove(int direction);
   /* курсор передвигается в соответствующем направлении */
void GCInfo(void);
   /* выводит на экран информацию о курсоре */
void GCMark(void);
   /* перекрашивает пиксель в запрашиваемый цвет */
void GCClearMark(void);
   /* очищает все маркеры */
int GCDefineMarker(int *l, int *t, int *r, int *b, int col);
   /* определяет реальные (не абсолютные !!!) координаты по двум маркерам
      заданного цвета
      возврат -  0 - если все удачно
		-1 - маркер не найден
   */
void GCMarkLineInit(int col);
void GCMarkLine(void);
void GCRestoreLine(void);
void GCDeleteLines(void);
void GCGetMarkersInfo(int col);
void GCMarkLineExit(void);

/**************************/  /* NEW */
double fsin(double x);
double fcos(double x);
