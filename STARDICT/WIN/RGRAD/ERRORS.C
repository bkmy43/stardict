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

 switch ( code )
  {
   case CAN_NOT_OPEN_IN      :
      strcpy( UserMessage, "Ошибка при открытии исходного файла");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_INDEX   :
      strcpy( UserMessage, "Ошибка при открытии файла index.tmp");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_TH      :
      strcpy( UserMessage, "Ошибка при открытии файла themes.tmp");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_DATA    :
      strcpy( UserMessage, "Ошибка при открытии файла data.tmp");
      ErrorType=CRITICAL;
      break;
   case NO_IN_NAME    :
      strcpy( UserMessage, "Не указано имя исходного файла");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_CREATE_TEMP  :
      strcpy( UserMessage, "Ошибка при создании temp-файла");
      ErrorType=CRITICAL;
      break;
   case ERROR_READING	:
      strcpy( UserMessage, "Ошибка при чтении из файла");
      ErrorType=CRITICAL;
      break;
   case ART_NUMBER_NOT_FOUND :
      strcpy( UserMessage, "В исходном файле нет номера статьи");
      ErrorType=CRITICAL;
      break;
   case ART_NAME_NOT_FOUND   :
      strcpy( UserMessage, "В исходном файле нет названия статьи");
      ErrorType=CRITICAL;
      break;
   case WRONG_ART_NAME       :
      strcpy( UserMessage, "Недопустимое название статьи");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_TH_NAME        :
      strcpy( UserMessage, "Недопустимое название темы");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_ART_NUMBER     :
      strcpy( UserMessage, "Недопустимый номер статьи");
      ErrorType=NONCRITICAL;
      break;
   case TH_NOT_EXIST         :
      strcpy( UserMessage, "В файле тем нет такой темы");
      ErrorType=NONCRITICAL;
      break;
   case ERROR_READING_TH     :
      strcpy( UserMessage, "Ошибка при чтении файла тем");
      ErrorType=CRITICAL;
      break;
   case NOT_ENOUPH_MEMORY    :
      strcpy( UserMessage, "Недостаточно памяти");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_TH          :
      strcpy( UserMessage, "Файл тем испорчен");
      ErrorType=CRITICAL;
      break;
   case BODY_NOT_FOUND       :
      strcpy( UserMessage, "В исходном файле нет текста статьи");
      ErrorType=CRITICAL;
      break;
   case BODY_END_NOT_FOUND   :
      strcpy( UserMessage, "В исходном файле нет символа конца текста статьи");
      ErrorType=CRITICAL;
      break;
   case INVALID_REF          :
      strcpy( UserMessage, "Недопустимая ссылка");
      ErrorType=NONCRITICAL;
      break;
   case TH_EXISTS            :
      strcpy( UserMessage, "Тема с таким номером уже существует");
      ErrorType=NONCRITICAL;
      break;
   case ART_EXISTS           :
      strcpy( UserMessage, "Статья с таким номером уже существует");
      ErrorType=NONCRITICAL;
      break;
   case TH_NUMBER_NOT_FOUND  :
      strcpy( UserMessage, "В исходном файле нет номера темы");
      ErrorType=CRITICAL;
      break;
   case TH_NAME_NOT_FOUND    :
      strcpy( UserMessage, "В исходном файле нет названия темы");
      ErrorType=CRITICAL;
      break;
   case WRONG_TH_NUMBER      :
      strcpy( UserMessage, "Недопустимый номер темы");
      ErrorType=NONCRITICAL;
      break;
   case REF_TO_BAD_OBJECT    :
      strcpy( UserMessage, "В тексте есть ссылка на несуществующий объект");
      ErrorType=NONCRITICAL;
      break;
   case SELFREF              :
      strcpy( UserMessage, "В тексте есть ссылка на себя");
      ErrorType=NONCRITICAL;
      break;
   case NOT_ENOUPH_BUFFER    :
      strcpy( UserMessage, "Буфер переполнен. Вероятна ошибка в синтаксисе тэга");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_TAG    :
      strcpy( UserMessage, "Ошибка в синтаксисе тэга");
      ErrorType=NONCRITICAL;
      break;
   case WRONG_TAG_INCLUDE    :
      strcpy( UserMessage, "Запрещенная вложенность тэгов");
      ErrorType=NONCRITICAL;
      break;
   case FIRST_LEVEL_TAG_INCLUDE    :
      strcpy( UserMessage, "Вложенность тэгов первого уровня (number,name,theme)");
      ErrorType=NONCRITICAL;
      break;
   case FIRST_LEVEL_WRONG_TAG   :
      strcpy( UserMessage, "Обнаружен внутренний тэг на первом уровне");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_INDEX   :
      strcpy( UserMessage, "Ошибка в файле index.dat");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_DATA   :
      strcpy( UserMessage, "Ошибка в файле data.dat");
      ErrorType=CRITICAL;
      break;
   case ERROR_IN_THEMES   :
      strcpy( UserMessage, "Ошибка в файле themes.dat");
      ErrorType=CRITICAL;
      break;
   case CAN_NOT_OPEN_TEMP   :
      strcpy( UserMessage, "Ошибка при открытии temp-файла");
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
  fprintf( erlist, "В статье номер %d\n"
		 "Под названием: \"%s\"\n"
		 "В строке: %d"
		 "ОБНАРУЖЕНА ОШИБКА: %s.\n"
		  "----------------------------------------------------------------------\n\n",
		   /*artnum, artname,line, */ 0,"",0, UserMessage );
  fclose(erlist);

//выдать сообщение на экран.
  if( ErrorType == NONCRITICAL )
       printf( "В строке N %d\n ОШИБКА: %s.\n\n", line, UserMessage );
  else
     {
       printf( " В строке N %d\n КРИТИЧЕСКАЯ ОШИБКА: %s.\n", line, UserMessage );
       printf( " ПРОГРАММА БУДЕТ ОСТАНОВЛЕНА !!!\n\n");
       exit( code );
     }

}



