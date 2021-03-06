#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dos.h>
#include <string.h>
#include <fcntl.h>

#include "errors.h"
#include "rw.h"

extern BufRead brIn;

void TError :: RegisterError( int acode ) // �-� �������� ����� ᮮ�. �訡�� � ����� �����
{
 code = acode;
 ErrorRegisted = YES;
 Line = brIn.CurLine;

 switch ( code )
  {
   case CAN_NOT_OPEN_IN      :
      strcpy( UserMessage, "�訡�� �� ����⨨ ��室���� 䠩��");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_INDEX   :
      strcpy( UserMessage, "�訡�� �� ����⨨ 䠩�� index.tmp");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_TH      :
      strcpy( UserMessage, "�訡�� �� ����⨨ 䠩�� themes.tmp");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_DATA    :
      strcpy( UserMessage, "�訡�� �� ����⨨ 䠩�� data.tmp");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_KEY    :
      strcpy( UserMessage, "�訡�� �� ����⨨ 䠩�� keyword.tmp");
      ErrorType=CRITICAL;
      break;
   case NO_IN_NAME    :
      strcpy( UserMessage, "�� 㪠���� ��� ��室���� 䠩��");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_CREATE_TEMP  :
      strcpy( UserMessage, "�訡�� �� ᮧ����� temp-䠩��");
      ErrorType=CRITICAL;
      break;
   case ERROR_READING	:
      strcpy( UserMessage, "�訡�� �� �⥭�� �� 䠩��");
      ErrorType=CRITICAL;
      break;
   case ART_NUMBER_NOT_FOUND :
      strcpy( UserMessage, "� ��室��� 䠩�� ��� ����� ����");
      ErrorType=CRITICAL;
      break;
   case ART_NAME_NOT_FOUND   :
      strcpy( UserMessage, "� ��室��� 䠩�� ��� �������� ����");
      ErrorType=CRITICAL;
      break;
   case WRONG_ART_NAME       :
      strcpy( UserMessage, "�������⨬�� �������� ����");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_TH_NAME        :
      strcpy( UserMessage, "�������⨬�� �������� ⥬�");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_ART_NUMBER     :
      strcpy( UserMessage, "�������⨬� ����� ����");
      ErrorType=NONCRITICAL;
      break;
   case TH_NOT_EXIST         :
      strcpy( UserMessage, "� 䠩�� ⥬ ��� ⠪�� ⥬�");
      ErrorType=NONCRITICAL;
      break;
   case ERROR_READING_TH     :
      strcpy( UserMessage, "�訡�� �� �⥭�� 䠩�� ⥬");
      ErrorType=CRITICAL;
      break;
   case NOT_ENOUPH_MEMORY    :
      strcpy( UserMessage, "�������筮 �����");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_TH          :
      strcpy( UserMessage, "���� ⥬ �ᯮ�祭");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_KEYS          :
      strcpy( UserMessage, "���� ���祢�� ᫮� �ᯮ�祭");
      ErrorType=CRITICAL;
      break;

   case BODY_NOT_FOUND       :
      strcpy( UserMessage, "� ��室��� 䠩�� ��� ⥪�� ����");
      ErrorType=CRITICAL;
      break;
   case BODY_END_NOT_FOUND   :
      strcpy( UserMessage, "� ��室��� 䠩�� ��� ᨬ���� ���� ⥪�� ����");
      ErrorType=CRITICAL;
      break;
   case INVALID_REF          :
      strcpy( UserMessage, "�������⨬�� ��뫪�");
      ErrorType=NONCRITICAL;
      break;
   case TH_EXISTS            :
      strcpy( UserMessage, "���� � ⠪�� ����஬ 㦥 �������");
      ErrorType=NONCRITICAL;
      break;
   case ART_EXISTS           :
      strcpy( UserMessage, "����� � ⠪�� ����஬ 㦥 �������");
      ErrorType=NONCRITICAL;
      break;
   case TH_NUMBER_NOT_FOUND  :
      strcpy( UserMessage, "� ��室��� 䠩�� ��� ����� ⥬�");
      ErrorType=CRITICAL;
      break;
   case TH_NAME_NOT_FOUND    :
      strcpy( UserMessage, "� ��室��� 䠩�� ��� �������� ⥬�");
      ErrorType=CRITICAL;
      break;
   case REF_TO_BAD_OBJECT    :
      strcpy( UserMessage, "� ⥪�� ���� ��뫪� �� ���������騩 ��ꥪ�");
      ErrorType=NONCRITICAL;
      break;
   case SELFREF              :
      strcpy( UserMessage, "� ⥪�� ���� ��뫪� �� ᥡ�");
      ErrorType=NONCRITICAL;
      break;
   case NOT_ENOUPH_BUFFER    :
      strcpy( UserMessage, "���� ��९�����. ����⭠ �訡�� � ᨭ⠪�� ����");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_TAG    :
      strcpy( UserMessage, "�訡�� � ᨭ⠪�� ����");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_TAG_INCLUDE    :
      strcpy( UserMessage, "����饭��� ����������� �����");
      ErrorType=NONCRITICAL;
      break;
   case FIRST_LEVEL_TAG_INCLUDE    :
      strcpy( UserMessage, "����������� ����� ��ࢮ�� �஢�� (number,name,theme)");
      ErrorType=NONCRITICAL;
      break;
   case FIRST_LEVEL_WRONG_TAG   :
      strcpy( UserMessage, "�����㦥� ����७��� ��� �� ��ࢮ� �஢��");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_INDEX   :
      strcpy( UserMessage, "�訡�� � 䠩�� index.dat");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_DATA   :
      strcpy( UserMessage, "�訡�� � 䠩�� data.dat");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_THEMES   :
      strcpy( UserMessage, "�訡�� � 䠩�� themes.dat");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_TEMP   :
      strcpy( UserMessage, "�訡�� �� ����⨨ temp-䠩��");
      ErrorType=CRITICAL;
      break;
   case WRONG_TAG_IN_TH_MODE   :
      strcpy( UserMessage, "� ०��� �⥭�� ⥬ �����㦥� ����饭�� ���");
      ErrorType=CRITICAL;
      break;

   case TOO_MANY_KEYWORDS_IN_ART :
      strcpy( UserMessage, "������⢮ �����म� � ���� ����� �।��쭮 �����⨬���");
      ErrorType=CRITICAL;
      break;

   case TOO_MANY_KEYWORDS_IN_LINK :
      strcpy( UserMessage, "������⢮ �����म� ����� ��뫪� ����� �।��쭮 �����⨬���");
      ErrorType=CRITICAL;
      break;

   case WRONG_KEYWORD :
      strcpy( UserMessage, "�訡�� � ���祢�� ᫮��");
      ErrorType=CRITICAL;
      break;
   case INVALID_ARGUMENTS :
      strcpy( UserMessage, "������ ��㬥��� ��������� ��ப�");
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
  fprintf( erlist, "� ���� (⥬�) ����� %d\n"
		 "��� ���������: \"%s\"\n"
		 "�� ��ࠡ�⪥ %d ��ப� ��室���� 䠩��\n"
		 "���������� ������: %s.\n"
		  "----------------------------------------------------------------------\n\n",
		   num, name, Line, UserMessage );
  fclose(erlist);

//�뤠�� ᮮ�饭�� �� ��࠭.
  if( ErrorType == NONCRITICAL )
     {
       textcolor(RED);
       cprintf( "� ��ப� N %d\n\r������: %s.", Line, UserMessage );
       textcolor(LIGHTGRAY);
       cprintf( "\n\r");
     }
  else
     {
       textcolor(RED);
       cprintf( /*"� ��ப� N %d\n*/" ����������� ������: %s.\n\r", /*line,*/ UserMessage );
       cprintf( " ��������� ����� ����������� !!!\n\r");
       textcolor(LIGHTGRAY);
       cprintf( "\n\r");
       exit( code );
     }

}



