/* ************************************************************************
                                NBTN.H
    v2.0.
    �������� �������� ������� ����� ������� ���������� ��� StarDict.
    ������:
        TTipButton, ������ � ����������� ����������,
        TSmartButton, ������ � ������ �������,
        TPositionLists, ������� � ������� ��� ������ ���� � ������,
        TThemeDialog, ������ ������ ���� � ������,
        TAboutDialog,
        TDrawingWindow,
        TChangeLookDialog.
    (c) �.�����.
************************************************************************ */

#if !defined (_NBTN_H)
#define _NBTN_H

extern int AfterRun;
extern int AfterOpen;
extern int BKGround;
extern BOOL ShowTips;

// -----------------------------------------------------------------
//  ������ � ���������� ������� :
// -----------------------------------------------------------------
class TSmartButton : public TButton {

    public :

        TSmartButton(TWindow* parent, int id, LPSTR title,
                     int x, int y, int w, int h, BOOL isDefault = FALSE);

        void SetupWindow();
};

// -----------------------------------------------------------------
//  ������-bitmap � ������������ �����������:
// -----------------------------------------------------------------
class TTipButton : public TButtonGadget {

        TExFont* tipFont;
        LPSTR   txtTip;             // ����� ���������.
        int     tipLength;          // ����� ��������� � ����., � ������ 2�� ��������.
        TBitmap*   ScreenShot;      // ���������� ������ ��� ������ ���������.
        TRect   rShot;              // ���������� ��������� �� ������.

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
//  ������� ������ ���� � ������:
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
//  ������� ������ ���� � ������:
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

