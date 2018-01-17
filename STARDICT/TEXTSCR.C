#include <graphics.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <bios.h>
#include "textscr.h"
#include "graph.h"

//************************************************************************
extern struct ViewerColors VColors;

TextScreen :: TextScreen( int GDr, int fsy, int LMLet, int RMLet,
			  int UMStr, int DMStr )
 {
      GDriver = GDr;
      LineSizeY = fsy;
      LeftMarginLetters = LMLet ;
      RightMarginLetters = RMLet;
      UpMarginStrings = UMStr;
      DownMarginStrings = DMStr;

      MAX_X = 640;
      MAX_Y = ( GDriver == VGA ) ? 480 : 350 ;
      BYTES_IN_VIDEO_LINE = MAX_X/8;
      FONT_SIZE_Y = FONT_HEIGHT ;

      LEFT_MARGIN_BYTES = LeftMarginLetters;
      RIGHT_MARGIN_BYTES = RightMarginLetters;
      UP_MARGIN_BYTES = LineSizeY*UpMarginStrings*BYTES_IN_VIDEO_LINE;
      DOWN_MARGIN_BYTES=LineSizeY*DownMarginStrings*BYTES_IN_VIDEO_LINE;
      BYTES_IN_LINE = BYTES_IN_VIDEO_LINE - LEFT_MARGIN_BYTES - RIGHT_MARGIN_BYTES;
      LINES_ON_SCREEN = MAX_Y/LineSizeY - UpMarginStrings - DownMarginStrings;

      colors.ClrText       =  VColors.ClrText;
      colors.ClrBack       =  VColors.ClrBack;
      colors.ClrRef        =  VColors.ClrRef;
      colors.ClrActRef     =  VColors.ClrActRef;
      colors.ClrBackActRef =  VColors.ClrBackActRef;
      colors.ClrBord       =  VColors.ClrBord;
 }

void TextScreen :: SetColors( int aClrText, int aClrBack, int aClrRef,
	   int aClrActRef,int aClrBackActRef, int aClrBord )
 {
     colors.ClrText       = aClrText;
     colors.ClrBack       = aClrBack;
     colors.ClrRef        = aClrRef;
     colors.ClrActRef     = aClrActRef;
     colors.ClrBackActRef = aClrBackActRef;
     colors.ClrBord       = aClrBord;
 }

void TextScreen :: TextToPixel( int* x, int* y )
{
   *x = (LeftMarginLetters+ *x)*8 + 1;
   *y = ( UpMarginStrings + *y )*LineSizeY + 1 ;
}

int TextScreen :: PutLine( long number, char* line, int color, int clear  )
// печать строки в заданную позицию
{
   int x1=0;
   int y1=number;
   int x2= MAX_X/8 - LeftMarginLetters - RightMarginLetters;
   int y2=y1+1;

   // если при вызове ф-ии не задан цвет
   if ( color == NOT_DEFINED )    color = colors.ClrText;
   if( clear == OVERWRITE ) // если нужно стереть старую строку
	 ClearLine( number );
   TextToPixel( &x1, &y1);  // из текстовых координат в реальные
   TextToPixel( &x2, &y2);  // из текстовых координат в реальные
   y1+=(( LineSizeY - FONT_SIZE_Y )/2); // выравнивание по вертикали по
   y2-=(( LineSizeY - FONT_SIZE_Y )/2); // центру строки
   Rbackconvert( &x1, &y1 ); // из реальных в стандартные
   Rbackconvert( &x2, &y2 ); // из реальных в стандартные
   Rcol( color );

   char* buffer = new char[256];
   sprintf(buffer, " %s", line );
   Rtext( x1, y1, buffer);
   delete buffer;

   return GM_OK;
}

void TextScreen :: ClearLine( int lnum )
{
   int x1=0;
   int y1=lnum;
   int x2=MAX_X/8 - LeftMarginLetters - RightMarginLetters;
   int y2=y1+1;

   TextToPixel( &x1, &y1);  // из текстовых координат в реальные
   TextToPixel( &x2, &y2);  // из текстовых координат в реальные
   Rbackconvert( &x1, &y1 ); // из реальных в стандартные
   Rbackconvert( &x2, &y2 ); // из реальных в стандартные
   Rbarc( x1, y1, x2, y2, SOLID_FILL, colors.ClrBack ); // стереть старую строку
}

