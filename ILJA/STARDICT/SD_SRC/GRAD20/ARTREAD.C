/********************************************************************
-- STARDICT Project --
�㭪樨 ����ᮢ TArtR � CTag. ��������� ������ ����� �� 䠩�� ������
� �ଠ� RSTEXT, �ࢥ��� �� �ࠢ��쭮��� � ������� �� � DAT 䠩��
(c) I.Vladimirsky 1997
********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "app.h"

extern filehand in, index, themes, data, tmp1, tmp2 ;
extern WorkMode;

extern TError CurError;
TArtR :: TArtR( BufRead* apbrInput, BufWrite* abwIndex, BufWrite* abwTh,
		BufWrite* abwData, long aposition )
{
  pbrInput = apbrInput;
  pbwIndex = abwIndex;
  pbwTh = abwTh;
  pbwData = abwData;

  position = aposition;

  artname = new char[256];

// ���㫥��� ��� ����� ��뫮�
  for( int i=0 ; i <  256; i++ )
	refTable[i] = 0;

// �������� ������ ⥬������ 䠩��
  if ( (tmp1 = _creat("temp1.tmp", FA_ARCH | O_BINARY)) == -1 )
	CurError.ProceedError( CAN_NOT_CREATE_TEMP );
  tmp1 = open( "temp1.tmp", O_RDWR | O_BINARY );
  bwTemp.SetFileHandler(  tmp1 );

  for( i=0; i<256; i++ )
      ths[i] = NULL;

  CurError.code = NONE;
}

TArtR :: ~TArtR( )
{
  int i;
  delete artname;
  bwTemp.RenewBuf();
  for( i=0; i<256; i++ )
   if( ths[i] != NULL )
     {
       delete ths[i];
       ths[i] = 0;
     }
  CurError.code = NONE;
  close( tmp1 );
}

void TArtR :: ClearAll()
{
// 㤠����� ��� ��ப
 for( int i=0; i<256 && ths[i]!= NULL; i++ )
	delete ths[i];
// ���㫥��� ��� 㪠��⥫�� �� ��ப�
 for( i=0; i<256; i++ )
	ths[i] = NULL;
// ���㫥��� ��� ����� ��뫮�
 for( i=0 ; i <  256; i++ )
	refTable[i] = 0;

 artnum = 0;
 strcpy( artname, "None" );

 bwTemp.RenewBuf();

 CurError.code = NONE;

  close( tmp1 );
// �������� ������ ⥬������ 䠩��
  if ( (tmp1 = _creat("temp1.tmp", FA_ARCH | O_BINARY)) == -1 )
	CurError.ProceedError( CAN_NOT_CREATE_TEMP );
  bwTemp.SetFileHandler(  tmp1 );
}

int TArtR :: ReadInfo()
{
 ClearAll();

 int isNumberReady=NO;
 int isNameReady=NO;
 int isBodyReady=NO;

 char buffer[1024];
 int NumberOfThemes = 0 ; // ����쪮 ⥬ ���⥭�

 TCTag* ptag = NULL;
 while( ! (isNumberReady==YES && isNameReady==YES &&  isBodyReady==YES) )
  {
     if (ptag != NULL) { delete ptag; ptag = NULL; }
     ptag = ProceedUntilTag_buf( *pbrInput, buffer, 1024 ); // ���� ���
// �᫨ ��室�� 䠩� �����稫��
     if( ptag == NULL && isNumberReady==NO &&
	       isNameReady==NO &&  isBodyReady==NO )
	      return STOP_READ;

     if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��

     switch ( ptag->TGCode )
      {
       case TAG_NAMEBEGIN :

	   if(isNameReady == YES ) //��� 㦥 �뫮 ���⥭�
	      if( isNumberReady == YES )  CurError.ProceedError(BODY_NOT_FOUND);
	      else                        CurError.ProceedError(ART_NUMBER_NOT_FOUND);

	   ptag->ProceedTag( *pbrInput, bwTemp ); //������ ���
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	   strcpy( artname, ((TCTagName *)ptag)->str );
	   isNameReady = YES;
	   break;

       case TAG_NUMBER :

	   if(isNumberReady == YES ) //����� 㦥 �뫮 ���⥭�
	      if( isNameReady == YES )  CurError.ProceedError(BODY_NOT_FOUND);
	      else                      CurError.ProceedError(ART_NAME_NOT_FOUND);

	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	   artnum = ((TCTagNum*)ptag)->artnum;
	   isNumberReady = YES;
	   break;

       case TAG_BODYBEGIN :

	   if(isBodyReady == YES ) //⥫� 㦥 �뫮 ���⥭�
	      if( isNumberReady == YES )  CurError.ProceedError(ART_NAME_NOT_FOUND);
	      else                        CurError.ProceedError(ART_NUMBER_NOT_FOUND);

	   ptag->ProceedTag( *pbrInput, bwTemp ); //������ ⥫�
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
           // !!!!!!!!!!!!!!!!!!!!!!!!!!!!
           for( int i=0; i< ((TCTagBody*)ptag)->KeywordsNum; i++ )
               if(  AddArtToKeyword(artnum, ((TCTagBody*)ptag)->KeywordsTable[i])
                      == KEYWORD_NOT_EXISTS )
                {
                  CreateNewKeyword(((TCTagBody*)ptag)->KeywordsTable[i]);
                  AddArtToKeyword(artnum, ((TCTagBody*)ptag)->KeywordsTable[i]);
                }

           // !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	   isBodyReady = YES;
	   break;

       case TAG_THEMEBEGIN :

	   ptag->ProceedTag( *pbrInput, bwTemp ); //������ �������� ⥬�
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	   ths[NumberOfThemes] = new char[strlen( ((TCTagTheme*)ptag)->str ) +1 ];
	   strcpy( ths[NumberOfThemes], ((TCTagTheme*)ptag)->str );
	   NumberOfThemes++;
	   break;

       default :
	   CurError.ProceedError(FIRST_LEVEL_WRONG_TAG);
      };
  }

 if (ptag != NULL) { delete ptag; ptag = NULL; }

 bwTemp.RenewBuf();

 if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
 return OK;
}

void TArtR :: Data2Win1251()
{
 Dos2Win1251( artname );

 for( int i=0; ths[i]!=NULL; i++ )
    Dos2Win1251( ths[i] );
}

int TArtR :: test()
{
  int i;

  if( artnum == 0 )
     {  CurError.ProceedError(ART_NUMBER_NOT_FOUND); return NOK; }
  if( artnum > 20000 )
     {  CurError.ProceedError(WRONG_ART_NUMBER); return NOK; }
  if( strcmp( artname,"none" ) == 0 )
     {  CurError.ProceedError(ART_NAME_NOT_FOUND); return NOK; }
  // �᫨ ⠪�� ����� 㦥 �������
  if( ArtExists( artnum ) == 1 )
     {  CurError.ProceedError(ART_EXISTS); return NOK; }

  if( TestThemes() != OK )  return NOK;

  return OK;
}

int TArtR :: TestThemes()
// �஢����, �� �� ⥬� ���� ��������
{
  lseek( themes, 0L, SEEK_SET );
  BufRead brTh;
  brTh.SetFileHandler(  themes );

 // �஢�ઠ ��������a 䠩��
 char filetype;
 ReadFileHeader( brTh, &filetype );
 if(filetype!='T') { CurError.ProceedError(ERROR_IN_THEMES); return NOK; }

  int ibuf=0;
  char name[256] = {0};

  for( int i = 0; ths[i] != NULL; i++ )
// ���� � themes.dat ������ ⥬�
  {
	lseek( themes, 0L, SEEK_SET );
	brTh.RenewBuf();
	ReadFileHeader( brTh, &filetype );
	while( strcmp( ths[i], name) != 0 ) // ���� ��� �� ᮢ������
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // ����� ����� ⥬�
		 { CurError.ProceedError( TH_NOT_EXIST ); return NOK; }

	     if( :: ReadName( brTh, name ) != 1 )    // ����� ��� ⥬�
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }

// ������ ��� ⥬� �� ����
             do
		if( ReadInt( brTh, &ibuf ) != 1 )
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }
	     while(  ibuf != 0 );
	   }
  }
  return OK;
}


#define NUMBER_MODE 0
#define NAME_MODE 1


/*************************************************************************
		     ������ ���������� � ����� ������
*************************************************************************/
int TArtR :: TransferInfo()
{
   if( TransferIndex() != OK )
    { CurError.ProceedError(ERROR_IN_INDEX ); return NOK; }
   if( TransferThemes() != OK )
    { CurError.ProceedError(ERROR_IN_THEMES ); return NOK; }
   if( TransferData() != OK )
    { CurError.ProceedError(ERROR_IN_DATA ); return NOK; }

   pbwIndex->RenewBuf();
   pbwTh->RenewBuf();
   pbwData->RenewBuf();
   position = filelength( data );
   return OK;
}

