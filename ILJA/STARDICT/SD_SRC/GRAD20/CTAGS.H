/********************************************************************
-- STARDICT Project --
��।������ ᨬ����� ����� �ଠ� RSTEXT (2.0)
���ᠭ�� ����� TCTag � ��� ��᫥������.
(c) I.Vladimirsky 1997
********************************************************************/
#include "tags.h"
#include "rw.h"
#include "defines.h"

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

// ���� �뤥����� ���⮬
// �ਬ�� <F 10 100 >�� ���᭮<\F>
//  10 - ࠧ��� ����
//  100 - ����, 010 - ���ᨢ, 001 - ����ભ���, 101 - ���� &����ભ���, ...
#define    CTG_FONT       'F'

//  �������. �ᯮ������ � ������ ���ᨨ ��� ᮧ����� ⠡���.
//  �ਬ��
//  ������<O>����⥫쭠� ���� ����⨭�
//  ��蠤�<O>�����
//    ...
#define    CTG_TAB        'O'

// ����, � ���ன ����饭 ��७�� ��ப.
#define    CTG_NOWARP       'W'

//----------------  ���� 㭨�⮦����� ����� (����� 1-�� �஢�� ) --------------
//���� �����, ����� �� ��������� � ⥪�� ����

enum  {
// ����� ����
// ��ࠬ����: WORD ���������.
   TAG_NUMBER = 33,

// ��砫� �������� ����.
// ��ࠬ����: ���.
   TAG_NAMEBEGIN,
// ����砭�� �������� ����.
   TAG_NAMEEND,

// �������� ⥬�, � ���ன �ਭ������� ������ �����
// ��ࠬ����: WORD ���������.
   TAG_THEMEBEGIN,
// ����砭�� �������� ⥬�.
   TAG_THEMEEND,

// ��砫� ⥫� ����.
// ��ࠬ����: ���.
   TAG_BODYBEGIN,
// ����砭�� ⥫� ����.
   TAG_BODYEND,
};

//----------------   ������祭�� �㦥���� ������ ---------------------

// ��砫� ����
#define    TG_BEGIN_BULLET   '<'

// ����� ����
#define    TG_END_BULLET     '>'

// ���� ����뢠�饣� ����
#define    TG_CLOSE          '/'

