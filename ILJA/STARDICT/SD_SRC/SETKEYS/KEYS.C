#include <string.h>
#include <stdio.h>

#include "errors.h"
#include "defines.h"
#include "keys.h"

char* UP_LETTERS  = "QWERTYUIOPASDFGHJKLZXCVBNMЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
char* LOW_LETTERS= "qwertyuiopasdfghjklzxcvbnmйцукенгшщзхъфывапролджэячсмитьбю";

extern filehand fhKeys;
extern TError CurError;

// Базовый класс - элемент списка
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
// Базовый класс - однонаправленный список
TBaseList :: ~TBaseList()
{
  if( pfirst != NULL ) delete pfirst;
  pfirst = NULL;
  ItemsNum = 0;
}

char* TBaseList :: GetNthStr( int i ) // получить number i-го элемента списка
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
// добавляет элемент в конец списка
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
  // найти последний элемент в списке
  for( ptmp1=pfirst; ptmp1->pnext != NULL; ptmp1=ptmp1->pnext );

  ptmp2 = new TBaseListElem( str, NULL );
  if( ptmp2 == NULL ) return NOK;

  ptmp1->pnext = ptmp2;

  ItemsNum++;
  return OK;
}

//**********************************************************************
// Класс Список Ключевых слов является потомком TBaseList
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

// Заполняет список keyword'ами очередной строки файла keys.txt
// Первым в списке стоит основная форма keyword'а? а дальше различные варианты
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
// Функции необходимые для расстановки тэгов ключевых слов
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

  char* pos = text;  // начальное положение - в начале текста
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

// Функция находит в тексте первое вхождение подстроки
// и возвращает указатель на ее начало или NULL, если подстрока в тексте не содержится.
// Регистр не учитывается. !!!
char* FindSubString( char * text, char * key )
{
    int res;
    while( (res=CompStr( text, key )) != 0 )  //пока text не закончился
       if( res == 1 )
	   return text;
       else
           SeekToNextDelimiter( &text );

    return NULL;

}
// Функция определяет совпадает ли *str2 с началом *str1
// возвращает 1, -1
// или 0 если str1 закончилась
int CompStr( char* s1, char* s2 )
{
    for(  ; IsSameLetter(*s1, *s2) == 1; s1++, s2++ );
    if( *s2 == 0 && (IsDelimiter(*s1) == 1 /*&& *s1 != '<'*/))
             // != '<' требуется, чтобы отсечь keyword'ы внутри ссылок
             // иначе все глючит...

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

    if( c > 159 && c < 176 )   return 0;  //а-п
    if( c > 223 && c < 240 )   return 0; //р-я
    if( c > 127 && c < 160 )   return 0; // А-Я
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

    if(  ((c1 > 127 && c1 < 144) &&  ( c2 == c1 + 32 )) ||   // А-П
         ((c2 > 127 && c2 < 144) &&  ( c1 == c2 + 32 )) )
            return 1;

    if(  ((c1 > 143 && c1 < 159) &&  ( c2 == c1 + 80 )) ||   // П-Я
         ((c2 > 143 && c2 < 159) &&  ( c1 == c2 + 80 )) )
            return 1;

    return 0;
}

//**********************************************************************
// тэг тела статьи
#define    CTG_BODY       'B'

// начало тэга
#define    TG_BEGIN_BULLET   '<'

// конец тэга
#define    TG_END_BULLET     '>'

// знак закрывающего тэга
#define    TG_CLOSE          '/'

// Ф-я записывает в буфер байты из входного потока, пока не встретит
// тэг начала статьи <B>
int ReadUntilBodyBegin( BufRead & brIn , char* buffer, int buflenth )
{
   int ibuf=0;
   char ch;
   int i=0;

// обнулить буффер
   for(int j=0 ; j<buflenth-1 ; buffer[j] = 0, j++ );

   ibuf=0;
   while ( 1 )
    {
      if( readByte( brIn, &ibuf ) != 1 )   return NOK;

      if(ibuf == TG_BEGIN_BULLET)
      {
       if( SeekSpaces( brIn ) != 1) // промотать пробелы
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( ibuf == TG_CLOSE )
        {
          if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
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
            {  // этот тэг нужно обработать до конца - записать '>'
             if( SeekSpaces( brIn ) != 1) // промотать пробелы
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             if( readByte( brIn, &ibuf ) != 1 ) // прочитать  '>'
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

// Ф-я записывает в буфер байты из входного потока, пока не встретит
// тэг конца статьи </B>
int ReadUntilBodyEnd( BufRead & brIn , char* buffer, int buflenth )
{
   int ibuf=0;
   char ch;
   int i=0;

// обнулить буффер
   for(int j=0 ; j<buflenth-1 ; buffer[j] = 0, j++ );

   ibuf=0;
   while ( 1 )
    {
      if( readByte( brIn, &ibuf ) != 1 )   return NOK;

      if(ibuf == TG_BEGIN_BULLET)
      {
       if( SeekSpaces( brIn ) != 1) // промотать пробелы
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
          {  CurError.ProceedError( WRONG_TAG ); return NOK; }
       if( ibuf == TG_CLOSE )
        {
          if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
        	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
          buffer[i++] = TG_BEGIN_BULLET;
          buffer[i++] = TG_CLOSE;
          buffer[i++] = ibuf;
          if( ibuf == CTG_BODY )
            {  // этот тэг нужно обработать до конца - записать '>'
             if( SeekSpaces( brIn ) != 1) // промотать пробелы
                {  CurError.ProceedError( WRONG_TAG ); return NOK; }
             if( readByte( brIn, &ibuf ) != 1 ) // прочитать  '>'
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