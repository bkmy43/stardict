#include "app.h"

FILE* infile;
FILE* outfile;

filehand index, themes, data ;

BufWrite bwIndex, bwTh, bwData;

int WorkMode=0;
int DelFirstString=OFF;
 // ०��, �� ���஬ ��ࢠ� ��ப� 㤠�����. �㦥� ��� ᮢ���⨬��� �
 // ���묨 ��室�묨 䠩���� � ������ � ��ࢮ� ��ப� ��﫮 ��������
 // ����. ����� STARDICT 2.0 ��⮬���᪨ ����⠢��� �� ��������
 // � caption ���� �⮡ࠦ��饣� �����.

unsigned long CurLine=1;

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;

      SetWorkMode( argc, argv );

      if( WorkMode == HELP )
       {
	 printf(
	     "  �ணࠬ�� �믮���� ��४���஢�� ��室��� ⥪�⮢ ���஫����᪮�\n"
	     "  ��横������� Stardict �� �ଠ� STEXT (1.0) � �ଠ� RSTEXT (2.0).\n\n"
	     "   ������       1 �� 15.10.97\n\n"
	     "   ���⠪��:   CONV220 [-t] [-f] <infile> <outfile>\n\n"
	     "   ��ࠬ����:   -t  ०�� �⥭�� ⥬\n"
	     "                -f  ०�� 㤠����� ��ࢮ� ��ப� ⥪�� ����\n\n");
	 return OK;
       }

      printf( "��४���஢�� ��室��� ⥪�⮢ ���஫����᪮� ��横������� Stardict\n"
	      "�� �ଠ� STEXT (1.0) � �ଠ� RSTEXT (2.0).\n\n"
	      "   ������       1.0 �� 1.12.97\n\n");
      printf( "   ����� ������  %s\n\n", (WorkMode == ART_READ) ? "�⥭�� ��⥩.":"�⥭�� ⥬.");
      if( OpenDataFiles(  argc, argv ) != 1 )
	 return NOK;

      if( WorkMode == ART_READ )
       {
	 int res = 0;
	 TArtR artr( infile, outfile );
	 while( (res = artr.ReadInfo()) != STOP_READ  )
           {
              printf( "  �⥭�� ���� ����� %d\n"
                      "  ��� ��������� \"%s\". \n",
                         artr.artnum, artr.artname );
              if( res==OK )
                {
                   printf( "  �訡�� �� �����㦥��...\n");
                   printf( "  ������...\n\n");

                   artr.WriteInfo();
		}
              else
                {
                   ErrorMessage( res );
                   FErrorMessage( res, artr.artnum, artr.artname );
                }
           }
       }
      else
       {
         ThemeR thr( infile, outfile );
         int res=0;
         while( (res=thr.ReadInfo()) != STOP_READ  )
	   {
              printf( "  �⥭�� ⥬� ����� %d \n"
                      "  ��� ��������� \"%s\". \n",
                      thr.thnum, thr.thname );

              printf( "  �஢�ઠ...\n");
              if( res == OK )
                {
                   printf( "  �訡�� �� �����㦥��...\n");
                   printf( "  ������...\n\n");
                   thr.WriteInfo();
		}
              else
                {
                   ErrorMessage( res );
                   FErrorMessage( res, thr.thnum, thr.thname );
                }
           }
       }
      bwData.RenewBuf();
      bwTh.RenewBuf();
      bwIndex.RenewBuf();
      fclose( infile);
      fclose( outfile);
      close( index );
      close( themes );
      close( data );

 //     SaveChanges();

      printf("��������� �����襭�... ��࠭⨩ ���. :-)\n\n");
      if( WorkMode == ART_READ )
      printf("���᮪ �訡�� �. � 䠩�� TROUBLES.LST\n");
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

      infile = fopen( argv[argc-2], "r");
      if( infile == NULL  )
       { ErrorMessage(CAN_NOT_OPEN_IN); return 0; }

      if (  _creat(argv[argc-1],FA_ARCH | O_BINARY) == -1 )
            { ErrorMessage(CAN_NOT_CREATE_OUT); return 0; }
      outfile = fopen( argv[argc-1], "w");

      return 1;
}
//*************************************************************************
void BuildErrorMessage( int ec, char* dest )
{
 switch ( ec )
  {
   case CAN_NOT_OPEN_IN      : strcpy( dest, "�訡�� �� ����⨨ ��室���� 䠩��"); break;
   case CAN_NOT_OPEN_OUT     : strcpy( dest, "�訡�� �� ����⨨ out-䠩��. ��������� ����."); break;
   case CAN_NOT_CREATE_OUT   : strcpy( dest, "�訡�� �� ᮧ����� out-䠩��"); break;
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
       "�� ��ࠡ�⪥ %d ��ப�.\n"
		 "���������� ������: %s.\n"
		  "----------------------------------------------------------------------\n\n",
		   artnum, artname, CurLine, mes );
  fclose(erlist);
}
//*************************************************************************

void SetWorkMode(int argc, char* argv[])
{
      if( argc >= 3 )
	 WorkMode = ART_READ;

      if( argc == 2 && (strcmp( argv[1],"-h" ) == 0 || strcmp( argv[1],"-?" ) == 0
		    || strcmp( argv[1],"-H" ) == 0))
	 WorkMode = HELP;

      for(int i=1; i<argc-2; i++)
       {
	 if( strcmp( argv[i],"-t" ) == 0 || strcmp( argv[i],"-T" ) == 0)
	    WorkMode = TH_READ;
	 if( strcmp( argv[i],"-f" ) == 0 || strcmp( argv[i],"-F" ) == 0)
	    DelFirstString=ON; // ०�� �ࢥન CRC  dat-䠩���
       }

      // �᫨ �� ���� �� �।���� ०���� �� ��࠭
      if (WorkMode == 0)    WorkMode=HELP;
}
