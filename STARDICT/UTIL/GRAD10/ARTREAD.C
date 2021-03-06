#include <stdio.h>
#include <string.h>
//#include <stdio.h>

#include "app.h"
extern filehand in, index, themes, data ;
extern WorkMode;

TArtR :: TArtR( FILE* afr, BufWrite* abwIndex, BufWrite* abwTh,
		BufWrite* abwData, long aposition )
{
  fr = afr;
  pbwIndex = abwIndex;
  pbwTh = abwTh;
  pbwData = abwData;
  position = aposition;

  artname = new char[256];
// ���㫥��� ��� 㪠��⥫�� �� ��ப�
  for( int i=0; i<= MAX_LINES; i++ )
      artline[ i ]= NULL ;
  for( i=0; i<256; i++ )
      ths[i] = NULL;
}

TArtR :: ~TArtR( )
{
  int i;
  delete artname;
  for( i=0; i<MAX_LINES; i++ )
   if( artline[i] != NULL )
     {
       delete artline[i];
       artline[i] = 0;
     }
  for( i=0; i<256; i++ )
   if( ths[i] != NULL )
     {
       delete ths[i];
       ths[i] = 0;
     }
}

void TArtR :: ClearAll()
{
// 㤠����� ��� ��ப
 for( int i=0; i< MAX_LINES && artline[i]!=NULL; i++ )
	delete artline[i];
 for( i=0; i<256 && ths[i]!= NULL; i++ )
	delete ths[i];
// ���㫥��� ��� 㪠��⥫�� �� ��ப�
 for( i=0; i< MAX_LINES; i++ )
	artline[ i ]= NULL ;
 for( i=0; i<256; i++ )
	ths[i] = NULL;
// ���㫥��� ��� ����� ��뫮�
 for( i=0 ; i <  256; i++ )
	refTable[i].clear();

 artnum = 0;
 strcpy( artname, "None" );
}

int TArtR :: ReadInfo()
{
 char str[MAX_CHARACTERS_IN_LINE];
 int count = 0;  // ���稪 ������ '#'
 int FirstLine = YES;
 int ec;
 long prevpos; // ��� ����������� �।��饩 ����樨 � 䠩��
 ClearAll();
// ���� ��ப� �� ���� ����
 do
  {
    prevpos = ftell( fr );    // ��������� ���������
    if (fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL) // ����� 䠩��
	if( FirstLine == YES ) // �᫨ �⥭�� ���� ⮫쪮 �� ����
	    return STOP_READ; // � 䠩�� ����� ��� ��⥩
	else
	    break;    // �⥭�� �����祭�
    FirstLine = NO; // ��ࢠ� ��ப� 㦥 ���⠭�
    if(  str[ strlen(str) - 1 ] == '\n' )
	str[ strlen(str) - 1 ] = 0; // ���� \n

    switch( str[0] )
	  { case '#':   if( ++count == 1 )
			  if((ec = ReadNumber( str + 1 )) != OK )
			     return ec;
			if( count == 2 )
			  if((ec = ReadName( str + 1 )) != OK )
			     return ec;
			break;
	    case '@':   ReadTheme( str + 1 ); break;
	    case '$':   if((ec = ReadRef( str + 1 )) != OK )
			     return ec;
			break;
	    case '~':   if((ec = ReadText( str )) != OK )
			     return ec;
			break;
	  };
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

int TArtR :: test()
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

int TArtR :: ReadNumber( char *  str )
{
  artnum = atoi( str );
  if ( artnum == 0 )  return WRONG_ART_NUMBER;
  return OK;
}

int TArtR :: ReadName( char * str )
{
  strcpy( artname, str );
  return OK;
}

int TArtR :: ReadTheme( char * str )
{
  for( int i=0; ths[i] != NULL; i++ ); // ���� ��᫥���� ��������� ⥬�
  ths[i] = new char[ strlen( str ) + 1];
  if( ths[i] == NULL )
     return NOT_ENOUGH_MEMORY;
  strcpy( ths[i], str );
  return OK;
}

int TArtR :: ReadText( char * str )
{
  int lastline = NO; // �� �� �� ��᫥���� ��ப�
  int firstline = YES;

  for( int i=0; str[0] != '^' && lastline != YES; i++)
   {
// �᫨ � ��ࢮ� ��ப� ��� ⥪��
    if( firstline == YES )
     {
      if( strlen(str) == 1 )
       {
	if ((fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL)
	     || str[0] =='#')
	    return TEXT_END_NOT_FOUND;
	}
      else
	str[0] = ' ';
      firstline = NO;
     }

// �᫨ �� ��᫥���� ��ப�
    if( str[ strlen(str) - 1 ] == '^')
       { lastline = YES;
	 str[ strlen(str) - 1 ] = 0;
       }
// ���������� ��ப�
    artline[i] = new char[ strlen( str ) + 1];
    if( artline[i] == NULL )
	 return NOT_ENOUGH_MEMORY;
    strcpy( artline[i], str );
    if ((fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL) || str[0] =='#')
	 return TEXT_END_NOT_FOUND;
// ���� \n
    if(  str[ strlen(str) - 1 ] == '\n' )
	str[ strlen(str) - 1 ] = 0;
   }
  return OK;
}

int TArtR :: ReadRef( char* str )
{
    for( int i=0; refTable[i].code != 0; i++ );  // ���� ��᫥���� ��뫪�
    refTable[i].code = str[0];
    refTable[i].num  = atoi( str + 2 );

    return OK;
}

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
		  return TH_NOT_EXIST;
	     if( :: ReadName( brTh, name ) != 1 )    // ����� ��� ⥬�
		   return ERROR_IN_TH;
// ������ ��� ⥬� �� ����
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		     return ERROR_IN_TH;
	   }
  }
  return OK;
}


