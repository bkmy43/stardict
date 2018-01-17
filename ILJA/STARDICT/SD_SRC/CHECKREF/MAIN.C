/********************************************************************
-- STARDICT Project --
main file Программы CheckRef
Программа проверяет целостность
файлах Stardict
(c) I.Vladimirsky 1997
********************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <dos.h>

#include "defines.h"
#include "rw.h"
#include "errors.h"
#include "chutil.h"

// номера всех существующих объектов
int AllNumbers[ MAX_OBJECTS ];

// Имена исходных файлов
char InNames[MAX_IN_FILES][MAX_FILE_NAME_LEN];
// Буфера для всех исходных файлов
BufRead* pbrIn[MAX_IN_FILES];

// сюда будет занесена информация обо всех произошедших ошибках
TError CurError;

void BeginMessages( void );
void EndMessages( void );
int  OpenFiles( int argc, char* argv[]  );
void CloseInFiles();
// chutil.c

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;
// сообщения user'у
      BeginMessages( );
      if( argc == 1 )
          return OK;

      textcolor(GREEN);

      if( OpenFiles(  argc, argv ) != 1 )
      	 return NOK;

      printf("   * Получение списка существующих статей.\n");
      int s;
      GetAllNumbers( AllNumbers,  &s );

      printf("\n   * Проверка целостности ссылок.\n");
      CheckAllRefs( AllNumbers, s );

      CloseInFiles();

// сообщения user'у
      EndMessages( );

      return 1 ;
  }

//***********************************************************************
// сообщения, выдаваемые user'у в начале работы

void BeginMessages( )
{
      textcolor(WHITE);
      cprintf(
	      "    Данная программа проверяет целостность перекрестных ссылок\n\r"
         "    в текстах статей в файлах данных астрологической энциклопедии\n\r"
         "    STARDICT версии 2.0.\n\r"

	      "   ВЕРСИЯ  1.0  от 15.12.97\n\r\n\r"
	      "   Синтаксис:  \n\r"
         "     CHECKREF <infile1> [<infile2> ... <infileN>]\n\r\n\r");
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

      cprintf( "   Проверка целостности ссылок завершена.\n\r");

      textcolor(LIGHTGRAY);
      cprintf("\n\r");
}
//*************************************************************************
int OpenFiles( int argc, char* argv[] )
{
      filehand fhIn;
      int i;

      if( argc > MAX_IN_FILES + 1 )
              CurError.ProceedError(TOO_MANY_ARGUMENTS);

      for( i=0; i< MAX_IN_FILES; i++ )
       {
         pbrIn[i] = NULL;
         strcpy(InNames[i], "None");
       }

      for( i=0; i < argc-1; i++ )
       {
         fhIn = open( argv[i+1], O_RDONLY | O_BINARY );
         if( fhIn == -1 )     CurError.ProceedError(CAN_NOT_OPEN_IN);
         pbrIn[i] = new BufRead();
         if(pbrIn == NULL)     CurError.ProceedError(NOT_ENOUPH_MEMORY);
         pbrIn[i]->SetFileHandler( fhIn );
         strcpy(InNames[i], argv[i+1]);
       }

      return 1;
}

void CloseInFiles()
{
      for( int i=0; i < MAX_IN_FILES && pbrIn[i] != NULL; i++ )
       {
          close(pbrIn[i]->hand);
          strcpy(InNames[i], "None");
          delete pbrIn[i];
       }
}

