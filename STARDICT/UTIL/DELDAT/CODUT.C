#include  "rw.h"

// Ф-ии для работы с файлами

//*************************************************************************
char UncodeChar(  int* buffer, int length )
//   Функция получает буффер содерж. 1 2 или 3 полубайта, и их количество
// ( length ).  Переводит его в символ и возвращает этот символ.
 {
     char ch=0;
//-------------------------------------------------------------------------
     if( length == 1 )   // если это символ 1 типа
       switch( buffer[0] )
	 {
	   case 0:    return ' ';
	   case 1 :   return 'в';
	   case 2 :   return 'е';
	   case 3 :   return 'и';
	   case 4 :   return 'к';
	   case 5 :   return 'л';
	   case 6 :   return 'а';
	   case 7 :   return 'н';
	   case 8 :   return 'о';
	   case 9 :   return 'п';
	   case 10:   return 'р';
	   case 11:   return 'с';
	   case 12:   return 'т';
	   case 13:   return 'м';
	 };
//-------------------------------------------------------------------------
      if( length == 2 )
	switch( buffer[1] )
	  {
	   case  0:    return 'б';
	   case  1:    return 'г';
	   case  2:    return 'д';
	   case  3:    return 'з';
	   case  4:    return 'й';
	   case  5:    return 'у';
	   case  6:    return 'х';
	   case  7:    return 'ц';
	   case  8:    return 'ч';
	   case  9:    return 'ы';
	   case 10:    return 'ь';
	   case 11:    return 'я';
	   case 12:    return '.';
	   case 13:    return ',';
	   case 14:    return 'ю';
	   case 15:    return '\n';
	  };
//-------------------------------------------------------------------------
      if( length == 3 )
	       {
		 if( buffer[1]==0xF && buffer[2]==0xF )
		     return EOF;
		 ch = ch | (buffer[1] << 4);
		 ch = ch | buffer[2];
		 return ch;
	       }
//-------------------------------------------------------------------------

 }

//***************************************************************************

