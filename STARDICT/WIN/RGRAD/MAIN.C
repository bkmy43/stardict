/********************************************************************
-- STARDICT Project --
main файл программы создания файлов данных RGRAD
программа конвертирует файлы в формате RSTEXT 2.0 в DAT-файлы
(c) I.Vladimirsky 1997
********************************************************************/

#include "app.h"

filehand index, themes, data, infile, tmp1, tmp2 ;

BufRead brIn;
BufWrite bwIndex, bwTh, bwData;

// режим работы программы
int WorkMode=0;

// сюда будет занесена информация обо всех произошедших ошибках
TError CurError;

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;
// установка режима работы
      if( argc == 2 )
	 WorkMode = ART_READ;
      if( argc == 2 && (strcmp( argv[1],"-h" ) == 0 || strcmp( argv[1],"-H" ) == 0))
	 WorkMode = HELP;
      if( argc == 3 && (strcmp( argv[2],"-t" ) == 0 || strcmp( argv[2],"-T" ) == 0))
	 WorkMode = TH_READ;
      // если ни один из предыдущих режимов не выбран
      if( WorkMode == 0 )
	    WorkMode = HELP;

// сообщения user'у
      BeginMessages( );

      if (WorkMode == HELP)     return OK;


//ф-я создает файлы данных если они не существуют
      printf("*     проверка существования dat-файлов. \n");
      CreateDat();
// создание bak-файлов
      printf("*     создаю bak-файлы \n");
      if( CreateBakFiles() != 1 )
       return NOK;
// создание tmp-файлов
      printf("**    cоздаю tmp-файлы \n");
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
	      printf( "  Чтение статьи номер %d\n"
		      "  под названием \"%s\". \n",
			 artr.artnum, artr.artname );
	      if( res==OK )
		{ printf( "  Запись...\n\n");  artr.TransferInfo();  }
	      else
		printf( "  В статье обнаружены ошибки. Она не будет записана !\n\n");
	   }
       }
/*
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
  */

      bwData.RenewBuf();
      bwTh.RenewBuf();
      bwIndex.RenewBuf();

      close( infile );
      close( index );
      close( themes );
      close( data );

      SaveChanges();
// сообщения user'у
      EndMessages( );

      return 1 ;
  }

//***********************************************************************
// сообщения, выдаваемые user'у в начале работы

void BeginMessages( )
{
      if( WorkMode == HELP )
       {
	 printf( "********************************************************************\n"
	      "    Данная программа прибавляет информацию о статье или \n"
	      "   теме к файлам данных для астрологической энциклопедии \n"
	      "   Формат входных файлов - RSTEXT 2.0 \n"
	      "--------------------------------------------------------------------\n\n"
	      "   ВЕРСИЯ  2.0  от 21.09.97\n\n"
	      "   УЛУЧШЕНИЯ:\n"
	      "    Каждый DAT-файл теперь содержит заголовок.  При обнаружении\n"
	      "    ошибок в статье программа продолжает работу, но неверная статья\n"
	      "    не заносится в файлы данных.\n"
	      "    Названия всех некорректных статей - в файле TROUBLES.LST.\n\n"
	      "   НЕДОСТАТКИ:\n"
	      "    Ответственность за ссылки на несуществующие статьи ( в которых\n"
	      "    были обнаружены ошибки ) возложена на пользователя.\n\n"
	      "   ПАРАМЕТРЫ:   1. Режим чтения статей:   RGRAD FILENAME\n"
	      "                2. Режим чтения тем:      RGRAD FILENAME -t\n"
	      "                3. Режим HELP:            RGRAD -h\n "
	      "********************************************************************\n\n");
       }

      else
       printf( "**********************************************************\n"
	      "    Данная программа прибавляет информацию о статье или \n"
	      "   теме к файлам данных для  STARDICT. \n"
	      "   Режим работы: %s. \n\n", (WorkMode == ART_READ) ? "Чтение статей":"Чтение тем" );
}
//***********************************************************************
// сообщения, выдаваемые user'у в конце работы
void EndMessages( )
{
      sound(200); delay(20); nosound();
      sound(500); delay(20); nosound();
      sound(900); delay(20); nosound();

      if( CurError.code != NONE )
	 printf(" ВНИМАНИЕ в ходе проверки некоторых статей были обнаружены ошибки !"
	     " Информация об этих статьях не была занесена в файлы данных.\n"
	     " Но если в других статьях были ссылки на эти статьи, то их вызов\n"
	     " в ходе работы программы STARDICT приведет к ошибке !!!\n"
	     " Список ошибок см. в файле TROUBLES.LST\n" );

      printf( "   Информация из исходного файла занесена в соответ-\n"
	      " ствующие файлы данных. Но не стоит забывать о возможных \n"
	      " ошибках, которые эта программа не заметила, поэтому \n"
	      " необходимо все проверить программой STARDICT.\n"
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


      lseek( index, 0L, SEEK_END );  // перейти в конец
      lseek(  data, 0L, SEEK_END );  // перейти в конец

      bwIndex.SetFileHandler( index );
      bwData.SetFileHandler( data );
      bwTh.SetFileHandler( themes );
      brIn.SetFileHandler( infile );

      return 1;
}
//*************************************************************************