//----------------   ����� TCTag ---------------------
class TCTag
    {
       public:
         BYTE TGCode;  // ��� ����

       public:
         TCTag( ) { TGCode = 0; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
         virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
                                               // �� �室���� ��⮪�
    };

class TCTagNum : public TCTag // ��� ����� ����
    {
       public:
         int artnum;
       public:
         TCTagNum() { artnum=0; TGCode = TAG_NUMBER; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // �-� ��ࠡ�⪨ ����
         virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
    };

class TCTagName : public TCTag // ���뢠�騩 ��� ����� ����
    {
       public:
         char str[256];
       public:
         TCTagName() { TGCode = TAG_NAMEBEGIN; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) ; // �-� ��ࠡ�⪨ ����
    };

class TCTagEndName : public TCTag // ����뢠�騩 ��� ����� ����
    {
       public:
         TCTagEndName() { TGCode = TAG_NAMEEND; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // �-� ��ࠡ�⪨ ����
    };

class TCTagTheme : public TCTag // ���뢠�騩 ��� ⥬�
    {
       public:
         char str[256];
       public:
         TCTagTheme() { TGCode = TAG_THEMEBEGIN; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
    };

class TCTagEndTheme : public TCTag // ����뢠�騩 ��� ⥬�
    {
       public:
         TCTagEndTheme() { TGCode = TAG_THEMEEND; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // �-� ��ࠡ�⪨ ����
    };

class TCTagBody : public TCTag // ���뢠�騩 ��� ⥫� ����
    {
       public:
         char* KeywordsTable[MAX_KEYWORDS_IN_ART];
         int KeywordsNum;

       public:
         TCTagBody();
         ~TCTagBody();
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
    };

class TCTagEndBody : public TCTag // ����뢠�騩 ��� ⥫� ����
    {
       public:
         TCTagEndBody() { TGCode = TAG_BODYEND; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ) { return OK; }; // �-� ��ࠡ�⪨ ����
    };

class TCTagParagraph : public TCTag // ��� ��ࠣ��
    {
         int align;  // ࠧ��饭�� ⥪�� � ��ࠣ��
       public:
         TCTagParagraph() { TGCode = TAG_PARAGRAPH; align=0;};
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
         virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
    };

class TCTagTabulation : public TCTag // ��� ⠡��樨
    {
        int size; //ࠧ��� ⠡��樨
       public:
         TCTagTabulation() { TGCode = TAG_TABULATION; size=0;};
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
         virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
    };

class TCTagEndOfLine : public TCTag // ��� ���� ��ப�
    {
       public:
         TCTagEndOfLine() { TGCode = TAG_ENDOFLINE; };
    };


class TCTagBitmap : public TCTag // ��� ��⠢�� ������
    {
         int BitmapNumber;
       public:
         TCTagBitmap() { TGCode = TAG_BITMAP; };
         virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
         virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
    };

class TCTagLink : public TCTag // ��� ��砫� ��뫪�
    {
         int LinkArtNum;

       public:
         char* KeywordsTable[MAX_KEYWORDS_IN_LINK];
         int KeywordsNum;

       public:
         TCTagLink();
         ~TCTagLink();
         int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
         int GetArguments( BufRead & br ); // �⠥� ����� ����
    };

class TCTagEndLink : public TCTag // ��� ���� ��뫪�
    {
       public:
         TCTagEndLink() { TGCode = TAG_LINKEND; };
    };


class TCTagKey : public TCTag // ��� ��砫� ���祢��� ᫮��
    {
       public:
         char str[256];
       public:
         TCTagKey() { TGCode = TAG_KEYWORDBEGIN; };

         int GetArguments( BufRead & brIn ); // �⠥� �᭮���� ��� �����ठ
         int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
    };

class TCTagEndKey : public TCTag // ��� ���� ���祢��� ᫮��
    {
       public:
         TCTagEndKey() { TGCode = TAG_KEYWORDEND; };
    };

class TCTagFont : public TCTag // ��� ��砫� �뤥����� ���⮬
    {
         int FontSize;
         int FontType;
       public:
         TCTagFont() { TGCode = TAG_FONTBEGIN; };
         int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
         int GetArguments( BufRead & br ); // �⠥� ����� ����
    };

class TCTagEndFont : public TCTag // ��� ���� �뤥����� ���⮬
    {
       public:
         TCTagEndFont() { TGCode = TAG_FONTEND; };
    };


class TCTagNowarp : public TCTag // ��� ��砫� nowarp ����
    {
       public:
         TCTagNowarp() { TGCode = TAG_NOWARPBEGIN; };
         int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
    };

class TCTagEndNowarp : public TCTag // ��� ���� nowarp ����
    {
       public:
         TCTagEndNowarp() { TGCode = TAG_NOWARPEND; };
    };


//----------------  ��. �㭪樨 ---------------------
// �-� �����뢠�� � ���� ����� �� �室���� ��⮪�, ���� �� ������
// IG_OPEN_BULLET ('<'). ��⥬ �⠥� char ��� ����, ॣ�������
// ᮮ⢥�����騩 ����� ��⮬�� CTag, �⠥� ᮮ좥�����騥 ��㬥��� ����
// � �����頥� 㪠��⥫� �� ����.
// !!! �� �⮬ �� ����� ����� ��ॢ������ � Win1251
// !!! 㡨����� ����� \n, \r, \t, ��७��� � ��譨� �஡���.
TCTag* ProceedUntilTag_buf( BufRead & brIn , char* buffer, int buflenth );

// �-� �����뢠�� � ��室��� ��⮪ ����� �� �室���� ��⮪�, ���� �� ������
// IG_OPEN_BULLET ('<'). ��⥬ �⠥� char ��� ����, ॣ�������
// ᮮ⢥�����騩 ����� ��⮬�� CTag, �⠥� ᮮ좥�����騥 ��㬥��� ����
// � �����頥� 㪠��⥫� �� ����.
// !!! 㡨����� ����� \n, \r, \t, ��७��� � ��譨� �஡���.
TCTag* ProceedUntilTag( BufRead & brIn , BufWrite & bwOut );

void strtrim(char* str); //�-� 㡨ࠥ� �஡��� � ���� ��ப�

#endif