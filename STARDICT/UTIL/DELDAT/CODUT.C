#include  "rw.h"

// �-�� ��� ࠡ��� � 䠩����

//*************************************************************************
char UncodeChar(  int* buffer, int length )
//   �㭪�� ����砥� ����� ᮤ��. 1 2 ��� 3 ���㡠��, � �� ������⢮
// ( length ).  ��ॢ���� ��� � ᨬ��� � �����頥� ��� ᨬ���.
 {
     char ch=0;
//-------------------------------------------------------------------------
     if( length == 1 )   // �᫨ �� ᨬ��� 1 ⨯�
       switch( buffer[0] )
	 {
	   case 0:    return ' ';
	   case 1 :   return '�';
	   case 2 :   return '�';
	   case 3 :   return '�';
	   case 4 :   return '�';
	   case 5 :   return '�';
	   case 6 :   return '�';
	   case 7 :   return '�';
	   case 8 :   return '�';
	   case 9 :   return '�';
	   case 10:   return '�';
	   case 11:   return '�';
	   case 12:   return '�';
	   case 13:   return '�';
	 };
//-------------------------------------------------------------------------
      if( length == 2 )
	switch( buffer[1] )
	  {
	   case  0:    return '�';
	   case  1:    return '�';
	   case  2:    return '�';
	   case  3:    return '�';
	   case  4:    return '�';
	   case  5:    return '�';
	   case  6:    return '�';
	   case  7:    return '�';
	   case  8:    return '�';
	   case  9:    return '�';
	   case 10:    return '�';
	   case 11:    return '�';
	   case 12:    return '.';
	   case 13:    return ',';
	   case 14:    return '�';
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