// �����뢠�� ����� � ���� � index.dat, ��祬 ���� �ᯮ�������� �
// ��䠢�⭮� ���浪�
int TArtR :: TransferIndex( )
{
 const ART_HAS_BEEN_WRITTEN             = 1;
 const ART_HAS_NOT_BEEN_WRITTEN      = 0;
 int ind = open( "index.tmp", O_RDWR | O_BINARY );

 if( ( tmp2 = _creat("temp2.tmp", FA_ARCH | O_BINARY)) == -1 )
    { CurError.ProceedError(CAN_NOT_CREATE_TEMP); return NOK; }

 BufRead br;
 BufWrite bw;
 br.SetFileHandler(ind);
 bw.SetFileHandler(tmp2);
 int num;
 char name[256];
 long pos;
 int mode = ART_HAS_NOT_BEEN_WRITTEN;

 // �஢�ઠ ��������a 䠩��
 char filetype;
 ReadFileHeader( br, &filetype );
 if(filetype!='I') { CurError.ProceedError(ERROR_IN_INDEX); return NOK; }

 // ������ ��������� 䠩�� � ���� 䠩�
 WriteFileHeader( bw, 'I' );

 while( ReadInt( br, &num ) == 1 )  // ����� ���� ���⠭
  {
    if( ::ReadName( br, name ) != 1 || ReadLong( br, &pos ) != 1 )
	  { CurError.ProceedError(ERROR_IN_INDEX); return NOK; }

    if( mode == ART_HAS_NOT_BEEN_WRITTEN ) // ����� �� �� �뫠 ����ᠭ�
       if( StrAlphComp(name, artname) == 2 )  // �� ��䠢��� artname < name
	   { // ������ ������ � ��襩 ����
		// ������ ����� ����
		WriteInt(  bw, artnum );
		// ������ �������� ����
		for(int i=0; artname[ i ] != 0; i++ )
		    bw.WriteByte( artname[i] );
		bw.WriteByte( '^' );
		// ������ ����樨 ���� � 䠩�� ������
		bw.WriteByte( position >> 24 );
		bw.WriteByte( position >> 16 );
		bw.WriteByte( position >> 8 );
		bw.WriteByte( position & 0x000000FF );
		mode = ART_HAS_BEEN_WRITTEN;
	   }
    // ������� ���⥭��� ���ଠ��
    WriteInt(  bw, num );
    for(int i=0; name[ i ] != 0; i++ )
	 bw.WriteByte( name[i] );
    bw.WriteByte( '^' );
    bw.WriteByte( pos >> 24 );
    bw.WriteByte( pos >> 16 );
    bw.WriteByte( pos >> 8 );
    bw.WriteByte( pos & 0x000000FF );
  }
// � 䠩�� ����� ��� ��⥩
 if( mode == ART_HAS_NOT_BEEN_WRITTEN ) // ����� �� �� �뫠 ����ᠭ�
    { // ������ ������ � ��襩 ����
     // ������ ����� ����
	WriteInt(  bw, artnum );
	// ������ �������� ����
	for(int i=0; artname[ i ] != 0; i++ )
	    bw.WriteByte( artname[i] );
	bw.WriteByte( '^' );
	// ������ ����樨 ���� � 䠩�� ������
	bw.WriteByte( position >> 24 );
	bw.WriteByte( position >> 16 );
	bw.WriteByte( position >> 8 );
	bw.WriteByte( position & 0x000000FF );
	mode = ART_HAS_BEEN_WRITTEN;
    }

 bw.RenewBuf();
 close( ind );
 close( tmp2 );
 copyfile("temp2.tmp", "index.tmp");
 return OK;
}

