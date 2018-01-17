#include "rw.h"

int GoToChar( BufRead& brIn,char ch )
// Ф-я переходит в файле brIn на символ ch возвр. код ошибки
  {
     int res=0;
     int ec;

     while(  res != ch  )
	{
	   ec=brIn.ReadByte( &res );

	   if ( ec == -1 || ec == EOF )
	       return ec;
	}
    return 1;
  }
//***********************************************************************

int ReadInt( BufRead& brIn, int* res )
//  Ф-я читает из файла 2 байта и преобразует их в целое
  {
     int b1,b2;

     if (  brIn.ReadByte(  &b1 ) != 1 || brIn.ReadByte(  &b2 ) != 1 )
	    return 0;

     *res = ( b1 << 8)|b2 ;
     return 1;
  }
//**************************************************************************
int WriteInt( BufWrite& bwOut, int i )
  {
     int b1,b2;

     if( i<0 || i> 4095 )
	 return 0;

     b1 = i>>8;
     b2 = i&0x00FF;


     if (  bwOut.WriteByte(  b1 ) != 1 || bwOut.WriteByte(  b2 ) != 1 )
	      return 0;

     return 1;
  }
//***************************************************************************
int ReadString( BufRead& brIn, char* str )
  {
    int i;

    for(  i=0 ; ; i++ )
       {
	 if( brIn.ReadByte( (int*)(str + i) )!=1 )
	    return 0;

	 if ( str[i] == '\r' )
	       break;
       }

    str[i]=0;

    return 1;

  }

//*********************************************************************
int ReadName( BufRead& brIn, char* name )
// Ф-я читает строку с текущего положения по символ '^'
  {
    int i;
    name[0] = 0 ;

    for(  i=0 ; name[ i-1 ] != '^' ; i++ )
	if( brIn.ReadByte( (int*)(name + i) ) != 1 )
	    return 0;


    name[i-1]=0;

    return 1;
 }
//*********************************************************************
int WriteName( BufWrite& bwOut, char* str )
  {
    for( int i=0 ; str[ i ] != 0 ; i++ )
	if( bwOut.WriteByte( (int)( *(str + i) ) ) != 1 )
	    return 0;

    bwOut.WriteByte( (int)'^' );
    return 1;
 }
//****************************************************************************

int ReadLong( BufRead& brIn, long* res )
//  Ф-я читает из файла 4 байта и преобразует их в long
  {
     int b1, b2, b3, b4;

     if (  brIn.ReadByte(  &b1 ) != 1 || brIn.ReadByte(  &b2 ) != 1 ||
	   brIn.ReadByte(  &b3 ) != 1 || brIn.ReadByte(  &b4 ) != 1  )
	    return 0;

     *res = ((long)b1<<24) | ((long)b2<<16) | ((long)b3 << 8) | (long)b4 ;
     return 1;
  }

//**************************************************************************
int WriteLong( BufWrite& bwOut, long l )
  {
     int b1 = l>>24;
     int b2 = (l&0x00FF0000) >> 16;
     int b3 = (l&0x0000FF00) >> 8;
     int b4 =  l&0x000000FF;

     if (  bwOut.WriteByte( b1 ) != 1 || bwOut.WriteByte( b2 ) != 1 ||
	   bwOut.WriteByte( b3 ) != 1 ||  bwOut.WriteByte( b4 ) != 1   )
	    return 0;
     else
	    return 1;
  }
