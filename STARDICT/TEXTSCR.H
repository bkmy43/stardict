#include "defines.h"
#include "keys.h"

void MemMoveLineUp( int addr_from, int addr_to, int LineSizeY,
		    int BYTES_IN_VIDEO_LINE, int BYTES_IN_LINE);
void MemMoveLineDown( int addr_from, int addr_to, int LineSizeY,
		      int BYTES_IN_VIDEO_LINE, int BYTES_IN_LINE);

/*int ConvertLine( char* string , char* s1, char* s2 );
// ��ॢ���� ��ப� ���� � �ଠ� �ਭ������ �㭪樥�
//  Rtext_with_shablon ��뢠���� �� �-�� put line*/


struct ViewerColors
   {
   int ClrText;
   int ClrBack;
   int ClrRef;
   int ClrActRef;
   int ClrBackActRef;
   int ClrBord;
   };


class TextScreen
 {
   public :
     int  GDriver ;
     int LineSizeY ;
     int LeftMarginLetters;
     int RightMarginLetters;
     int UpMarginStrings;
     int DownMarginStrings;
// colors
     ViewerColors colors;
   protected :
     int MAX_X ;
     int MAX_Y ;
     int FONT_SIZE_Y;
     int BYTES_IN_VIDEO_LINE ;
     int LEFT_MARGIN_BYTES   ;
     int RIGHT_MARGIN_BYTES  ;
     int UP_MARGIN_BYTES     ;
     int DOWN_MARGIN_BYTES   ;
     int BYTES_IN_LINE       ;
     int LINES_ON_SCREEN     ;

     void SetMoveMode( void );   // ����ன�� - ��⠭���� ���⮢
     void UnSetMoveMode();

   public :
     TextScreen ( int GDr, int fsy, int LMLet, int RMLet,
		  int UMStr, int DMStr );
     void SetColors( int ClrText, int ClrBack, int ClrRef,int ClrActRef,
		int ClrBackActRef, int aClrBord );
     void MoveLineUp( int lnum );  // ������ ���न��� � ���� � �����,
				  // �맮� MemMoveLineUp
     void MoveLineDown( int lnum );
     void ScrollUp( char* newline = 0 );
     void ScrollDown( char* newline = 0 );
     int PutLine( long number, char* lin, int color = NOT_DEFINED,
		  int clear = OVERWRITE );
      // ����� ��ப� � �������� ������ ������� 梥⮬ ( �� 㬮�砭��
     //  ClrText. ��祬 ��뫪� ��⮬���᪨ �뤥������ 梥⮬ ClrRef
    //   clear - ����� �� � �� �����⠭� � �⮬ ����
     void PutTextBackground( int line, int beg, int end, int color );
     void TextScreen :: GetTextRect( int line, int beg, int end,
				int* px1, int* py1, int* px2, int* py2 );
     void ClearLine( int lnum );
     void TextToPixel( int* character, int* line  ); // ������ ���न��� ��
		       // ( ᨬ���, ��ப� ) � ( x, y )
 };

class TVScroller
 {
      int x1, y1, x2, y2;
      int col1,col2,col3;
      float pos; // � ����� �������
      int y; //⥪�饥 ��������� �����
      void ShowHead();
      void HideHead();
     public:
      TVScroller( int ax1, int ay1, int ax2, int ay2,
		  int acol1, int acol2, int acol3);
      void Draw();
      void SetPos( float f );
// ���������� ����⨥ ��� �����頥� UP_ARROW, DOWN_ARROW, PG_UP, PG_DOWN
      int Analisys( int x, int y );
 };

