#include "app.h"

FILE* infile;
filehand index, themes, data ;

BufWrite bwIndex, bwTh, bwData;

int WorkMode=0;

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;

      if( argc == 2 )
         WorkMode = ART_READ;
      if( argc == 2 && (strcmp( argv[1],"-h" ) == 0 || strcmp( argv[1],"-H" ) == 0))
         WorkMode = HELP;
      if( argc == 3 && (strcmp( argv[2],"-t" ) == 0 || strcmp( argv[2],"-T" ) == 0))
         WorkMode = TH_READ;
      if( argc == 3 && (strcmp( argv[2],"-notest" ) == 0 || strcmp( argv[2],"-NOTEST" ) == 0))
         WorkMode = ART_READ_NOREFTEST;
      // �᫨ �� ���� �� �।���� ०���� �� ��࠭
      if( WorkMode == 0 )
            WorkMode = HELP;

      if( WorkMode == HELP )
       {
         printf( "********************************************************************\n"
              "    ������ �ணࠬ�� �ਡ����� ���ଠ�� � ���� ��� \n"
              "   ⥬� � 䠩��� ������ ��� ���஫����᪮� ��横������� \n"
              "--------------------------------------------------------------------\n\n"
	      "   ������       10 �� 3.03.97\n\n"
              "   ���������:\n"
              "    ��  ᮧ�����  DAT-䠩���  ��������  �����.  ��  �����㦥���\n"
              "    �訡��   �   ����  �ணࠬ��  �த������  ࠡ���.  ��������\n"
              "    ��� �����४��� ��⥩ �������� � ᯨ᮪ � 䠩�� TROUBLES.LST.\n\n"
              "   ����������:\n"
              "    �⢥��⢥������ �� ��뫪� �� ���������騥 ���� ( � ������\n"
              "    �뫨 �����㦥�� �訡�� ) ��������� �� ���짮��⥫�.\n\n"
              "   ���������:   1. ����� �⥭�� ��⥩:   GRAD FILENAME\n"
              "                2. ����� �⥭�� ��⥩ ��� ���஢����:\n"
              "                                          GRAD FILENAME -notest\n"
              "                3. ����� �⥭�� ⥬:      GRAD FILENAME -t\n"
              "                4. ����� HELP:            GRAD -h\n "
              "********************************************************************\n\n");
         return OK;
       }


      printf( "**********************************************************\n"
              "    ������ �ணࠬ�� �ਡ����� ���ଠ�� � ���� ��� \n"
              "   ⥬� � 䠩��� ������ ���  STARDICT. \n\n");
      //�-� ᮧ���� 䠩�� ������ �᫨ ��� �� ��������
      CreateDat();
      printf("*     ᮧ��� bak-䠩�� \n");
      if( CreateBakFiles() != 1 )
       return NOK;
      printf("**    c����� tmp-䠩�� \n");
      if( CreateTmpFiles() != 1 )
       return NOK;

      if( OpenDataFiles(  argc, argv ) != 1 )
         return NOK;

      if( WorkMode == ART_READ || WorkMode == ART_READ_NOREFTEST )
       {
         int res = 0;
         TArtR artr( infile, &bwIndex, &bwTh, &bwData, filelength( data ) );
         while( (res = artr.ReadInfo()) != STOP_READ  )
           {
              printf( "  �⥭�� ���� ����� %d\n"
                      "  ��� ��������� \"%s\". \n",
                         artr.artnum, artr.artname );
              if( res==OK )
                  { printf( "  ������...\n\n");
                    artr.TransferInfo();  }
              else
                {
                   ErrorMessage( res );
                   FErrorMessage( res, artr.artnum, artr.artname );
                }
           }
       }
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
      bwData.RenewBuf();
      bwTh.RenewBuf();
      bwIndex.RenewBuf();
      fclose( infile);
      close( index );
      close( themes );
      close( data );

      SaveChanges();
      printf( "   ���ଠ�� �� ��室���� 䠩�� ����ᥭ� � ᮮ⢥�-\n"
              " ����騥 䠩�� ������. �� �� �⮨� ���뢠�� � ��������� \n"
              " �訡���, ����� �� �ணࠬ�� �� ����⨫�, ���⮬� \n"
              " ����室��� �� �஢���� �ணࠬ��� STARDICT.\n" );
      if( WorkMode == ART_READ || WorkMode == ART_READ_NOREFTEST )
      printf("���᮪ �訡�� �. � 䠩�� TROUBLES.LST\n");
      printf("**********************************************************\n\n");
      return 1 ;
  }