void TextScreen :: PutTextBackground( int line, int beg, int end, int color )
{
   int x1=beg;
   int y1=line;
   int x2=end;
   int y2=y1+1;

   TextToPixel( &x1, &y1);  // из текстовых координат в реальные
   TextToPixel( &x2, &y2);  // из текстовых координат в реальные
   Rbackconvert( &x1, &y1 ); // из реальных в стандартные
   Rbackconvert( &x2, &y2 ); // из реальных в стандартные
   y1+=2;   y2-=9;
   Rbarc( x1, y1, x2, y2, SOLID_FILL, color );
}

void TextScreen :: GetTextRect( int line, int beg, int end,
				int* px1, int* py1, int* px2, int* py2 )
{
   int x1=beg;
   int y1=line;
   int x2=end;
   int y2=y1+1;

   TextToPixel( &x1, &y1);  // из текстовых координат в реальные
   TextToPixel( &x2, &y2);  // из текстовых координат в реальные
   x2+=20; y1-=3; y2-=5;
   *px1=x1, *px2=x2; *py1=y1; *py2=y2;
}

void TextScreen :: ScrollUp( char* newline )
{
   SetMoveMode();
    for( int i = 1; i < LINES_ON_SCREEN ; i++  )
	MoveLineUp( i );
   UnSetMoveMode();

   PutLine( LINES_ON_SCREEN - 1, newline ); // напечатать новую строку
}

void TextScreen :: ScrollDown( char* newline )
{
   SetMoveMode();
   for( int i = LINES_ON_SCREEN - 2; i >=0 ; i--  )
	MoveLineDown( i );
   UnSetMoveMode();

   PutLine( 0, newline );
}


//************************************************************************
void TextScreen :: SetMoveMode( void ) // установка маски битов, и. т. д.
{
   asm  {
	  mov dx, 0x3CE
	  mov ax, 0x0FF08
	  out dx, ax
//	  mov dx, 0x3CE
//	  mov ax, 0x0003
	  out dx, ax
	  mov dx, 0x3CE
	  mov ax, 0x0105
	  out dx, ax
	  mov dx, 0x3C4
	  mov ax, 0x0F02
	  out dx, ax
	};
}

void TextScreen ::  UnSetMoveMode()
{
   asm  {
	  mov dx, 0x3C4
	  mov ax, 0x0F00
	  out dx, ax
	};
}

//*********************************************************************
void TextScreen :: MoveLineUp( int lnum  )
{
   int addr_from =  UP_MARGIN_BYTES +  LEFT_MARGIN_BYTES
		    + BYTES_IN_VIDEO_LINE*lnum*LineSizeY ;
   int addr_to =    UP_MARGIN_BYTES +  LEFT_MARGIN_BYTES
		    + BYTES_IN_VIDEO_LINE*(lnum-1)*LineSizeY ;

   MemMoveLineUp( addr_from, addr_to, LineSizeY,
		  BYTES_IN_VIDEO_LINE, BYTES_IN_LINE);
}

//***********************************************************************
void TextScreen :: MoveLineDown( int lnum )  // пересчет координат в адрес в памяти
{
   int addr_from =  UP_MARGIN_BYTES +  LEFT_MARGIN_BYTES
		    + BYTES_IN_VIDEO_LINE*lnum*LineSizeY   ;
   int addr_to =    UP_MARGIN_BYTES +  LEFT_MARGIN_BYTES
		    + BYTES_IN_VIDEO_LINE*(lnum+ 1)*LineSizeY ;

   MemMoveLineDown( addr_from, addr_to, LineSizeY,
		  BYTES_IN_VIDEO_LINE, BYTES_IN_LINE);

}

