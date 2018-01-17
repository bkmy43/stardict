/* ***********************************************************************
                               STD.H
    v2.0
    Содержит описания классов для базовых окон StarDict.
    Классы:
        TStarDictClient,
        TSDecoratedMDIFrame,
        TStarDictApp.
    (c) В.Перов.
*********************************************************************** */

#if !defined (_STD_H)
#define _STD_H

#define MAX_MENU_LEN        50

#define     txtMainWindowClassName      "StarDictMainWin"

class TStatusBar;
class TControlBar;
class TTextGadget;
class TExFont;
class TDrawingWindow;

extern TModule* resModule;

/* -----------------------------------------------------------------------
 Оконные классы для StarDict :
----------------------------------------------------------------------- */

class TStarDictClient : public TMDIClient { // Окно, обрабатывающее команды меню и пр.

        friend class TArticleWindow;

    protected :

        TBitmap*    pForeground;
        TChooseFontDialog :: TData data;
        BOOL              isWindowOpened;
        TPositionLists    pos, posKeys;
        int               articlePos;
        TColor   ColorLink, ColorKeyword;
        TExFont* pFont; // фонт простого текста в статье
        TExFont* pTitleFont;
        TExFont* pLinkFont;
        TExFont* pKeywordFont;
        HCURSOR     hcUsual, hcHand;
        char            strLinkHere[MAX_MENU_LEN],
                        strLinkNew[MAX_MENU_LEN],
                        strArtCap[MAX_MENU_LEN],
                        strCancel[MAX_MENU_LEN];


        void WriteProfileFont();
        void ReadProfileFont();

        void DeleteAllFonts();
        void DeleteFontsDependable();

        void CreateAllFonts();
        void CreateFontsDependable();

    public :
        TExFont* pTipFont; // фонт подсказки

        TStarDictClient();
        virtual ~TStarDictClient();

        void GetWindowClass(WNDCLASS&);
        void SetupWindow();
        void Paint(TDC&, BOOL, TRect&);
        virtual TArticleWindow* InitChild();
        void TrackRButton(TPoint&);

// *********** Команды меню:

        LPARAM WmNewArticle(WPARAM, LPARAM);
        LPARAM WmBKChanged(WPARAM, LPARAM);
//        void    EvKeyDown(uint key, uint repeatCount, uint flags);

        void CmEsc();
        void CmAbout();
        void CmQuitDict();
        void CmAboutProgrammers();
        void CmShowFonts();
        void CmShowThemeList();
        void CmShowArticleList();
        void CmSearchKeyword();
        void CmChangeLook();
        void CmMakeEnglish();
        void CmMakeRussian();
        void CmMoveBack();
        void CmMoveForward();

        void CeMoveBack(TCommandEnabler&);
        void CeMoveForward(TCommandEnabler&);
        void CeEnablePopup(TCommandEnabler&);
        void CeMakeEnglish(TCommandEnabler&);
        void CeMakeRussian(TCommandEnabler&);

        void EvRButtonDown(uint, TPoint&);

        DECLARE_RESPONSE_TABLE(TStarDictClient);
};

class TSDecoratedMDIFrame : public TDecoratedMDIFrame { // Главное окно программы.

        int isMaximized;
        BOOL fTimerIsOn;
        BOOL OldTips;
        TDrawingWindow* Drawing;
        int curFrame, increment;

        void ShowDrawing();
        void SetCurrentIcon();
    public :
        TIcon* fr[6];

        TSDecoratedMDIFrame(LPCSTR title, TResId id, TStarDictClient& client);
        ~TSDecoratedMDIFrame();

        virtual void SetupWindow();
        LPSTR GetClassName() { return txtMainWindowClassName; };

        void EvSize(uint sizeType, TSize& size);
        void EvTimer(uint);

        DECLARE_RESPONSE_TABLE(TSDecoratedMDIFrame);
};

class TStarDictApp : public TApplication {

        TStarDictClient* stdClient;
        TStatusBar* statusBar;
        TControlBar* controlBar;
        TTextGadget* sbtxtStatus;

        void InitializeBars(TDecoratedMDIFrame*);
        void CheckInstallation();
        void CheckNames();

    public :

        TStarDictApp(LPSTR t);//, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
        ~TStarDictApp();

        void InitInstance();
        void InitMainWindow();
};

#endif // of _STD_H

