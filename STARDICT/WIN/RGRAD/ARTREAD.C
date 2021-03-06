/********************************************************************
-- STARDICT Project --
�㭪樨 ����ᮢ TArtR � CTag. ��������� ������ ����� �� 䠩�� ������
� �ଠ� RSTEXT, �ࢥ��� �� �ࠢ��쭮��� � ������� �� � DAT 䠩��
(c) I.Vladimirsky 1997
********************************************************************/

#include <stdio.h>
#include <string.h>
//#include <stdio.h>

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
	   isBodyReady = YES;
	   break;

       case TAG_THEMEBEGIN :

	   ptag->ProceedTag( *pbrInput, bwTemp ); //������ ���
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
 return OK;
}

/*int TArtR :: test()
{
  int i;

  if( artnum == 0 )
      return ART_NUMBER_NOT_FOUND;
  if( artnum > 0xF00 )
      return WRONG_ART_NUMBER;
  if( strcmp( artname,"none" ) == 0 )
      return ART_NAME_NOT_FOUND;
  if( artline[0] == NULL )
      return TEXT_NOT_FOUND;
  // �᫨ ⠪�� ����� 㦥 �������
  if( ArtExists( artnum ) == 1 )
      return ART_EXISTS;

  if((i=TestThemes()) != OK )  return i;

  if(WorkMode != ART_READ_NOREFTEST)
   {
     if( (i=TestRefs1()) != OK )   return i;
     if( (i=TestRefs2()) != OK )   return i;
     if( (i=TestRefs3()) != OK )   return i;
   }

  return OK;
}


}
 */
int TArtR :: TestThemes()
// �஢����, �� �� ⥬� ���� ��������
{
  lseek( themes, 0L, SEEK_SET );
  BufRead brTh;
  brTh.SetFileHandler(  themes );
  int ibuf=0;
  char name[256] = {0};

  for( int i = 0; ths[i] != NULL; i++ )
// ���� � themes.dat ������ ⥬�
  {
	lseek( themes, 0L, SEEK_SET );
	brTh.RenewBuf();
	while( strcmp( ths[i], name) != 0 ) // ���� ��� �� ᮢ������
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // ����� ����� ⥬�
		 { CurError.ProceedError( TH_NOT_EXIST ); return NOK; }

	     if( :: ReadName( brTh, name ) != 1 )    // ����� ��� ⥬�
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }

// ������ ��� ⥬� �� ����
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }
	   }
  }
  return OK;
}


#define NUMBER_MODE 0
#define NAME_MODE 1

