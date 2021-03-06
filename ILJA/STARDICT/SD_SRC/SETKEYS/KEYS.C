#include <string.h>
#include <stdio.h>

#include "errors.h"
#include "defines.h"
#include "keys.h"

char* UP_LETTERS  = "QWERTYUIOPASDFGHJKLZXCVBNM��������������������������������";
char* LOW_LETTERS= "qwertyuiopasdfghjklzxcvbnm��㪥�������뢠�஫�����ᬨ���";

extern filehand fhKeys;
extern TError CurError;

// ������ ����� - ������� ᯨ᪠
TBaseListElem :: TBaseListElem( char* astr, TBaseListElem* apnext)
{
  str = NULL;
  str = new char[ strlen(astr) +1 ];

  if( str == NULL )
     CurError.ProceedError( NOT_ENOUPH_MEMORY );

  strcpy(str,astr);

  pnext=apnext;
}

TBaseListElem :: ~TBaseListElem()
{
  if( str != NULL ) delete str;
  if( pnext != NULL ) delete pnext;
}

//**********************************************************************
// ������ ����� - �������ࠢ����� ᯨ᮪
TBaseList :: ~TBaseList()
{
  if( pfirst != NULL ) delete pfirst;
  pfirst = NULL;
  ItemsNum = 0;
}

char* TBaseList :: GetNthStr( int i ) // ������� number i-�� ������� ᯨ᪠
{
  if( i > ItemsNum ) return 0;

  TBaseListElem* ptmp;
  int j=0;

  for( ptmp=pfirst; ptmp != NULL; ptmp=ptmp->pnext )
     if( j++ == i )
        return ptmp->str;

  return 0;
}

int TBaseList :: AppendElem( char* str )
// �������� ������� � ����� ᯨ᪠
{
  TBaseListElem* ptmp1;
  TBaseListElem* ptmp2=NULL;

  if( pfirst==NULL )
     {
       pfirst = new TBaseListElem( str, NULL );
       if( pfirst == NULL ) return NOK;
       ItemsNum++;
       return OK;
     }

  // the list has more then one element
  // ���� ��᫥���� ������� � ᯨ᪥
  for( ptmp1=pfirst; ptmp1->pnext != NULL; ptmp1=ptmp1->pnext );

  ptmp2 = new TBaseListElem( str, NULL );
  if( ptmp2 == NULL ) return NOK;

  ptmp1->pnext = ptmp2;

  ItemsNum++;
  return OK;
}

//**********************************************************************
// ����� ���᮪ ���祢�� ᫮� ���� ��⮬��� TBaseList
TKeysList :: TKeysList() : TBaseList()
{
   pbrKeys = NULL;
   pbrKeys = new BufRead();
//   if( pbrIndex == NULL )
       // return !!!!!

   pbrKeys->SetFileHandler( fhKeys );

   pfirst = NULL;

//   BuildList();
}

TKeysList :: ~TKeysList()
{
   if( pbrKeys != NULL ) delete pbrKeys;
}

void TKeysList :: ClearList()
{
  if( pfirst != NULL ) delete pfirst;
  pfirst = NULL;
  ItemsNum = 0;
}

// �������� ᯨ᮪ keyword'��� ��।��� ��ப� 䠩�� keys.txt
// ���� � ᯨ᪥ �⮨� �᭮���� �ଠ keyword'�? � ����� ࠧ���� ��ਠ���
int TKeysList :: BuildList()
{
   TBaseListElem* ptmp1=NULL;
   TBaseListElem* ptmp2=NULL;

   char buffer[MAX_NAME_LEN] = {0};
   char string[1024] = {0};
   int  ibuf;

   if( ReadString( *pbrKeys, string ) != 1 )
      return NOK;

   for( int i=0; GetWordFromString( string, buffer, ':' ) == 1; i++ )
      {
        if( i==0 )
          {
//            Str2LowCase( buffer );
            pfirst = ptmp1 = new TBaseListElem( buffer, NULL);
          }
        else
	  {
            ptmp2 = new TBaseListElem( buffer, NULL);
            ptmp1->pnext = ptmp2;
            ptmp1 = ptmp2;
	  }
        ItemsNum++;
      }

   return OK;
}

