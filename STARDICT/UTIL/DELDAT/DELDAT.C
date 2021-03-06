#include "deldat.h"

filehand index, data, themes;
filehand nindex, ndata, nthemes;

int OpenDataFiles( void );
int CloseDataFiles( void );
int MakeBak();          // ������� dat-䠩�� � tmp-䠩��
int SaveChanges();      // ������� tmp-䠩�� � dat-䠩��

void main( void )
{
  int num, res;

  if( (res = OpenDataFiles()) != OK )
     { errres( res );  return;  }     // ����� ࠡ���

  printf("********************************************************\n"
	 "   ������ �ணࠬ�� 㭨�⮦��� �����  �. �. 㡨ࠥ� ���\n "
	 " ���ଠ�� � ��� �� 䠩��� � ����묨 ���஫����᪮�\n"
	 " ��横�������.\n\n"
	 " ������ ����� 㤠�塞��� ��ꥪ�\n"
	 "( 0 ��� �����襭�� �ணࠬ��)\n");
  scanf( "%d", &num );
  if( num == 0 )
     return; // ����� ࠡ���

  printf( "*   ᮧ��� bak-䠩��\n" );
  if ( (res=MakeBak()) != OK )
    {  errres( res ); return; }

  if( (res=DelArt( num )) != OK )
    {  errres( res );  return; }

  CloseDataFiles( );

  printf( "**  ��࠭�� ��������� � dat-䠩���\n" );
  if ( (res=SaveChanges()) != OK )
    {  errres( res ); return; }

  printf( "********************************************************\n" );
}
//*************************************************************************
int OpenDataFiles( void )
{
   _fmode = O_BINARY;
   themes = open( "themes.dat", O_RDWR | O_BINARY );
   index = open( "index.dat", O_RDWR | O_BINARY );
   data = open( "data.dat", O_RDWR | O_BINARY );
   nthemes = creat( "themes.tmp", S_IREAD|S_IWRITE );
   nindex = creat( "index.tmp", S_IREAD|S_IWRITE );
   ndata = creat( "data.tmp", S_IREAD|S_IWRITE );

   if( index == -1  || themes==-1  ||  data==-1  )
      return CAN_NOT_OPEN_DATA;

   if( nindex == -1  || nthemes==-1  || ndata==-1 )
      return CAN_NOT_CREATE;

   return OK;
}

int CloseDataFiles( void )
{
   close( themes );
   close( index );
   close( data );
   close( nthemes );
   close( nindex );
   close( ndata );
   return OK;
}
//************************************************************************
int MakeBak()          // ������� dat-䠩�� � tmp-䠩��
{
  if ( copyfile("data.dat", "data.bak" ) != OK ||
       copyfile("index.dat", "index.bak" ) != OK ||
       copyfile("themes.dat", "themes.bak" ) != OK )
    return CAN_NOT_MAKE_BAK;
  return OK;
}

int SaveChanges()      // ������� tmp-䠩�� � dat-䠩��
{
  if ( copyfile("data.tmp", "data.dat" ) != OK ||
       copyfile("index.tmp", "index.dat" ) != OK ||
       copyfile("themes.tmp", "themes.dat" ) != OK )
    return CAN_NOT_SAVE;
  return OK;
}
//*********************************************************************
void errres( int ec )
{
 switch( ec )
  {
   case ART_NOT_FOUND:    printf("������: ����� �� �������\n"); break;
   case CAN_NOT_CREATE:   printf("������: �� ���� ᮧ���� tmp-䠩�\n"); break;
   case CAN_NOT_MAKE_BAK: printf("������: �� ���� ᮧ���� bak-䠩�\n"); break;
   case CAN_NOT_OPEN_DATA:printf("������: �� ���� ������ dat-䠩�\n"); break;
   case CAN_NOT_SAVE     :printf("������: �� ���� ��࠭��� १����\n"); break;
  };
}