/********************************************************************
-- STARDICT Project --
Описание класса TError и коди ошибок, возникающих в ходе работы
программы RGRAD.
(c) I.Vladimirsky 1997
********************************************************************/
#if !defined _ERRORS_H
#define _ERRORS_H

#include <conio.h>
#define CRITICAL            1
#define NONCRITICAL         0

#define YES  1
#define  NO  0

enum {
        NONE = 0,                  // ошибок нет

        ERROR_READING,
        ERROR_WRITING,

        CAN_NOT_OPEN_IN,       // не могу открыть входной файл

        NOT_ENOUPH_MEMORY,     // нехватка памяти
        NOT_ENOUPH_BUFFER,     // слишком маленький буфер

        REF_TO_BAD_OBJECT,     // объекта ссылки не существует
        WRONG_TAG,             // объекта в синтаксисе
        TOO_MANY_ARGUMENTS, // слишком много исходных файлов
 };

class TError
  {
     public:
       int code;
       int ErrorRegisted; // YES/NO - была ли зафиксирована ошибка
       int ErrorType; // критическая ли эта ошибка.
                      // Нужно ли прекращать работу программы при ее появлении
       char UserMessage[256]; // какое сообщение выдаваемое пользователю
       int line; // строка, в которой обнаружена ошибка

     public:
       TError( ) {code=NONE; ErrorType=NONCRITICAL; ErrorRegisted=NO; };
       void RegisterError( int code ); // ф-я заполняет данные соотв. ошибке с данным кодом
       void ReactError(); //ф-я обрабатывает ошибку в зависимости от ее типа.
       void ProceedError(int code) { RegisterError(code); ReactError(); };
  };

#endif
