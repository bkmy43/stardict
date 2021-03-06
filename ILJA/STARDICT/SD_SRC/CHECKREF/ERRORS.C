#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dos.h>
#include <string.h>
#include <fcntl.h>

#include "errors.h"
void TError :: RegisterError( int acode ) // �-� �������� ����� ᮮ�. �訡�� � ����� �����
{
 code = acode;
 ErrorRegisted = YES;

 switch ( code )
  {
   case CAN_NOT_OPEN_IN      :
      strcpy( UserMessage, "�訡�� �� ����⨨ ��室���� 䠩��");
      ErrorType=CRITICAL;
      break;
   case ERROR_READING   :
      strcpy( UserMessage, "�訡�� �� �⥭�� �� 䠩��");
      ErrorType=CRITICAL;
      break;
   case NOT_ENOUPH_MEMORY    :
      strcpy( UserMessage, "�������筮 �����");
      ErrorType=CRITICAL;
      break;
   case REF_TO_BAD_OBJECT    :
      strcpy( UserMessage, "� ⥪�� ���� ��뫪� �� ���������騩 ��ꥪ�");
      ErrorType=NONCRITICAL;
      break;
   case NOT_ENOUPH_BUFFER    :
      strcpy( UserMessage, "���� ��९�����. ����⭠ �訡�� � ᨭ⠪�� ����");
      ErrorType=NONCRITICAL;
      break;
   case TOO_MANY_ARGUMENTS:
      strcpy( UserMessage, "���誮� ����� ��㬥�⮢ ��������� ��ப�");
      ErrorType=CRITICAL;
      break;
   case WRONG_TAG    :
      strcpy( UserMessage, "�訡�� � ᨭ⠪�� ����");
      ErrorType=CRITICAL;
      break;

   default :
      strcpy( UserMessage, "����������� ������");
      ErrorType=CRITICAL;
  };

}
//**********************************************************************
void TError :: ReactError() //�-� ��ࠡ��뢠�� �訡�� � ����ᨬ��� �� �� ⨯�.
{
  sound(1000); delay(200); nosound();

// ������� � ��⮪��.
  FILE* erlist;
  if( (erlist = fopen( "troubles.lst", "a" )) == NULL )
      {
         _creat("troubles.lst", FA_ARCH | O_BINARY);
         erlist = fopen( "troubles.lst", "a" );
      }

  if( line == 0 )
     fprintf( erlist, "���������� ������: %s.\n", UserMessage );
  else
     fprintf( erlist, "� ��ப� N %d\n���������� ������: %s.\n", line, UserMessage );

  fprintf( erlist, "----------------------------------------------------------------------\n\n");
  fclose(erlist);

//�뤠�� ᮮ�饭�� �� ��࠭.
  if( ErrorType == NONCRITICAL )
     {
       textcolor(RED);
       if( line == 0 )
         cprintf( "������: %s.\n\r", UserMessage );
       else
         cprintf( "� ��ப� N %d\n\r������: %s.\n\r", line, UserMessage );
       textcolor(LIGHTGRAY);
       cprintf( "\n\r");
     }
  else
     {
       textcolor(RED);
       if( line == 0 )
         cprintf( "����������� ������: %s.\n\r", UserMessage );
       else
         cprintf( "� ��ப� N %d\n\r����������� ������: %s.\n\r", line, UserMessage );

       cprintf( " ��������� ����� ����������� !!!\n\r");
       textcolor(LIGHTGRAY);
       cprintf( "\n\r");
       exit( code );
     }

}



