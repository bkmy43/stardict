#include <conio.h>

int txtx=0;
int txty=0;
int txtcolor = LIGHTGRAY;
int txtback  = BLACK;

unsigned far* pv=( unsigned far* )(0xB8000000);   /* начало видеопамяти */

void charout( char ch, int color = LIGHTGRAY, int back = BLACK );
void cls();
// установить цвет части строки
void SetPartColor( int line, int beg, int end, int color);
// установить фон части строки
void SetPartBack( int line, int beg, int end, int color);


void cls()
 {
   for( int a=0;a<21*80; a++ )
     *( pv + a ) = (BLACK<<12 | LIGHTGRAY<<8 | 0);     /* фон */

   txtx=0;
   txty=0;
 }


void charout( char ch, int color , int back  )
 {
   txtcolor = color;
   txtback = back;

   if( ch == '\n' )
     {
	txtx=0;
	txty++;
	return;
     }

   *( pv+txtx+txty*80 ) = (txtback<<12 | txtcolor<<8 | (unsigned char)ch );

   txtcolor = LIGHTGRAY;
   txtback  = BLACK;

   if( txtx == 79 )
     {
       txtx=0;
       if( txty < 24 )
	  txty++;
       else
	  txty=0;
     }
   else
      txtx++;
 }

void SetPartBack( int line, int beg, int end, int color)
// установить фон части строки
 {
   for( int i=beg; i<=end; i++ )
     {
       *( pv+i+line*80 ) &= 0x0FFF;
       *( pv+i+line*80 ) |= color<<12;
     }
 }

void SetPartColor( int line, int beg, int end, int color)
// установить цвет части строки
 {
   for( int i=beg; i<=end; i++ )
     {
       *( pv+i+line*80 ) &= 0xF0FF;
       *( pv+i+line*80 ) |= color<<8;
     }
 }
