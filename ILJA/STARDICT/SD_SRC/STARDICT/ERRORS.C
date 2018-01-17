/* *************************************************************************
                         ERRORS.C
    Содержит коды ошибок от низкоуровневых функций.
    (c) I.Vladimirsky
************************************************************************* */

case EC_WRONG_INDEXDAT:   e = "Error in index.dat file.";
                          r = "Ошибка в файле index.dat.";
                          break;
case EC_WRONG_DATADAT:    e = "Error in data.dat file.";
                          r = "Ошибка в файле data.dat.";
                          break;

case EC_WRONG_THEMESDAT:  e = "Error in themes.dat file.";
                          r = "Ошибка в файле themes.dat.";
                          break;

case EC_WRONG_KEYWORDDAT: e = "Error in keyword.dat file.";
                          r = "Ошибка в файле keyword.dat.";
                          break;

case EC_WRONG_CRC:        e = "Wrong CRC.";
                          r = "Неверная контрольная сумма.";
                          break;

case EC_CANTOPEN_INDEXDAT: e = "An error occured while opening index.dat file.\n"
                               "Please make sure, that this file is placed\n"
                               "in Stardict work directory.";

                           r = "Произошла ошибка при открытии файла index.dat.\n"
                               "Пожалуйста убедитесь в том, что указанный файл\n"
                               "находится в рабочем каталоге Stardict.";
                           break;

case EC_CANTOPEN_DATADAT:  e = "An error occured while opening data.dat file.\n"
                               "Please make sure, that this file is placed\n" 
                               "in Stardict work directory.";

                           r = "Произошла ошибка при открытии файла data.dat.\n"
                               "Пожалуйста убедитесь в том, что указанный файл\n"
                               "находится в рабочем каталоге Stardict.";
                           break;

case EC_CANTOPEN_THEMESDAT: e ="An error occured while opening themes.dat file.\n"
                               "Please make sure, that this file is placed\n"
                               "in Stardict work directory.";

                            r ="Произошла ошибка при открытии файла themes.dat.\n"
                               "Пожалуйста убедитесь в том, что указанный файл\n"
                               "находится в рабочем каталоге Stardict.";
                            break;

case EC_CANTOPEN_KEYWORDDAT: e="An error occured while opening keyword.dat file.\n"
                               "Please make sure, that this file is placed\n"
                               "in Stardict work directory.";

                             r="Произошла ошибка при открытии файла keyword.dat.\n"
                               "Пожалуйста убедитесь в том, что указанный файл\n"
                               "находится в рабочем каталоге Stardict";
                             break;

case EC_WRONG_DATA_VERSION :  e="Wrong data-file version.";
                              r="Неверная версия файлов-данных.";
                              break;

case EC_WRONG_FORMAT_VERSION: e="Wrong data-file format.";
                              r="Неверный формат файлов-данных.";
                              break;

case EC_NO_MEMORY:            e="Not enough memory.";
                              r="Недостаточно памяти для выполнения операции.";
                              break;

case EC_NOT_ENOUPH_BUF:       e="Buffer too small.";
                              r="Недостаточный размер буфера.";
                              break;

