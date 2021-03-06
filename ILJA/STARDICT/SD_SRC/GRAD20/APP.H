/********************************************************************
-- STARDICT Project --
���ᠭ�� �㭪権 ��� main 䠩�� �ணࠬ�� ᮧ����� 䠩��� ������ RGRAD
�ணࠬ�� ���������� 䠩�� � �ଠ� RSTEXT 2.0 � DAT-䠩��
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
#include "defines.h"

#define FORMAT_VERSION 20
#define DATA_VERSION 00

typedef    int  filehand ;
//typedef    char BYTE;

//०��� ࠡ���
const TH_READ  = 1;
const ART_READ = 2;
const ART_READ_NOREFTEST = 3;
const HELP     = 4;

const STOP_READ   = 10;

// apput.c
int CharToInt( char ch, int* res );  // ��ॢ�� 16-�� ���� � ���. �᫮
int ReadCompName ( BufRead& brIn, char* name );
int codeChar( char ch, int* buffer );
int WriteCodedString( BufWrite& bw, char* str );
int SetTextEnd( BufWrite& bw );

void CreateDat();            // �� ������⢨� DAT-䠩��� ᮧ���� ����
int WriteFileHeader( BufWrite & bwOut, char filetype );
int ReadFileHeader(  BufRead & brIn, char* filetype );
unsigned GetCRC(  BufRead& brIn  );
int SetCRC(  filehand fhIn  );

int CreateBakFiles( void );      // ������� dat-䠩�� � bak-䠩��
int CreateTmpFiles( void );      // ������� dat-䠩�� � tmp-䠩��
int SaveChanges( void );         // ������� tmp-䠩�� � dat-䠩��
int copyfile(char *fromfile, char *tofile);
int ThExists( int number );  // �஢�ઠ
int ArtExists( int number );
int StrAlphComp(char* str1, char* str2);
void strtrim(char* str); //�-� 㡨ࠥ� �஡��� � ���� ��ப�
void Dos2Win1251(char* str); //��४���஢�� ��ப� �� ��� ����஢�� � Win1251
void Dos2Win1251(char& ch); //��४���஢�� ᨬ���� �� ��� ����஢�� � Win1251
void Dos2Win1251(int& i);  //��४���஢�� ᨬ���� �� ��� ����஢�� � Win1251

int  OpenDataFiles(int argc, char* argv[]); //���뢠�� 䠩�� � ����묨
void BeginMessages( );
void EndMessages( );
void SetWorkMode(int argc, char* argv[]);
int AddArtToTheme( int artnum, char* thname );
int CreateNewTheme( int thnum, char* thname );
int CreateNewKeyword( char* keyword );
int AddArtToKeyword( int artnum, char* keyword );
int RenumberKeywords();

#endif