//*********************************************************************
void MemMoveLineUp( int addr_from, int addr_to, int LineSizeY,
		    int BYTES_IN_VIDEO_LINE, int BYTES_IN_LINE)
{
  asm  {
	 push es
	 push ds
	 mov  si, addr_from
	 mov  di, addr_to
	 mov  ax, 0xA000
	 mov  es, ax
	 mov  ds, ax
	 mov  cx, LineSizeY
	 mov  bx, BYTES_IN_VIDEO_LINE
	 sub  bx, BYTES_IN_LINE
    };

 M1:
 asm  {  push cx
	 mov  cx, BYTES_IN_LINE
	 rep  movsb
	 add  di, bx
	 add  si, bx
	 pop cx
	 loop M1
	 pop  ds
	 pop  es
      }

}

void MemMoveLineDown( int addr_from, int addr_to, int LineSizeY,
		      int BYTES_IN_VIDEO_LINE, int BYTES_IN_LINE)
{
  asm  {
	 push es
	 push ds
	 mov  si, addr_from
	 mov  di, addr_to
	 mov  ax, 0xA000
	 mov  es, ax
	 mov  ds, ax
	 mov  cx, LineSizeY
	 mov  bx, BYTES_IN_VIDEO_LINE
	 sub  bx, BYTES_IN_LINE
    };

 M1:
 asm  {  push cx
	 mov  cx, BYTES_IN_LINE
	 rep  movsb
	 add  di, bx
	 add  si, bx
	 pop cx
	 loop M1
	 pop  ds
	 pop  es
      }

}
/***********************************************************************
			  class TVScroller
**********************************************************************/
TVScroller::TVScroller( int ax1, int ay1, int ax2, int ay2,
			int acol1, int acol2, int acol3 )
{
  pos = 0;
  x1=ax1; x2=ax2; y1=ay1; y2=ay2;
  col1=acol1; col2=acol2;col3=acol3;
  y=y1+SCD1+SCD2;
}

void TVScroller::SetPos( float f )
{
  HideHead();
  pos = f;
  ShowHead();
}

void TVScroller:: Draw()
{
  Rbarc( x1, y1, x2, y2, SOLID_FILL, col1 );

  Rramc( x1, y1, x2, y1+SCD2, col2);
  Rlinec( x1+SCD1, y1+SCD2-SCD1, x2-SCD1, y1+SCD2-SCD1, col2);
  Rlinec( x1+SCD1, y1+SCD2-SCD1, (x2+x1)/2, y1+SCD1, col2);
  Rlinec( x2-SCD1, y1+SCD2-SCD1, (x2+x1)/2, y1+SCD1, col2);

  Rramc( x1, y2-SCD2, x2, y2, col2);
  Rlinec( x1+SCD1, y2-SCD2+SCD1, x2-SCD1, y2-SCD2+SCD1, col2);
  Rlinec( x1+SCD1, y2-SCD2+SCD1, (x2+x1)/2, y2-SCD1, col2);
  Rlinec( x2-SCD1, y2-SCD2+SCD1, (x2+x1)/2, y2-SCD1, col2);
  ShowHead();
}

void TVScroller::ShowHead()
{
  y=y1+SCD1+SCD2+pos*(y2-y1-2*SCD1-4*SCD2);
  Rbarc( x1, y, x2, y+SCD2*2, INTERLEAVE_FILL, col3 );
}

void TVScroller::HideHead()
{
  Rbarc( x1, y, x2, y+SCD2*2, SOLID_FILL, col1 );
}

int  TVScroller::Analisys( int xm, int ym )
{
  if( xm>=x1 && xm<=x2 && ym>=y1 && ym<=y2 )
   {
     if( ym<=y )   // верхняя половина
	if( ym<=y1+SCD2 )
	    return UP; // верхняя кнопка
	else
	    return PGUP;
     if( ym>=y+2*SCD2 )  // нижняя половина
	if( ym>=y2-SCD2 )
	    return DOWN; // нижняя кнопка
	else
	    return PGDN;
     else      // в области каретки
	return  ENTER;
   }
  return 1;
}