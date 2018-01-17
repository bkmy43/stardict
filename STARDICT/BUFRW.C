/**************************************************************************
 � ������ ����� ��������� ��������  �������, ����������� ���
 ���������� ����������������� ������/������  �� �����/� ����.
 ��� ������������� ������ ������� ����������� ������������� �������� ������,
 � ��� ������, ����� ���������� ���������/���������� ������ � �����/�� ����
 ���������� ��������.
 **************************************************************************/
#include "rw.h"



//------------------------- �⥭�� -------------------------------

BufRead :: BufRead()
  {
	AllocMem();
	ReachEOF=NO;
	CurPos=0;
	UsedBufSize=0;
	wh=0;
	ErrorCode=0;

  }

void BufRead :: AllocMem()
  {
	FileBuf = NULL;
	FileBuf = new unsigned char[ FILE_BUF_SIZE ];
	if( FileBuf  == NULL )
	    gerrmes(NOMEM,"BufRead :: AllocMem",0,SYSTEM_ERROR);
  }

BufRead :: ~BufRead()
 {
	FreeMem();
 }

void BufRead :: FreeMem()
 {
	delete FileBuf;
 }

//*************************************************************************

int BufRead :: ReadByte( int* buf )
// ���ਧ�஢����� ����� read
{
  if( CurPos >= UsedBufSize )   // CurPos ��⠥��� �� 0, ���⮬� ��᫥����
		      // ���� � ���� �⮨� ��� ����஬ (UsedBufSize-1)
     if( ReachEOF )
	return  EOF;
     else
	RenewBuf();

  *buf = FileBuf[ CurPos ];
  FF();  // ��३� � ᫥���饬� �����

  return 1;
}
//*************************************************************************

int BufRead :: RenewBuf()
// ���������� ���� ��� �⥭��
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

//------------------------- ������ -------------------------------

BufWrite :: BufWrite()
  {
	AllocMem();
	CurPos=0;
	UsedBufSize=0;
	wh=0;
	ErrorCode=0;
  }

void BufWrite :: AllocMem()
  {
	FileBuf = NULL;
	FileBuf = new unsigned char[ FILE_BUF_SIZE ];
	if( FileBuf  == NULL )
	    gerrmes(NOMEM,"BufWrite :: AllocMem",0,SYSTEM_ERROR);
  }
//*************************************************************************
BufWrite :: ~BufWrite()
 {
	FreeMem();
	if( UsedBufSize != 0 )
		printf("   �������� ! � ,������ ��� ������ ��������   \n"
		       "  � � � � � � � � � � � � �   � � � � � � !!!\n");
 }

void BufWrite :: FreeMem()
 {
	delete FileBuf;
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
// �-� �����뢠�� � 䠩� ����� ��� �����
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