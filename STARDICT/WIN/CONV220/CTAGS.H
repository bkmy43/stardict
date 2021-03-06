/********************************************************************
-- STARDICT Project --
��।������ ᨬ����� ����� �ଠ� RSTEXT (2.0)
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _CTAGS_H
#define _CTAGS_H

//----------------   ������祭�� �����  --------------------------

// ����� ���� (⥬�).    �ਬ��  <# 13>
#define    CTG_NUMBER     '#'

// �������� ���� (⥬�). �ਬ��  <N>����� � �।� ��७��</N>
#define    CTG_NAME       'N'

// �������� ⥬�, � ���ன �⭮���� �����. �ਬ��  <T>�।�� �ਢ�窨<\T>
#define    CTG_THEME      'T'

// ��砫� ⥪�� ����. �ਬ��  <B> .... </B>
#define    CTG_BODY       'B'

// ��ࠣ�� (����� � ��᭮� ��ப�). �ਬ��  <P>����� �祭� �।��
#define    CTG_PARAGRAPH  'P'

// �������� ������ ��ப�
#define    CTG_LINEEND    'E'

// Bitmap. �ਬ��  <A 12>
#define    CTG_BITMAP     'A'

// ��뫪� �� ����� �����. �ਬ��   <L>�����<\L>
#define    CTG_LINK       'L'

//  ���祢�� ᫮�� (�� ���祢� ᫮��� �����⢫���� ����).
//  �ਬ�� <K>����⨭<\K>
#define    CTG_KEYWORD    'K'

// �������� ����.
// �ਬ��
//#define    CTG_FONT       F

//----------------   ������祭�� �㦥���� ������ ---------------------

// ��砫� ����
#define    TG_BEGIN_BULLET   '<'

// ����� ����
#define    TG_END_BULLET     '>'

// ���� ����뢠�饣� ����
#define    TG_CLOSE          '/'

//----------------   �㦥��� ����⠭�� ---------------------
#define    MAX_TAG_ATTRIB_SYMBOLS 10

//----------------   ����� TCTag ---------------------
// ��ਡ��� ���� -
/*struct TCTagAttrib
    {
         union unum {                   // � ����ᨬ��� �� ⨯� ����
                     char byte;        // �������� ࠧ��
                     int  word;       // �᫮�� ��ਡ���
                     long dword;
                    } num;

         char symbols[MAX_TAG_ATTRIB_SYMBOLS];
    };

class TCTag
    {
       public:
         char TGType;
         TCTagAttrib TGAttrib;

       public:
         TCTag( aTGType, char anum, char* asymbols = NULL);
         TCTag( aTGType, int  anum, char* asymbols = NULL);
         TCTag( aTGType, long anum, char* asymbols = NULL);
         TCTag( aTGType );
    };

           {TGType = aTGType; TGAttrib.num.byte = anum;
            for(int i=0; asymbols[i]) }


  */

#endif