/**************************************************************************
 В ДАННОМ ФАЙЛЕ НАХОДЯТСЯ ОПИСАНИЯ  ФУНКЦИЙ, НЕОБХОДИМЫХ ДЛЯ
 ВЫПОЛНЕНИЯ БУФЕРИЗИРОВАННОГО ЧТЕНИЯ/ЗАПИСИ  ИЗ ФАЙЛА/В ФАЙЛ.
 ПРИ ИСПОЛЬЗОВАНИИ ДАННЫХ ФУНКЦИЙ ЗНАЧИТЕЛЬНО УВЕЛИЧИВАЕТСЯ СКОРОСТЬ РАБОТЫ,
 В ТОМ СЛУЧАЕ, КОГДА НЕОБХОДИМО СЧИТЫВАТЬ/ЗАПИСЫВАТЬ ДАННЫЕ С ДИСКА/НА ДИСК
 НЕБОЛЬШИМИ ПОРЦИЯМИ.
 **************************************************************************/
#include "rw.h"



//------------------------- чтение -------------------------------

BufRead :: BufRead()
  {
	FileBuf = new unsigned char[ FILE_BUF_SIZE ];
	ReachEOF=NO;
	CurPos=0;
	UsedBufSize=0;
	wh=0;
	ErrorCode=0;

  }

//*************************************************************************
BufRead :: ~BufRead()
 {
	delete FileBuf;
 }
//*************************************************************************

int BufRead :: ReadByte( int* buf )
// Буферизированная версия read
{
  if( CurPos >= UsedBufSize )   // CurPos считается от 0, поэтому последний
		      // байт в буфере стоит под номером (UsedBufSize-1)
     if( ReachEOF )
	return  EOF;
     else
	RenewBuf();

  *buf = FileBuf[ CurPos ];
  FF();  // перейти к следующему байту

  return 1;
}
//*************************************************************************

int BufRead :: RenewBuf()
// обновление буфера для чтения
{
  int res;

  if( ErrorCode == 1 )
       return 0;

  res=read( hand, FileBuf, FILE_BUF_SIZE );

  if( res != FILE_BUF_SIZE )
    {
      UsedBufSize = res;
      ReachEOF = YES;
    }
  else
      UsedBufSize = FILE_BUF_SIZE;

  CurPos=0;
  return res;
}
//*************************************************************

//------------------------- запись -------------------------------

BufWrite :: BufWrite()
  {
	FileBuf = new unsigned char[ FILE_BUF_SIZE ];
	CurPos=0;
	UsedBufSize=0;
	wh=0;
	ErrorCode=0;
  }

//*************************************************************************
BufWrite :: ~BufWrite()
 {
	delete FileBuf;
	if( UsedBufSize != 0 )
		printf("   ВНИМАНИЕ ! В ,БУФЕРЕ ДЛЯ ЗАПИСИ ОСТАЛИСЬ   \n"
		       "  Н Е С О Х Р А Н Е Н Н Ы Е   Д А Н Н Ы Е !!!\n");
 }
//*************************************************************************
int BufWrite :: WriteByte( int info )
  {
    int res=1;

    if( UsedBufSize == FILE_BUF_SIZE )
       res=RenewBuf();

    FileBuf[ CurPos ] = info;
    UsedBufSize++;
    FF();

    return res;
  }

//************************************************************
int BufWrite :: RenewBuf()
// Ф-я записывает в файл буффер для записи
{
  if( ErrorCode == 1 )
       return 0;

  int res=write( hand, FileBuf, UsedBufSize );

  if( res != UsedBufSize )
     {
       printf( " ОШИБКА ПРИ ЗАПИСИ \n " );
       return 0;
     }

  CurPos=0;
  UsedBufSize=0;
  return 1;
}