int TArtR :: TransferThemes( )
{
    int res;

    for( int i = 0; ths[i] != NULL; i++ )
      if( (res = AddArtToTheme( artnum, ths[i] )) != OK )
          {  CurError.ProceedError( res ); return NOK; }

    return OK;
}
//**************************************************************************
int TArtR :: TransferData( )
{
   int refnum;
   BufRead brTmp;
   int ibuf,j;
   int buffer[3]={0};
   int length=0;

   bwTemp.RenewBuf();
   close(tmp1);
   tmp1 = open( "temp1.tmp", O_RDWR | O_BINARY );

   brTmp.SetFileHandler(  tmp1 );
// ������� � data.dat ����� ������ ���� � �����.
// ��� ����� 32000 - ����� �ᯮ�짮���� ��� �� �࠭���� ��� ����
   WriteInt( *pbwData, (unsigned)filelength(tmp1) );
// ⥪�� ���� ᥩ�� ��室���� � brTmp;
// ������� ⥫� ���� � dat-䠩�, ������� ���
   while( readByte(brTmp, &ibuf) == 1 )
    {
     char ch=(char)ibuf;
     length = codeChar( ch, buffer );
     for( j=0; j<length; j++ )
	   {
	     if( writeHalf( *pbwData, buffer[j] ) == 0 )
	       CurError.ProceedError( ERROR_WRITING );
	   }
    }

   if (pbwData->wh == 1 )
 // �᫨ ��᫥���� ᨬ��� ����ᠭ � ��ࢮ� �������� ���� ��������� ����
      if( writeHalf( *pbwData, 0 ) == 0 )
            CurError.ProceedError( ERROR_WRITING );

   WriteInt( *pbwData, artnum );

   pbwData->RenewBuf();
   return OK;
 }


