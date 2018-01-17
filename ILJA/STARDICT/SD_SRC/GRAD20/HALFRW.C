/********************************************************************
-- STARDICT Project --

Ф-ии осуществляющие запись/чтение  в файлы порциями по пол-байта.
Основаны на использовании буферизации BufRead и BufWrite.
Используются при кодировании текстов в data.dat

(c) I.Vladimirsky 1997
********************************************************************/
#include <stdio.h>
#include "rw.h"

int readHalf( BufRead& br, int* res )
//    Ф-я  читает из файлового буфера в переменную res первую или вторую
// ( в зависимости от значения пер. wh этого буфера) половину  байта по текущему адресу.
// Возвращает результат чтения ( 1 в случае удачи  или EOF )
 {
   *res=0;
   int CurLineOld = br.CurLine;

   if( br.ReadByte( res ) != 1 )
       return EOF;

   if( br.wh == 0 ) // если нужна первая половина байта
       {
         *res = (*res) >> 4;
         br.Rew(); // вернуться на байт назад
         br.CurLine = CurLineOld;
       }
   else
       *res =(*res)&0x0F;

   br.wh=++br.wh%2; // перейти  к следующей половине

   return 1;
 }
//************************************************************************

int writeHalf( BufWrite& bw, int info )
//    Ф-я  записывает в буффер значение переменной info в первую или вторую
// ( в зависимости от значения пер. wh ) половину  байта по текущему адресу.
// Возвращает результат записи ( 1 в случае удачи )
 {
   int res, temp=0;

   if( bw.wh== 0 ) // если нужно записать первую половину байта
       {
         info = info << 4;
         res=bw.WriteByte( info );
         bw.Rew(); // вернуться на байт назад
         bw.UsedBufSize--; // Этот байт не считается использованным
       }
   else
       {
         temp=bw.FileBuf[ bw.CurPos ]; // Последняя запись
         temp=temp&0xF0;              // обнулить вторую половину
         temp=temp|info;            // и записать туда знач. buffer
         info=temp;
         res=bw.WriteByte( info );
       }

   bw.wh=++bw.wh%2; // перейти  к следующей половине
   return res;
 }
//*************************************************************************
int readByte( BufRead& brIn, int* ch ) // Ф-я читает из brIn два полубайта
                                // и объединяет их в ch
 {
    int fHalf,sHalf;
    int res=1;

    if( (res=readHalf( brIn, &fHalf )) != 1 )
         return res;
    if( (res=readHalf( brIn, &sHalf )) != 1 )
         return res;

    *ch = (fHalf<<4) | sHalf;
    return res;
 }
//*******************************************************************
int writeByte( BufWrite& bwOut, int ch ) // Ф-я записывает в bwOut два
                                     // полубайта из ch
 {
    int res=1;

    if( (res=writeHalf( bwOut, ch>>4 )) != 1 )
         return res;
    if( (res=writeHalf( bwOut, ch & 0x0F )) != 1 )
         return res;
    return res;
 }