//**********************************************************************
// �㭪樨 ����室��� ��� ���⠭���� ����� ���祢�� ᫮�
//**********************************************************************
int GetWordFromString( char* string, char* buffer, char dev )
{
 if( strlen(string) == 0 )
    return 0;

 for( int i=0; string[i] != 0 && string[i] != dev; i++ )
    buffer[i] = string[i];

 if(string[i] == 0)
   strcpy(string, "");
 else
   strcpy( string, string+i+1 );

 buffer[i] = 0;
 return 1;
}

char* SetKeywordInText( char* text, char* key, char* form )
{
  char keywordbeg[256] = "<K ";
  char* keywordend = "</K>";
  char* result = NULL;
  result = new char[MAX_BODY_SIZE];
  if( result == NULL )  CurError.ProceedError( NOT_ENOUPH_MEMORY );
  int i=0;

  strncat( keywordbeg, key, 255 );
  strncat( keywordbeg, ">", 255 );

  char* pos = text;  // ��砫쭮� ��������� - � ��砫� ⥪��
  char* findpos;

//  strcpy( result, text );

  while( (findpos = FindSubString( pos, form )) != NULL )
  {
      for( char* pch=pos; pch < findpos; pch++ )
           result[i++] = *pch;
      result[i++] = 0;

      strncat( result, keywordbeg, MAX_BODY_SIZE );
      i+= strlen(keywordbeg) - 1;

      for(int j=0; j<strlen(form); findpos++, j++ )
           result[i++] = *findpos;
      result[i++] = 0;

      strncat( result, keywordend, MAX_BODY_SIZE );

      i += 3;
      pos = findpos;
  }

  for( char* pch=pos; *pch != 0; pch++ )
      result[i++] = *pch;
  result[i] = 0;

  return result;
}

// �㭪�� ��室�� � ⥪�� ��ࢮ� �宦����� �����ப�
// � �����頥� 㪠��⥫� �� �� ��砫� ��� NULL, �᫨ �����ப� � ⥪�� �� ᮤ�ন���.
// ������� �� ���뢠����. !!!
char* FindSubString( char * text, char * key )
{
    int res;
    while( (res=CompStr( text, key )) != 0 )  //���� text �� �����稫��
       if( res == 1 )
	   return text;
       else
           SeekToNextDelimiter( &text );

    return NULL;

}
// �㭪�� ��।���� ᮢ������ �� *str2 � ��砫�� *str1
// �����頥� 1, -1
// ��� 0 �᫨ str1 �����稫���
int CompStr( char* s1, char* s2 )
{
    for(  ; IsSameLetter(*s1, *s2) == 1; s1++, s2++ );
    if( *s2 == 0 && (IsDelimiter(*s1) == 1 /*&& *s1 != '<'*/))
             // != '<' �ॡ����, �⮡� ����� keyword'� ����� ��뫮�
             // ���� �� �����...

          return 1;
    if( *s1 == 0 ) return 0;
    else          return -1;
}

void SeekToNextDelimiter( char** str)
{
    for( ; **str != 0 && IsDelimiter(**str) != 1 ; (*str)++ );

    (*str)++;
}

int IsDelimiter( char ch )
{
    unsigned char c = ch;

    if( c > 159 && c < 176 )   return 0;  //�-�
    if( c > 223 && c < 240 )   return 0; //�-�
    if( c > 127 && c < 160 )   return 0; // �-�
    if( c > 47 && c < 58 )   return 0; // 0-9
    if( c > 96 && c < 123 )   return 0; // a-z
    if( c > 64 && c < 91 )   return 0; // A-Z

    return 1; // YES !!! This is delimiter...
}

int IsSameLetter( char ch1, char ch2)
{
    if (ch1 == ch2 )   return 1;

    unsigned char c1 = ch1;
    unsigned char c2 = ch2;

    if(  ((c1 > 64 && c1 < 91) &&  ( c2 == c1 + 95 )) ||   // A-Z
         ((c2 > 64 && c2 < 91) &&  ( c1 == c2 + 95 )) )
            return 1;

    if(  ((c1 > 127 && c1 < 144) &&  ( c2 == c1 + 32 )) ||   // �-�
         ((c2 > 127 && c2 < 144) &&  ( c1 == c2 + 32 )) )
            return 1;

    if(  ((c1 > 143 && c1 < 159) &&  ( c2 == c1 + 80 )) ||   // �-�
         ((c2 > 143 && c2 < 159) &&  ( c1 == c2 + 80 )) )
            return 1;

    return 0;
}

