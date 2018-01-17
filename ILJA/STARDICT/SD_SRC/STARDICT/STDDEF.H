/* ***********************************************************************
                               STDDEF.H
    v2.0
    Содержит константы, ассоциированные со всеми ресурсами StarDict.
    (c) Перов В.
*********************************************************************** */

#define   NUM_AR_STRINGS    4
#define   NUM_AO_STRINGS    3
#define   NUM_BK_STRINGS    4

#define   AR_CHOOSEART      0
#define   AR_CHOOSETH       1
#define   AR_SEARCHKEYWORD  2
#define   AR_NOTHING        3

#define   AO_WIDEN          0
#define   AR_CENTER         1
#define   AO_NOTHING        2

#define   BK_FLECK          0
#define   BK_FON            1
#define   BK_LIGHT          2
#define   BK_NOTHING        3

// сообщение для перерисовки всех дочерних окон (шрифт изменился)
#define     wmInvalidate        (WM_USER + 1)
// сообщение для создания нового окна со станьей
#define     wmNewArticle        (WM_USER + 2)
#define     wmMoveBack          (WM_USER + 3)
#define     wmMoveForward       (WM_USER + 4)
#define     wmCloseLogoPlease   (WM_USER + 5)
#define     wmBKChanged         (WM_USER + 6)
// --------------------------------------------------------------------------

#define curStarDict     1101
#define curHand         1102
#define curMSHand       1103

#define icoStarDict     1100
#define icoArticle      1105
#define icoStarDict2    1102
#define icoArticle2     1107
#define icoFrame1       1110
#define icoFrame2       1111
#define icoFrame3       1112
#define icoFrame4       1113
#define icoFrame5       1114
#define icoFrame6       1115

#define mnuMainMenu     400
#define accMainMenu     402
#define accF1           403

#define dlgTheme        2000
#define dlgArticle      2001
#define dlgKeyword      2002
#define dlgChangeLook   2003
#define dlgAbout        2004

#define idListArticles  2005
#define idListThemes    2006
#define idEditTheme     2007
#define idEditArticle   2008

#define idNothingDo     3000
#define idThemeChoose   3001
#define idArtChoose     3002
#define idNeverChange   3003
#define idArtCenter     3004
#define idArtWiden      3005
#define idAfterRun      3006
#define idAfterOpen     3007
#define idShowTips      3009
#define idTxtOwner      3010
#define idBackgrounds   3011
#define idIconID        3012
//#define idHelp          3050

#define bmpThemes       1000
#define bmpAbout        1001
#define bmpExit         1002
#define bmpArticles     1003
#define bmpSearch       1004
#define bmpForeground   1005
#define bmpForw         1006
#define bmpBack         1007
#define bmpDrawing      1011
#define bmpTriangle     1012

#define bmpFon          1040
// 1041
// 1042

#define stringStatus    11
#define stringAbout     12
#define stringError     13
#define stringArtCap    14
#define stringCancel    15
#define stringLinkHere  16
#define stringLinkNew   17
#define stringAR_CHOOSEART      20
#define stringAO_WIDEN  30
#define stringBK_FLECK  40

// *****************************************
//#define cmNewWindow         301
#define cmQuitDict          302

#define cmShowArticleList   311
#define cmShowThemeList     312
#define cmSearchKeyword     313

#define cmShowFonts         321
#define cmChangeLook        322
#define cmMakeEnglish       325
#define cmMakeRussian       326

#define cmAbout             330
#define cmAboutProgrammers  331

#define cmOpenHere          340
#define cmOpenNew           341

