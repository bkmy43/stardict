const MAX_LINES = 2000;
const MAX_CHARACTERS_IN_LINE = 100;

class TRef
      {
	 public:
	  char code;
	  int  num;

	  TRef( ) { code=0; num=0; };
	  TRef( char c, int n) { code = c; num = n; };
	  void clear( void ) { code=0; num=0; };
      };

class TArtR
    {
	  FILE* fr;
	  BufWrite* pbwIndex;
	  BufWrite* pbwTh;
	  BufWrite* pbwData;
	  long position; // адрес в файле data.dat
       public :
	  char* artname;          // Название
	  int artnum;
	  char* artline[MAX_LINES];
	  TRef refTable[256];    // Таблица ссылок на другие статьи и темы
	  char* ths[256];         // Темы к которым относится данная статья

	  TArtR( FILE* fr, BufWrite *bwIndex, BufWrite *bwTh,
		 BufWrite *bwData, long position );
	  ~TArtR( );
	  void ClearAll();
	  int ReadNumber( char * str );
	  int ReadName( char * str );
	  int ReadText( char * str );
	  int ReadRef( char * str );
	  int ReadTheme( char * str );
	  int ReadInfo();      // диспетчер
	  int test(); // проверяет правильность прочитанной информации
	  // test вызывает четыре вспомогательные ф-ии :
	  int TestRefs1(); // ф-я проверяет все ли ссылки из таблицы встречаются в тексте
	  int TestRefs2(); // ф-я проверяет все ли ссылки из текста встречаются в таблице
	  int TestRefs3(); // ф-я проверяет все ли объекты на которые ссылаются существуют
	  int TestThemes(); // все ли темы существуют

	  int TransferInfo( );
	  int TransferData( );
	  int TransferThemes( );
	  int TransferIndex( );
    };

class ThemeR
    {
	  FILE* fr;
	  BufWrite* pbwTh;
       public :
	  char* thname;
	  int thnum;

	  ThemeR( FILE* afr, BufWrite* abwTh);
	  ~ThemeR( );
	  void ClearAll();
	  int ReadInfo();      // диспетчер
	  int test(); // проверяет правильность прочитанной информации

	  int TransferInfo( );
    };
