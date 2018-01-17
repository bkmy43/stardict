/********************************************************************
-- STARDICT Project --
Описания ф-ий и классов для записи/чтения  в файлы порциями по пол-байта.
Основаны на использовании буферизации BufRead и BufWrite.
Используются при кодировании текстов в data.dat
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _RW_H
#define _RW_H

#include <stdio.h>
typedef  unsigned char boolean;
const STOP_CODE_FOUND = -2; // для ф-ии GoToChar

// размер файлового буфера
#define FILE_BUF_SIZE    1024
#define YES      1
#define NO       0

//*****************    Буферизированное чтение  ******************
class BufRead
     {
       public:
	      int hand ;      //  Файл
	      unsigned char* FileBuf;    // буффер для чтения

	      int wh;    // Активная половина байта
		               //  Для функций, которые используют  readHalf()
	      boolean ReachEOF;        // достигнут ли конец файла
	      int CurPos;      // текущее положение в буфферe для чтения
         unsigned long CurLine; // Текущая строка
	      int UsedBufSize; // Сколько байт считанных из файла находится в буфферe для чтения
	      boolean ErrorCode; // Была ли допущена ошибка при записи


     	 BufRead();
     	 ~BufRead();
       void SetFileHandler( int h ) {  hand = h;  RenewBuf(); }
       int RenewBuf();   // Ф-я обновляет буффер для чтения
       int  ReadByte( int* buf );   // Буф. Версия

       void FF() { CurPos++; }
       void Rew() { CurPos--; }
       void SetError() { ErrorCode = 1 ; }
       void bigRew();
     };

//*****************    Буферизированная запись  ******************
class BufWrite
     {
       public:
         int       hand ;      //  Файл
         int       CurPos;      // текущее положение в буфферe для записи
         unsigned char*     FileBuf;    // буффер для записи

         int UsedBufSize; // Сколько байт ожидающих записи в файл
                         // находится в буфферe для записи
         int wh;        // Активная половина байта
                       //  Для функций, которые используют  writeHalf()
         boolean   ErrorCode; // Была ли допущена ошибка при записи

         BufWrite();
         ~BufWrite();
         void SetFileHandler( int h ) {  hand = h; }

         int RenewBuf();   // Ф-я обновляет буффер : списывает на диск его содержимое

         int  WriteByte( int info );
         void FF() { CurPos++; }
         void Rew() { CurPos--; }
         void SetError() { ErrorCode = 1 ; }

         friend int writeHalf( BufWrite& bw, int info ); // этой
                        // функции необходим доступ к prived части класса
         friend int SetTextEnd( BufWrite& bw );

     };

/*********************************************************************
              функции для чтения/записи
*********************************************************************/

// halfrw.c
int readHalf( BufRead& br, int* res );
int writeHalf( BufWrite& bw, int info );
int readByte( BufRead& brIn, int* ch ); // Ф-я читает из brIn два полубайта
                                // и объединяет их в ch
int writeByte( BufWrite& bwOut, int ch ); // Ф-я записывает в bwOut два
                                     // полубайта из ch

// utilrw.c

int GoToChar( BufRead& brIn, char ch, char stopcode = EOF );
// Ф-я переходит в файле brIn на символ ch возвр. код ошибки
// поиск осуществляется до тех пор пока не встретится stopcode,
// причем если stopcode не указан при вызове то по умолчанию он = EOF
int GoToNthChar( int n, BufRead& brIn, char ch );
int SeekSpaces( BufRead& brIn ); //промотка пробелов и '\n'
// Ф-я переходит в файле brIn на n-й символ 'ch' возвр. код ошибки
int ReadInt( BufRead& brIn, int* res ); //  Ф-я читает из файла 2 байта и преобразует их в целое
int ReadInt( BufRead& brIn, unsigned* res );
int ReadIntFromChar( BufRead& brIn, int* res ); // Читает строку до пробела
                                              // и преобразует ее в int
int WriteInt( BufWrite& bwOut, int i );
int ReadString( BufRead& brIn, char* str );
int ReadName( BufRead& brIn, char* name ); // Ф-я читает строку с текущего положения по символ '^'
int WriteString( BufWrite& bwOut, char* str );
int WriteName( BufWrite& bwOut, char* name ); // Ф-я записывает строку, заканчивает ее символом '^'
int ReadLong( BufRead& brIn, long* res );
unsigned GetCRC(  BufRead& brIn  );

#endif