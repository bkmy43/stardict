/********************************************************************
-- STARDICT Project --
main файл программы создания файлов данных RGRAD
программа конвертирует файлы в формате RSTEXT 2.0 в DAT-файлы
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

// режим работы программы
int WorkMode=0;

// Версия dat-файлов
int data_version   = DEFAULT_DATA_VERSION;
int format_version = DEFAULT_FORMAT_VERSION;

// сюда будет занесена информация обо всех произошедших ошибках
TError CurError;

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;
// установка режима работы
      SetWorkMode( argc, argv);

// сообщения user'у
      BeginMessages( );

      if (WorkMode == HELP)     return OK;

      textcolor(GREEN);

//ф-я создает файлы данных если они не существуют
      cprintf("*     Проверка существования dat-файлов. \n\r");
      CreateDat();
// создание bak-файлов
      cprintf("*     Создание bak-файлов \n\r");
      if( CreateBakFiles() != 1 )
       return NOK;
// создание tmp-файлов
      cprintf("**    Создание tmp-файлов \n\r");
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
	         printf( " Чтение статьи номер %d\n"
		              " под названием \"%s\". \n",
			            artr.artnum, artr.artname );
            CurError.num = artr.artnum;
            strcpy( CurError.name, artr.artname);
            artr.Data2Win1251();
   	      printf(" Проверка...\n");
	         if( artr.test() == NOK )   res=NOK;

	         if( res==OK )
	     	    { printf( " Запись...\n\n");  artr.TransferInfo();  }
	         else
             {
		         printf( " В статье обнаружены ошибки. Она не будет записана !\n\n");
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
	           printf( " Чтение темы номер %d\n"
 		                " под названием \"%s\". \n",
			               thr.thnum, thr.thname );

              CurError.num = thr.thnum;
              strcpy( CurError.name, thr.thname);

              thr.thnum+=20000;  // Во внутренем представлении темы нумеруются с 20000
              thr.Data2Win1251( ); // перекодировка в Win1251

              printf(" Проверка...\n");
              if( thr.test() == NOK ) res=NOK;

  	           if(res == OK)
  		          { printf( " Запись...\n\n");  thr.TransferInfo(); }
 	           else
		        printf( " В теме обнаружены ошибки. Она не будет записана !\n\n");
	           CurError.code = NONE;
	        }
       }

      bwData.RenewBuf();
      bwTh.RenewBuf();
      bwIndex.RenewBuf();

// перенумерация keyword'ов
      if( (res = RenumberKeywords()) != OK )
         CurError.ProceedError(res);

// Установка CRC
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
// сообщения user'у
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
// сообщения, выдаваемые user'у в начале работы

void BeginMessages( )
{
      textcolor(WHITE);
      if( WorkMode == HELP )
       {
	     cprintf(
	      "    Данная программа прибавляет информацию о статье или \n\r"
	      "   теме к файлам данных для астрологической энциклопедии \n\r"
	      "   STARDICT версии 2.0. Формат входных файлов - RSTEXT 2.0 \n\r\n\r"
	      "   ВЕРСИЯ  2.0  от 1.12.97\n\r\n\r"
	      "   Синтаксис:        GRAD20 [-t] [-v<num>] <filename>\n\r\n\r"
	      "   Режимы работы:\n\r"
	      "               1. Режим чтения статей:   GRAD20 <filename>\n\r"
	      "               2. Режим чтения тем:      GRAD20 -t <filename>\n\r\n\r"
	      "   Дополнительные ключи:\n\r"
	      "                   -?, -h       Help Screen\n\r"
	      "                   -v<число>   Задает версию dat-файлов\n\r\n\r");
       }

      else
       printf(
	      "    Данная программа прибавляет информацию о статье или \n\r"
	      "   теме к файлам данных для  STARDICT 2.0. \n\r"
	      "   Режим работы: %s. \n\r\n\r", (WorkMode == ART_READ) ? "Чтение статей":"Чтение тем" );
      textcolor(LIGHTGRAY);
      cprintf("\n\r");

}
//***********************************************************************
// сообщения, выдаваемые user'у в конце работы
void EndMessages( )
{
      sound(200); delay(20); nosound();
      sound(500); delay(20); nosound();
      sound(900); delay(20); nosound();

      textcolor(WHITE);

      if( CurError.ErrorRegisted == YES )
	   cprintf(" ВНИМАНИЕ в ходе проверки некоторых статей (тем) были обнаружены ошибки !\n\r"
	     " Информация об этих статьях не была занесена в файлы данных.\n\r"
	     " Но если в других статьях были ссылки на эти статьи, то их вызов\n\r"
	     " в ходе работы программы STARDICT приведет к ошибке !!!\n\r"
	     " Список ошибок см. в файле TROUBLES.LST\n\r\n\r" );

      cprintf( "   Информация из исходного файла занесена в соответ-\n\r"
	      " ствующие файлы данных. Но не стоит забывать о возможных \n\r"
	      " ошибках, которые эта программа не заметила, поэтому \n\r"
	      " необходимо все проверить программой STARDICT.\n\r" );

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


      lseek( index, 0L, SEEK_END );  // перейти в конец
      lseek(  data, 0L, SEEK_END );  // перейти в конец

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

      // если ни один из предыдущих режимов не выбран
      if (WorkMode == 0)    WorkMode=HELP;
}