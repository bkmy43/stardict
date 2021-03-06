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
   case WRONG_TH_NUMBER      :
      strcpy( UserMessage, "�������⨬� ����� ⥬�");
      ErrorType=NONCRITICAL;
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
  fprintf( erlist, "� ���� ����� %d\n"
		 "��� ���������: \"%s\"\n"
		 "� ��ப�: %d"
		 "���������� ������: %s.\n"
		  "----------------------------------------------------------------------\n\n",
		   /*artnum, artname,line, */ 0,"",0, UserMessage );
  fclose(erlist);

//�뤠�� ᮮ�饭�� �� ��࠭.
  if( ErrorType == NONCRITICAL )
       printf( "� ��ப� N %d\n ������: %s.\n\n", line, UserMessage );
  else
     {
       printf( " � ��ப� N %d\n ����������� ������: %s.\n", line, UserMessage );
       printf( " ��������� ����� ����������� !!!\n\n");
       exit( code );
     }

}