/*int TArtR :: TestRefs3()
// �-� �஢���� �� �� ��뫪� 㪠�뢠�� �� �������騥 ��ꥪ��
{
 long prevpos; // ��� ����������� �।��饩 ����樨 � 䠩��
 int readmode = NUMBER_MODE;
 char str[MAX_CHARACTERS_IN_LINE];
 for( int k=0; refTable[k].code != 0; k++ )
  {
    // ��뫪� �� ᥡ�
    if( refTable[k].num == artnum )
	return SELFREF;

    // �஢���� ������� �� ⠪�� ��ꥪ�
    if( !(ArtExists( refTable[k].num ) == 1  ||
	  ThExists( refTable[k].num ) == 1) )
       {
	prevpos = ftell( fr );    // ��������� ���������
	fseek( fr, 0L, SEEK_SET);
	readmode = NUMBER_MODE;
	while(1)
	 {
	  if( fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL )
	    { fseek( fr, prevpos, SEEK_SET);
	      return REF_TO_BAD_OBJECT;
	    }
	  if( readmode == NAME_MODE )
	    {
	      readmode = NUMBER_MODE;
	      continue;
	    }
	  if( str[0] == '#' && readmode == NUMBER_MODE )
	   {
	    if( atoi(str+1) == refTable[k].num )
	      break;
	    else
	      readmode = NAME_MODE;
	   }
	 }
	fseek( fr, prevpos, SEEK_SET);
       }
  }
 return OK;
} */

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

 while( ReadInt( br, &num ) == 1 )  // ����� ���� ���⠭
  {
    if( ::ReadName( br, name ) != 1 || ReadLong( br, &pos ) != 1 )
	  return NOK;

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
    lseek( themes, 0L, SEEK_SET );
    BufRead brTh;
    brTh.SetFileHandler(  themes );

    char* buffer = NULL;
    buffer = new char[100*1024];
    if( buffer == NULL )
       { CurError.ProceedError( NOT_ENOUPH_MEMORY ); return NOK; }
    int ibuf=0;
    char name[256] = {0};
    int i, j, k;

    for( i = 0; ths[i] != NULL; i++ )
     {
// ���� � themes.dat ������ ⥬�
	 while( strcmp( ths[i], name) != 0 ) // ���� ��� �� ᮢ������
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // ����� ����� ⥬�
		 { delete buffer; CurError.ProceedError( TH_NOT_EXIST ); return NOK; }
	     if( :: ReadName( brTh, name ) != 1 )    // ����� ��� ⥬�
		 { delete buffer; return NOK; }
// ������ ��� ⥬� �� ����
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		     { delete buffer; return NOK; }
	   }
// �㦭�� ⥬� �������
// ���࠭���� 䠩�� � ⥪�饣� ��������� �� ���� � ��६����� buffer
	 for( j=0; brTh.ReadByte( (int*)(buffer + j) ) == 1; j++ ) ;

	 lseek( themes, -j - 2 , SEEK_CUR );
	 WriteInt( *pbwTh, artnum );   // ������� ����� ����
	 WriteInt( *pbwTh, 0x0FFF );   // ������ ���� ����

	 for( k=0; k<j; k++ )                // ������� ����
	    pbwTh->WriteByte( buffer[k] );   // 䠩� �� ����

	 pbwTh->RenewBuf( );
	 lseek( themes, 0L, SEEK_SET  );  // ��३� � ��砫� 䠩��
	 brTh.RenewBuf();
     }

    delete buffer;
    return OK;
}

int TArtR :: TransferData( )
{
   int refnum;
   BufRead brTmp;
   int ibuf,j;
   int buffer[3]={0};
   int length=0;

   brTmp.SetFileHandler(  tmp1 );

// ⥪�� ���� ᥩ�� ��室���� � brTemp;
// ������� ⥫� ���� � dat-䠩�, ������� ���
   while( readByte(brTmp, &ibuf) == 1 )
    {
     length = codeChar( (char)ibuf, buffer );
     for( j=0; j<length; j++ )
	{
	  if( writeHalf( *pbwData, buffer[j] ) == 0 )
	     CurError.ProceedError( ERROR_WRITING );
	}
    }

   SetTextEnd( *pbwData );
   WriteInt( *pbwData, artnum );

   pbwData->RenewBuf();
   return OK;
 }


