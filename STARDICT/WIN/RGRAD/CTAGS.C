/********************************************************************
-- STARDICT Project --
Определения символов тэгов формата RSTEXT (2.0)
Определения ф-ий класса TCTag и его наследников.
(c) I.Vladimirsky 1997
********************************************************************/
#include "ctags.h"
#include "errors.h"

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

   ptag = ProceedUntilTag_buf( br, str, 255 );
   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка

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
	 char str[256]
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // ф-я обработки тэга
    };*/

// Обработки нет
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
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };*/

int TCTagBody :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
// теперь нужно найти закрывающий тэг
   TCTag* ptag = NULL;
   do
   {
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка

//проверить найденный тэг не является ли он тэгом первого уровня
     if( ptag->TGCode == TAG_NUMBER  ||  ptag->TGCode == TAG_NAMEBEGIN
	 ||  ptag->TGCode == TAG_NAMEEND ||  ptag->TGCode == TAG_THEMEBEGIN
	 ||  ptag->TGCode == TAG_THEMEEND || ptag->TGCode == TAG_BODYBEGIN )
	 { CurError.ProceedError(FIRST_LEVEL_TAG_INCLUDE); return NOK; }

     if( !(ptag->TGCode == TAG_LINKEND ||  ptag->TGCode == TAG_KEYWORDEND
	 ||  ptag->TGCode == TAG_FONTEND ) )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	 }
//иначе ошибка
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
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
   tProperty = ((align%100)<<2)|(((align%100)/10)<<2)|(align%10<<1);
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
/*class TCTagLink : public TCTag // тэг начала ссылки
    {
	 int LinkArtNum;
       public:
	 TCTagLink() { TGCode = TAG_LINKBEGIN; };
	 int ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
	 int GetArguments( BufRead & br ); // читает номер статьи
    };*/
int TCTagLink :: GetArguments( BufRead & brIn ) // читает номер статьи
{
   if( SeekSpaces( brIn ) != 1) // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &LinkArtNum ) != 1 ) // прочитать номер битмэпа
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
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
//проверить найденный тэг на допустимость внутри ссылки
     if( ptag->TGCode == TAG_KEYWORDBEGIN  ||  ptag->TGCode == TAG_LINKEND )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
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
/*class TCTagKey : public TCTag // тэг начала ключевого слова
    {
       public:
	 TCTagKey() { TGCode = TAG_KEYWORDBEGIN; };
	 int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };  */
// аргументов нет

int TCTagKey :: ProceedTag(  BufRead& br, BufWrite & bw ) // ф-я обработки тэга
{
// запись кода
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

// теперь нужно найти закрывающий тэг
   TCTag* ptag = NULL;
   do
   {
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
//проверить найденный тэг на допустимость внутри ключевого слова
     if( (ptag->TGCode == TAG_KEYWORDEND) )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	 }
//иначе ошибка
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_KEYWORDEND);

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
   if( FontSize < -5 || FontSize > 25 )
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
   fProperty = ( ((FontSize+FNP_BASE) << 3) | ((FontType/100)<<2) |
	       (((FontType%100)/10)<<1) | (FontType%10) );
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



//*********************** Внешние ф-ии ***********************************

// Ф-я записывает в выходной поток байты из входного потока, пока не встретит
// IG_OPEN_BULLET ('<'). Затем читает char код тэга, регистрирует
// соответствующий класс потомок CTag, читает соответствующие аргументы тэга
// и возвращает указатель на него.

TCTag* ProceedUntilTag( BufRead & brIn , BufWrite & bwOut )
{
   TCTag * ptag = NULL;
   int buf=0;

   if( readByte( brIn, &buf ) != 1 )   return NULL;

   while ( buf != TG_BEGIN_BULLET )
    {
      if( writeByte( bwOut, buf ) != 1 )   CurError.ProceedError(ERROR_WRITING);
      if( readByte( brIn, &buf ) != 1 )   return NULL;
    }

   if( SeekSpaces( brIn ) != 1)  // промотать пробелы
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
   if( readByte( brIn, &buf ) != 1 ) // прочитать char код тэга
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }

   if( buf == TG_CLOSE )
    {
      if( readByte( brIn, &buf ) != 1 ) // прочитать char код тэга
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
      switch( buf )
       {
	 case CTG_NAME       :  ptag = new TCTagEndName(); break;
	 case CTG_THEME      :  ptag = new TCTagEndTheme(); break;
	 case CTG_BODY       :  ptag = new TCTagEndBody(); break;
	 case CTG_LINK       :  ptag = new TCTagEndLink(); break;
	 case CTG_KEYWORD    :  ptag = new TCTagEndKey(); break;
	 case CTG_FONT       :  ptag = new TCTagEndFont(); break;
	 default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };
    }

   else
      switch( buf )
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

TCTag* ProceedUntilTag_buf( BufRead & brIn , char* buffer, int buflenth )
{
   TCTag * ptag = NULL;
   int ibuf=0;
   int i=0;
// обнулить буффер
   for(int j=0 ; j<buflenth-1 ; buffer[j] = 0, j++ );

   if( readByte( brIn, &ibuf ) != 1 )   return NULL;

   while ( ibuf != TG_BEGIN_BULLET )
    {
      buffer[i] = (char) ibuf;
      if( i++ == buflenth ) CurError.ProceedError( NOT_ENOUPH_BUFFER );
      if( readByte( brIn, &ibuf ) != 1 )   return NULL;
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
	 default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };

// получить параметры
   ptag->GetArguments( brIn );
   if( CurError.code != NONE )   return NULL;
   return ptag;
}

//**********************************************************************
