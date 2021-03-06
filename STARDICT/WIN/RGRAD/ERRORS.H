/********************************************************************
-- STARDICT Project --
���ᠭ�� ����� TError � ���� �訡��, ���������� � 室� ࠡ���
�ணࠬ�� RGRAD.
(c) I.Vladimirsky 1997
********************************************************************/
#if !defined _ERRORS_H
#define _ERRORS_H

#define CRITICAL            1
#define NONCRITICAL         0

enum {
	NONE = 0,                  // �訡�� ���

	ERROR_READING,
	ERROR_WRITING,

	CAN_NOT_OPEN_IN,       // �� ���� ������ �室��� 䠩�
	CAN_NOT_OPEN_INDEX,    // �� ���� ������ index 䠩�
	CAN_NOT_OPEN_TH,       // �� ���� ������ themes 䠩�
	CAN_NOT_OPEN_DATA,     // �� ���� ������ data 䠩�
	NO_IN_NAME,            // �� 㪠���� ��� ��室���� 䠩��
	CAN_NOT_CREATE_TEMP,   // �� ���� ᮧ���� temp-䠩�
	CAN_NOT_OPEN_TEMP,   // �� ���� ������ temp-䠩�

	ERROR_IN_INDEX,     // �訡�� � 䠩�� index.dat
	ERROR_IN_DATA,      // �訡�� � 䠩�� data.dat
	ERROR_IN_THEMES,    // �訡�� � 䠩�� themes.dat

        ART_NUMBER_NOT_FOUND,  //�� ������ ����� ����
	TH_NUMBER_NOT_FOUND,   // ����� ���� �� ������
	ART_NAME_NOT_FOUND,    // �� ������� ��� ����
	TH_NAME_NOT_FOUND,     // �� ������� ��� ⥬�
	BODY_NOT_FOUND,        // ⥪�� ���� �� ������
	BODY_END_NOT_FOUND,    // ����� ⥪�� ���� �� ������

	WRONG_ART_NUMBER,      // �訡�� � ����� ����
	WRONG_TH_NUMBER,       // �訡�� � ����� ⥬�
	WRONG_ART_NAME,        // �訡�� � ����� ����
	WRONG_TH_NAME,         // �訡�� � ����� ⥬�

	WRONG_TAG,             // �訡�� ᨭ⠪�� ����
	WRONG_TAG_INCLUDE,     // ����饭��� ����������� �����
	FIRST_LEVEL_TAG_INCLUDE, // ����������� ����� ��ࢮ�� �஢��
			       // ���, �����, ⥬�
	FIRST_LEVEL_WRONG_TAG, // �� ��ࢮ� �஢�� ������ ����饭�� ���

	TH_NOT_EXIST,          // ⥬�, 㯮���⮩ � ���� �� �������
	ERROR_READING_TH,      // �訡�� �� �⥭�� ⥬�
	ERROR_IN_TH,           // �訡�� � ⥬�

	NOT_ENOUPH_MEMORY,     // ��墠⪠ �����
	NOT_ENOUPH_BUFFER,     // ᫨誮� �����쪨� ���� -> WRONG_TAG
	TH_EXISTS,             // ⠪�� ⥬� 㦥 ����
	ART_EXISTS,            // ⠪�� ����� 㦥 ����

	INVALID_REF,           // ����ୠ� ��뫪�
	REF_TO_BAD_OBJECT,     // ��ꥪ� ��뫪� �� �������
	SELFREF                // ��뫪� �� ᥡ�
      };

class TError
  {
     public:
       int code;
       int ErrorType; // ����᪠� �� �� �訡��.
		      // �㦭� �� �४���� ࠡ��� �ணࠬ�� �� �� ������
       char UserMessage[256]; // ����� ᮮ�饭�� �뤠������ ���짮��⥫�
       int line; // ��ப�, � ���ன �����㦥�� �訡��

     public:
       TError( ) {code=NONE; ErrorType=NONCRITICAL; };
       void RegisterError( int code ); // �-� �������� ����� ᮮ�. �訡�� � ����� �����
       void ReactError(); //�-� ��ࠡ��뢠�� �訡�� � ����ᨬ��� �� �� ⨯�.
       void ProceedError(int code) { RegisterError(code); ReactError(); };
  };

#endif
