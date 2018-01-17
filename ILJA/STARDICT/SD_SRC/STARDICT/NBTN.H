/* ************************************************************************
                                NBTN.H
    v2.0.
    Содержит описания классов новых органов управления для StarDict.
    Классы:
        TTipButton, кнопка с всплывающей подсказкой,
        TSmartButton, кнопка с легким шрифтом,
        TPositionLists, позиции в списках при выборе темы и статьи,
        TThemeDialog, диалог выбора темы и статьи,
        TAboutDialog,
        TDrawingWindow,
        TChangeLookDialog.
    (c) В.Перов.
************************************************************************ */

#if !defined (_NBTN_H)
#define _NBTN_H

extern int AfterRun;
extern int AfterOpen;
extern int BKGround;
extern BOOL ShowTips;

// -----------------------------------------------------------------
//  Кнопка с улучшенным шрифтом :
// -----------------------------------------------------------------
class TSmartButton : public TButton {

    public :

        TSmartButton(TWindow* parent, int id, LPSTR title,
                     int x, int y, int w, int h, BOOL isDefault = FALSE);

        void SetupWindow();
};

// -----------------------------------------------------------------
//  Кнопка-bitmap с всплывающими подсказками:
// -----------------------------------------------------------------
class TTipButton : public TButtonGadget {

        TExFont* tipFont;
        LPSTR   txtTip;             // текст подсказки.
        int     tipLength;          // длина подсказки в пикс., с учетом 2ух пробелов.
        TBitmap*   ScreenShot;      // фотография экрана при печати подсказки.
        TRect   rShot;              // координаты подсказки на экране.

        void    GetScreenShot(TDC& dc, TRect& r);
        void    ReturnScreenShot();

    public :

        TTipButton(TResId resource, int id, TExFont* tipFont);
        ~TTipButton();

        virtual void MouseEnter(uint, TPoint&);
        virtual void MouseLeave(uint, TPoint&);
        virtual void LButtonDown(uint, TPoint&);
};

// -----------------------------------------------------------------
//  Диалоги выбора темы и статьи:
// -----------------------------------------------------------------
struct TPositionLists {
        int     oldThemePos;
        int     oldArticlePos;
        TPositionLists(int a=0, int b=0)
            { oldThemePos =a; oldArticlePos =b; }
};

class TThemeDialog : public TBaseDialog {

        TPositionLists*     oldPos;
        TThemeList          ThemeList;
        int                 thIndex, arIndex;

        void    FillThemeList();
        void    FillArticleList();

    public :

        TThemeDialog(TWindow*, TResId, TPositionLists*, TExFont*);
        ~TThemeDialog();
        void SetupWindow();

        void CmOk();
        void CmLbnSelChangeTheme();
        void CmEnChangeTheme();

        void CmLbnSelChangeArticle();
        void CmEnChangeArticle();

        DECLARE_RESPONSE_TABLE(TThemeDialog);
};

class TArticleDialog : public TBaseDialog {

        int*  oldPos;
        TArticleList  ArtList;

        void FillArticleList();

    public :

        TArticleDialog(TWindow*, TResId, int*, TExFont*);
        ~TArticleDialog();
        void SetupWindow();

        void CmOk();
        void CmLbnSelChangeArticle();
        void CmEnChangeArticle();

        DECLARE_RESPONSE_TABLE(TArticleDialog);
};

// -----------------------------------------------------------------
//  Диалоги выбора темы и статьи:
// -----------------------------------------------------------------
class TChangeLookDialog : public TBaseDialog {

/*        int*    pAfterRun;
        int*    pAfterOpen;
*/
    public :

        TChangeLookDialog(TWindow*, TResId, TExFont*);
//        ~TChangeLookDialog();
        void SetupWindow();

        void CmOk();
        DECLARE_RESPONSE_TABLE(TChangeLookDialog);
};

class TKeywordDialog : public TBaseDialog {

        TPositionLists*     oldPos;
        TKeywordList        KeywordList;
        int                 kwIndex, arIndex;

        void    FillKeywordList();
        void    FillArticleList();

    public :

        TKeywordDialog(TWindow*, TResId, TPositionLists*, TExFont*);
        ~TKeywordDialog();
        void SetupWindow();

        void CmOk();
        void CmLbnSelChangeTheme();
        void CmEnChangeTheme();

        void CmLbnSelChangeArticle();
        void CmEnChangeArticle();

        DECLARE_RESPONSE_TABLE(TKeywordDialog);
};

class TAboutDialog : public TBaseDialog {

        LPSTR RegNumber;
        LPSTR UserName;
        LPSTR InstallLeft;

        char  OwnerText[300];
        TIcon* fr[6];
        int curFrame, increment;

    public :
        TAboutDialog(TWindow*, TResId, LPSTR RegNumber, LPSTR UserName, LPSTR left, TExFont*);
        ~TAboutDialog();
        void SetupWindow();
        void EvTimer(uint);

        DECLARE_RESPONSE_TABLE(TAboutDialog);
};

class TDrawingWindow : public TWindow {

        TBitmap* dib;
        int dibx, diby;

        void ChangeDIB(TDib* );

    public :

        TDrawingWindow(TWindow* p, HINSTANCE, TModule*);
        ~TDrawingWindow();

        void SetupWindow();

        virtual LPSTR GetClassName() { return "TDrawingWindow"; }
        virtual void GetWindowClass(WNDCLASS&);
        virtual void Paint(TDC& dc, bool erase, TRect& rect);
};

#endif // of #if

