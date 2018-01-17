#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>

#include "rw.h"

int readHalf( BufRead& br, int* res )
//    ‘-€  читает из файлового буфера в переменную res первую или вторую
// ( в зависимости от значени€ пер. wh этого буфера) половину  байта по текущему адресу.
// ¬озвращает результат чтени€ ( 1 в случае удачи  или EOF )
 {
   *res=0;

   if( br.ReadByte( res ) != 1 )
       return EOF;


   if( br.wh == 0 ) // если нужна перва€ половина байта
       {
	 *res = (*res) >> 4;
	 br.Rew(); // вернутьс€ на байт назад
       }
   else
       *res =(*res)&0x0F;

   br.wh=++br.wh%2; // перейти  к следующей половине

   return 1;
 }
//************************************************************************

int writeHalf( BufWrite& bw, int info )
//    ‘-€  записывает в буффер значение переменной info в первую или вторую
// ( в зависимости от значени€ пер. wh ) половину  байта по текущему адресу.
// ¬озвращает результат записи ( 1 в случае удачи )
 {
   int res, temp=0;

   if( bw.wh== 0 ) // если нужно записать первую половину байта
       {
	 info = info << 4;
	 res=bw.WriteByte( info );
	 bw.Rew(); // вернутьс€ на байт назад
	 bw.UsedBufSize--; // Ётот байт не считаетс€ использованным
       }
   else
       {
	 temp=bw.FileBuf[ bw.CurPos ]; // ѕоследн€€ запись
	 temp=temp&0xF0;              // обнулить вторую половину
	 temp=temp|info;            // и записать туда знач. buffer
	 info=temp;
	 res=bw.WriteByte( info );
       }

   bw.wh=++bw.wh%2; // перейти  к следующей половине
   return res;
 }
//*************************************************************************
int readByte( BufRead& brIn, int* ch ) // ‘-€ читает из brIn два полубайта
				// и объедин€ет их в ch
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
int writeByte( BufWrite& bwOut, int ch ) // ‘-€ записывает в bwOut два
				     // полубайта из ch
 {
    int res=1;

    if( (res=writeHalf( bwOut, ch>>4 )) != 1 )
	 return res;
    if( (res=writeHalf( bwOut, ch & 0x0F )) != 1 )
	 return res;
    return res;

 }
