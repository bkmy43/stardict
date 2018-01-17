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
	CAN_NOT_OPEN_INDEX,    // не могу открыть index файл
	CAN_NOT_OPEN_TH,       // не могу открыть themes файл
	CAN_NOT_OPEN_DATA,     // не могу открыть data файл
	NO_IN_NAME,            // не указано имя исходного файла
	CAN_NOT_CREATE_TEMP,   // не могу создать temp-файл
	CAN_NOT_OPEN_TEMP,   // не могу открыть temp-файл

	ERROR_IN_INDEX,     // ошибка в файле index.dat
	ERROR_IN_DATA,      // ошибка в файле data.dat
	ERROR_IN_THEMES,    // ошибка в файле themes.dat

        ART_NUMBER_NOT_FOUND,  //не найден номер статьи
	TH_NUMBER_NOT_FOUND,   // номер статьи не найден
	ART_NAME_NOT_FOUND,    // не найдено имя статьи
	TH_NAME_NOT_FOUND,     // не найдено имя темы
	BODY_NOT_FOUND,        // текст статьи не найден
	BODY_END_NOT_FOUND,    // конец текста статьи не найден

	WRONG_ART_NUMBER,      // ошибка в номере статьи
	WRONG_ART_NAME,        // ошибка в имени статьи
	WRONG_TH_NAME,         // ошибка в имени темы

	WRONG_TAG,             // ошибка синтаксиса тэга
	WRONG_TAG_INCLUDE,     // запрещенная вложенность тэгов
	FIRST_LEVEL_TAG_INCLUDE, // вложенность тэгов первого уровня
			       // имя, номер, тема
	FIRST_LEVEL_WRONG_TAG, // на первом уровне найден запрещенный тэг
	WRONG_TAG_IN_TH_MODE,  // запрещенный тэг при чтении тем (не имя и не номер)

	TH_NOT_EXIST,          // темы, упомянутой в статье не существует
	ERROR_READING_TH,      // ошибка при чтении темы
	ERROR_IN_TH,           // ошибка в теме

	NOT_ENOUPH_MEMORY,     // нехватка памяти
	NOT_ENOUPH_BUFFER,     // слишком маленький буфер -> WRONG_TAG
	TH_EXISTS,             // такая тема уже есть
	ART_EXISTS,            // такая статья уже есть

	INVALID_REF,           // неверная ссылка
	REF_TO_BAD_OBJECT,     // объекта ссылки не существует
	SELFREF,                // ссылка на себя

        WRONG_ARGUMENTS,   // неверные аргументы
        CAN_NOT_OPEN_KEYS, // не могу открыть файл со списком ключевых слов
        CAN_NOT_CREATE_OUT // не могу создать Out файл
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
