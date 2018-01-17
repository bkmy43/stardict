#include <time.h>

extern time_t Ftime;


char *CordInts2String(int g, int m, char *s, char rezhim);
   /*
   преобразует географическую координату в строку типа 25N03 или 123W56
		   rezhim:  T - широта; N - долгота
   */
char *CordInts3String(int g, int m, int sg, char *s, char rezhim);
					   /* T - широта; N - долгота*/
   /* устроена также, но полушарие задается не знаком g или m,
      а соответствующей буквой N/S W/E (sg) */

char *CordDouble2String(double k, char *s, char rezhim);
   /*
   преобразует географическую координату в строку типа 25N03 или 123W56
		   rezhim:  T - широта; N - долгота
   */

void CordString2Ints(char *str, int *g, int *m);
void CordString2Double(char *str, double *k);
void CordString3Ints(char *str, int *g, int *m, int *sg);

time_t GetCurrentTime(void);
   /*
   возвращает текущее время в секундах
   устанавливает Ftime
   */

long GetPassedTime(void);
     /*
     возвращает время (в секундах !), прощедшее с последнего вызова этой
     функции или GetCurrentTime
     устанавливает Ftime
     рекомендуется до первого вызова использовать функцию GetCurrentTime
     */

void Seconds2String(long seconds, char *str, char *format);
     /* преобразует количество секунд в строку по форматам
     типа HH:MM:SS или MM:SS и т.п. */

char *Kord2String(double kord, char *str);
     /* преобразует координату планеты в строку типа ГГЗММ
	в качестве З - номер знака в inner */

char *Date2String(int d, int m, int y, char *str, char *format);
		  /* DD:MM:YY  MM:DD:YYYY */
char *Date2StringE(int d, int m, int y, char epoch, char *str, char *format);
					   /* B/A */
void String2Date(char *str, int *d, int *m, int *y, char *format);
void String2DateE(char *str, int *d, int *m, int *y, char *epoch, char *format);
					   /* B/A */

void CordString2Int(char *str, int *cord);
char *CordInt2String(int cord, char *str, char rezhim);
char *Time2String(int h, int m, char *str, char *format);
void String2Time(char *str, int *h, int *m, char *format);
