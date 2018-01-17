#include "app.h"

FILE* infile;
FILE* outfile;

filehand index, themes, data ;

BufWrite bwIndex, bwTh, bwData;

int WorkMode=0;

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;

      if( argc == 3 )
         WorkMode = ART_READ;
      if( argc == 2 && (strcmp( argv[1],"-h" ) == 0 || strcmp( argv[1],"-H" ) == 0))
         WorkMode = HELP;
      if( argc == 4 && (strcmp( argv[3],"-t" ) == 0 || strcmp( argv[2],"-T" ) == 0))
         WorkMode = TH_READ;
      // если ни один из предыдущих режимов не выбран
      if( WorkMode == 0 )
            WorkMode = HELP;

      if( WorkMode == HELP )
       {
         printf(
             "********************************************************************\n"
             "Перекодировка исходных текстов астрологической энциклопедии Stardict\n"
             "из формата STEXT (1.0) в формат RSTEXT (2.0).\n"
             "--------------------------------------------------------------------\n\n"
             "   ВЕРСИЯ       1 от 10.09.97\n\n"
             "   ПАРАМЕТРЫ:   CONV220 infile outfile [-t]\n\n"
             "********************************************************************\n\n");
         return OK;
       }

      printf( "********************************************************************\n"
              "Перекодировка исходных текстов астрологической энциклопедии Stardict\n"
              "из формата STEXT (1.0) в формат RSTEXT (2.0).\n"
              "--------------------------------------------------------------------\n\n"
              "   ВЕРСИЯ       1 от 10.09.97\n\n");
      printf( "   РЕЖИМ РАБОТЫ  %s\n\n", (WorkMode == ART_READ) ? "чтение статей.":"чтение тем.");
      if( OpenDataFiles(  argc, argv ) != 1 )
         return NOK;

      if( WorkMode == ART_READ )
       {
         int res = 0;
         TArtR artr( infile, outfile );
         while( (res = artr.ReadInfo()) != STOP_READ  )
           {
              printf( "  Чтение статьи номер %d\n"
                      "  под названием \"%s\". \n",
                         artr.artnum, artr.artname );
              if( res==OK )
                {
                   printf( "  Ошибок не обнаружено...\n");
                   printf( "  Запись...\n\n");

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
              printf( "  Чтение темы номер %d \n"
                      "  под названием \"%s\". \n",
                      thr.thnum, thr.thname );
              if( res == OK )
                   printf( "  Проверка...\n\n");
              else
                {  ErrorMessage( res ); exit(0); }
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

      printf("----------------------------------------------------------\n\n"
             "Конвертация завершена... Гарантий нет. :-)\n\n");
      if( WorkMode == ART_READ )
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

      infile = fopen( argv[1], "r");
      if( infile == NULL  )
       { ErrorMessage(CAN_NOT_OPEN_IN); return 0; }

      if (  _creat(argv[2],FA_ARCH | O_BINARY) == -1 )
            { ErrorMessage(CAN_NOT_CREATE_OUT); return 0; }
      outfile = fopen( argv[2], "w");

      return 1;
}
//*************************************************************************
void BuildErrorMessage( int ec, char* dest )
{
 switch ( ec )
  {
   case CAN_NOT_OPEN_IN      : strcpy( dest, "Ошибка при открытии исходного файла"); break;
   case CAN_NOT_OPEN_OUT     : strcpy( dest, "Ошибка при открытии out-файла. Создается новый."); break;
   case CAN_NOT_CREATE_OUT   : strcpy( dest, "Ошибка при создании out-файла"); break;
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


