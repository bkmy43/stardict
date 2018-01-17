/********************************************************************
-- STARDICT Project --
								 exart.h

Описание базового класса TExArticle
Описание классов TBaseList, TArticleList, TThemeList
Позволяют получать списки статей, тем и keyword'ов
и работать с ними
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _TEXART_H
#define _TEXART_H

#include "datutil.h"

class TExArticle       // Статья
    {
       protected :
          char artname[MAX_NAME_LEN];          // Название статьи
          int artnum;                // Номер статьи
			 char* artbody;            // тело (текст статьи)
			 unsigned bodylen;            //  длина текста статьи
			 BufRead *pbrIndex;      // Указатели на буферы файлов
			 BufRead *pbrData;      // с данными

			 virtual void DelOldInfo();
			 // Ф-я читает название статьи N num и адрес
			 // по которому ее текст можно найти в файле data.dat
			 int ReadNamePosition ( int num, long* position );
			 // Ф-я читает номер статьи name и адрес
			 // по которому ее текст можно найти в файле data.dat
			 int ReadNumPosition ( char* name, long* position );

		 public:
			 TExArticle( );  /* === Function Can Throw Exeptions === */
			 TExArticle( int inumber );  /* === Function Can Throw Exeptions === */
			 virtual ~TExArticle( );

			 char* GetArtName() { return artname; };
			 int GetArtNum() { return artnum; };
			 unsigned GetArtBodyLen() { return bodylen; };
			 char* GetArtBody(){ return artbody; };

			 // ф-я читает информацию о
			 // статье N num и заполняет данные класса
			 // в случае ошибки возвращает ее код
          /* === Function Can Throw Exeptions === */
			 virtual int ChangeArt( int num );

			 // Ф-я читает текст статьи
			 int ReadBody( long position );
	 };


// Базовый класс - элемент списка
class TBaseListElem
		 {
			 public:
				int num;
				char* name;

				TBaseListElem* pnext;

            /* === Function Can Throw Exeptions === */
				TBaseListElem( int a, char* str, TBaseListElem* apnext=NULL);
				~TBaseListElem();
				TBaseListElem* GetNext() { return pnext; };
		  };

// Базовый класс - однонаправленный список
class TBaseList
		  {
			 protected:
				int ItemsNum;
				TBaseListElem* pfirst;
				virtual void BuildList()=0;  // Заполняет элементы списка конкретными
												// данными. Переопределяется в зависимости
												// от назначения конкретного списка
				int AppendElem( int i, char* str ); // добавляет в конец списка
												  // элемент с параметрами (i,str)

			 public:
				TBaseList() {pfirst=NULL; ItemsNum=0; };
				~TBaseList();
				int GetItemsNum() { return ItemsNum; }
				char* GetNthName( int n ); // получить name n-го элемента списка
				int GetNthNumber( int n ); // получить number n-го элемента списка
				char* GetName( int num ); // получить имя соответствующее номеру num
				int GetNumber( char* str ); // получить номер соответствующий имени str
		  };

// Класс Список статей является потомком TBaseList
// Переопределяется функция  BuildList - данные читаются из index.dat
class TArticleList : public TBaseList
		  {
			 protected:
				BufRead* pbrIndex;

            /* === Function Can Throw Exeptions === */
				virtual void BuildList();

			 public:
            /* === Function Can Throw Exeptions === */
				TArticleList( int mode=BUILD_LIST); // вызывает BuildList
				~TArticleList( );
				  // получить имя статьи соответствующее номеру num
				char* GetArticleName( int num ) { return GetName(num); };
				 // получить номер статьи с именем "str"
				int GetArticleNumber( char* str ) { return GetNumber(str); };

				friend class TThemeList;
				friend class TKeywordList;
		  };

// Класс Список тем является потомком TBaseList
// Переопределяется функция  BuildList - данные читаются из themes.dat
class TThemeList : public TBaseList
		  {
			 protected:
				BufRead* pbrThemes;
				TArticleList* AllArtList; // список всех статей строится
												  // в конструкторе

 // массив целых чисел, содержащий информацию о составе всех тем.
 // формат: номер_темы_1, номер_статьи_11, номер_статьи_12, ..., 0,
 //         номер_темы_2, номер_статьи_21, номер_статьи_22, ..., 0,
 //                               .  .  .
 //         номер_темы_n, номер_статьи_n1, номер_статьи_n2, ..., 0,
 //         0

				int* ThemesTable;
            /* === Function Can Throw Exeptions === */
				int  BuildThemesTable();
            /* === Function Can Throw Exeptions === */
				virtual void BuildList();

			 public:
				TArticleList* ThArtList;  // Список статей из данной темы
					  // строится функцией GetNthArtList на основе AllArtList

            /* === Function Can Throw Exeptions === */
				TThemeList();
				~TThemeList( );
 			   // получить имя темы соответствующее номеру num
				char* GetThemeName( int num ) { return GetName(num); };
				 // получить номер темы с именем "str"
				int GetThemeNumber( char* str ) { return GetNumber(str); };

				// функция строит список статей из N-ой темы и возвращает
				// указатель на него
				TArticleList* GetNthArtList( int n );
		  };

// Класс Список keyword'ов является потомком TBaseList
// Переопределяется функция  BuildList - данные читаются из keyword.dat
class TKeywordList : public TBaseList
		  {
			 protected:
				BufRead* pbrKeys;
				TArticleList* AllArtList; // список всех статей строится
												  // в конструкторе

 // массив целых чисел, содержащий информацию о составе всех keyword'ов.
 // формат: номер_keyword'а_1, номер_статьи_11, номер_статьи_12, ..., 0,
 //         номер_keyword'а_2, номер_статьи_21, номер_статьи_22, ..., 0,
 //                               .  .  .
 //         номер_keyword'а_n, номер_статьи_n1, номер_статьи_n2, ..., 0,
 //         0

				int* KeywordsTable;
            /* === Function Can Throw Exeptions === */
				int  BuildKeywordsTable();
            /* === Function Can Throw Exeptions === */
				virtual void BuildList();

			 public:
				TArticleList* KeyArtList;  // Список статей в которых встречается
													// данный keyword
					  // строится функцией GetNthArtList на основе AllArtList

            /* === Function Can Throw Exeptions === */
				TKeywordList();

				~TKeywordList( );
				// получить имя keyword соответствующее номеру num
				char* GetThemeName( int num ) { return GetName(num); };
				// получить номер keyword с именем "str"
				int GetThemeNumber( char* str ) { return GetNumber(str); };

				// функция строит список статей из N-ого keyword'а и возвращает
				// указатель на него
				TArticleList* GetNthArtList( int n );
		  };

#endif

