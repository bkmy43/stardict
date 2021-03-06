/********************************************************************
-- STARDICT Project --
main 䠩� �ணࠬ�� ᮧ����� 䠩��� ������ RGRAD
�ணࠬ�� ���������� 䠩�� � �ଠ� RSTEXT 2.0 � DAT-䠩��
(c) I.Vladimirsky 1997
********************************************************************/
#include <dos.h>
#include "app.h"
#include "defines.h"

extern unsigned _stklen, _heaplen;
unsigned _stklen = 30000;
unsigned _heaplen = 30000;

filehand index, themes, data, keys, infile, tmp1, tmp2 ;

BufRead brIn;
BufWrite bwIndex, bwTh, bwData, bwKeys;

// ०�� ࠡ��� �ணࠬ��
int WorkMode=0;

// ����� dat-䠩���
int data_version   = DEFAULT_DATA_VERSION;
int format_version = DEFAULT_FORMAT_VERSION;

// � �㤥� ����ᥭ� ���ଠ�� ��� ��� �ந��襤�� �訡���
TError CurError;

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;
// ��⠭���� ०��� ࠡ���
      SetWorkMode( argc, argv);

// ᮮ�饭�� user'�
      BeginMessages( );

      if (WorkMode == HELP)     return OK;

      textcolor(GREEN);

//�-� ᮧ���� 䠩�� ������ �᫨ ��� �� ��������
      cprintf("*     �஢�ઠ ����⢮����� dat-䠩���. \n\r");
      CreateDat();
// ᮧ����� bak-䠩���
      cprintf("*     �������� bak-䠩��� \n\r");
      if( CreateBakFiles() != 1 )
       return NOK;
// ᮧ����� tmp-䠩���
      cprintf("**    �������� tmp-䠩��� \n\r");
      if( CreateTmpFiles() != 1 )
       return NOK;

      textcolor(LIGHTGRAY);
      cprintf("\n\r");

      if( OpenDataFiles(  argc, argv ) != 1 )
	      return NOK;

      if( WorkMode == ART_READ )
       {
	      int res = OK;
         TArtR artr( &brIn, &bwIndex, &bwTh, &bwData, filelength( data ) );
         while( (res=artr.ReadInfo()) != STOP_READ  )
	       {
	         printf( " �⥭�� ���� ����� %d\n"
		              " ��� ��������� \"%s\". \n",
			            artr.artnum, artr.artname );
            CurError.num = artr.artnum;
            strcpy( CurError.name, artr.artname);
            artr.Data2Win1251();
   	      printf(" �஢�ઠ...\n");
	         if( artr.test() == NOK )   res=NOK;

	         if( res==OK )
	     	    { printf( " ������...\n\n");  artr.TransferInfo();  }
	         else
             {
		         printf( " � ���� �����㦥�� �訡��. ��� �� �㤥� ����ᠭ� !\n\n");
	            CurError.code = NONE;
	          }
          }
       }

      if( WorkMode == TH_READ )
       {
  	      int res = OK;
	      ThemeR thr( &brIn, &bwTh );
	      while( (res=thr.ReadInfo()) != STOP_READ  )
	        {
	           printf( " �⥭�� ⥬� ����� %d\n"
 		                " ��� ��������� \"%s\". \n",
			               thr.thnum, thr.thname );

              CurError.num = thr.thnum;
              strcpy( CurError.name, thr.thname);

              thr.thnum+=20000;  // �� ����७�� �।�⠢����� ⥬� �㬥������ � 20000
              thr.Data2Win1251( ); // ��४���஢�� � Win1251

              printf(" �஢�ઠ...\n");
              if( thr.test() == NOK ) res=NOK;

  	           if(res == OK)
  		          { printf( " ������...\n\n");  thr.TransferInfo(); }
 	           else
		        printf( " � ⥬� �����㦥�� �訡��. ��� �� �㤥� ����ᠭ� !\n\n");
	           CurError.code = NONE;
	        }
       }

      bwData.RenewBuf();
      bwTh.RenewBuf();
      bwIndex.RenewBuf();

// ��७㬥��� keyword'��
      if( (res = RenumberKeywords()) != OK )
         CurError.ProceedError(res);

// ��⠭���� CRC
      SetCRC( index );
      SetCRC( themes );
      SetCRC( data );
      SetCRC( keys );

      close( infile );
      close( index );
      close( themes );
      close( data );
      close( keys );

      SaveChanges();
// ᮮ�饭�� user'�
      EndMessages( );

      remove("index.tmp");
      remove("data.tmp");
      remove("themes.tmp");
      remove("keyword.tmp");
      remove("temp1.tmp");
      remove("temp2.tmp");

      return 1 ;
  }

//***********************************************************************
// ᮮ�饭��, �뤠����� user'� � ��砫� ࠡ���

