/* ************************************************************************
                              TAGS.H
 v2.0.
 ����ন�: ���ᠭ�� � ���� ����� ���� �� �뢮�� � ����,
           ���ᠭ�� �������, �ਬ��塞�� ��� ��।�� ������ ����� ��ப�,
           ���ᠭ�� ⨯�� ��뫮�.
 (c) ���� ��஢.
 ( �� enum'� - �, �� ��� �� int �� 㬮�砭�� - �� �� 䨣��, �������
 �������� � ����, ��� � ���⠬�! )
************************************************************************ */

#if !defined(_TAGS_H)
#define _TAGS_H

#if !defined(__WINDOWS_H)
typedef unsigned char BYTE;  // ��⮬ 㡥६, �.�. BYTE ���� � windows.h
#endif // of __WINDOWS_H

//
// ��ࠬ���� ��������� �⨫� ⥪�� � ����. ��� TAG_PARAGRAPH.
//
struct TTextProperty {

    BYTE    txpProperty;

};
enum {

        TXP_CENTERED  = 0x01,  // � ⠪ ����⭮...
        TXP_LEFT      = 0x02,
        TXP_RIGHT     = 0x04,

        TXP_REDLINE   = 0x08   // ������ � ����室����� ��᭮� ��ப� - ����� ��ࠣ��.

};

//
// ��ࠬ���� ��������� �⨫�� ����. ��� ����� TAG_FONTBEGIN-TAG_FONTEND.
//
struct TFontProperty {

    BYTE    fpProperty;

};
enum { // �ਬ������ � ᮤ�ন���� fpProperty :

        FNP_BOLD        = 0x80,
        FNP_ITALIC      = 0x40,
        FNP_UNDERLINED  = 0x20,

        FNP_BASE	= 5  // ������� ᬥ饭�� ��� ��।������ ��������� ����.

};
/*  �����⢠ ����:

            FNP_BOLD == 1, ���� �⮫饭��,
                     == 0, ���� ���筮� ⮫騭�.

          FNP_ITALIC == 1, ���� ��������,
                     == 0, ���� ��אַ�.

      FNP_UNDERLINED == 1, ���� ����ભ��,
                     == 0, ���� �� ����ભ��.

               ���� 0..4 - ����������� �᫮, ࠢ�� ��� �㭪⮢, �� ���஥ �㦭� 㢥����� ࠧ��� ����.
                           �᫨ �᫮==FNP_BASE - ������ �� �㦭�.
                           �������� ���祭�� �᫠ - [0, 31].
                           �������� ���祭�� ���饭�� - [-FNP_BASE, 31-FNP_BASE]
                           ���饭�� ��।������ ��� (��᫮-FNP_BASE).
*/

// ***********************************************************************
// *** ���� ����.
// ��饥: ����� ��� �������� 1 ����, �����⨬� ���祭�� [1..31],
// �᫥� �� ����� � ��ப� ᫥��� ���� ��ࠬ��஢, ��।��塞��
// ⨯�� ����. ���ਬ�� - ��� ���� TAG_PARAGRAPH ������� ��ࠬ��஢ ���,
// ��� TAG_BITMAP - 㪠�뢠���� ���� (32 bit) ���⨭�� � �����.
// � ������⥫��� ����� ��ࠬ��஢ ���:
// TAG_LINKEND, TAG_FONTEND, TAG_KEYWORDEND.
//
// ����� �. �ਬ�砭�� � TAG_KEYWORD-�����.
// ***********************************************************************

enum TArticleTags {

// ����室��� ��⠢�� ��᭮� ��ப� ��� ��������� ࠧ��������� ��ப�.
// ��ࠬ����: TTextProperty �����⢠�����. (BYTE).
// LPSTR p = "<TAG_PARAGRAPH><#00>����� � ��᭮� ��ப�...";
   TAG_PARAGRAPH = 5,

// ����砭�� ��ப�. �뢮� �த�������� � ����� ��ப�.
// ��ࠬ����: ���.
// LPSTR p = "���� ��ப�<TAG_ENDOFLINE>����� � ����� ��ப�...";
   TAG_ENDOFLINE,

// ������� ��砫� ���祢��� (-��) ᫮�.
// ��ࠬ����: ���.
// !!!! ���祢� ᫮�� �� ������ ᮤ�ঠ�� ��� ���� (��뫪� � �.�.) - ���� �����
//      ��뫪� ���� ᮤ�ঠ�� ���� ���祢�� ᫮�.
// LPSTR p = "<TAG_KEYWORDBEGIN>���ய������᪨� �������<TAG_KEYWORDEND>";
   TAG_KEYWORDBEGIN,
// ����砭�� ���祢�� ᫮�.
   TAG_KEYWORDEND,

// ������� ��砫� ��뫪� �� ����� � ����� ����.
// ��ࠬ����: WORD ���������.
// LPSTR p = "<TAG_LINKBEGIN><#0003>����� ��뫪�<TAG_LINKEND>";
   TAG_LINKBEGIN,
// ����砭�� ��뫪�.
   TAG_LINKEND,

// ��⠢�� ��㭪� � �ଠ� BITMAP.
// ��ࠬ����: TBitmap far* �����⥫썠TBitmap (DWORD).
// LPSTR p = "<TAG_BITMAP><(TBitmap far*)pBitmap>..."
   TAG_BITMAP,

// ��������� ᢮��� ⥪�饣� ����.
// ��ࠬ����: TFontProperty �����⢠���������.
// LPSTR p = "<TAG_FONTBEGIN><01100010b>��������,����ભ��� ���� �� 2 �㭪� ���<TAG_FONTEND>.."
   TAG_FONTBEGIN,
// ����砭�� ���������, ���� �����頥� ᢮� �।��騥 �⨫�.
   TAG_FONTEND

};

#endif // of #if !defined ( _TAGS_H )