//***********************************************************************
int OpenDataFiles( int argc, char* argv[] )
{
      if( argc == 1 )
       {
         printf("������: �� 㪠���� ��� ��室���� 䠩��.\n");
         return 0;
       }

      themes = open( "themes.tmp", O_RDWR | O_BINARY );
      index = open( "index.tmp", O_RDWR | O_BINARY );
      data = open( "data.tmp", O_RDWR | O_BINARY );
      infile = fopen( argv[1], "r");
      if( infile == NULL  )
       { ErrorMessage(CAN_NOT_OPEN_IN); return 0; }

      if( index == -1  )
       { ErrorMessage(CAN_NOT_OPEN_INDEX); return 0; }

      if(  themes==-1  )
       { ErrorMessage(CAN_NOT_OPEN_TH); return 0; }

      if(  data==-1  )
       { ErrorMessage(CAN_NOT_OPEN_DATA); return 0; }


      lseek( index, 0L, SEEK_END );  // ��३� � �����
      lseek(  data, 0L, SEEK_END );  // ��३� � �����

      bwIndex.SetFileHandler( index );
      bwData.SetFileHandler( data );
      bwTh.SetFileHandler( themes );
      return 1;
}
//*************************************************************************
void BuildErrorMessage( int ec, char* dest )
{
 switch ( ec )
  {
   case CAN_NOT_OPEN_IN      : strcpy( dest, "�訡�� �� ����⨨ ��室���� 䠩��"); break;
   case CAN_NOT_OPEN_INDEX   : strcpy( dest, "�訡�� �� ����⨨ 䠩�� index.tmp");  break;
   case CAN_NOT_OPEN_TH      : strcpy( dest, "�訡�� �� ����⨨ 䠩�� themes.tmp"); break;
   case CAN_NOT_OPEN_DATA    : strcpy( dest, "�訡�� �� ����⨨ 䠩�� data.tmp"); break;
   case READING_ERROR        : strcpy( dest, "�訡�� �� �⥭�� �� ��室���� 䠩��"); break;
   case ART_NUMBER_NOT_FOUND : strcpy( dest, "� ��室��� 䠩�� ��� ����� ����"); break;
   case ART_NAME_NOT_FOUND   : strcpy( dest, "� ��室��� 䠩�� ��� �������� ����"); break;
   case WRONG_ART_NAME       : strcpy( dest, "�������⨬�� �������� ����"); break;
   case WRONG_TH_NAME        : strcpy( dest, "�������⨬�� �������� ⥬�"); break;
   case WRONG_ART_NUMBER     : strcpy( dest, "�������⨬� ����� ����"); break;
   case TH_NOT_EXIST         : strcpy( dest, "� 䠩�� ⥬ ��� ⠪�� ⥬�"); break;
   case ERROR_READING_TH     : strcpy( dest, "�� �⥭�� 䠩�� ⥬"); break;
   case NOT_ENOUGH_MEMORY    : strcpy( dest, "�������筮 �����"); break;
   case ERROR_IN_TH          : strcpy( dest, "���� ⥬ �ᯮ�祭"); break;
   case TEXT_NOT_FOUND       : strcpy( dest, "� ��室��� 䠩�� ��� ⥪�� ����"); break;
   case TEXT_END_NOT_FOUND   : strcpy( dest, "� ��室��� 䠩�� ��� ᨬ���� ���� ⥪�� ����"); break;
   case INVALID_REF          : strcpy( dest, "�������⨬�� ��뫪�"); break;
   case TH_EXISTS            : strcpy( dest, "���� � ⠪�� ����஬ 㦥 �������"); break;
   case ART_EXISTS           : strcpy( dest, "����� � ⠪�� ����஬ 㦥 �������"); break;
   case TH_NUMBER_NOT_F      : strcpy( dest, "����� ⥬� ������ ���� ����� 3840"); break;
   case TH_NUMBER_NOT_FOUND  : strcpy( dest, "� ��室��� 䠩�� ��� ����� ⥬�"); break;
   case TH_NAME_NOT_FOUND    : strcpy( dest, "� ��室��� 䠩�� ��� �������� ⥬�"); break;
   case WRONG_TH_NUMBER      : strcpy( dest, "�������⨬� ����� ⥬�"); break;
   case BAD_REF_IN_TABLE     : strcpy( dest,  "� ⠡��� ��뫮� ���� ��뫪�, ����� �� ����砥��� � ⥪��"); break;
   case BAD_REF_IN_TEXT      : strcpy( dest, "� ⥪�� ���� ��뫪�, ���ன ��� � ⠡��� ��뫮�"); break;
   case REF_TO_BAD_OBJECT    : strcpy( dest, "� ⥪�� ���� ��뫪� �� ���������騩 ��ꥪ�");break;
   case SELFREF              : strcpy( dest, "� ⥪�� ���� ��뫪� �� ᥡ�"); break;

   default :                   strcpy( dest, "����������� ������");
  };

}
//**********************************************************************
void ErrorMessage( int ec, char* message )
{
  sound(1000); delay(200); nosound();
  if( message != NULL )
   {
       printf("%s", message);
       return;
   }

  int mode;
  char mes[256];
  if( ec==ART_NUMBER_NOT_FOUND || ec== WRONG_ART_NUMBER
      || ec == TH_NUMBER_NOT_F || ec == TH_NUMBER_NOT_FOUND
      || ec == WRONG_TH_NUMBER )
               mode = CRITICAL_ERROR;
  else
               mode = NOT_CRITICAL_ERROR;

  BuildErrorMessage( ec, mes );

  if( mode == NOT_CRITICAL_ERROR )
       printf( "������: %s.\n\n",mes );
  else
     {
       printf( "����������� ������: %s.\n",mes );
       printf( "��������� ����� ����������� !!!\n");
       exit(NOK);
     }
}
//************************************************************************
void FErrorMessage( int ec, int artnum, char* artname )
{
  FILE* erlist;
  if( (erlist = fopen( "troubles.lst", "a" )) == NULL )
      {
	 _creat("troubles.lst", FA_ARCH | O_BINARY);
	 erlist = fopen( "troubles.lst", "a" );
      }

  char mes[256];
  BuildErrorMessage( ec, mes );

  fprintf( erlist, "� ���� ����� %d\n"
                 "��� ���������: \"%s\"\n"
                 "���������� ������: %s.\n"
                  "----------------------------------------------------------------------\n\n",
                   artnum, artname, mes );
  fclose(erlist);
}
//*************************************************************************