#define IN_THE_REFTEXT      0
#define OUT_OF_THE_REFTEXT  1
#define NOT_FOUND           0
#define FOUND               1

int TArtR :: TestRefs1()
// �-� �஢���� �� �� ��뫪� �� ⠡���� ��������� � ⥪��
{
  int ReadMode = OUT_OF_THE_REFTEXT;
  int result = NOT_FOUND;

  for( int k=0; refTable[k].code != 0; k++ )
   { // ��ᬮ���� ���� ��� ����
    ReadMode = OUT_OF_THE_REFTEXT;
    result = NOT_FOUND;
    for( int i=0; artline[i] != NULL && result == NOT_FOUND; i++ )
       for( int j=0; artline[i][j] != 0 && result == NOT_FOUND; j++ )
	   if( artline[i][j] == '&' )
	    {// �᫨ ������� ��砫� ��뫪�
	     if( ReadMode == OUT_OF_THE_REFTEXT )
	      {
		ReadMode = IN_THE_REFTEXT;
		if( artline[i][j+1] == refTable[k].code )
		       result = FOUND;
	      }
	     // �᫨ ������ ����� ��뫪�
	     else ReadMode = OUT_OF_THE_REFTEXT;
	    }

    if(  result == NOT_FOUND )  return BAD_REF_IN_TABLE;
   }
  return OK;
}

int TArtR :: TestRefs2()
// �-� �஢���� �� �� ��뫪� �� ⥪�� ��������� � ⠡���
{
  int ReadMode = OUT_OF_THE_REFTEXT;
  // ��ᬮ���� ���� ��� ����
  for( int i=0; artline[i] != NULL; i++ )
    for( int j=0; artline[i][j] != 0 ; j++ )
	{
	   if( artline[i][j] == '&')
	     {
		if( ReadMode == OUT_OF_THE_REFTEXT )  // �᫨ ������� ��砫� ��뫪�
		 {
		  ReadMode = IN_THE_REFTEXT;
		  // �᪠�� ��� ��뫪� � ⠡���
		  for( int k=0; refTable[k].code != 0; k++ )
		     if( refTable[k].code == artline[i][j+1] )
			break;
		  if(  refTable[k].code == 0 )  return BAD_REF_IN_TEXT;
		 }
		// �᫨ ������ ����� ��뫪�
		else  ReadMode = OUT_OF_THE_REFTEXT;
	     }
	}
  return OK;
}
#define NUMBER_MODE 0
#define NAME_MODE 1

int TArtR :: TestRefs3()
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
}
/*************************************************************************
		     ������ ���������� � ����� ������
*************************************************************************/
int TArtR :: TransferInfo()
{
   int ec;
   TransferIndex();
   if(  (ec = TransferThemes()) != OK )
      { ErrorMessage( ec ); exit(NOK); }
   TransferData();
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
 const ART_HAS_BEEN_WRITTEN		= 1;
 const ART_HAS_NOT_BEEN_WRITTEN      = 0;
 int ind = open( "index.tmp", O_RDWR | O_BINARY );
 _fmode = O_BINARY;
 int tmp = creat("temp.tmp", S_IREAD|S_IWRITE );
 BufRead br;
 BufWrite bw;
 br.SetFileHandler(ind);
 bw.SetFileHandler(tmp);
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
 close( tmp );
 copyfile("temp.tmp", "index.tmp");
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
       return NOT_ENOUGH_MEMORY;

    int ibuf=0;
    char name[256] = {0};
    int i, j, k;

    for( i = 0; ths[i] != NULL; i++ )
     {
// ���� � themes.dat ������ ⥬�
	 while( strcmp( ths[i], name) != 0 ) // ���� ��� �� ᮢ������
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // ����� ����� ⥬�
		{ delete buffer; return TH_NOT_EXIST ; }
	     if( :: ReadName( brTh, name ) != 1 )    // ����� ��� ⥬�
		{ delete buffer; return ERROR_IN_TH; }
// ������ ��� ⥬� �� ����
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		     { delete buffer; return ERROR_IN_TH; }
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
   int i=0;
   int refnum;

   for( i= 0; artline[i] != NULL; i++ )
      WriteCodedString( *pbwData, artline[i] );

   SetTextEnd( *pbwData );

   for( i = 0; refTable[i].code != 0; i++ )
      {
	 pbwData->WriteByte( refTable[i].code );
	 WriteInt( *pbwData, refTable[i].num );
      }

   pbwData->WriteByte( 0xFF );
   WriteInt( *pbwData, artnum );
   return OK;
 }


/************************************************************************
			   ������� ������ ThemeR
************************************************************************/
ThemeR :: ThemeR( FILE* afr, BufWrite* abwTh)
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
 const TH_HAS_BEEN_WRITTEN		= 1;
 const TH_HAS_NOT_BEEN_WRITTEN          = 0;
 int thm = open( "themes.tmp", O_RDWR | O_BINARY );
 _fmode = O_BINARY;
 int tmp = creat("temp.tmp", S_IREAD|S_IWRITE );
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
 copyfile("temp.tmp", "themes.tmp");
 return OK;
}

