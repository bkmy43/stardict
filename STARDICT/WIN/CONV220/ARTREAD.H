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
	  FILE* fw;

       public :
	  char* artname;          // ��������
	  int artnum;
	  char* artline[MAX_LINES];
	  TRef refTable[256];    // ������ ��뫮� �� ��㣨� ���� � ⥬�
	  char* ths[256];         // ���� � ����� �⭮���� ������ �����

	  TArtR( FILE* fr, FILE* fw );
	  ~TArtR( );
	  void ClearAll();
	  int ReadNumber( char * str );
	  int ReadName( char * str );
	  int ReadText( char * str );
	  int ReadRef( char * str );
	  int ReadTheme( char * str );
	  int ReadInfo();      // ��ᯥ���
	  int test(); // �஢���� �ࠢ��쭮��� ���⠭��� ���ଠ樨
	  // test ��뢠�� ���� �ᯮ����⥫�� �-�� :
	  int TestRefs1(); // �-� �஢���� �� �� ��뫪� �� ⠡���� ��������� � ⥪��
	  int TestRefs2(); // �-� �஢���� �� �� ��뫪� �� ⥪�� ��������� � ⠡���
//	  int TestRefs3(); // �-� �஢���� �� �� ��ꥪ�� �� ����� ��뫠���� ��������
//	  int TestThemes(); // �� �� ⥬� ��������

	  int GetRefNumber( char refcode);
	  int ConvertLine(int number, int & mode);
// �-� ��ॢ���� ��ப� N � �ଠ� RSTEXT.
// ����⠢��� ���� ��뫮�. mode 㪠�뢠�� ��稭����� ��ப� ����� ��뫪� ��� ���.
	  int WriteInfo( );
//	  int TransferData( );
//	  int TransferThemes( );
//	  int TransferIndex( );
    };

class ThemeR
    {
	  FILE* fr;
	  FILE* fw;

       public :
	  char* thname;
	  int thnum;

	  ThemeR( FILE* afr, FILE* afw);
	  ~ThemeR( );
	  void ClearAll();
	  int ReadInfo();      // ��ᯥ���
	  int test(); // �஢���� �ࠢ��쭮��� ���⠭��� ���ଠ樨

	  int TransferInfo( );
    };
