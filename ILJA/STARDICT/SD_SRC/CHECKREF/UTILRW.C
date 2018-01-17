/********************************************************************
-- STARDICT Project --
Дополнительные утилиты осуществляющие запись/чтение  в файлы
порциями по пол-байта.
Основаны на использовании буферизации BufRead и BufWrite.
(c) I.Vladimirsky 1997
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>

#include "rw.h"

int GoToChar( BufRead& brIn, char ch, char stopcode )
// Ф-я переходит в файле brIn на символ ch возвр. код ошибки
// поиск осуществляется до тех пор пока не встретится stopcode,
// причем если stopcode не указан при вызове то по умолчанию он = EOF
  {
     int res=0;

     while(  res != ch  )
        {
           if ( brIn.ReadByte( &res ) == EOF )
               return EOF;

           if( res == stopcode )
               return STOP_CODE_FOUND;
        }
    return 1;
  }
//***********************************************************************
int GoToNthChar( int n, BufRead& brIn, char ch )
// Ф-я переходит в файле brIn на n-й символ 'ch' возвр. код ошибки
{
  int res=0;
  for( int i=0; i<n; i++ )
    if( (res = GoToChar( brIn, ch )) != 1 )    // перейти на след. ch
        return res;                           // вернуть код ошибки
  return 1;
}
//***********************************************************************
int ReadInt( BufRead& brIn, int* res )
//  Ф-я читает из файла 2 байта и преобразует их в целое
  {
     int b1,b2;

     if (  brIn.ReadByte(  &b2 ) != 1 || brIn.ReadByte(  &b1 ) != 1 )
            return 0;

     *res = ( b1 << 8)|b2 ;
     return 1;
  }

int ReadInt( BufRead& brIn, unsigned* res )
//  Ф-я читает из файла 2 байта и преобразует их в целое
  {
     int b1,b2;

     if (  brIn.ReadByte(  &b2 ) != 1 || brIn.ReadByte(  &b1 ) != 1 )
            return 0;

     *res = ( b1 << 8)|b2 ;
     return 1;
  }

//**************************************************************************
int ReadIntFromChar( BufRead& brIn, int* res )
//  Ф-я читает из файла набор цифр до пробела или '\n'
// и преобразует их в целое
  {
     char buffer[256] = {0};
     int ibuf;
     int i=0;
     do
      {
         if( i == 255 )  return 0;
         if( brIn.ReadByte( &ibuf ) != 1 )    return 0;
         buffer[i] = (char)ibuf;
         i++;
      }
     while(! ((buffer[i-1]==' ') || (buffer[i-1]=='\n') ||
                 (buffer[i-1]=='>')) );

     *res = atoi( buffer );
     brIn.Rew();
     return 1;
  }
//**************************************************************************
int SeekSpaces( BufRead& brIn ) //промотка пробелов и '\n'
{
// пропуск пробелов и '\n'
   int buffer;
   do
      if( readByte( brIn, &buffer) != 1 )
            return 0;
   while( buffer == ' ' || buffer == '\n' || buffer == '\r');

   brIn.Rew();
   return 1;
}
//**************************************************************************
int WriteInt( BufWrite& bwOut, int i )
  {
     int b1,b2;

     if( i<0 || i> 4095 )
         return 0;

     b1 = i>>8;
     b2 = i&0x00FF;


     if (  bwOut.WriteByte(  b2 ) != 1 || bwOut.WriteByte(  b1 ) != 1 )
              return 0;

     return 1;
  }
//***************************************************************************
int ReadString( BufRead& brIn, char* str )
  {
    int i;

    for(  i=0 ; ; i++ )
       {
         if( brIn.ReadByte( (int*)(str + i) )!=1 )
              if( i==0 )   return 0;
              else         break;

         if ( str[i] == '\r' )    i--;

         if ( str[i] == '\n' )    break;
       }

    str[i]=0;

    return 1;

  }
//****************************************************************************
int ReadName( BufRead& brIn, char* name )
// Ф-я читает строку с текущего положения по символ '^'
  {
    int i;
    name[0] = 0 ;

    for(  i=0 ; name[ i-1 ] != '^' ; i++ )
        if( brIn.ReadByte( (int*)(name + i) ) != 1 )
            return 0;


    name[i-1]=0;

    return 1;

 }

//*********************************************************************
int ReadLong( BufRead& brIn, long* res )
//  Ф-я читает из файла 4 байта и преобразует их в long
  {
     int b1, b2, b3, b4;

     if (  brIn.ReadByte(  &b1 ) != 1 || brIn.ReadByte(  &b2 ) != 1 ||
           brIn.ReadByte(  &b3 ) != 1 || brIn.ReadByte(  &b4 ) != 1  )
            return 0;

     *res = ((long)b1<<24) | ((long)b2<<16) | ((long)b3 << 8) | (long)b4 ;
     return 1;
  }

//**************************************************************************
