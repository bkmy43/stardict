/********************************************************************
-- STARDICT Project --
Описания функций для main файла программы создания файлов данных RGRAD
программа конвертирует файлы в формате RSTEXT 2.0 в DAT-файлы
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _APP_H
#define _APP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <alloc.h>
#include <errno.h>
#include <dos.h>
#include <conio.h>

#include "rw.h"
#include "artread.h"
#include "errors.h"
#include "ctags.h"

#define _ERLIST_TO_FILE

typedef    int  filehand ;
typedef    char BYTE;
typedef    int  WORD;
typedef    long DWORD;

//режимы работы
const TH_READ  = 1;
const ART_READ = 2;
const ART_READ_NOREFTEST = 3;
const HELP     = 4;

const STOP_READ   = 10;

const OFTEN_CODE  = 0xE;
const SELDOM_CODE = 0xF;

// apput.c
int CharToInt( char ch, int* res );  // перевод 16-ой цифры в дес. число
int ReadCompName ( BufRead& brIn, char* name );
int codeChar( char ch, int* buffer );
int WriteCodedString( BufWrite& bw, char* str );
int SetTextEnd( BufWrite& bw );

void BeginMessages( );
void EndMessages( );

void CreateDat();            // при отсутствии DAT-файлов создает новые
int CreateBakFiles( void );      // копирует dat-файлы в bak-файлы
int CreateTmpFiles( void );      // копирует dat-файлы в tmp-файлы
int SaveChanges( void );         // копирует tmp-файлы в dat-файлы
int copyfile(char *fromfile, char *tofile);
int ThExists( int number );  // проверка
int ArtExists( int number );
int StrAlphComp(char* str1, char* str2);

// appdata.c
int  OpenDataFiles(int argc, char* argv[]); //открывает файлы с данными

#endif