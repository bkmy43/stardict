/********************************************************************
-- STARDICT Project --
main file Программы SetKeys.
Программа расставляет тэги ключевых слов  в исходных
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
#include "keys.h"

filehand fhIn, fhOut, fhKeys;

BufRead brIn;
BufWrite bwOut;

// сюда будет занесена информация обо всех произошедших ошибках
TError CurError;

void BeginMessages( void );
void EndMessages( void );
int  OpenFiles( int argc, char* argv[]  );

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

      TKeysList* pkeys = new TKeysList();

/*      SetKeywordInText("Привет это просто test и ничего более...\
       самый обычный test\n просто test. Вы не думайте, что это что-то\
       кроме test'а", "test" ); */

      char* buffer=NULL;
      buffer = new char[ MAX_BODY_SIZE ];
      if( buffer == NULL ) CurError.ProceedError( NOT_ENOUPH_MEMORY );

      char* BodyWithKeys=NULL;
      int i,j=1;

      while( ReadUntilBodyBegin( brIn , buffer, MAX_BODY_SIZE ) == OK )
       {
         printf( "   Обрабатываю %d статью\n", j++ );
         for( i=0; i<=MAX_BODY_SIZE && buffer[i]!=0; i++)
             bwOut.WriteByte(buffer[i]);
         ReadUntilBodyEnd( brIn , buffer, MAX_BODY_SIZE );

         pkeys->SeekKeyfileToBegin();  // отмотать файл кейвордов к началу
         while( pkeys->BuildList() == OK )  // читать следующий набор keyword'ов
          {
            for( i=0; i< pkeys->GetItemsNum(); i++ )
              {
                char* keyword = pkeys->GetNthStr(0); // основная форма
                char* keyform = pkeys->GetNthStr(i); // дополнительная форма
                BodyWithKeys = SetKeywordInText( buffer, keyword, keyform );
                strcpy( buffer, BodyWithKeys );
                delete BodyWithKeys;
              }
            pkeys->ClearList();
          }
         // записать тело статьи с расставленными keyword'ами
         for( i=0; i<=MAX_BODY_SIZE && buffer[i]!=0; i++)
             bwOut.WriteByte(buffer[i]);
       }

      if( buffer!=NULL ) delete buffer;
      brIn.RenewBuf();
      bwOut.RenewBuf();

      close( fhIn );
      close( fhOut );
      close( fhKeys );

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
	      "    Данная программа расставляет тэги ключевых слов в текстах\n\r"
              "   статей в файлах данных астрологической энциклопедии\n\r"
              "   STARDICT версии 2.0.\n\r"

	      "   ВЕРСИЯ  1.0  от 1.12.97\n\r\n\r"
	      "   Синтаксис:  \n\r"
              "     SETKEYS <файл_список_ключевых_слов> <infile> <outfile>\n\r\n\r");
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

      cprintf( "   Тэги ключевых слов успешно расставлены в исходном файле\n\r");

      textcolor(LIGHTGRAY);
      cprintf("\n\r");
}
//*************************************************************************
int OpenFiles( int argc, char* argv[] )
{
      if( argc != 4 )     CurError.ProceedError(WRONG_ARGUMENTS);

      fhIn = open( argv[argc-2], O_RDWR | O_BINARY );
      if( fhIn == -1 )     CurError.ProceedError(CAN_NOT_OPEN_IN);
      fhKeys = open( argv[argc-3], O_RDWR | O_BINARY );
      if( fhKeys == -1 )     CurError.ProceedError(CAN_NOT_OPEN_KEYS);
      fhOut = _creat( argv[argc-1], FA_ARCH | O_BINARY);
      if( fhOut == -1 )     CurError.ProceedError(CAN_NOT_CREATE_OUT);

      brIn.SetFileHandler( fhIn );
      bwOut.SetFileHandler( fhOut );

      return 1;
}


