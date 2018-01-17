/********************************************************************
-- STARDICT Project --
Определения символов тэгов формата RSTEXT (2.0)
Определения ф-ий класса TCTag и его наследников.
(c) I.Vladimirsky 1997
********************************************************************/
#include "stdio.h"

#include "ctags.h"
#include "errors.h"
#include "app.h"

extern TError CurError;

//**********************************************************************
// Основной класс.
/*class TCTag
    {
       public:
         BYTE TGPriority;
         BYTE TGCode;  // код тэга

       public:
         TCTag( ) {TGPriority = 0; TGCode = 0; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
                                               // из входного потока
    };
*/

int TCTag :: GetArguments( BufRead & brIn )
// Если у тэга есть аргументы, эта функция будет переопределена в его классе.
// В противном случае чтение аргументов представляет из себя поиск символа
// конца тэга '>'.
{
   int buffer;
   SeekSpaces( brIn ); // промотать пробелы
   if( readByte( brIn, &buffer ) != 1 )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
// здесь должен быть '>'
   if( buffer != TG_END_BULLET )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTag :: ProceedTag( BufRead& brIn, BufWrite & bwOut ) // ф-я обработки тэга
// Часто обработка тэга представляет из себя только запись его кода в
// выходной поток. В противном случае эта ф-я будет переопределена
{
   if( writeByte( bwOut, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   return OK;
}

//**********************************************************************
/*class TCTagNum : public TCTag // тэг номера статьи
    {
         int artnum;
       public:
         TCTagNum() { artnum=0;};
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {} ;// ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    }; */

int TCTagNum :: GetArguments( BufRead & brIn ) // читает номер статьи
{
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &artnum ) != 1 ) // прочитать номер
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( artnum == 0 )
      {  CurError.ProceedError( WRONG_ART_NUMBER ); return NOK; }

// дочитать до '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
// здесь должен быть '>'
   if( buffer != TG_END_BULLET )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

//**********************************************************************
/*class TCTagName : public TCTag // открывающий тэг имени статьи
    {
         char str[256];
       public:
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };*/
// Аргументов нет

// ф-я читает имя статьи в str.
int TCTagName :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
   TCTag* ptag = NULL;

   SeekSpaces( br ); // промотать пробелы
   ptag = ProceedUntilTag_buf( br, str, 255 );
   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
   strtrim(str); //убрать пробелы в конце строки

//если это не тэг конца имени - ошибка
   if( ! ptag->TGCode == TAG_NAMEEND  )
         { CurError.ProceedError(WRONG_ART_NAME); return NOK; }

   if(ptag != NULL) delete ptag;
   return OK;
}

//*********************************************************************
/*class TCTagEndName : public TCTag // закрывающий тэг имени статьи
    {
       public:
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // ф-я обработки тэга
    };*/

// Обработки нет
// Аргументов нет
//**********************************************************************
/*class TCTagTheme : public TCTag // открывающий тэг темы
    {
       public:
         char str[256];
       public:
         TCTagTheme() { TGCode = TAG_THEMEBEGIN; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };     */

// Аргументов нет
int TCTagTheme :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
   TCTag* ptag = NULL;

   ptag = ProceedUntilTag_buf( br, str, 255 );
   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка

//если это не тэг конца имени - ошибка
   if( ! ptag->TGCode == TAG_NAMEEND  )
         { CurError.ProceedError(WRONG_TH_NAME); return NOK; }

   if(ptag != NULL) delete ptag;
   return OK;
}
//**********************************************************************
/*class TCTagEndTheme : public TCTag // закрывающий тэг темы
    {
       public:
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // ф-я обработки тэга
    };*/

// Обработки нет
// Аргументов нет

//**********************************************************************
/*class TCTagBody : public TCTag // открывающий тэг тела статьи
    {
         char* KeywordsTable[MAX_KEYWORDS_IN_ART];
         KeywordsNum;

       public:
         TCTagBody();
         ~TCTagBody();
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };*/
TCTagBody :: TCTagBody() : TCTag()
{
  TGCode = TAG_BODYBEGIN;
  for(int i=0; i<MAX_KEYWORDS_IN_ART; i++ )  KeywordsTable[i] = NULL;
  KeywordsNum = 0;
}

TCTagBody :: ~TCTagBody()
{
  int i=0;
  while( KeywordsTable[i] != NULL )
   {
     delete KeywordsTable[i];
     KeywordsTable[i] = NULL;
     i++;
   }
}

