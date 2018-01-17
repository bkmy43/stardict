/********************************************************************
-- STARDICT Project --
Программа CheckRef
Описания функций для проверки целостности ссылок

(c) I.Vladimirsky 1997
********************************************************************/
#include "defines.h"
#include "rw.h"
#include "errors.h"

// Функция заполняет массив, содержащий номера всех существующих статей
void GetAllNumbers( int * AllNumbers, int* size );

// Функция добавляет в массив номера содержащихся в данном файле статей
void GetNumbersFromFile( BufRead* pbrIn, int * AllNumbers, int * size);

// Проверка всех ссылок во всех файлах
void CheckAllRefs( int * AllNumbers, int size );

//Проверка всех ссылок в файле
void CheckRefsInFile( BufRead* pbrIn, int * AllNumbers, int size);
