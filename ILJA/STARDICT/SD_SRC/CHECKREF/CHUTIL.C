/********************************************************************
-- STARDICT Project --
Программа CheckRef
Дополнительные утилиты для проверки целостности ссылок

(c) I.Vladimirsky 1997
********************************************************************/
#include <stdio.h>
#include "chutil.h"

// Имена исходных файлов
extern char InNames[MAX_IN_FILES][MAX_FILE_NAME_LEN];

// Буфера для всех исходных файлов
extern BufRead* pbrIn[MAX_IN_FILES];

// сюда будет занесена информация обо всех произошедших ошибках
extern TError CurError;

//********************************************************************
// Функция заполняет массив, содержащий номера всех существующих статей
void GetAllNumbers( int * AllNumbers, int* size )
{
   int s=0;
   *size = 0;

   for( int i=0; i< MAX_IN_FILES && pbrIn[i] != NULL; i++ )
    {
      printf("      * Получение списка номеров статей из файла %s.\n", InNames[i]);
      (pbrIn[i])->bigRew();
      GetNumbersFromFile( pbrIn[i], AllNumbers + *size, &s );
      *size += s;
    }
}

//********************************************************************
// Функция добавляет в массив номера содержащихся в данном файле статей
void GetNumbersFromFile( BufRead* pbrIn, int * AllNumbers, int * size)
{
   *size = 0;
   int ibuf;

   while( GoToChar( *pbrIn, '<' ) != EOF )
    {
       SeekSpaces(*pbrIn);
       if(  pbrIn->ReadByte( &ibuf ) != 1 )
           CurError.ProceedError( WRONG_TAG );

       if( (char)ibuf != '#' )
           continue;
       SeekSpaces(*pbrIn);
       if( ReadIntFromChar( *pbrIn, AllNumbers + *size ) != 1 )
           CurError.ProceedError( WRONG_TAG );
       (*size)++;
    }
}

//********************************************************************
// Проверка всех ссылок во всех файлах
void CheckAllRefs( int * AllNumbers, int size )
{
   for( int i=0; i< MAX_IN_FILES && pbrIn[i] != NULL; i++ )
    {
      printf("      * Проверка ссылок в файле %s.\n", InNames[i]);
      (pbrIn[i])->bigRew();
      CheckRefsInFile( pbrIn[i], AllNumbers, size );
    }
}

//********************************************************************
//Проверка всех ссылок в файле
void CheckRefsInFile( BufRead* pbrIn, int * AllNumbers, int size)
{
   int ibuf;

   while( GoToChar( *pbrIn, '<' ) != EOF )
    {
       SeekSpaces(*pbrIn);
       if(  pbrIn->ReadByte( &ibuf ) != 1 )
           CurError.ProceedError( WRONG_TAG );

       if( (char)ibuf != 'L' )
           continue;
       SeekSpaces(*pbrIn);
       if( ReadIntFromChar( *pbrIn, &ibuf ) != 1 )
           CurError.ProceedError( WRONG_TAG );

       for( int i=0; i<size, AllNumbers[i] != ibuf; i++ )
       if( i== size )
         {
            CurError.line = pbrIn->CurLine;
            CurError.ProceedError( REF_TO_BAD_OBJECT );
            CurError.line = 0;
         }

    }
}

