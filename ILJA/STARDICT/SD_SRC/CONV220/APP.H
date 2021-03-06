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
#include "ctags.h"
#include "defines.h"

#define _ERLIST_TO_FILE

typedef    int  filehand ;

//०��� ࠡ���
const TH_READ  = 1;
const ART_READ = 2;
const ART_READ_NOREFTEST = 3;
const HELP     = 4;

//⨯� �訡��
const NOT_CRITICAL_ERROR = 0;
const CRITICAL_ERROR     = 1;

const STOP_READ   = -1;

const ART_NUMBER_NOT_FOUND = 2;
const ART_NAME_NOT_FOUND   = 3;
const WRONG_ART_NAME       = 4;
const WRONG_TH_NAME        = 6;
const WRONG_ART_NUMBER     = 7;
const TH_NOT_EXIST         = 9;
const ERROR_READING_TH     = 10;
const NOT_ENOUGH_MEMORY    = 12;
const ERROR_IN_TH          = 13;
const TEXT_NOT_FOUND       = 14;
const TEXT_END_NOT_FOUND   = 15;
const INVALID_REF          = 16;
const TH_EXISTS            = 17;
const ART_EXISTS           = 18;
const CAN_NOT_OPEN_IN      = 19;
const CAN_NOT_OPEN_INDEX   = 20;
const CAN_NOT_OPEN_TH      = 21;
const CAN_NOT_OPEN_DATA    = 22;
const READING_ERROR	   = 23;
const TH_NUMBER_NOT_F      = 24;
const WRONG_TH_NUMBER      = 25;
const TH_NAME_NOT_FOUND    = 26;
const TH_NUMBER_NOT_FOUND  = 27;
const BAD_REF_IN_TABLE     = 28;
const BAD_REF_IN_TEXT      = 29;
const REF_TO_BAD_OBJECT    = 30;
const SELFREF		   = 31;
const CAN_NOT_OPEN_OUT     = 32;
const CAN_NOT_CREATE_OUT   = 33;

// apput.c
int CharToInt( char ch, int* res );  // ��ॢ�� 16-�� ���� � ���. �᫮
int ReadCompName ( BufRead& brIn, char* name );
int ReadTextToInt(  BufRead& brIn, int* res );
int codeChar( char ch, int* buffer );
int WriteCodedString( BufWrite& bw, char* str );
int SetTextEnd( BufWrite& bw );
void CreateDat();            // �� ������⢨� DAT-䠩��� ᮧ���� ����
int CreateBakFiles( void );      // ������� dat-䠩�� � bak-䠩��
int CreateTmpFiles( void );      // ������� dat-䠩�� � tmp-䠩��
int SaveChanges( void );         // ������� tmp-䠩�� � dat-䠩��
int copyfile(char *fromfile, char *tofile);
int ThExists( int number );  // �஢�ઠ
int ArtExists( int number );
int StrAlphComp(char* str1, char* str2);
void LowCase( char* str); // �-� ���������� �� ᨬ���� �஬� ��ࢮ�� � lower case
void LowCase( char& ch); // �-� ���������� ᨬ��� � lower case �-�, a-z

int WriteCTag( FILE* out, char TGtype, int number=0, char attrib=0);
int WriteCTagEnd( FILE* out, char TGtype);
int NewLine( FILE* out );
// main.c
int  OpenDataFiles(int argc, char* argv[]); //���뢠�� 䠩�� � ����묨
void BuildErrorMessage( int ec, char* dest );
void ErrorMessage( int ec, char* message=NULL );
void FErrorMessage( int ec, int artnum, char* artname );
void SetWorkMode(int argc, char* argv[]);
