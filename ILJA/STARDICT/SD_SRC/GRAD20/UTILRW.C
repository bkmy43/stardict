/********************************************************************
-- STARDICT Project --
�������⥫�� �⨫��� �����⢫��騥 ������/�⥭��  � 䠩��
����ﬨ �� ���-����.
�᭮���� �� �ᯮ�짮����� ���ਧ�樨 BufRead � BufWrite.
(c) I.Vladimirsky 1997
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "rw.h"

int GoToChar( BufRead& brIn, char ch, char stopcode )
// �-� ���室�� � 䠩�� brIn �� ᨬ��� ch �����. ��� �訡��
// ���� �����⢫���� �� �� ��� ���� �� �������� stopcode,
// ��祬 �᫨ stopcode �� 㪠��� �� �맮�� � �� 㬮�砭�� �� = EOF
  {
     int res=0;

     while(  res != ch  )
        {
           if ( brIn.ReadByte( &res ) == EOF )
               return EOF;

           if( res == stopcode )
               return STOP_CODE_FOUND;
        }
    return 1;
  }
//***********************************************************************
int GoToNthChar( int n, BufRead& brIn, char ch )
// �-� ���室�� � 䠩�� brIn �� n-� ᨬ��� 'ch' �����. ��� �訡��
{
  int res=0;
  for( int i=0; i<n; i++ )
    if( (res = GoToChar( brIn, ch )) != 1 )    // ��३� �� ᫥�. ch
        return res;                           // ������ ��� �訡��
  return 1;
}
//***********************************************************************
int ReadInt( BufRead& brIn, int* res )
//  �-� �⠥� �� 䠩�� 2 ���� � �८�ࠧ�� �� � 楫��
  {
     int b1,b2;

     if (  brIn.ReadByte(  &b2 ) != 1 || brIn.ReadByte(  &b1 ) != 1 )
            return 0;

     *res = ( b1 << 8)|b2 ;
     return 1;
  }
//**************************************************************************
int ReadIntFromChar( BufRead& brIn, int* res )
//  �-� �⠥� �� 䠩�� ����� ��� �� �஡��� ��� '\n'
// � �८�ࠧ�� �� � 楫��
  {
     char buffer[256] = {0};
     int ibuf;
     int i=0;
     do
      {
         if( i == 255 )  return 0;
         if( brIn.ReadByte( &ibuf ) != 1 )    return 0;
         buffer[i] = (char)ibuf;
         i++;
      }
     while(! ((buffer[i-1]==' ') || (buffer[i-1]=='\n')
                || (buffer[i-1]=='>')) );

     *res = atoi( buffer );
     brIn.Rew();
     return 1;
  }
//**************************************************************************
int SeekSpaces( BufRead& brIn ) //�஬�⪠ �஡���� � '\n'
{
// �ய�� �஡���� � '\n'
   int buffer;
   do
      if( readByte( brIn, &buffer) != 1 )
            return 0;
   while( buffer == ' ' || buffer == '\n' || buffer == '\r');

   brIn.Rew();
   return 1;
}
//**************************************************************************
int WriteInt( BufWrite& bwOut, int i )
  {
     int b1,b2;

     if( i<0 || i> 0x7FFE )
         return 0;

     b1 = i>>8;
     b2 = i&0x00FF;


     if (  bwOut.WriteByte(  b2 ) != 1 || bwOut.WriteByte(  b1 ) != 1 )
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
//****************************************************************************
int ReadName( BufRead& brIn, char* name )
// �-� �⠥� ��ப� � ⥪�饣� ��������� �� ᨬ��� '^'
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
int WriteName( BufWrite& bwOut, char* name )
// �-� �����뢠�� ��ப�, �����稢��� �� ᨬ����� '^'
  {
    int i;

    for(  i=0 ; name[ i ] != 0 ; i++ )
        if( bwOut.WriteByte( (int)(name[i]) ) != 1 )
            return 0;

    if( bwOut.WriteByte( '^' ) != 1 )
            return 0;

    return 1;
 }

//*********************************************************************
int WriteString( BufWrite& bwOut, char* str )
{
    int i;

    for(  i=0 ; str[ i ] != 0 ; i++ )
        if( bwOut.WriteByte( (int)(str[i]) ) != 1 )
            return 0;

    return 1;
}

//*********************************************************************

int ReadLong( BufRead& brIn, long* res ) // ���������� ������ �������������
//  �-� �⠥� �� 䠩�� 4 ���� � �८�ࠧ�� �� � long
  {
     int b1, b2, b3, b4;

     if (  brIn.ReadByte(  &b1 ) != 1 || brIn.ReadByte(  &b2 ) != 1 ||
           brIn.ReadByte(  &b3 ) != 1 || brIn.ReadByte(  &b4 ) != 1  )
            return 0;

     *res = ((long)b1<<24) | ((long)b2<<16) | ((long)b3 << 8) | (long)b4 ;
     return 1;
  }

//**************************************************************************
