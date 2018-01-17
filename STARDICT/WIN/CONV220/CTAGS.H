/********************************************************************
-- STARDICT Project --
Определения символов тэгов формата RSTEXT (2.0)
(c) I.Vladimirsky 1997
********************************************************************/

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

// изменить шрифт.
// Пример
//#define    CTG_FONT       F

//----------------   обозначения служебных знаков ---------------------

// начало тэга
#define    TG_BEGIN_BULLET   '<'

// конец тэга
#define    TG_END_BULLET     '>'

// знак закрывающего тэга
#define    TG_CLOSE          '/'

//----------------   служебные константы ---------------------
#define    MAX_TAG_ATTRIB_SYMBOLS 10

//----------------   класс TCTag ---------------------
// атрибуты тэга -
/*struct TCTagAttrib
    {
         union unum {                   // В зависимости от типа тэга
                     char byte;        // возможны разные
                     int  word;       // числовые атрибуты
                     long dword;
                    } num;

         char symbols[MAX_TAG_ATTRIB_SYMBOLS];
    };

class TCTag
    {
       public:
         char TGType;
         TCTagAttrib TGAttrib;

       public:
         TCTag( aTGType, char anum, char* asymbols = NULL);
         TCTag( aTGType, int  anum, char* asymbols = NULL);
         TCTag( aTGType, long anum, char* asymbols = NULL);
         TCTag( aTGType );
    };

           {TGType = aTGType; TGAttrib.num.byte = anum;
            for(int i=0; asymbols[i]) }


  */

#endif