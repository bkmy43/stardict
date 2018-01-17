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
      // если ни один из предыдущих режимов не выбран
      if( WorkMode == 0 )
            WorkMode = HELP;

      if( WorkMode == HELP )
       {
         printf( "********************************************************************\n"
              "    Данная программа прибавляет информацию о статье или \n"
              "   теме к файлам данных для астрологической энциклопедии \n"
              "--------------------------------------------------------------------\n\n"
	      "   ВЕРСИЯ       10 от 3.03.97\n\n"
              "   УЛУЧШЕНИЯ:\n"
              "    При  создании  DAT-файлов  задается  версия.  При  обнаружении\n"
              "    ошибок   в   статье  программа  продолжает  работу.  Названия\n"
              "    всех некорректных статей вносятся в список в файле TROUBLES.LST.\n\n"
              "   НЕДОСТАТКИ:\n"
              "    Ответственность за ссылки на несуществующие статьи ( в которых\n"
              "    были обнаружены ошибки ) возложена на пользователя.\n\n"
              "   ПАРАМЕТРЫ:   1. Режим чтения статей:   GRAD FILENAME\n"
              "                2. Режим чтения статей без тестирования:\n"
              "                                          GRAD FILENAME -notest\n"
              "                3. Режим чтения тем:      GRAD FILENAME -t\n"
              "                4. Режим HELP:            GRAD -h\n "
              "********************************************************************\n\n");
         return OK;
       }


      printf( "**********************************************************\n"
              "    Данная программа прибавляет информацию о статье или \n"
              "   теме к файлам данных для  STARDICT. \n\n");
      //ф-я создает файлы данных если они не существуют
      CreateDat();
      printf("*     создаю bak-файлы \n");
      if( CreateBakFiles() != 1 )
       return NOK;
      printf("**    cоздаю tmp-файлы \n");
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
              printf( "  Чтение статьи номер %d\n"
                      "  под названием \"%s\". \n",
                         artr.artnum, artr.artname );
              if( res==OK )
                  { printf( "  Запись...\n\n");
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
              printf( "  Чтение темы номер %d \n"
                      "  под названием \"%s\". \n",
                      thr.thnum, thr.thname );
              if( res == OK )
                  { printf( "  Запись...\n\n");
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
      printf( "   Информация из исходного файла занесена в соответ-\n"
              " ствующие файлы данных. Но не стоит забывать о возможных \n"
              " ошибках, которые эта программа не заметила, поэтому \n"
              " необходимо все проверить программой STARDICT.\n" );
      if( WorkMode == ART_READ || WorkMode == ART_READ_NOREFTEST )
      printf("Список ошибок см. в файле TROUBLES.LST\n");
      printf("**********************************************************\n\n");
      return 1 ;
  }

//***********************************************************************
int OpenDataFiles( int argc, char* argv[] )
{
      if( argc == 1 )
       {
         printf("ОШИБКА: Не указано имя исходного файла.\n");
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


      lseek( index, 0L, SEEK_END );  // перейти в конец
      lseek(  data, 0L, SEEK_END );  // перейти в конец

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
   case CAN_NOT_OPEN_IN      : strcpy( dest, "Ошибка при открытии исходного файла"); break;
   case CAN_NOT_OPEN_INDEX   : strcpy( dest, "Ошибка при открытии файла index.tmp");  break;
   case CAN_NOT_OPEN_TH      : strcpy( dest, "Ошибка при открытии файла themes.tmp"); break;
   case CAN_NOT_OPEN_DATA    : strcpy( dest, "Ошибка при открытии файла data.tmp"); break;
   case READING_ERROR        : strcpy( dest, "Ошибка при чтении из исходного файла"); break;
   case ART_NUMBER_NOT_FOUND : strcpy( dest, "В исходном файле нет номера статьи"); break;
   case ART_NAME_NOT_FOUND   : strcpy( dest, "В исходном файле нет названия статьи"); break;
   case WRONG_ART_NAME       : strcpy( dest, "Недопустимое название статьи"); break;
   case WRONG_TH_NAME        : strcpy( dest, "Недопустимое название темы"); break;
   case WRONG_ART_NUMBER     : strcpy( dest, "Недопустимый номер статьи"); break;
   case TH_NOT_EXIST         : strcpy( dest, "В файле тем нет такой темы"); break;
   case ERROR_READING_TH     : strcpy( dest, "при чтении файла тем"); break;
   case NOT_ENOUGH_MEMORY    : strcpy( dest, "Недостаточно памяти"); break;
   case ERROR_IN_TH          : strcpy( dest, "Файл тем испорчен"); break;
   case TEXT_NOT_FOUND       : strcpy( dest, "В исходном файле нет текста статьи"); break;
   case TEXT_END_NOT_FOUND   : strcpy( dest, "В исходном файле нет символа конца текста статьи"); break;
   case INVALID_REF          : strcpy( dest, "Недопустимая ссылка"); break;
   case TH_EXISTS            : strcpy( dest, "Тема с таким номером уже существует"); break;
   case ART_EXISTS           : strcpy( dest, "Статья с таким номером уже существует"); break;
   case TH_NUMBER_NOT_F      : strcpy( dest, "Номер темы должен быть больше 3840"); break;
   case TH_NUMBER_NOT_FOUND  : strcpy( dest, "В исходном файле нет номера темы"); break;
   case TH_NAME_NOT_FOUND    : strcpy( dest, "В исходном файле нет названия темы"); break;
   case WRONG_TH_NUMBER      : strcpy( dest, "Недопустимый номер темы"); break;
   case BAD_REF_IN_TABLE     : strcpy( dest,  "В таблице ссылок есть ссылка, которая не встречается в тексте"); break;
   case BAD_REF_IN_TEXT      : strcpy( dest, "В тексте есть ссылка, которой нет в таблице ссылок"); break;
   case REF_TO_BAD_OBJECT    : strcpy( dest, "В тексте есть ссылка на несуществующий объект");break;
   case SELFREF              : strcpy( dest, "В тексте есть ссылка на себя"); break;

   default :                   strcpy( dest, "НЕИЗВЕСТНАЯ ОШИБКА");
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
       printf( "ОШИБКА: %s.\n\n",mes );
  else
     {
       printf( "КРИТИЧЕСКАЯ ОШИБКА: %s.\n",mes );
       printf( "ПРОГРАММА БУДЕТ ОСТАНОВЛЕНА !!!\n");
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

  fprintf( erlist, "В статье номер %d\n"
                 "Под названием: \"%s\"\n"
                 "ОБНАРУЖЕНА ОШИБКА: %s.\n"
                  "----------------------------------------------------------------------\n\n",
                   artnum, artname, mes );
  fclose(erlist);
}
//*************************************************************************


