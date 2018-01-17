#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include "defines.h"
#include <graphics.h>
#include "graph.h"

//*****************    Буферизированное чтение  ******************
class BufRead
     {
	 unsigned char*     FileBuf;    // буффер для чтения

       public:
	 int       hand ;      //  Файл
	 int   wh;            // Активная половина байта
		       //  Для функций, которые используют  readHalf()
	 boolean   ReachEOF;        // достигнут ли конец файла
	 int       CurPos;      // текущее положение в буфферe для чтения
	 int       UsedBufSize; // Сколько байт считанных из файла находится в буфферe для чтения
	 boolean   ErrorCode; // Была ли допущена ошибка при записи


	 BufRead();
	 virtual void AllocMem(); // аллокирует память
	 virtual void FreeMem();  // освобождает память
	 virtual ~BufRead();
	 void SetFileHandler( int h ) {  hand = h;  RenewBuf(); }
	 int RenewBuf();   // Ф-я обновляет буффер для чтения
	 int  ReadByte( int* buf );   // Буф. Версия

	 void FF() { CurPos++; }
	 void Rew() { CurPos--; }
	 void bigRew() { lseek( hand, 0L, SEEK_SET );
			 ReachEOF = 0;    RenewBuf();  }
	 void SetError() { ErrorCode = 1 ; }

     };


//*****************    Буферизированная запись  ******************
class BufWrite
     {
	 int       hand ;      //  Файл
	 int       CurPos;      // текущее положение в буфферe для записи
	 unsigned char*     FileBuf;    // буффер для записи

       public:
	 int       UsedBufSize; // Сколько байт ожидающих записи в файл
			       // находится в буфферe для записи
	 int   wh;            // Активная половина байта
		       //  Для функций, которые используют  writeHalf()
	 boolean   ErrorCode; // Была ли допущена ошибка при записи

	 BufWrite();
	 virtual void AllocMem(); // аллокирует память
	 virtual void FreeMem();  // освобождает память
	 virtual ~BufWrite();
	 void SetFileHandler( int h ) {  hand = h; }

	 int RenewBuf();   // Ф-я обновляет буффер : списывает на диск его содержимое

	 int  WriteByte( int info );  // Буферизированная версия
	 void FF() { CurPos++; }
	 void Rew() { CurPos--; }
	 void SetError() { ErrorCode = 1 ; }

	 friend int writeHalf( BufWrite& bw, int info ); // этой
			// функции необходим доступ к prived части класса
	 friend int SetTextEnd( BufWrite& bw );

     };


/*********************************************************************
	      Декларации функций для чтения/записи
*********************************************************************/

// halfrw.c
int readHalf( BufRead& br, int* res );
int writeHalf( BufWrite& bw, int info );
int readByte( BufRead& brIn, int* ch ); // Ф-я читает из brIn два полубайта
				// и объединяет их в ch
int writeByte( BufWrite& bwOut, int ch ); // Ф-я записывает в bwOut два
				     // полубайта из ch

// utilrw.c
int GoToChar( BufRead& brIn,char ch ); // Ф-я переходит в файле brIn на символ ch возвр. код ошибки
int ReadInt( BufRead& brIn, int* res ); //  Ф-я читает из файла 2 байта и преобразует их в целое
int WriteInt( BufWrite& bwOut, int i );
int ReadString( BufRead& brIn, char* str );
int ReadName( BufRead& brIn, char* name ); // Ф-я читает строку с текущего положения по символ '^'
int ReadLong( BufRead& brIn, long* res );

char UncodeChar(  int* buffer, int length );
// ф-я читает закодированную строку и возвращает NOK при EOF
int ReadCodedLine( BufRead & brIn, char* str );
//  находит в pbrIndex статью с заданным номером и возвр. ее имя
int GetArtName( BufRead* pbrIndex, int num, char* name );
int GetArtNum( BufRead* pbrIndex, char* artname, int* pnum  );
int GetThNum( BufRead* pbrThemes, char* thname, int* pnum  );
int GetThName( BufRead* pbrThemes, char* thname, int num  );
int GetNextThNameNum( BufRead* pbrThemes, char* buffer, int* pnum );
int GetNextArtNameNum( BufRead* pbrIndex, char* buffer, int* pibuf );
int GetArtNamesList( BufRead* pbrIndex, char** names, int* pmaxnames );
int GetThNamesList( BufRead* pbrThemes, char** names, int* pmaxnames );
int GetHistoryList( BufRead* pbrIndex, char** names, int* qarts );
int GetArtNamesFromTh( int thnum, BufRead* pbrIndex, BufRead* pbrThemes,
		       char** names, int* pmaxnames );
