#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dos.h>
#include <string.h>
#include <fcntl.h>

#include "errors.h"
void TError :: RegisterError( int acode ) // ф-я заполняет данные соотв. ошибке с данным кодом
{
 code = acode;
 ErrorRegisted = YES;

 switch ( code )
  {
   case CAN_NOT_OPEN_IN      :
      strcpy( UserMessage, "Ошибка при открытии исходного файла");
      ErrorType=CRITICAL;
      break;
   case ERROR_READING   :
      strcpy( UserMessage, "Ошибка при чтении из файла");
      ErrorType=CRITICAL;
      break;
   case NOT_ENOUPH_MEMORY    :
      strcpy( UserMessage, "Недостаточно памяти");
      ErrorType=CRITICAL;
      break;
   case REF_TO_BAD_OBJECT    :
      strcpy( UserMessage, "В тексте есть ссылка на несуществующий объект");
      ErrorType=NONCRITICAL;
      break;
   case NOT_ENOUPH_BUFFER    :
      strcpy( UserMessage, "Буфер переполнен. Вероятна ошибка в синтаксисе тэга");
      ErrorType=NONCRITICAL;
      break;
   case TOO_MANY_ARGUMENTS:
      strcpy( UserMessage, "Слишком много аргументов командной строки");
      ErrorType=CRITICAL;
      break;
   case WRONG_TAG    :
      strcpy( UserMessage, "Ошибка в синтаксисе тэга");
      ErrorType=CRITICAL;
      break;

   default :
      strcpy( UserMessage, "НЕИЗВЕСТНАЯ ОШИБКА");
      ErrorType=CRITICAL;
  };

}
//**********************************************************************
void TError :: ReactError() //ф-я обрабатывает ошибку в зависимости от ее типа.
{
  sound(1000); delay(200); nosound();

// записать в протокол.
  FILE* erlist;
  if( (erlist = fopen( "troubles.lst", "a" )) == NULL )
      {
         _creat("troubles.lst", FA_ARCH | O_BINARY);
         erlist = fopen( "troubles.lst", "a" );
      }

  if( line == 0 )
     fprintf( erlist, "ОБНАРУЖЕНА ОШИБКА: %s.\n", UserMessage );
  else
     fprintf( erlist, "В строке N %d\nОБНАРУЖЕНА ОШИБКА: %s.\n", line, UserMessage );

  fprintf( erlist, "----------------------------------------------------------------------\n\n");
  fclose(erlist);

//выдать сообщение на экран.
  if( ErrorType == NONCRITICAL )
     {
       textcolor(RED);
       if( line == 0 )
         cprintf( "ОШИБКА: %s.\n\r", UserMessage );
       else
         cprintf( "В строке N %d\n\rОШИБКА: %s.\n\r", line, UserMessage );
       textcolor(LIGHTGRAY);
       cprintf( "\n\r");
     }
  else
     {
       textcolor(RED);
       if( line == 0 )
         cprintf( "КРИТИЧЕСКАЯ ОШИБКА: %s.\n\r", UserMessage );
       else
         cprintf( "В строке N %d\n\rКРИТИЧЕСКАЯ ОШИБКА: %s.\n\r", line, UserMessage );

       cprintf( " ПРОГРАММА БУДЕТ ОСТАНОВЛЕНА !!!\n\r");
       textcolor(LIGHTGRAY);
       cprintf( "\n\r");
       exit( code );
     }

}