void BeginMessages( )
{
      textcolor(WHITE);
      if( WorkMode == HELP )
       {
	     cprintf(
	      "    ������ �ணࠬ�� �ਡ����� ���ଠ�� � ���� ��� \n\r"
	      "   ⥬� � 䠩��� ������ ��� ���஫����᪮� ��横������� \n\r"
	      "   STARDICT ���ᨨ 2.0. ��ଠ� �室��� 䠩��� - RSTEXT 2.0 \n\r\n\r"
	      "   ������  2.0  �� 1.12.97\n\r\n\r"
	      "   ���⠪��:        GRAD20 [-t] [-v<num>] <filename>\n\r\n\r"
	      "   ������ ࠡ���:\n\r"
	      "               1. ����� �⥭�� ��⥩:   GRAD20 <filename>\n\r"
	      "               2. ����� �⥭�� ⥬:      GRAD20 -t <filename>\n\r\n\r"
	      "   �������⥫�� ����:\n\r"
	      "                   -?, -h       Help Screen\n\r"
	      "                   -v<�᫮>   ������ ����� dat-䠩���\n\r\n\r");
       }

      else
       printf(
	      "    ������ �ணࠬ�� �ਡ����� ���ଠ�� � ���� ��� \n\r"
	      "   ⥬� � 䠩��� ������ ���  STARDICT 2.0. \n\r"
	      "   ����� ࠡ���: %s. \n\r\n\r", (WorkMode == ART_READ) ? "�⥭�� ��⥩":"�⥭�� ⥬" );
      textcolor(LIGHTGRAY);
      cprintf("\n\r");

}
//***********************************************************************
// ᮮ�饭��, �뤠����� user'� � ���� ࠡ���
void EndMessages( )
{
      sound(200); delay(20); nosound();
      sound(500); delay(20); nosound();
      sound(900); delay(20); nosound();

      textcolor(WHITE);

      if( CurError.ErrorRegisted == YES )
	   cprintf(" �������� � 室� �஢�ન �������� ��⥩ (⥬) �뫨 �����㦥�� �訡�� !\n\r"
	     " ���ଠ�� �� ��� ������ �� �뫠 ����ᥭ� � 䠩�� ������.\n\r"
	     " �� �᫨ � ��㣨� ������ �뫨 ��뫪� �� �� ����, � �� �맮�\n\r"
	     " � 室� ࠡ��� �ணࠬ�� STARDICT �ਢ���� � �訡�� !!!\n\r"
	     " ���᮪ �訡�� �. � 䠩�� TROUBLES.LST\n\r\n\r" );

      cprintf( "   ���ଠ�� �� ��室���� 䠩�� ����ᥭ� � ᮮ⢥�-\n\r"
	      " ����騥 䠩�� ������. �� �� �⮨� ���뢠�� � ��������� \n\r"
	      " �訡���, ����� �� �ணࠬ�� �� ����⨫�, ���⮬� \n\r"
	      " ����室��� �� �஢���� �ணࠬ��� STARDICT.\n\r" );

      textcolor(LIGHTGRAY);
      cprintf("\n\r");
}

//*************************************************************************
int OpenDataFiles( int argc, char* argv[] )
{
      if( argc == 1 )     CurError.ProceedError(NO_IN_NAME);

      themes = open( "themes.tmp", O_RDWR | O_BINARY );
      index = open( "index.tmp", O_RDWR | O_BINARY );
      data = open( "data.tmp", O_RDWR | O_BINARY );
      keys = open( "keyword.tmp", O_RDWR | O_BINARY );
      infile = open( argv[argc-1], O_RDWR | O_BINARY );

      if( infile == -1 )  CurError.ProceedError(CAN_NOT_OPEN_IN);
      if( index == -1  )  CurError.ProceedError(CAN_NOT_OPEN_INDEX);
      if( themes == -1 )  CurError.ProceedError(CAN_NOT_OPEN_TH);
      if(  data==-1  )    CurError.ProceedError(CAN_NOT_OPEN_DATA);
      if(  keys==-1  )    CurError.ProceedError(CAN_NOT_OPEN_KEY);


      lseek( index, 0L, SEEK_END );  // ��३� � �����
      lseek(  data, 0L, SEEK_END );  // ��३� � �����

      bwIndex.SetFileHandler( index );
      if( filelength(index) == 0)   WriteFileHeader(bwIndex, 'I');

      bwData.SetFileHandler( data );
      if( filelength(data) == 0)    WriteFileHeader(bwData, 'D');

      bwTh.SetFileHandler( themes );
      if( filelength(themes) == 0)  WriteFileHeader(bwTh, 'T');

      bwKeys.SetFileHandler( keys );
      if( filelength(keys) == 0)  WriteFileHeader(bwKeys, 'K');

      brIn.SetFileHandler( infile );

      return 1;
}
//*************************************************************************
void SetWorkMode(int argc, char* argv[])
{
      if( argc >= 2 )
	     WorkMode = ART_READ;

      if( argc == 2 && (strcmp( argv[1],"-h" ) == 0 || strcmp( argv[1],"-?" ) == 0
		    || strcmp( argv[1],"-H" ) == 0))
	     WorkMode = HELP;

      for(int i=1; i<argc-1; i++)
       {
	      if( strcmp( argv[i],"-t" ) == 0 || strcmp( argv[i],"-T" ) == 0)
	         WorkMode = TH_READ;
	      if( argv[i][0] == '-' && (argv[i][1] == 'v' || argv[i][1] == 'V'))
            {
              data_version=atoi( argv[i] + 2 );
              if( data_version <= 0 ) CurError.ProceedError( INVALID_ARGUMENTS );
            }
       }

      // �᫨ �� ���� �� �।���� ०���� �� ��࠭
      if (WorkMode == 0)    WorkMode=HELP;
}