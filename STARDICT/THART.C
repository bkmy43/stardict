#include "thart.h"

TRef :: TRef( )
 {
   code=0;
   num=0;
 }

/*************************************************************************
			   �����  TArticle
*************************************************************************/

TArticle :: TArticle( BufRead  *Index, BufRead *Data )
 {
    pbrIndex = Index;
    pbrData = Data;
// ���㫥��� ��� 㪠��⥫�� �� ��ப�
    for( int i=0; i< MAX_LINES; i++ )
	artline[ i ]= NULL ;
    artnum = 0;
 }

//************************************************************************
void TArticle :: DelOldInfo()
{
// 㤠����� ��� ��ப
    for( int i=0; i< MAX_LINES && artline[i]!=NULL; i++ )
	delete artline[i];

// ���㫥��� ��� 㪠��⥫�� �� ��ப�
    for( i=0; i< MAX_LINES; i++ )
	artline[ i ]= NULL ;

// ���㫥��� ��� ����� ��뫮�
    for( i=0 ; i <  256; i++ )
	refTable[i].clear();
}

int TArticle :: ChangeArt( int num )
 {
    long position;
    int ec;

    //DelOldInfo(); //�������� !!!  ������ �㦭� �᢮������� �� ���譥� �஢��

    if( (ec = ReadNamePosition( num, &position )) != OK )
	 return ec;

    if( (ec=ReadTextRef( position )) != OK )
	 return ec;
    return OK;
 }
//************************************************************************
TArticle :: ~TArticle(  )
 {
   // DelOldInfo();  // �������� !!! �������� �� �᢮������� ������
 }

//************************************************************************
int TArticle :: ReadNamePosition ( int num, long* position )
// �-� �⠥� �� 䠩�� index.dat �������� ���� � ����
// �� ���஬� �� ⥪�� ����� ���� � 䠩�� data.dat
 {
    BufRead& brIn = *pbrIndex ;
    brIn.bigRew( );

    do
      if( ReadInt( brIn, &artnum ) != 1 || ReadName( brIn, artname ) != 1
	  || ReadLong( brIn, position ) != 1 )
	    return WRONG_INDEXDAT;
    while( artnum != num );

    return OK;
 }

//************************************************************************
int TArticle :: ReadTextRef( long position )
// �-� �⠥� �� 䠩�� data.dat ⥪�� ���� � ��������
// ⠡���� ��뫮�
{
    char buffer[120]={0};
  //--------------------------------------------------------
  // �⥭�� ⥪�� ����
    pbrData->ReachEOF = NO;                // � ��砫� 䠩��  data.dat
    pbrData->wh = 0;
    lseek( pbrData->hand, position, SEEK_SET );
    pbrData->RenewBuf();

    for( int i=0; ReadCodedLine( *pbrData, buffer )==OK; i++ )
      {   // ���࠭���� ���⠭����
	 artline[ i ] = new char[ strlen( buffer ) +1 ];
	 if( artline[ i ] == NULL )
	      gerrmes(NOMEM,"TArticle :: ChangeArt",0,SYSTEM_ERROR);
	 strcpy( artline[ i ] , buffer );
      }
  //--------------------------------------------------------
  // ���������� ⠡���� ��뫮�
  int ibuf1=0, ibuf2=0, ec;
  for( i = 0; i< 256; i++ )
    {
      pbrData->ReadByte( &ibuf1 );
      ReadInt( *pbrData, &ibuf2 );

      if( ibuf1 == 0xFF )
	if( ibuf2 != artnum )
	   return WRONG_DATADAT;
	else
	   return OK;

      refTable[i].code = (char)ibuf1;
      refTable[i].num  = ibuf2;
    }
// ࠧ �� ��諨 �� 横�� -> �� 256 ��뫮� ������
   gerrmes(NOMEM,"TArticle :: ChangeArt",1,SYSTEM_ERROR);
   return NOK;
}

