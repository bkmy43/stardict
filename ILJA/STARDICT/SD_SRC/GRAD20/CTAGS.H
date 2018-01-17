/********************************************************************
-- STARDICT Project --
Определения символов тэгов формата RSTEXT (2.0)
Описания класса TCTag и его наследников.
(c) I.Vladimirsky 1997
********************************************************************/
#include "tags.h"
#include "rw.h"
#include "defines.h"

#if !defined _CTAGS_H
#define _CTAGS_H

//----------------   обозначения тэгов  --------------------------

// номер статьи (темы).    Пример  <# 13>
#define    CTG_NUMBER     '#'

// название статьи (темы). Пример  <N>Статья о вреде курения</N>
#define    CTG_NAME       'N'

// название темы, к которой относится статья. Пример  <T>Вредные привычки<\T>
#define    CTG_THEME      'T'

// начало текста статьи. Пример  <B> .... </B>
#define    CTG_BODY       'B'

// параграф (начать с красной строки). Пример  <P>Курить очень вредно
#define    CTG_PARAGRAPH  'P'

// закончить данную строку
#define    CTG_LINEEND    'E'

// Bitmap. Пример  <A 12>
#define    CTG_BITMAP     'A'

// ссылка на другую статью. Пример   <L>пепел<\L>
#define    CTG_LINK       'L'

//  ключевое слово (по ключевым словам осуществляется поиск).
//  Пример <K>никотин<\K>
#define    CTG_KEYWORD    'K'

// шрифт выделение шрифтом
// пример <F 10 100 >Это опасно<\F>
//  10 - размер шрифта
//  100 - болд, 010 - Курсив, 001 - Подчеркнутый, 101 - Болд &подчеркнутый, ...
#define    CTG_FONT       'F'

//  Табуляция. Используется в данной версии для создания таблиц.
//  Пример
//  Животные<O>Смертельная доза никотина
//  Лошадь<O>Капля
//    ...
#define    CTG_TAB        'O'

// Зона, в которой запрещен перенос строк.
#define    CTG_NOWARP       'W'

//----------------  коды уничтожаемых тэгов (тэгов 1-го уровня ) --------------
//коды тэгов, которые не включаются в текст статьи

enum  {
// номер статьи
// Параметры: WORD НомерСтатьи.
   TAG_NUMBER = 33,

// Начало названия статьи.
// Параметры: нет.
   TAG_NAMEBEGIN,
// Окончание названия статьи.
   TAG_NAMEEND,

// Название темы, к которой принадлежит данная статья
// Параметры: WORD НомерСтатьи.
   TAG_THEMEBEGIN,
// Окончание названия темы.
   TAG_THEMEEND,

// Начало тела статьи.
// Параметры: нет.
   TAG_BODYBEGIN,
// Окончание тела статьи.
   TAG_BODYEND,
};

//----------------   обозначения служебных знаков ---------------------

// начало тэга
#define    TG_BEGIN_BULLET   '<'

// конец тэга
#define    TG_END_BULLET     '>'

// знак закрывающего тэга
#define    TG_CLOSE          '/'

