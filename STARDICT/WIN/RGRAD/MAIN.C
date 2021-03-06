/********************************************************************
-- STARDICT Project --
main 䠩� �ணࠬ�� ᮧ����� 䠩��� ������ RGRAD
�ணࠬ�� ���������� 䠩�� � �ଠ� RSTEXT 2.0 � DAT-䠩��
(c) I.Vladimirsky 1997
********************************************************************/

#include "app.h"

filehand index, themes, data, infile, tmp1, tmp2 ;

BufRead brIn;
BufWrite bwIndex, bwTh, bwData;

// ०�� ࠡ��� �ணࠬ��
int WorkMode=0;

// � �㤥� ����ᥭ� ���ଠ�� ��� ��� �ந��襤�� �訡���
TError CurError;

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;
// ��⠭���� ०��� ࠡ���
      if( argc == 2 )
	 WorkMode = ART_READ;
      if( argc == 2 && (strcmp( argv[1],"-h" ) == 0 || strcmp( argv[1],"-H" ) == 0))
	 WorkMode = HELP;
      if( argc == 3 && (strcmp( argv[2],"-t" ) == 0 || strcmp( argv[2],"-T" ) == 0))
	 WorkMode = TH_READ;
      // �᫨ �� ���� �� �।���� ०���� �� ��࠭
      if( WorkMode == 0 )
	    WorkMode = HELP;

// ᮮ�饭�� user'�
      BeginMessages( );

      if (WorkMode == HELP)     return OK;


//�-� ᮧ���� 䠩�� ������ �᫨ ��� �� ��������
      printf("*     �஢�ઠ ����⢮����� dat-䠩���. \n");
      CreateDat();
// ᮧ����� bak-䠩���
      printf("*     ᮧ��� bak-䠩�� \n");
      if( CreateBakFiles() != 1 )
       return NOK;
// ᮧ����� tmp-䠩���
      printf("**    c����� tmp-䠩�� \n");
      if( CreateTmpFiles() != 1 )
       return NOK;

      if( OpenDataFiles(  argc, argv ) != 1 )
	 return NOK;

      if( WorkMode == ART_READ )
       {
	 int res = OK;
	 TArtR artr( &brIn, &bwIndex, &bwTh, &bwData, filelength( data ) );
	 while( (res=artr.ReadInfo()) != STOP_READ  )
	   {
	      printf( "  �⥭�� ���� ����� %d\n"
		      "  ��� ��������� \"%s\". \n",
			 artr.artnum, artr.artname );
	      if( res==OK )
		{ printf( "  ������...\n\n");  artr.TransferInfo();  }
	      else
		printf( "  � ���� �����㦥�� �訡��. ��� �� �㤥� ����ᠭ� !\n\n");
	   }
       }
/*
      else
       {
	 ThemeR thr( infile, &bwTh );
	 int res=0;
	 while( (res=thr.ReadInfo()) != STOP_READ  )
	   {
	      printf( "  �⥭�� ⥬� ����� %d \n"
		      "  ��� ��������� \"%s\". \n",
		      thr.thnum, thr.thname );
	      if( res == OK )
		  { printf( "  ������...\n\n");
		    thr.TransferInfo();  }
	      else
		{  ErrorMessage( res ); exit(0); }
	   }
       }
  */

      bwData.RenewBuf();
      bwTh.RenewBuf();
      bwIndex.RenewBuf();

      close( infile );
      close( index );
      close( themes );
      close( data );

      SaveChanges();
// ᮮ�饭�� user'�
      EndMessages( );

      return 1 ;
  }

//***********************************************************************
// ᮮ�饭��, �뤠����� user'� � ��砫� ࠡ���

