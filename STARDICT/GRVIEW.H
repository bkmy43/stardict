#include "thart.h"
#include <bios.h>
#include "textscr.h"

#ifdef _HISTORY
#include "history.h"
#endif

int ChooseTheme( BufRead *pbrThemes );
int ShowTheme( BufRead* pbrIndex, BufRead* pbrThemes, int thnum );

class  TFullRef : public TRef
 {
   public:
    int begline, endline, begchar, endchar;
    TFullRef* pnext;
    TFullRef* pprev;

    TFullRef( char code, int num, int bl, int el, int bc,
	      int ec, TFullRef* pprev );
// при удалении ссылки будут удаляться все следующие за ней
    ~TFullRef() { if( pnext!=NULL) delete pnext; }
 };

class TGraphView : public TArticle, public TextScreen
  {
     private:
	int fline;
  // список ссылок из данной статьи
	TFullRef* references;
	TFullRef* ActiveRef;
	int MAX_LINES_ON_SCREEN;
	int LINES_IN_TEXT; // вспомотательная информация о количестве строк в статье
	BufRead *pbrThemes;

	int MouseMode;
	int FkeysStatus;  // были ли инициированы F-кнопки
	TVScroller* pScr;

	int setrefs();  // заполняет таблицу ссылок

	void DelOldInfo();
	int keyanalisys( );    // анализирует мышь и клавиатуру

	void VErrorReact( int ec );
	void gotoline( int linenum );
	void movedown();
	void moveup();
	void pagedown();
	void pageup();
	void InsActRef();    //выделяет в тексте активную ссылку
	void UnInsActRef();
	void InsRefs( int mode=IN_ALL_SCREEN, int nline=0 );
// ф-я выделяет цветом ссылки. При mode == IN_ALL_SCREEN все, а при
// mode == IN_SINGLE_LINE в строке номер nline
	void DrawBord();
	void NameNumOut( void );  // информация о статье
	void InitViewerKeys( void );
	void RemoveViewerKeys( void );
	void SwitchViewerKeys( void );
	void SwitchViewerKeysOff();
	void SetUserColors();

     public:
	TGraphView( BufRead * Index, BufRead *Data, BufRead *Themes,
	int GDr,int fsy,int LMLet,int RMLet,int UMStr,int DMStr,int mm);

	TGraphView( BufRead * Index, BufRead *Data, int GDr, int fsy,
		    int LMLet, int RMLet, int UMStr, int DMStr, int mm = ON);

	virtual ~TGraphView( );
	int ChangeArt( int num );
#ifdef _HISTORY
	void Return( ); // возврат на одну статью назад
#endif
	int view();       // просмотр
	void show(); // печать строк с fline
#ifdef _PRINT_ALLOWED
	int PrintArt();
	int PrintArtToFile( char* fname );
#endif
	int ChooseArt( void );
	int ChooseArtFromHistory( void );
	void SetMouseMode( int mm ) { MouseMode = mm; };
	void ClearClient( );
  };