//**********************************************************************
// ��� ⥫� ����
#define    CTG_BODY       'B'

// ��砫� ����
#define    TG_BEGIN_BULLET   '<'

// ����� ����
#define    TG_END_BULLET     '>'

// ���� ����뢠�饣� ����
#define    TG_CLOSE          '/'

// �-� �����뢠�� � ���� ����� �� �室���� ��⮪�, ���� �� ������
// ��� ��砫� ���� <B>
int ReadUntilBodyBegin( BufRead & brIn , char* buffer, int buflenth )
{
   int ibuf=0;
   char ch;
   int i=0;

// ���㫨�� �����
   for(int j=0 ; j<buflenth-1 ; buffer[j] = 0, j++ );

   ibuf=0;
   while ( 1 )
    {
      if( readByte( brIn, &ibuf ) != 1 )   return NOK;

      if(ibuf == TG_BEGIN_BULLET)
      {
       if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( readByte( brIn, &ibuf ) != 1 ) // ������ char ��� ����
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( ibuf == TG_CLOSE )
        {
          if( readByte( brIn, &ibuf ) != 1 ) // ������ char ��� ����
        	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
          buffer[i++] = TG_BEGIN_BULLET;
          buffer[i++] = TG_CLOSE;
          buffer[i++] = ibuf;
          continue;
        }
       else
        {
          buffer[i++] = TG_BEGIN_BULLET;
          buffer[i++] = ibuf;
          if( ibuf == CTG_BODY )
            {  // ��� ��� �㦭� ��ࠡ���� �� ���� - ������� '>'
             if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             if( readByte( brIn, &ibuf ) != 1 ) // ������  '>'
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             if( ibuf != TG_END_BULLET )
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             buffer[i++] = TG_END_BULLET;
             return OK;
            }
          continue;
        }
      }

      buffer[i] = (char) ibuf;
      if( i++ == buflenth ) CurError.ProceedError( NOT_ENOUPH_BUFFER );
    }
}

// �-� �����뢠�� � ���� ����� �� �室���� ��⮪�, ���� �� ������
// ��� ���� ���� </B>
int ReadUntilBodyEnd( BufRead & brIn , char* buffer, int buflenth )
{
   int ibuf=0;
   char ch;
   int i=0;

// ���㫨�� �����
   for(int j=0 ; j<buflenth-1 ; buffer[j] = 0, j++ );

   ibuf=0;
   while ( 1 )
    {
      if( readByte( brIn, &ibuf ) != 1 )   return NOK;

      if(ibuf == TG_BEGIN_BULLET)
      {
       if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( readByte( brIn, &ibuf ) != 1 ) // ������ char ��� ����
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( ibuf == TG_CLOSE )
        {
          if( readByte( brIn, &ibuf ) != 1 ) // ������ char ��� ����
        	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
          buffer[i++] = TG_BEGIN_BULLET;
          buffer[i++] = TG_CLOSE;
          buffer[i++] = ibuf;
          if( ibuf == CTG_BODY )
            {  // ��� ��� �㦭� ��ࠡ���� �� ���� - ������� '>'
             if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             if( readByte( brIn, &ibuf ) != 1 ) // ������  '>'
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             if( ibuf != TG_END_BULLET )
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             buffer[i++] = TG_END_BULLET;
             return OK;
            }
          continue;
        }
       else
        {
          buffer[i++] = TG_BEGIN_BULLET;
          buffer[i++] = ibuf;
          continue;
        }
      }

      buffer[i] = (char) ibuf;
      if( i++ == buflenth ) CurError.ProceedError( NOT_ENOUPH_BUFFER );
    }
}

//*************************************************************************
void Str2LowCase( char* buf )
{
     int i,j;

     for( i=0; buf[i] != 0; i++ )
       {
        for( j=0; UP_LETTERS[j] != buf[i] && UP_LETTERS[j] != 0 ; j++ );
        if( UP_LETTERS[j] != 0 )
                buf[i] = LOW_LETTERS[j];
       }
}