int TCTagBody :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
   int i=0;
// теперь нужно найти закрывающий тэг
   TCTag* ptag = NULL;
   do
   {
     if(ptag != NULL) { delete ptag; ptag = NULL; }
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
// если первый тэг в статье не <P>, записать <P>
     if( i++ == 0 && ptag->TGCode != TAG_PARAGRAPH)
     {  TCTag* ptagtmp = new TCTagParagraph();
        ptagtmp->ProceedTag( br, bw );
        if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
        delete ptagtmp;
     }

//проверить найденный тэг не является ли он тэгом первого уровня
     if( ptag->TGCode == TAG_NUMBER  ||  ptag->TGCode == TAG_NAMEBEGIN
         ||  ptag->TGCode == TAG_NAMEEND ||  ptag->TGCode == TAG_THEMEBEGIN
         ||  ptag->TGCode == TAG_THEMEEND || ptag->TGCode == TAG_BODYBEGIN )
         { CurError.ProceedError(FIRST_LEVEL_TAG_INCLUDE); return NOK; }
 //если это допустимый тэг
     if( !(ptag->TGCode == TAG_LINKEND ||  ptag->TGCode == TAG_KEYWORDEND
	 ||  ptag->TGCode == TAG_FONTEND ) )
         {
           ptag->ProceedTag( br, bw );
           if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
         }
//иначе ошибка
     else
         { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }

// тэг кейворда требует дополнительной обработки на внешнем уровне
// --> найденный кейворд заносится в список KeywordsTable.
// и KeywordsNum++
     if( ptag->TGCode == TAG_KEYWORDBEGIN )
         {
           if( KeywordsNum == MAX_KEYWORDS_IN_ART - 1)
             { CurError.ProceedError(TOO_MANY_KEYWORDS_IN_ART); return NOK; }
           char* string = ((TCTagKey *)ptag)->str;

           KeywordsTable[ KeywordsNum ] = NULL;
           KeywordsTable[ KeywordsNum ] = new char[ strlen(string) +1 ];
           if( KeywordsTable[ KeywordsNum ] == NULL )
             { CurError.ProceedError(NOT_ENOUPH_MEMORY); return NOK; }
           strcpy(KeywordsTable[ KeywordsNum ], string);
           KeywordsNum++;
         }

// Внутри ссылки могут содержаться кейворды
     if( ptag->TGCode == TAG_LINKBEGIN )
       for( int j=0; ((TCTagLink *)ptag)->KeywordsTable[j] != NULL; j++ )
         {
           if( KeywordsNum == MAX_KEYWORDS_IN_ART - 1)
             { CurError.ProceedError(TOO_MANY_KEYWORDS_IN_ART); return NOK; }
           char* string = ((TCTagLink *)ptag)->KeywordsTable[j];

           KeywordsTable[ KeywordsNum ] = NULL;
           KeywordsTable[ KeywordsNum ] = new char[ strlen(string) +1 ];
           if( KeywordsTable[ KeywordsNum ] == NULL )
             { CurError.ProceedError(NOT_ENOUPH_MEMORY); return NOK; }
           strcpy(KeywordsTable[ KeywordsNum ], string);
           KeywordsNum++;
         }
   }
   while(ptag->TGCode != TAG_BODYEND);

   if(ptag != NULL) delete ptag;
   return OK;
}
//**********************************************************************
/*class TCTagEndBody : public TCTag // закрывающий тэг тела статьи
    {
       public:
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // ф-я обработки тэга
    };*/

// Обработки нет
// Аргументов нет

//**********************************************************************
/*class TCTagParagraph : public TCTag // тэг параграфа
    {
         int align;  // размещение текста в параграфе
       public:
         TCTagParagraph() { TGCode = TAG_PARAGRAPH; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    };*/

