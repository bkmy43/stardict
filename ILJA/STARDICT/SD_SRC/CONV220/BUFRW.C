/********************************************************************
-- STARDICT Project --
-จจ ชซ แแฎข BufRead จ BufWrite. ฎงขฎซ๏๎โ ฎแใ้ฅแโขจโ์ กใไฅเจงจเฎข ญญ๋ฅ
็โฅญจฅ จ ง ฏจแ์.
(c) I.Vladimirsky 1997
********************************************************************/

/**************************************************************************
       ,  
   /   / .
        ,
   ,   /   / 
  .
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>

#include "rw.h"

//------------------------- ็โฅญจฅ -------------------------------

BufRead :: BufRead()
  {
        FileBuf = new unsigned char[ FILE_BUF_SIZE ];
        ReachEOF=NO;
        CurPos=0;
        CurLine=0;
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
// ใไฅเจงจเฎข ญญ ๏ ขฅเแจ๏ read
{
  if( CurPos >= UsedBufSize )   // CurPos แ็จโ ฅโแ๏ ฎโ 0, ฏฎํโฎฌใ ฏฎแซฅคญจฉ
                      // ก ฉโ ข กใไฅเฅ แโฎจโ ฏฎค ญฎฌฅเฎฌ (UsedBufSize-1)
     if( ReachEOF )
        return  EOF;
     else
        RenewBuf();

  *buf = FileBuf[ CurPos ];
  if( *buf == '\n' )  CurLine++;

  FF();  // ฏฅเฅฉโจ ช แซฅคใ๎้ฅฌใ ก ฉโใ

  return 1;
}
//*************************************************************************

int BufRead :: RenewBuf()
// ฎกญฎขซฅญจฅ กใไฅเ  คซ๏ ็โฅญจ๏
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

void BufRead :: bigRew()
{
  lseek( hand, 0L, SEEK_SET );
  ReachEOF = 0;    RenewBuf();
  CurLine = 1;
}

//*************************************************************

//------------------------- ง ฏจแ์ -------------------------------

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
                printf("    !        \n"
                       "                       !!!\n");
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
// -๏ ง ฏจแ๋ข ฅโ ข ไ ฉซ กใไไฅเ คซ๏ ง ฏจแจ
{
  if( ErrorCode == 1 )
       return 0;

  int res=write( hand, FileBuf, UsedBufSize );

  if( res != UsedBufSize )
     {
       printf( "    \n " );
       return 0;
     }

  CurPos=0;
  UsedBufSize=0;
  return 1;
}