//----------------   класс TCTag ---------------------
class TCTag
    {
       public:
         BYTE TGCode;  // код тэга

       public:
         TCTag( ) { TGCode = 0; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
                                               // из входного потока
    };

class TCTagNum : public TCTag // тэг номера статьи
    {
       public:
         int artnum;
       public:
         TCTagNum() { artnum=0; TGCode = TAG_NUMBER; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    };

class TCTagName : public TCTag // открывающий тэг имени статьи
    {
       public:
         char str[256];
       public:
         TCTagName() { TGCode = TAG_NAMEBEGIN; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) ; // ф-я обработки тэга
    };

class TCTagEndName : public TCTag // закрывающий тэг имени статьи
    {
       public:
         TCTagEndName() { TGCode = TAG_NAMEEND; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // ф-я обработки тэга
    };

class TCTagTheme : public TCTag // открывающий тэг темы
    {
       public:
         char str[256];
       public:
         TCTagTheme() { TGCode = TAG_THEMEBEGIN; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };

class TCTagEndTheme : public TCTag // закрывающий тэг темы
    {
       public:
         TCTagEndTheme() { TGCode = TAG_THEMEEND; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // ф-я обработки тэга
    };

class TCTagBody : public TCTag // открывающий тэг тела статьи
    {
       public:
         char* KeywordsTable[MAX_KEYWORDS_IN_ART];
         int KeywordsNum;

       public:
         TCTagBody();
         ~TCTagBody();
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };

class TCTagEndBody : public TCTag // закрывающий тэг тела статьи
    {
       public:
         TCTagEndBody() { TGCode = TAG_BODYEND; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // ф-я обработки тэга
    };

class TCTagParagraph : public TCTag // тэг параграфа
    {
         int align;  // размещение текста в параграфе
       public:
         TCTagParagraph() { TGCode = TAG_PARAGRAPH; align=0;};
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    };

class TCTagTabulation : public TCTag // тэг табуляции
    {
        int size; //размер табуляции
       public:
         TCTagTabulation() { TGCode = TAG_TABULATION; size=0;};
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    };

class TCTagEndOfLine : public TCTag // тэг конца строки
    {
       public:
         TCTagEndOfLine() { TGCode = TAG_ENDOFLINE; };
    };


class TCTagBitmap : public TCTag // тэг вставки битмэпа
    {
         int BitmapNumber;
       public:
         TCTagBitmap() { TGCode = TAG_BITMAP; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         virtual int GetArguments( BufRead & br ); // читает аргументы
    };

class TCTagLink : public TCTag // тэг начала ссылки
    {
         int LinkArtNum;

       public:
         char* KeywordsTable[MAX_KEYWORDS_IN_LINK];
         int KeywordsNum;

       public:
         TCTagLink();
         ~TCTagLink();
         int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         int GetArguments( BufRead & br ); // читает номер статьи
    };

class TCTagEndLink : public TCTag // тэг конца ссылки
    {
       public:
         TCTagEndLink() { TGCode = TAG_LINKEND; };
    };


class TCTagKey : public TCTag // тэг начала ключевого слова
    {
       public:
         char str[256];
       public:
         TCTagKey() { TGCode = TAG_KEYWORDBEGIN; };

         int GetArguments( BufRead & brIn ); // читает основную форму кейворда
         int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };

class TCTagEndKey : public TCTag // тэг конца ключевого слова
    {
       public:
         TCTagEndKey() { TGCode = TAG_KEYWORDEND; };
    };

class TCTagFont : public TCTag // тэг начала выделения шрифтом
    {
         int FontSize;
         int FontType;
       public:
         TCTagFont() { TGCode = TAG_FONTBEGIN; };
         int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
         int GetArguments( BufRead & br ); // читает номер статьи
    };

class TCTagEndFont : public TCTag // тэг конца выделения шрифтом
    {
       public:
         TCTagEndFont() { TGCode = TAG_FONTEND; };
    };


class TCTagNowarp : public TCTag // тэг начала nowarp зоны
    {
       public:
         TCTagNowarp() { TGCode = TAG_NOWARPBEGIN; };
         int ProceedTag(  BufRead& br, BufWrite & bw ); // ф-я обработки тэга
    };

class TCTagEndNowarp : public TCTag // тэг конца nowarp зоны
    {
       public:
         TCTagEndNowarp() { TGCode = TAG_NOWARPEND; };
    };


//----------------  служ. функции ---------------------
// Ф-я записывает в буфер байты из входного потока, пока не встретит
// IG_OPEN_BULLET ('<'). Затем читает char код тэга, регистрирует
// соответствующий класс потомок CTag, читает сооьветствующие аргументы тэга
// и возвращает указатель на него.
// !!! при этом при записи данные переводятся в Win1251
// !!! убираются знаки \n, \r, \t, переносы и лишние пробелы.
TCTag* ProceedUntilTag_buf( BufRead & brIn , char* buffer, int buflenth );

// Ф-я записывает в выходной поток байты из входного потока, пока не встретит
// IG_OPEN_BULLET ('<'). Затем читает char код тэга, регистрирует
// соответствующий класс потомок CTag, читает сооьветствующие аргументы тэга
// и возвращает указатель на него.
// !!! убираются знаки \n, \r, \t, переносы и лишние пробелы.
TCTag* ProceedUntilTag( BufRead & brIn , BufWrite & bwOut );

void strtrim(char* str); //ф-я убирает пробелы в конце строки

#endif