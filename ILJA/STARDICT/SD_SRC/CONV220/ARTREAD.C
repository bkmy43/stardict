#include <stdio.h>
#include <string.h>
//#include <stdio.h>

#include "app.h"

char * ALL_LETTERS = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm����������������������������������㪥�������뢠�஫�����ᬨ���1234567890";
extern filehand in; //, index, themes, data ;
extern WorkMode;
extern DelFirstString;
extern unsigned long CurLine;

TArtR :: TArtR( FILE* afr, FILE* afw )
{
  fr = afr;
  fw = afw;

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
    CurLine++;
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
  if( artnum > 20000 )
      return WRONG_ART_NUMBER;
  if( strcmp( artname,"none" ) == 0 )
      return ART_NAME_NOT_FOUND;
  if( artline[0] == NULL )
      return TEXT_NOT_FOUND;

  if( (i=TestRefs1()) != OK )   return i;
  if( (i=TestRefs2()) != OK )   return i;

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
  LowCase(str); // �-� ���������� �� ᨬ���� �஬� ��ࢮ�� � lower case
  strcpy( artname, str );
  return OK;
}

int TArtR :: ReadTheme( char * str )
{
  for( int i=0; ths[i] != NULL; i++ ); // ���� ��᫥���� ��������� ⥬�
  ths[i] = new char[ strlen( str ) + 1];
  if( ths[i] == NULL )
     return NOT_ENOUGH_MEMORY;

  LowCase(str); // �-� ���������� �� ᨬ���� �஬� ��ࢮ�� � lower case
  strcpy( ths[i], str );
  return OK;
}

