/* ***********************************************************************
                               STDDEF.H
    v2.0
    —одержит константы, ассоциированные со всеми ресурсами StarDict.
    (c) ѕеров ¬.
    ƒоговоренность о нумерации:
        коды загружаемых самосто€тельно строк занимают диапазон [1..200]
        команды меню [250..390]
        идентификаторы меню [400..500]
        коды картинок (bitmap'ов), курсоров, иконок и прочее [1000..1500]
*********************************************************************** */

#define icoStarDict     1100
#define curStarDict     1101
#define bmpContents     1000
#define bmpAbout        1001
#define bmpExit         1002

#define stringStatus    1
#define stringAbout     2


#define mnuMainMenu     400

#define cmShowContents  300
#define cmShowOptions   301
#define cmAbout         302
#define cmQuitDict      310
#define cmCloseAll      308
#define cmNewWindow     306
#define cmCascade       307

