/********************************************************************
-- STARDICT Project --
���ᠭ�� ����� TError � ���� �訡��, ���������� � 室� ࠡ���
�ணࠬ�� RGRAD.
(c) I.Vladimirsky 1997
********************************************************************/
#if !defined _ERRORS_H
#define _ERRORS_H

#include <conio.h>
#define CRITICAL            1
#define NONCRITICAL         0

#define YES  1
#define  NO  0

enum {
        NONE = 0,                  // �訡�� ���

        ERROR_READING,
        ERROR_WRITING,

        CAN_NOT_OPEN_IN,       // �� ���� ������ �室��� 䠩�

        NOT_ENOUPH_MEMORY,     // ��墠⪠ �����
        NOT_ENOUPH_BUFFER,     // ᫨誮� �����쪨� ����

        REF_TO_BAD_OBJECT,     // ��ꥪ� ��뫪� �� �������
        WRONG_TAG,             // ��ꥪ� � ᨭ⠪��
        TOO_MANY_ARGUMENTS, // ᫨誮� ����� ��室��� 䠩���
 };

class TError
  {
     public:
       int code;
       int ErrorRegisted; // YES/NO - �뫠 �� ��䨪�஢��� �訡��
       int ErrorType; // ����᪠� �� �� �訡��.
                      // �㦭� �� �४���� ࠡ��� �ணࠬ�� �� �� ������
       char UserMessage[256]; // ����� ᮮ�饭�� �뤠������ ���짮��⥫�
       int line; // ��ப�, � ���ன �����㦥�� �訡��

     public:
       TError( ) {code=NONE; ErrorType=NONCRITICAL; ErrorRegisted=NO; };
       void RegisterError( int code ); // �-� �������� ����� ᮮ�. �訡�� � ����� �����
       void ReactError(); //�-� ��ࠡ��뢠�� �訡�� � ����ᨬ��� �� �� ⨯�.
       void ProceedError(int code) { RegisterError(code); ReactError(); };
  };

#endif