void BeginMessages( )
{
      if( WorkMode == HELP )
       {
	 printf( "********************************************************************\n"
	      "    ������ �ணࠬ�� �ਡ����� ���ଠ�� � ���� ��� \n"
	      "   ⥬� � 䠩��� ������ ��� ���஫����᪮� ��横������� \n"
	      "   ��ଠ� �室��� 䠩��� - RSTEXT 2.0 \n"
	      "--------------------------------------------------------------------\n\n"
	      "   ������  2.0  �� 21.09.97\n\n"
	      "   ���������:\n"
	      "    ����� DAT-䠩� ⥯��� ᮤ�ন� ���������.  �� �����㦥���\n"
	      "    �訡�� � ���� �ணࠬ�� �த������ ࠡ���, �� ����ୠ� �����\n"
	      "    �� �������� � 䠩�� ������.\n"
	      "    �������� ��� �����४��� ��⥩ - � 䠩�� TROUBLES.LST.\n\n"
	      "   ����������:\n"
	      "    �⢥��⢥������ �� ��뫪� �� ���������騥 ���� ( � ������\n"
	      "    �뫨 �����㦥�� �訡�� ) ��������� �� ���짮��⥫�.\n\n"
	      "   ���������:   1. ����� �⥭�� ��⥩:   RGRAD FILENAME\n"
	      "                2. ����� �⥭�� ⥬:      RGRAD FILENAME -t\n"
	      "                3. ����� HELP:            RGRAD -h\n "
	      "********************************************************************\n\n");
       }

      else
       printf( "**********************************************************\n"
	      "    ������ �ணࠬ�� �ਡ����� ���ଠ�� � ���� ��� \n"
	      "   ⥬� � 䠩��� ������ ���  STARDICT. \n"
	      "   ����� ࠡ���: %s. \n\n", (WorkMode == ART_READ) ? "�⥭�� ��⥩":"�⥭�� ⥬" );
}
//***********************************************************************
// ᮮ�饭��, �뤠����� user'� � ���� ࠡ���
void EndMessages( )
{
      sound(200); delay(20); nosound();
      sound(500); delay(20); nosound();
      sound(900); delay(20); nosound();

      if( CurError.code != NONE )
	 printf(" �������� � 室� �஢�ન �������� ��⥩ �뫨 �����㦥�� �訡�� !"
	     " ���ଠ�� �� ��� ������ �� �뫠 ����ᥭ� � 䠩�� ������.\n"
	     " �� �᫨ � ��㣨� ������ �뫨 ��뫪� �� �� ����, � �� �맮�\n"
	     " � 室� ࠡ��� �ணࠬ�� STARDICT �ਢ���� � �訡�� !!!\n"
	     " ���᮪ �訡�� �. � 䠩�� TROUBLES.LST\n" );

      printf( "   ���ଠ�� �� ��室���� 䠩�� ����ᥭ� � ᮮ⢥�-\n"
	      " ����騥 䠩�� ������. �� �� �⮨� ���뢠�� � ��������� \n"
	      " �訡���, ����� �� �ணࠬ�� �� ����⨫�, ���⮬� \n"
	      " ����室��� �� �஢���� �ணࠬ��� STARDICT.\n"
	     "**********************************************************\n\n");

}
//*************************************************************************
int OpenDataFiles( int argc, char* argv[] )
{
      if( argc == 1 )     CurError.ProceedError(NO_IN_NAME);

      themes = open( "themes.tmp", O_RDWR | O_BINARY );
      index = open( "index.tmp", O_RDWR | O_BINARY );
      data = open( "data.tmp", O_RDWR | O_BINARY );
      infile = open( argv[1], O_RDWR | O_BINARY );

      if( infile == -1  ) CurError.ProceedError(CAN_NOT_OPEN_IN);
      if( index == -1  )  CurError.ProceedError(CAN_NOT_OPEN_INDEX);
      if( themes == -1  ) CurError.ProceedError(CAN_NOT_OPEN_TH);
      if(  data==-1  )    CurError.ProceedError(CAN_NOT_OPEN_DATA);


      lseek( index, 0L, SEEK_END );  // ��३� � �����
      lseek(  data, 0L, SEEK_END );  // ��३� � �����

      bwIndex.SetFileHandler( index );
      bwData.SetFileHandler( data );
      bwTh.SetFileHandler( themes );
      brIn.SetFileHandler( infile );

      return 1;
}
//*************************************************************************

