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
// �� 㤠����� ��뫪� ���� 㤠������ �� ᫥���騥 �� ���
    ~TFullRef() { if( pnext!=NULL) delete pnext; }
 };

class TGraphView : public TArticle, public TextScreen
  {
     private:
	int fline;
  // ᯨ᮪ ��뫮� �� ������ ����
	TFullRef* references;
	TFullRef* ActiveRef;
	int MAX_LINES_ON_SCREEN;
	int LINES_IN_TEXT; // �ᯮ���⥫쭠� ���ଠ�� � ������⢥ ��ப � ����
	BufRead *pbrThemes;

	int MouseMode;
	int FkeysStatus;  // �뫨 �� ���樨஢��� F-������
	TVScroller* pScr;

	int setrefs();  // �������� ⠡���� ��뫮�

	void DelOldInfo();
	int keyanalisys( );    // ���������� ���� � ����������

	void VErrorReact( int ec );
	void gotoline( int linenum );
	void movedown();
	void moveup();
	void pagedown();
	void pageup();
	void InsActRef();    //�뤥��� � ⥪�� ��⨢��� ��뫪�
	void UnInsActRef();
	void InsRefs( int mode=IN_ALL_SCREEN, int nline=0 );
// �-� �뤥��� 梥⮬ ��뫪�. �� mode == IN_ALL_SCREEN ��, � ��
// mode == IN_SINGLE_LINE � ��ப� ����� nline
	void DrawBord();
	void NameNumOut( void );  // ���ଠ�� � ����
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
	void Return( ); // ������ �� ���� ����� �����
#endif
	int view();       // ��ᬮ��
	void show(); // ����� ��ப � fline
#ifdef _PRINT_ALLOWED
	int PrintArt();
	int PrintArtToFile( char* fname );
#endif
	int ChooseArt( void );
	int ChooseArtFromHistory( void );
	void SetMouseMode( int mm ) { MouseMode = mm; };
	void ClearClient( );
  };