// обработка - запись кода и параметров
int TCTagParagraph :: ProceedTag(  BufRead& br, BufWrite & bwOut ) // ф-я обработки тэга
{
   if( writeByte( bwOut, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

   int tProperty=0;

   if( align      ==   0)  tProperty = TXP_REDLINE;
   if( align      ==   1)  tProperty = TXP_CENTERED;
   if( align      ==  10)  tProperty = TXP_LEFT;
   if( align      == 100)  tProperty = TXP_RIGHT;


   if( writeByte( bwOut, tProperty ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   return OK;
}

// читает аргумент, задающий способ выравнивания
int TCTagParagraph :: GetArguments( BufRead & brIn )
{
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &align ) != 1 ) // прочитать выравнивание
      {  CurError.ProceedError( ERROR_READING  ); return NOK; }
   if( !( align == 0 || align == 1 || align == 10 || align == 100) )
      {  CurError.ProceedError( WRONG_TAG  ); return NOK; }

// дочитать до '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
// здесь должен быть '>'
   if( buffer != TG_END_BULLET )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

//*****************************************************************
/*class TCTagTabulation : public TCTag // тэг конца строки
    {
        int size; //размер табуляции
       public:
         TCTagTabulation() { TGCode = TAG_TABULATION; size=0;};
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    };
  */
// обработка - запись кода и параметров
int TCTagTabulation :: ProceedTag(  BufRead& br, BufWrite & bwOut ) // ф-я обработки тэга
{
   if( writeByte( bwOut, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

   int tSize=0;

   if( size      ==   0)  tSize = TS_BIG;
   if( size      ==   1)  tSize = TS_SMALL;


   if( writeByte( bwOut, tSize ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   return OK;
}

// читает аргумент, задающий размер табуляции
int TCTagTabulation :: GetArguments( BufRead & brIn )
{
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &size ) != 1 ) // прочитать размер
      {  CurError.ProceedError( ERROR_READING  ); return NOK; }
   if( !( size == 0 || size == 1 ) )
      {  CurError.ProceedError( WRONG_TAG  ); return NOK; }

// дочитать до '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
// здесь должен быть '>'
   if( buffer != TG_END_BULLET )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}


//**********************************************************************
/*class TCTagEndOfLine : public TCTag // тэг конца строки
    {
       public:
         TCTagEndOfLine() { TGCode = TAG_ENDOFLINE; };
    };*/
// обработка стандартная
// аргументов нет

//**********************************************************************
/*class TCTagBitmap : public TCTag // тэг вставки битмэпа
    {
         int BitmapNumber;
       public:
         TCTagBitmap() { TGCode = TAG_BITMAP; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    };*/

int TCTagBitmap :: GetArguments( BufRead & brIn ) // читает номер статьи
{
   if( SeekSpaces( brIn ) != 1)  // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &BitmapNumber ) != 1 ) // прочитать номер битмэпа
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( BitmapNumber == 0 )
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }

// дочитать до '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
// здесь должен быть '>'
   if( buffer != TG_END_BULLET )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTagBitmap :: ProceedTag(  BufRead& br, BufWrite & bwOut ) // ф-я обработки тэга
{
   if( writeByte( bwOut, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   if( WriteInt( bwOut, BitmapNumber ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   return OK;
}
//**********************************************************************
//class TCTagLink : public TCTag // тэг начала ссылки

TCTagLink :: TCTagLink() : TCTag()
{
  TGCode = TAG_LINKBEGIN;
  for(int i=0; i<MAX_KEYWORDS_IN_LINK; i++ )  KeywordsTable[i] = NULL;
  KeywordsNum = 0;
}

TCTagLink :: ~TCTagLink()
{
  int i=0;
  while( KeywordsTable[i] != NULL )
   {
     delete KeywordsTable[i];
     KeywordsTable[i] = NULL;
     i++;
   }
}


int TCTagLink :: GetArguments( BufRead & brIn ) // читает номер статьи
{
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &LinkArtNum ) != 1 ) // прочитать номер ссылки
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( LinkArtNum == 0 )
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }

// дочитать до '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
// здесь должен быть '>'
   if( buffer != TG_END_BULLET )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTagLink :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
// запись кода и номера статьи
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   if( WriteInt( bw, LinkArtNum ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

// теперь нужно найти закрывающий тэг
   TCTag* ptag = NULL;
   do
   {
     if(ptag != NULL) { delete ptag; ptag = NULL; }
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
//проверить найденный тэг на допустимость внутри ссылки
     if( ptag->TGCode == TAG_KEYWORDBEGIN  || ptag->TGCode == TAG_PARAGRAPH
	     || ptag->TGCode == TAG_LINKEND )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка

// тэг кейворда требует дополнительной обработки на внешнем уровне
// --> найденный кейворд заносится в список KeywordsTable.
// и KeywordsNum++
     if( ptag->TGCode == TAG_KEYWORDBEGIN )
	 { if( KeywordsNum == MAX_KEYWORDS_IN_LINK - 1)
	     { CurError.ProceedError(TOO_MANY_KEYWORDS_IN_LINK); return NOK; }
	   char* string = ((TCTagKey *)ptag)->str;

	   KeywordsTable[ KeywordsNum ] = NULL;
	   KeywordsTable[ KeywordsNum ] = new char[ strlen(string) +1 ];
	   if( KeywordsTable[ KeywordsNum ] == NULL )
	     { CurError.ProceedError(NOT_ENOUPH_MEMORY); return NOK; }
	   strcpy(KeywordsTable[ KeywordsNum ], string);
	   KeywordsNum++;
	 }

	 }
//иначе ошибка
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_LINKEND);

   if(ptag != NULL) delete ptag;
   return OK;
}

//**********************************************************************
/*class TCTagEndLink : public TCTag // тэг конца ссылки
    {
       public:
         TCTagEndLink() { TGCode = TAG_LINKEND; };
    }; */
// аргументов нет
// обработка стандартная - запись кода
//**********************************************************************
int TCTagKey :: GetArguments( BufRead & brIn ) // читает основную форму кейворда
{
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }

// дочитать до '>'
   int ibuf, i=-1;
   do
    {
      i++;
      if( readByte( brIn, &ibuf ) != 1 )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
      str[i]=(char)ibuf;
    }
   while( str[i] != TG_END_BULLET );
   str[i] = 0;
   strtrim( str );
   Dos2Win1251( str );
   return OK;
}

int TCTagKey :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
   TCTag* ptag = NULL;

// запись кода тэга начала кейворда
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

   ptag = ProceedUntilTag( br, bw );
   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка

//если это не тэг конца ключевого слова - ошибка
   if(  ptag->TGCode != TAG_KEYWORDEND  )
         { CurError.ProceedError(WRONG_KEYWORD); return NOK; }

// запись кода тэга конца кейворда
   if( writeByte( bw, ptag->TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

   if(ptag != NULL) delete ptag;
   return OK;
}

//**********************************************************************
/*class TCTagEndKey : public TCTag // тэг конца ключевого слова
    {
       public:
         TCTagEndKey() { TGCode = TAG_KEYWORDEND; };
    };*/
// аргументов нет
// обработка стандартная - запись кода

//**********************************************************************
/*class TCTagFont : public TCTag // тэг начала выделения шрифтом
    {
         int FontSize;
         int FontType;
       public:
         int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         int GetArguments( BufRead & br ); // читает номер статьи
    };   */

// читает размер и атрибуты устанавливаемого шрифта.
int TCTagFont :: GetArguments( BufRead & brIn )
{
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &FontSize ) != 1 ) // прочитать размер
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &FontType ) != 1 ) // прочитать атрибут шрифта
      {  CurError.ProceedError( ERROR_READING ); return NOK; }

// проверка правильности введенных данных
   if(!(FontType == 0 || FontType == 1 || FontType == 10 || FontType == 11 ||
        FontType == 100 || FontType == 101 || FontType == 110 || FontType == 111) )
        { CurError.ProceedError(WRONG_TAG); return NOK; }
   if( FontSize <= 0 || FontSize >= 31 )
        { CurError.ProceedError(WRONG_TAG); return NOK; }

// дочитать до '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
// здесь должен быть '>'
   if( buffer != TG_END_BULLET )
        {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTagFont :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
// запись кода
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
// запись параметров
   BYTE fProperty=0;
// FontType == 001, 010, ... 110, 111
   fProperty = FontSize;
   if( FontType/100 == 1 )         fProperty |= FNP_BOLD;
   if( (FontType%100)/10 == 1 )    fProperty |= FNP_ITALIC;
   if( FontType%10 == 1 )          fProperty |= FNP_UNDERLINED;

// запись параметров шрифта
   if( writeByte( bw, fProperty ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

// теперь нужно найти закрывающий тэг
   TCTag* ptag = NULL;
   do
   {
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
//проверить найденный тэг на допустимость внутри ключевого слова
     if( ptag->TGCode == TAG_PARAGRAPH || ptag->TGCode == TAG_ENDOFLINE  ||
            ptag->TGCode == TAG_FONTEND )
         {
           ptag->ProceedTag( br, bw );
           if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
         }
//иначе ошибка
     else
         { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_FONTEND);

   if(ptag != NULL) delete ptag;
   return OK;
}

//**********************************************************************
/*class TCTagEndFont : public TCTag // тэг конца выделения шрифтом
    {
       public:
         TCTagFont() { TGCode = TAG_FONTEND; };
    }; */
// аргументов нет
// обработка стандартная - запись кода

//**********************************************************************
/*class TCTagNowarp : public TCTag // тэг начала nowarp зоны
    {
       public:
         TCTagNowarp() { TGCode = TAG_NOWARPBEGIN; };
         int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    }; */

// аргументов нет

int TCTagNowarp :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
// запись кода
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

// теперь нужно найти закрывающий тэг
   TCTag* ptag = NULL;
   do
   {
     if(ptag != NULL) { delete ptag; ptag = NULL; }
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
//проверить найденный тэг на допустимость внутри nowarp зоны
     if( ptag->TGCode == TAG_PARAGRAPH || ptag->TGCode == TAG_ENDOFLINE  ||
	 ptag->TGCode == TAG_LINKBEGIN || ptag->TGCode == TAG_KEYWORDBEGIN ||
	 ptag->TGCode == TAG_FONTBEGIN || ptag->TGCode == TAG_TABULATION ||
	 ptag->TGCode == TAG_NOWARPEND )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	 }
//иначе ошибка
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_NOWARPEND);

   if(ptag != NULL) delete ptag;
   return OK;
}


//*********************** Внешние ф-ии ***********************************

// Ф-я записывает в выходной поток байты из входного потока, пока не встретит
// IG_OPEN_BULLET ('<'). Затем читает char код тэга, регистрирует
// соответствующий класс потомок CTag, читает соответствующие аргументы тэга
// и возвращает указатель на него.

// !!! при этом при записи данные переводятся в Win1251
// !!! убираются знаки \n, \r, \t и лишние пробелы.
int IsSpaceSymbol( int c )
 { return ( c=='\n' || c=='\r' || c=='\t' || c==' ' ); }

TCTag* ProceedUntilTag( BufRead & brIn , BufWrite & bwOut )
{
   TCTag * ptag = NULL;
   int ibuf=0;
   char ch;
   int FirstSpace=YES;  // если прочтен пробел или '\n' то эта переменная
   // устанавливается в YES и второй подряд пробел или '\n' записан не будет
   // таким образом например "\n    \n\r \t  "  запишется как " "

// запись данных из входного потока в выходной. Лишние пробелы и '\n' пропускаются
   while ( 1 )
    {
      if( readByte( brIn, &ibuf ) != 1 )   return NULL;
      if(ibuf == TG_BEGIN_BULLET)  break;

      ch=(char) ibuf;

      // если символ - второй подряд пробел, или символ новой строки
      // он не должен попадасть в выходной поток.
      if( FirstSpace==YES || (! IsSpaceSymbol(ch)) )
         {
           if( IsSpaceSymbol(ch) )  //Если первым символом в наборе
              ibuf=(int)' ';      // окажется \n он заменится на пробел
           Dos2Win1251(ibuf); // перекодировка в Win1251
           if( writeByte( bwOut, ibuf ) != 1 )
              CurError.ProceedError(ERROR_WRITING);
         }

      if( IsSpaceSymbol(ch) ) FirstSpace=NO;  //если прочтен пробел -> следующий за ним пробел будет уже не первым
      else FirstSpace=YES;
    }


   if( SeekSpaces( brIn ) != 1)  // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
   if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }

   if( ibuf == TG_CLOSE )
    {
      if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
      switch( ibuf )
       {
         case CTG_NAME       :  ptag = new TCTagEndName(); break;
         case CTG_THEME      :  ptag = new TCTagEndTheme(); break;
         case CTG_BODY       :  ptag = new TCTagEndBody(); break;
         case CTG_LINK       :  ptag = new TCTagEndLink(); break;
         case CTG_KEYWORD    :  ptag = new TCTagEndKey(); break;
         case CTG_FONT       :  ptag = new TCTagEndFont(); break;
         case CTG_NOWARP     :  ptag = new TCTagEndNowarp(); break;
         default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };
    }

   else
      switch( ibuf )
       {
         case CTG_NUMBER     :  ptag = new TCTagNum(); break;
         case CTG_NAME       :  ptag = new TCTagName(); break;
         case CTG_THEME      :  ptag = new TCTagTheme(); break;
         case CTG_BODY       :  ptag = new TCTagBody(); break;
         case CTG_PARAGRAPH  :  ptag = new TCTagParagraph(); break;
         case CTG_LINEEND    :  ptag = new TCTagEndOfLine(); break;
         case CTG_BITMAP     :  ptag = new TCTagBitmap(); break;
         case CTG_LINK       :  ptag = new TCTagLink(); break;
         case CTG_KEYWORD    :  ptag = new TCTagKey(); break;
         case CTG_FONT       :  ptag = new TCTagFont(); break;
         case CTG_TAB        :  ptag = new TCTagTabulation(); break;
         case CTG_NOWARP     :  ptag = new TCTagNowarp(); break;
         default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };

// получить параметры
   ptag->GetArguments( brIn );
   if( CurError.code != NONE )   return NULL;
   return ptag;
}

// Ф-я записывает в буфер байты из входного потока, пока не встретит
// IG_OPEN_BULLET ('<'). Затем читает char код тэга, регистрирует
// соответствующий класс потомок CTag, читает соответствующие аргументы тэга
// и возвращает указатель на него.

// !!! убираются знаки \n, \r, \t, и лишние пробелы.

TCTag* ProceedUntilTag_buf( BufRead & brIn , char* buffer, int buflenth )
{
   TCTag * ptag = NULL;
   int ibuf=0;
   char ch;
   int i=0;
   int FirstSpace=YES;  // если прочтен пробел то эта переменная
   // устанавливается в YES и второй подряд пробел записан не будет

// обнулить буффер
   for(int j=0 ; j<buflenth-1 ; buffer[j] = 0, j++ );

// запись данных из входного потока в выходной. Лишние пробелы и '\n' пропускаются
   ibuf=0;
   while ( 1 )
    {
      if( readByte( brIn, &ibuf ) != 1 )   return NULL;
      if(ibuf == TG_BEGIN_BULLET)  break;

      ch=(char) ibuf;

      // если символ - второй подряд пробел, или символ новой строки
      // он не должен попадасть в выходной поток.
      if( FirstSpace==YES || (! IsSpaceSymbol(ch)) ) // ch -первый пробел или не пробел
         {
           if( IsSpaceSymbol(ch) )  //Если первым символом в наборе
              ibuf=(int)' ';      // окажется \n он заменится на пробел
           buffer[i] = (char) ibuf;
           if( i++ == buflenth ) CurError.ProceedError( NOT_ENOUPH_BUFFER );
         }

      if( IsSpaceSymbol(ch) ) FirstSpace=NO;  //если прочтен пробел -> следующий за ним пробел будет уже не первым
      else FirstSpace=YES;
    }

   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
   if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }

   if( ibuf == TG_CLOSE )
    {
      if( readByte( brIn, &ibuf ) != 1 ) // прочитать char код тэга
        {  CurError.ProceedError( WRONG_TAG ); return NULL; }
      switch( ibuf )
       {
         case CTG_NAME       :  ptag = new TCTagEndName(); break;
         case CTG_THEME      :  ptag = new TCTagEndTheme(); break;
         case CTG_BODY       :  ptag = new TCTagEndBody(); break;
         case CTG_LINK       :  ptag = new TCTagEndLink(); break;
         case CTG_KEYWORD    :  ptag = new TCTagEndKey(); break;
         case CTG_FONT       :  ptag = new TCTagEndFont(); break;
         case CTG_NOWARP     :  ptag = new TCTagEndNowarp(); break;
         default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };
    }

   else
      switch( ibuf )
       {
         case CTG_NUMBER     :  ptag = new TCTagNum(); break;
         case CTG_NAME       :  ptag = new TCTagName(); break;
         case CTG_THEME      :  ptag = new TCTagTheme(); break;
         case CTG_BODY       :  ptag = new TCTagBody(); break;
         case CTG_PARAGRAPH  :  ptag = new TCTagParagraph(); break;
         case CTG_LINEEND    :  ptag = new TCTagEndOfLine(); break;
         case CTG_BITMAP     :  ptag = new TCTagBitmap(); break;
         case CTG_LINK       :  ptag = new TCTagLink(); break;
         case CTG_KEYWORD    :  ptag = new TCTagKey(); break;
         case CTG_FONT       :  ptag = new TCTagFont(); break;
         case CTG_TAB        :  ptag = new TCTagTabulation(); break;
         case CTG_NOWARP     :  ptag = new TCTagNowarp(); break;
         default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };

// получить параметры
   ptag->GetArguments( brIn );
   if( CurError.code != NONE )   return NULL;
   return ptag;
}

//**********************************************************************

