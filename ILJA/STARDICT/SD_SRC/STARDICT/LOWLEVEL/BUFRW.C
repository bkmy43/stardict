/********************************************************************
-- STARDICT Project --
								 bufrw.c

�-�� ������� BufRead � BufWrite. ��������� ����������� ����������������
������ � ������.
(c) I.Vladimirsky 1997
********************************************************************/

/**************************************************************************
 � ������ ����� ��������� ��������  �������, ����������� ���
 ���������� ����������������� ������/������  �� �����/� ����.
 ��� ������������� ������ ������� ����������� ������������� �������� ������,
 � ��� ������, ����� ���������� ���������/���������� ������ � �����/�� ����
 ���������� ��������.
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>

#include "rw.h"

//------------------------- ������ -------------------------------

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
// ���������������� ������ read
{
  if( CurPos >= UsedBufSize )   // CurPos ��������� �� 0, ������� ���������
                      // ���� � ������ ����� ��� ������� (UsedBufSize-1)
     if( ReachEOF )
        return  EOF;
     else
        RenewBuf();

  *buf = FileBuf[ CurPos ];
  FF();  // ������� � ���������� �����

  return 1;
}
//*************************************************************************

int BufRead :: RenewBuf()
// ���������� ������ ��� ������
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
}

//*************************************************************

//------------------------- ������ -------------------------------

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
                printf("   �������� ! � ������ ��� ������ ��������   \n"
                       "  � � � � � � � � � � � � �   � � � � � � !!!\n");
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
// �-� ���������� � ���� ������ ��� ������
{
  if( ErrorCode == 1 )
       return 0;

  int res=write( hand, FileBuf, UsedBufSize );

  if( res != UsedBufSize )
     {
       printf( " ������ ��� ������ \n " );
       return 0;
     }

  CurPos=0;
  UsedBufSize=0;
  return 1;
}