int TArtR :: ReadText( char * str )
{
  int lastline = NO; // �� �� �� ��᫥���� ��ப�
  int firstline = YES;
  long prevpos = 0;
  char buf[MAX_CHARACTERS_IN_LINE];
  int a,b;


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

// ��ࠡ���� ��७���
    if(  str[ strlen(str) - 1 ] == '-' )
 	  {
       str[ strlen(str) - 1 ] = 0;
       prevpos = ftell( fr );    // ��������� ���������
       // ������ ᫥������ ��ப�
      if ((fgets( buf, MAX_CHARACTERS_IN_LINE, fr ) == NULL) || buf[0] =='#')
      	 return TEXT_END_NOT_FOUND;

      b = strlen(str);
      for( a=0; IsItLetter( buf[a] ) == 1; a++ )
       {
        str[b] = buf[a];   b++;
       }
      str[b] = 0;
      fseek( fr, prevpos + a, SEEK_SET);
     }
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

int TArtR :: GetRefNumber( char refcode)
{
    for(int i=0; refTable[i].code != 0; i++)
       if( refTable[i].code == refcode )
	   return refTable[i].num;

    return NOK;
}

#define OUT_OF_REF  0
#define INSIDE_REF  1

//�������쭮� ���-�� �஡���� � ��ࢮ� ��ப� ��ࠣ��
#define    PARAGRAPH_SPACE_NUM 3
//�������쭮� ���-�� �஡���� � 業�஢����� ��ப�
#define    CENTERLINE_SPACE_NUM 10


int TArtR :: ConvertLine(int num, int & mode)
{
   char buffer[1024] = {0};
   char buffer2[16] = {0};

   int isChanges = NO;
   int i, j;
   i = j = 0;
// ��⠢�� ���� ��ࠣ��.
   for( ; artline[num][i] == ' '; i++);
// ⥯��� i - ������⢮ �஡���� � ��砫� ��ப�
   if( i >= PARAGRAPH_SPACE_NUM )
     {
  	   isChanges = YES;
	   buffer[j++] =   TG_BEGIN_BULLET;
	   buffer[j++] = CTG_PARAGRAPH;

	   if(i >= CENTERLINE_SPACE_NUM )
	    { buffer[j++] = ' ';
	      buffer[j++] = '1';
	      buffer[j++] = ' ';
	    }

	   buffer[j++] = TG_END_BULLET;
     }

   for( ; artline[num][i] != 0; i++, j++)
    if( artline[num][i] == '&' )
	  {
	   isChanges = YES;
	   if( mode == OUT_OF_REF)
	     {
	       mode = INSIDE_REF;
	       buffer[j] = TG_BEGIN_BULLET;
	       sprintf( buffer2, "%d", GetRefNumber( artline[num][++i]) );
	       buffer[++j]=CTG_LINK;
	       buffer[++j]=0;
	       strcat( buffer, buffer2 );
	       j+=strlen(buffer2);
	       buffer[j]=TG_END_BULLET;
	     }
	   else
	     {
	       buffer[j] =   TG_BEGIN_BULLET;
	       buffer[++j] = TG_CLOSE;
	       buffer[++j] = CTG_LINK;
	       buffer[++j] = TG_END_BULLET;
	       mode = OUT_OF_REF;
	     }

	  }
    else
	   buffer[j] = artline[num][i];

   buffer[j]=0;

   if(isChanges == YES)
     {
       delete artline[num];
       artline[num] = new char[strlen(buffer)+1];
       strcpy(artline[num], buffer);
     }

   return mode;
}
/*************************************************************************
		     ������ ���������� � ����� ������
*************************************************************************/
int TArtR :: WriteInfo()
{
// ������ ����� ����
   WriteCTag(fw, CTG_NUMBER, artnum);
   NewLine(fw);

// ������ ����� ����
   WriteCTag(fw, CTG_NAME);
   fprintf(fw, "%s", artname);
   WriteCTagEnd(fw, CTG_NAME);
   NewLine(fw);

// ������ ⥬ ����
   for(int i=0; ths[i]!=NULL; i++)
    {
      WriteCTag(fw, CTG_THEME);
      fprintf(fw, "%s", ths[i]);
      WriteCTagEnd(fw, CTG_THEME);
      NewLine(fw);
    }

// ������ ⥪�� ����
   NewLine(fw);
   WriteCTag(fw, CTG_BODY);
   int mode = OUT_OF_REF; // � ��砫� ��ࢮ� ��ப� �� ����� ��� ��뫪�
   int k=( DelFirstString==ON ? 1 : 0);
   for(i=k; artline[i] != NULL; i++)
     {
       mode = ConvertLine(i, mode);
       if( i == k )  // �᫨ �� ��ࢠ� ��ப� ⥫�
         if( strlen(artline[i]) == 0 ) // � ��� �����
           continue;                     // �� �����뢠�� ��
       fprintf( fw, "%s\n", artline[i] );
     }
   WriteCTagEnd(fw, CTG_BODY);
   NewLine(fw);

   NewLine(fw);
   return OK;
}

/************************************************************************
			   ������� ������ ThemeR
************************************************************************/
ThemeR :: ThemeR( FILE* afr, FILE* afw)
{
 fr = afr;
 fw = afw;
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

 thnum -= 0xF00; // � RSText 2.0 ��⥬� �㬥�樨 ⥬ ��������
 LowCase(thname); // �-� ���������� �� ᨬ���� �஬� ��ࢮ�� � lower case

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

//  if( ThExists( thnum ) == 1 )
 //     return TH_EXISTS;

  return OK;
}


int ThemeR :: WriteInfo( )
{
// ������ ����� ⥬�
   WriteCTag(fw, CTG_NUMBER, thnum);
   NewLine(fw);

// ������ ����� ⥬�
   WriteCTag(fw, CTG_NAME);
   fprintf(fw, "%s", thname);
   WriteCTagEnd(fw, CTG_NAME);
   NewLine(fw);
   NewLine(fw);

   return OK;
}


int IsItLetter( char ch )
{
  for( int i=0; ALL_LETTERS[i] != 0; i++ )
     if( ALL_LETTERS[i] == ch )
         return 1; //ch - �㪢�

  return 0; //ch - �� �㪢�
}