/************************************************************************
			   ������� ������ ThemeR
************************************************************************/

ThemeR :: ThemeR( BufRead* apbrInput, BufWrite* abwTh)
{
 pbrInput = apbrInput;
 pbwTh = abwTh;
 thnum = 0;
 thname = new char[256];
 strcpy( thname, "None");
}

ThemeR :: ~ThemeR( )
{
 delete thname;
}

void ThemeR :: ClearAll()
{
 thnum = 0;
 strcpy( thname, "None");
}

int ThemeR :: ReadInfo()      // ��ᯥ���
{
 ClearAll();

 int isNumberReady=NO;
 int isNameReady=NO;

 char buffer[1024];

 TCTag* ptag = NULL;

 while( ! (isNumberReady==YES && isNameReady==YES) )
  {
     if (ptag != NULL) { delete ptag; ptag = NULL; }
     ptag = ProceedUntilTag_buf( *pbrInput, buffer, 1024 ); // ���� ���
// �᫨ ��室�� 䠩� �����稫��
     if( ptag == NULL && isNumberReady==NO &&  isNameReady==NO )
	      return STOP_READ;

     if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��

     switch ( ptag->TGCode )
      {
       case TAG_NUMBER :

	   if(isNumberReady == YES ) //����� 㦥 �� ���⥭�
		CurError.ProceedError(TH_NAME_NOT_FOUND);
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	   thnum = ((TCTagNum*)ptag)->artnum;
	   isNumberReady = YES;
	   break;

       case TAG_NAMEBEGIN :

	   if(isNameReady == YES ) //��� 㦥 �뫮 ���⥭�
		CurError.ProceedError(TH_NUMBER_NOT_FOUND);
	   ptag->ProceedTag( *pbrInput, *pbwTh ); //������ ���
	   if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡��
	   strcpy( thname, ((TCTagName *)ptag)->str );
	   isNameReady = YES;
	   break;

       default :
	   CurError.ProceedError(WRONG_TAG_IN_TH_MODE);
      };
  }

 if (ptag != NULL)     { delete ptag; ptag = NULL; }

 if(CurError.code != NONE )  return NOK; // �᫨ ��ॣ����஢��� �訡�� return NOK;
 return OK;
}

void ThemeR :: Data2Win1251( ) // ��४���஢�� � Win1251
{
  Dos2Win1251( thname );
}

int ThemeR :: test()  // �஢���� �ࠢ��쭮��� ���⠭��� ���ଠ樨
{
  if( ThExists( thnum ) == 1 )
     {  CurError.ProceedError(TH_EXISTS); return NOK; }
  return OK;
}

int ThemeR :: TransferInfo( )
// �����뢠�� ���ଠ�� � ����� ⥬� � 䠩�
// themes.dat �� �⮬ ������ ⥬� � ��䠢�⭮� ���浪�
{
 if ( CreateNewTheme( thnum, thname ) != OK )
     return NOK;

 return OK;
}

