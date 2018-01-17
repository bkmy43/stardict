#include <stdio.h>
#include "rw.h"

//базовый класс - элемент списка
class TBaseListElem
       {
          public:
            char* str;

            TBaseListElem* pnext;

            TBaseListElem( char* astr, TBaseListElem* apnext=NULL);
            ~TBaseListElem();
            TBaseListElem* GetNext() { return pnext; };
        };

//базовый класс  - однонаправленный список
class TBaseList
        {
          protected:
            int ItemsNum;
            TBaseListElem* pfirst;

            int AppendElem( char* astr );

          public:
            TBaseList() {pfirst=NULL; ItemsNum=0; };
            virtual int BuildList()=0;  // Заполняет список конкр. данными
            ~TBaseList();
            int GetItemsNum() { return ItemsNum; }
            char* GetNthStr( int n ); // получить str n-го по порядку элемента
        };

// Класс Список Ключевых Слов является потомком TBaseList.
// Переопределяется функция  BuildList
class TKeysList : public TBaseList
        {
            BufRead* pbrKeys;

          public:
            TKeysList(); // не вызывает BuildList
            ~TKeysList( );
            virtual int BuildList();
            void ClearList();

            void SeekKeyfileToBegin() {pbrKeys->bigRew();}
            IsKeyword(char* str); // функция определяет, является ли переданное
                                   // ей значение ключевым словом из списка
        };

int ReadUntilBodyEnd( BufRead & brIn , char* buffer, int buflenth );
int ReadUntilBodyBegin( BufRead & brIn , char* buffer, int buflenth );

int GetWordFromString( char* string, char* buffer, char dev );
char* SetKeywordInText( char* text, char* key, char* form );
void Str2LowCase( char* buf );

char* FindSubString( char * text, char * key );
int CompStr( char* s1, char* s2 );
void SeekToNextDelimiter( char** str);
int IsDelimiter( char c );
int IsSameLetter( char ch1, char ch2);
