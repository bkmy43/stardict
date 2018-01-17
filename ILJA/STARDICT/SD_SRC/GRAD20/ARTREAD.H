/********************************************************************
-- STARDICT Project --
Описания ф-ий и классов TArtR и CTag. Позволяют прочитать статью из
файла данных в формате RSTEXT, прверить ее правильность и записать
ее в DAT файлы.
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _ARTREAD_H
#define _ARTREAD_H

// класс содержит ф-ии позволяющие считать из исходного файла информацию о
// статье, проверить ее и занести в файлы данных
class TArtR
    {
	  BufRead*  pbrInput;
	  BufWrite* pbwIndex;
	  BufWrite* pbwTh;
	  BufWrite* pbwData;
	  BufWrite  bwTemp; // сюда будет записан текст статьи.
			   // после проверки он копируется в data.dat
	  long position; // адрес в файле data.dat

       public :
	  char* artname;          // Название
	  int artnum;
	  int refTable[256];    // Таблица ссылок на другие статьи и темы
	  char* ths[256];         // Темы к которым относится данная статья

	  TArtR( BufRead* pbrInput, BufWrite *bwIndex, BufWrite *bwTh,
		 BufWrite *bwData, long position );
	  ~TArtR( );
	  void ClearAll();

	  int ReadInfo();
          void Data2Win1251(); // перекодировка в Win1251

//	  int TestRefs(); // ф-я проверяет все ли объекты на которые ссылаются существуют
	  int TestThemes(); // все ли темы существуют
	  int test(); // вызывает все тесты
	  int TransferInfo( ); // диспетчер вызывает следующие функции
	  int TransferData( );
	  int TransferThemes( );
	  int TransferIndex( );
    };

class ThemeR
    {
	  BufRead*  pbrInput;
	  BufWrite* pbwTh;
       public :
	  char* thname;
	  int thnum;

	  ThemeR( BufRead* pbrInput, BufWrite* abwTh);
	  ~ThemeR( );
	  void ClearAll();
	  int ReadInfo();      // диспетчер
          void Data2Win1251( ); // перекодировка в Win1251
	  int test(); // проверяет правильность прочитанной информации

	  int TransferInfo( );
    };


#endif