/************************************************************************
                           ������� ������ ThemeR
************************************************************************/
/*ThemeR :: ThemeR( FILE* afr, BufWrite* abwTh)
{
 fr = afr;
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
 int count = 0;  // ���稪 ������ '#'
 char str[MAX_CHARACTERS_IN_LINE];
 int ec, FirstLine = YES;
 long prevpos; // ��� ����������� �।��饩 ����樨 � 䠩��
 ClearAll();
// ���� ��ப� �� ���� ����
 do
  {
    prevpos = ftell( fr );    // ��������� ���������
    if (fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL) // ����� 䠩��
	if( FirstLine == YES ) // �᫨ �⥭�� ⥬� ⮫쪮 �� ����
	    return STOP_READ; // � 䠩�� ����� ��� ⥬
	else
	    break;    // �⥭�� �����祭�
    FirstLine = NO; // ��ࢠ� ��ப� 㦥 ���⠭�
    if(  str[ strlen(str) - 1 ] == '\n' )
	str[ strlen(str) - 1 ] = 0; // ���� \n

    if( str[0]  == '#')
      {
	if( ++count == 1 )
	  if( (thnum = atoi( str + 1 )) < 0xF00 || thnum<0 )
	      return WRONG_TH_NUMBER;

	if( count == 2 )
	  strcpy( thname, str+1 );
      }
  }
 while( !( str[0] == '#' && count == 3 )); // �᫨ '#' ����祭� � ��⨩ ࠧ
					  // �. �. �� ��ࢠ� ��ப� ᫥���饩 ����

 if( str[0] == '#' )  // �뫠 ���⠭� "�㦠�" ��ப�
    fseek( fr, prevpos, SEEK_SET);

 if( (ec = test()) != OK )
    return ec;
 else
    return OK;

}

int ThemeR :: test()  // �஢���� �ࠢ��쭮��� ���⠭��� ���ଠ樨
{
  if( thnum == 0 )
     return TH_NUMBER_NOT_FOUND;
  if( strcmp( thname, "None") == 0 )
     return TH_NAME_NOT_FOUND;

  if( ThExists( thnum ) == 1 )
      return TH_EXISTS;

  return OK;
}

int ThemeR :: TransferInfo( )
// ������ � ��䠢�⭮� ���浪�
{
 const TH_HAS_BEEN_WRITTEN              = 1;
 const TH_HAS_NOT_BEEN_WRITTEN          = 0;
 int thm = open( "themes.tmp", O_RDWR | O_BINARY );
 _fmode = O_BINARY;
 int tmp = creat("temp2.tmp", S_IREAD|S_IWRITE );
 BufRead br;
 BufWrite bw;
 br.SetFileHandler(thm);
 bw.SetFileHandler(tmp);
 int num;
 char name[256];
 int  members[256];
 int mode = TH_HAS_NOT_BEEN_WRITTEN;

 while( ReadInt( br, &num ) == 1 )  // ����� ⥬� ���⠭
  {
    // ������ ⥬�
    if( ::ReadName( br, name ) != 1 )
	  return NOK;
    for( int i=0; i<256; i++)  members[i]=0 ;
    for( i=0; ; i++ )
      { if( ReadInt( br, members+i ) != 1 )
	   return NOK;
	if( members[i] == 0x0FFF )
	   {  members[i] = 0;
	      break;
	   }
      }

    if( mode == TH_HAS_NOT_BEEN_WRITTEN ) // ⥬� �� �� �뫠 ����ᠭ�
       if( StrAlphComp(name, thname) == 2 )  // �� ��䠢��� thname < name
	    { // ������ ������ � ��襩 ⥬�
	      // ������ ����� ⥬�
	      WriteInt( bw, thnum );
	      // ������ �������� ⥬�
	      for(i=0; thname[ i ] != 0; i++ )
		    bw.WriteByte( thname[i] );
	      bw.WriteByte( '^' );

	      WriteInt( bw, 0x0FFF );
	      mode = TH_HAS_BEEN_WRITTEN;
            }
    // ������� ���⥭��� ���ଠ��
    if( WriteInt( bw, num ) != 1 )   return NOK;
    for( i=0; name[i] != 0; i++ )
          bw.WriteByte( name[i] );
    bw.WriteByte( '^' );
    for( i=0; members[i] != 0; i++ )
         if( WriteInt( bw, members[i] ) != 1 )   return NOK;
    WriteInt( bw, 0x0FFF );
  }
// � 䠩�� ����� ��� ⥬
 if( mode == TH_HAS_NOT_BEEN_WRITTEN ) // ����� �� �� �뫠 ����ᠭ�
    { // ������ ������ � ��襩 ⥬�
      // ������ ����� ⥬�
      WriteInt(  bw, thnum );
      // ������ �������� ⥬�
      for( int i=0; thname[ i ] != 0; i++ )
            bw.WriteByte( thname[i] );
      bw.WriteByte( '^' );

      WriteInt( bw, 0x0FFF );
      mode = TH_HAS_BEEN_WRITTEN;
    }

 bw.RenewBuf();
 close( thm );
 close( tmp );
 copyfile("temp2.tmp", "themes.tmp");
 return OK;
}

  */