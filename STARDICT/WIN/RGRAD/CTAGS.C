/********************************************************************
-- STARDICT Project --
��।������ ᨬ����� ����� �ଠ� RSTEXT (2.0)
��।������ �-�� ����� TCTag � ��� ��᫥������.
(c) I.Vladimirsky 1997
********************************************************************/
#include "ctags.h"
#include "errors.h"

extern TError CurError;

//**********************************************************************
// �᭮���� �����.
/*class TCTag
    {
       public:
	 BYTE TGPriority;
	 BYTE TGCode;  // ��� ����

       public:
	 TCTag( ) {TGPriority = 0; TGCode = 0; };
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
	 virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
					       // �� �室���� ��⮪�
    };
*/

int TCTag :: GetArguments( BufRead & brIn )
// �᫨ � ���� ���� ��㬥���, �� �㭪�� �㤥� ��८�।����� � ��� �����.
// � ��⨢��� ��砥 �⥭�� ��㬥�⮢ �।�⠢��� �� ᥡ� ���� ᨬ����
// ���� ���� '>'.
{
   int buffer;
   SeekSpaces( brIn ); // �஬���� �஡���
   if( readByte( brIn, &buffer ) != 1 )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
// ����� ������ ���� '>'
   if( buffer != TG_END_BULLET )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTag :: ProceedTag( BufRead& brIn, BufWrite & bwOut ) // �-� ��ࠡ�⪨ ����
// ���� ��ࠡ�⪠ ���� �।�⠢��� �� ᥡ� ⮫쪮 ������ ��� ���� �
// ��室��� ��⮪. � ��⨢��� ��砥 �� �-� �㤥� ��८�।�����
{
   if( writeByte( bwOut, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   return OK;
}

//**********************************************************************
/*class TCTagNum : public TCTag // ��� ����� ����
    {
	 int artnum;
       public:
	 TCTagNum() { artnum=0;};
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {} ;// �-� ��ࠡ�⪨ ����
	 virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
    }; */

int TCTagNum :: GetArguments( BufRead & brIn ) // �⠥� ����� ����
{
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &artnum ) != 1 ) // ������ �����
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( artnum == 0 )
      {  CurError.ProceedError( WRONG_ART_NUMBER ); return NOK; }

// ������ �� '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
// ����� ������ ���� '>'
   if( buffer != TG_END_BULLET )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

//**********************************************************************
/*class TCTagName : public TCTag // ���뢠�騩 ��� ����� ����
    {
	 char str[256];
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
    };*/
// ��㬥�⮢ ���

// �-� �⠥� ��� ���� � str.
int TCTagName :: ProceedTag(  BufRead& br, BufWrite & bw ) // �-� ��ࠡ�⪨ ����
{
   TCTag* ptag = NULL;

   ptag = ProceedUntilTag_buf( br, str, 255 );
   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��

//�᫨ �� �� ��� ���� ����� - �訡��
   if( ! ptag->TGCode == TAG_NAMEEND  )
	 { CurError.ProceedError(WRONG_ART_NAME); return NOK; }

   if(ptag != NULL) delete ptag;
   return OK;
}

//*********************************************************************
/*class TCTagEndName : public TCTag // ����뢠�騩 ��� ����� ����
    {
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // �-� ��ࠡ�⪨ ����
    };*/

// ��ࠡ�⪨ ���
// ��㬥�⮢ ���
//**********************************************************************
/*class TCTagTheme : public TCTag // ���뢠�騩 ��� ⥬�
    {
	 char str[256]
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // �-� ��ࠡ�⪨ ����
    };*/

// ��ࠡ�⪨ ���
// ��㬥�⮢ ���
int TCTagTheme :: ProceedTag(  BufRead& br, BufWrite & bw ) // �-� ��ࠡ�⪨ ����
{
   TCTag* ptag = NULL;

   ptag = ProceedUntilTag_buf( br, str, 255 );
   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��

//�᫨ �� �� ��� ���� ����� - �訡��
   if( ! ptag->TGCode == TAG_NAMEEND  )
	 { CurError.ProceedError(WRONG_TH_NAME); return NOK; }

   if(ptag != NULL) delete ptag;
   return OK;
}
//**********************************************************************
/*class TCTagEndTheme : public TCTag // ����뢠�騩 ��� ⥬�
    {
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // �-� ��ࠡ�⪨ ����
    };*/

// ��ࠡ�⪨ ���
// ��㬥�⮢ ���

//**********************************************************************
/*class TCTagBody : public TCTag // ���뢠�騩 ��� ⥫� ����
    {
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
    };*/

int TCTagBody :: ProceedTag(  BufRead& br, BufWrite & bw ) // �-� ��ࠡ�⪨ ����
{
// ⥯��� �㦭� ���� ����뢠�騩 ���
   TCTag* ptag = NULL;
   do
   {
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��

//�஢���� �������� ��� �� ���� �� �� ����� ��ࢮ�� �஢��
     if( ptag->TGCode == TAG_NUMBER  ||  ptag->TGCode == TAG_NAMEBEGIN
	 ||  ptag->TGCode == TAG_NAMEEND ||  ptag->TGCode == TAG_THEMEBEGIN
	 ||  ptag->TGCode == TAG_THEMEEND || ptag->TGCode == TAG_BODYBEGIN )
	 { CurError.ProceedError(FIRST_LEVEL_TAG_INCLUDE); return NOK; }

     if( !(ptag->TGCode == TAG_LINKEND ||  ptag->TGCode == TAG_KEYWORDEND
	 ||  ptag->TGCode == TAG_FONTEND ) )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	 }
//���� �訡��
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_BODYEND);

   if(ptag != NULL) delete ptag;
   return OK;
}
//**********************************************************************
/*class TCTagEndBody : public TCTag // ����뢠�騩 ��� ⥫� ����
    {
       public:
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ) {}; // �-� ��ࠡ�⪨ ����
    };*/

// ��ࠡ�⪨ ���
// ��㬥�⮢ ���

//**********************************************************************
/*class TCTagParagraph : public TCTag // ��� ��ࠣ��
    {
	 int align;  // ࠧ��饭�� ⥪�� � ��ࠣ��
       public:
	 TCTagParagraph() { TGCode = TAG_PARAGRAPH; };
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
	 virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
    };*/

// ��ࠡ�⪠ - ������ ���� � ��ࠬ��஢
int TCTagParagraph :: ProceedTag(  BufRead& br, BufWrite & bwOut ) // �-� ��ࠡ�⪨ ����
{
   if( writeByte( bwOut, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

   int tProperty=0;
   tProperty = ((align%100)<<2)|(((align%100)/10)<<2)|(align%10<<1);
   if( writeByte( bwOut, tProperty ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   return OK;
}

// �⠥� ��㬥��, �����騩 ᯮᮡ ��ࠢ�������
int TCTagParagraph :: GetArguments( BufRead & brIn )
{
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &align ) != 1 ) // ������ ��ࠢ�������
      {  CurError.ProceedError( ERROR_READING  ); return NOK; }
   if( !( align == 0 || align == 1 || align == 10 || align == 100) )
      {  CurError.ProceedError( WRONG_TAG  ); return NOK; }

// ������ �� '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
// ����� ������ ���� '>'
   if( buffer != TG_END_BULLET )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

//**********************************************************************
/*class TCTagEndOfLine : public TCTag // ��� ���� ��ப�
    {
       public:
	 TCTagEndOfLine() { TGCode = TAG_ENDOFLINE; };
    };*/
// ��ࠡ�⪠ �⠭���⭠�
// ��㬥�⮢ ���

//**********************************************************************
/*class TCTagBitmap : public TCTag // ��� ��⠢�� ������
    {
	 int BitmapNumber;
       public:
	 TCTagBitmap() { TGCode = TAG_BITMAP; };
	 virtual int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
	 virtual int GetArguments( BufRead & br ); // �⠥� ��㬥���
    };*/

int TCTagBitmap :: GetArguments( BufRead & brIn ) // �⠥� ����� ����
{
   if( SeekSpaces( brIn ) != 1)  // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &BitmapNumber ) != 1 ) // ������ ����� ������
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( BitmapNumber == 0 )
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }

// ������ �� '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
// ����� ������ ���� '>'
   if( buffer != TG_END_BULLET )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTagBitmap :: ProceedTag(  BufRead& br, BufWrite & bwOut ) // �-� ��ࠡ�⪨ ����
{
   if( writeByte( bwOut, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   if( WriteInt( bwOut, BitmapNumber ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   return OK;
}
//**********************************************************************
/*class TCTagLink : public TCTag // ��� ��砫� ��뫪�
    {
	 int LinkArtNum;
       public:
	 TCTagLink() { TGCode = TAG_LINKBEGIN; };
	 int ProceedTag(  BufRead& br, BufWrite & bw ) // �-� ��ࠡ�⪨ ����
	 int GetArguments( BufRead & br ); // �⠥� ����� ����
    };*/
int TCTagLink :: GetArguments( BufRead & brIn ) // �⠥� ����� ����
{
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &LinkArtNum ) != 1 ) // ������ ����� ������
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( LinkArtNum == 0 )
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }

// ������ �� '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
// ����� ������ ���� '>'
   if( buffer != TG_END_BULLET )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTagLink :: ProceedTag(  BufRead& br, BufWrite & bw ) // �-� ��ࠡ�⪨ ����
{
// ������ ���� � ����� ����
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
   if( WriteInt( bw, LinkArtNum ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

// ⥯��� �㦭� ���� ����뢠�騩 ���
   TCTag* ptag = NULL;
   do
   {
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
//�஢���� �������� ��� �� �����⨬���� ����� ��뫪�
     if( ptag->TGCode == TAG_KEYWORDBEGIN  ||  ptag->TGCode == TAG_LINKEND )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	 }
//���� �訡��
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_LINKEND);

   if(ptag != NULL) delete ptag;
   return OK;
}

//**********************************************************************
/*class TCTagEndLink : public TCTag // ��� ���� ��뫪�
    {
       public:
	 TCTagEndLink() { TGCode = TAG_LINKEND; };
    }; */
// ��㬥�⮢ ���
// ��ࠡ�⪠ �⠭���⭠� - ������ ����
//**********************************************************************
/*class TCTagKey : public TCTag // ��� ��砫� ���祢��� ᫮��
    {
       public:
	 TCTagKey() { TGCode = TAG_KEYWORDBEGIN; };
	 int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
    };  */
// ��㬥�⮢ ���

int TCTagKey :: ProceedTag(  BufRead& br, BufWrite & bw ) // �-� ��ࠡ�⪨ ����
{
// ������ ����
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

// ⥯��� �㦭� ���� ����뢠�騩 ���
   TCTag* ptag = NULL;
   do
   {
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
//�஢���� �������� ��� �� �����⨬���� ����� ���祢��� ᫮��
     if( (ptag->TGCode == TAG_KEYWORDEND) )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	 }
//���� �訡��
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_KEYWORDEND);

   if(ptag != NULL) delete ptag;
   return OK;
}

//**********************************************************************
/*class TCTagEndKey : public TCTag // ��� ���� ���祢��� ᫮��
    {
       public:
	 TCTagEndKey() { TGCode = TAG_KEYWORDEND; };
    };*/
// ��㬥�⮢ ���
// ��ࠡ�⪠ �⠭���⭠� - ������ ����

//**********************************************************************
/*class TCTagFont : public TCTag // ��� ��砫� �뤥����� ���⮬
    {
	 int FontSize;
	 int FontType;
       public:
	 int ProceedTag(  BufRead& br, BufWrite & bw ); // �-� ��ࠡ�⪨ ����
	 int GetArguments( BufRead & br ); // �⠥� ����� ����
    };   */

// �⠥� ࠧ��� � ��ਡ��� ��⠭����������� ����.
int TCTagFont :: GetArguments( BufRead & brIn )
{
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &FontSize ) != 1 ) // ������ ࠧ���
      {  CurError.ProceedError( ERROR_READING ); return NOK; }
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( ReadIntFromChar( brIn, &FontType ) != 1 ) // ������ ��ਡ�� ����
      {  CurError.ProceedError( ERROR_READING ); return NOK; }

// �஢�ઠ �ࠢ��쭮�� ��������� ������
   if(!(FontType == 0 || FontType == 1 || FontType == 10 || FontType == 11 ||
	FontType == 100 || FontType == 101 || FontType == 110 || FontType == 111) )
	{ CurError.ProceedError(WRONG_TAG); return NOK; }
   if( FontSize < -5 || FontSize > 25 )
	{ CurError.ProceedError(WRONG_TAG); return NOK; }

// ������ �� '>'
   int buffer;
   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NOK; }
   if( readByte( brIn, &buffer ) != 1 )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
// ����� ������ ���� '>'
   if( buffer != TG_END_BULLET )
	{  CurError.ProceedError( WRONG_TAG ); return NOK; }
   return OK;
}

int TCTagFont :: ProceedTag(  BufRead& br, BufWrite & bw ) // �-� ��ࠡ�⪨ ����
{
// ������ ����
   if( writeByte( bw, TGCode ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }
// ������ ��ࠬ��஢
   BYTE fProperty=0;
// FontType == 001, 010, ... 110, 111
   fProperty = ( ((FontSize+FNP_BASE) << 3) | ((FontType/100)<<2) |
	       (((FontType%100)/10)<<1) | (FontType%10) );
// ������ ��ࠬ��஢ ����
   if( writeByte( bw, fProperty ) != 1 )
      {  CurError.ProceedError( ERROR_WRITING ); return NOK; }

// ⥯��� �㦭� ���� ����뢠�騩 ���
   TCTag* ptag = NULL;
   do
   {
     ptag = ProceedUntilTag( br, bw );
     if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
//�஢���� �������� ��� �� �����⨬���� ����� ���祢��� ᫮��
     if( ptag->TGCode == TAG_PARAGRAPH || ptag->TGCode == TAG_ENDOFLINE  ||
	    ptag->TGCode == TAG_FONTEND )
	 {
	   ptag->ProceedTag( br, bw );
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	 }
//���� �訡��
     else
	 { CurError.ProceedError(WRONG_TAG_INCLUDE); return NOK; }
   }
   while(ptag->TGCode != TAG_FONTEND);

   if(ptag != NULL) delete ptag;
   return OK;
}

//**********************************************************************
/*class TCTagEndFont : public TCTag // ��� ���� �뤥����� ���⮬
    {
       public:
	 TCTagFont() { TGCode = TAG_FONTEND; };
    }; */
// ��㬥�⮢ ���
// ��ࠡ�⪠ �⠭���⭠� - ������ ����
//**********************************************************************



//*********************** ���譨� �-�� ***********************************

// �-� �����뢠�� � ��室��� ��⮪ ����� �� �室���� ��⮪�, ���� �� ������
// IG_OPEN_BULLET ('<'). ��⥬ �⠥� char ��� ����, ॣ�������
// ᮮ⢥�����騩 ����� ��⮬�� CTag, �⠥� ᮮ⢥�����騥 ��㬥��� ����
// � �����頥� 㪠��⥫� �� ����.

TCTag* ProceedUntilTag( BufRead & brIn , BufWrite & bwOut )
{
   TCTag * ptag = NULL;
   int buf=0;

   if( readByte( brIn, &buf ) != 1 )   return NULL;

   while ( buf != TG_BEGIN_BULLET )
    {
      if( writeByte( bwOut, buf ) != 1 )   CurError.ProceedError(ERROR_WRITING);
      if( readByte( brIn, &buf ) != 1 )   return NULL;
    }

   if( SeekSpaces( brIn ) != 1)  // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
   if( readByte( brIn, &buf ) != 1 ) // ������ char ��� ����
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }

   if( buf == TG_CLOSE )
    {
      if( readByte( brIn, &buf ) != 1 ) // ������ char ��� ����
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
      switch( buf )
       {
	 case CTG_NAME       :  ptag = new TCTagEndName(); break;
	 case CTG_THEME      :  ptag = new TCTagEndTheme(); break;
	 case CTG_BODY       :  ptag = new TCTagEndBody(); break;
	 case CTG_LINK       :  ptag = new TCTagEndLink(); break;
	 case CTG_KEYWORD    :  ptag = new TCTagEndKey(); break;
	 case CTG_FONT       :  ptag = new TCTagEndFont(); break;
	 default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };
    }

   else
      switch( buf )
       {
	 case CTG_NUMBER     :  ptag = new TCTagNum(); break;
	 case CTG_NAME       :  ptag = new TCTagName(); break;
	 case CTG_THEME      :  ptag = new TCTagTheme(); break;
	 case CTG_BODY       :  ptag = new TCTagBody(); break;
	 case CTG_PARAGRAPH  :  ptag = new TCTagParagraph(); break;
	 case CTG_LINEEND    :  ptag = new TCTagEndOfLine(); break;
	 case CTG_BITMAP     :  ptag = new TCTagBitmap(); break;
	 case CTG_LINK       :  ptag = new TCTagLink(); break;
	 case CTG_KEYWORD    :  ptag = new TCTagKey(); break;
	 case CTG_FONT       :  ptag = new TCTagFont(); break;
	 default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };

// ������� ��ࠬ����
   ptag->GetArguments( brIn );
   if( CurError.code != NONE )   return NULL;
   return ptag;
}

// �-� �����뢠�� � ���� ����� �� �室���� ��⮪�, ���� �� ������
// IG_OPEN_BULLET ('<'). ��⥬ �⠥� char ��� ����, ॣ�������
// ᮮ⢥�����騩 ����� ��⮬�� CTag, �⠥� ᮮ⢥�����騥 ��㬥��� ����
// � �����頥� 㪠��⥫� �� ����.

TCTag* ProceedUntilTag_buf( BufRead & brIn , char* buffer, int buflenth )
{
   TCTag * ptag = NULL;
   int ibuf=0;
   int i=0;
// ���㫨�� �����
   for(int j=0 ; j<buflenth-1 ; buffer[j] = 0, j++ );

   if( readByte( brIn, &ibuf ) != 1 )   return NULL;

   while ( ibuf != TG_BEGIN_BULLET )
    {
      buffer[i] = (char) ibuf;
      if( i++ == buflenth ) CurError.ProceedError( NOT_ENOUPH_BUFFER );
      if( readByte( brIn, &ibuf ) != 1 )   return NULL;
    }

   if( SeekSpaces( brIn ) != 1) // �஬���� �஡���
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }
   if( readByte( brIn, &ibuf ) != 1 ) // ������ char ��� ����
      {  CurError.ProceedError( WRONG_TAG ); return NULL; }

   if( ibuf == TG_CLOSE )
    {
      if( readByte( brIn, &ibuf ) != 1 ) // ������ char ��� ����
	{  CurError.ProceedError( WRONG_TAG ); return NULL; }
      switch( ibuf )
       {
	 case CTG_NAME       :  ptag = new TCTagEndName(); break;
	 case CTG_THEME      :  ptag = new TCTagEndTheme(); break;
	 case CTG_BODY       :  ptag = new TCTagEndBody(); break;
	 case CTG_LINK       :  ptag = new TCTagEndLink(); break;
	 case CTG_KEYWORD    :  ptag = new TCTagEndKey(); break;
	 case CTG_FONT       :  ptag = new TCTagEndFont(); break;
	 default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };
    }

   else
      switch( ibuf )
       {
	 case CTG_NUMBER     :  ptag = new TCTagNum(); break;
	 case CTG_NAME       :  ptag = new TCTagName(); break;
	 case CTG_THEME      :  ptag = new TCTagTheme(); break;
	 case CTG_BODY       :  ptag = new TCTagBody(); break;
	 case CTG_PARAGRAPH  :  ptag = new TCTagParagraph(); break;
	 case CTG_LINEEND    :  ptag = new TCTagEndOfLine(); break;
	 case CTG_BITMAP     :  ptag = new TCTagBitmap(); break;
	 case CTG_LINK       :  ptag = new TCTagLink(); break;
	 case CTG_KEYWORD    :  ptag = new TCTagKey(); break;
	 case CTG_FONT       :  ptag = new TCTagFont(); break;
	 default : CurError.ProceedError( WRONG_TAG ); return NULL;
       };

// ������� ��ࠬ����
   ptag->GetArguments( brIn );
   if( CurError.code != NONE )   return NULL;
   return ptag;
}

//**********************************************************************
