/********************************************************************
-- STARDICT Project --
                       datautil.h

описания функций реализующих утилиты работы с Dat-файлами
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _DATUTIL_H
#define _DATUTIL_H

#include "defines.h"
#include "rw.h"
#include "tags.h"

/* === Function Can Throw Exeptions === */
void OpenDataFiles();

void CloseDataFiles();

// Пороверка версии dat-файлов и CRC.
/* === Function Can Throw Exeptions === */
int CheckDataFiles();

unsigned GetCRC(  BufRead& brIn  );

int ReadFileHeader(  BufRead & brIn, char* filetype );
int ReadFileHeader(  BufRead & brIn, char* filetype, int* pforver,
                         int* pdatver, unsigned* pCRC);
int ReadCodedBody( BufRead & brIn, char* bigbuf, long maxbuf, unsigned* bodylen);
char UncodeChar(  int* buffer, int length );

int GetNextArtNameNum( BufRead* pbrIndex, char* buffer, int* pibuf );

int GetNextThNameNum( BufRead* pbrThemes, char* buffer, int* pnum );
int GetThemesTableSize( BufRead* pbrThemes, WORD * ps );
int GetThemesTable( BufRead* pbrThemes, int* ThemesTable );
int GetNextKeyNameNum( BufRead* pbrKes, char* buffer, int* pnum );
int GetKeywordsTableSize( BufRead* pbrKeys, WORD * ps );
int GetKeywordsTable( BufRead* pbrKeys, int* KeywordsTable );

#endif


