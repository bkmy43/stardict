/* **************************************************************************
                                NEWBUTTN.C
    v2.0.
    Содержит реализацию новых кнопок и др. органов управления в StarDict.
        TThemeDialog,
        TArticleDialog,
        TChangeLookDialog,
        TAboutDialog,
        TTipButton,
        TSmartButton.
    (c) В.Перов.
************************************************************************** */

#include <windows.h>
#include <owl\gadgetwi.h>
#include <owl\button.h>
#include <owl\buttonga.h>
#include <owl\dialog.h>
#include <owl\gdiobjec.h>
#include <stdio.h>

#pragma hdrstop

#include "exart.h"

#include "stddef.h"
#include "bd.h"
#include "nbtn.h"

#define TIMER_TICK     150

DEFINE_RESPONSE_TABLE1(TKeywordDialog, TBaseDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_LBN_DBLCLK(idListArticles, CmOk),

    EV_LBN_SELCHANGE(idListThemes, CmLbnSelChangeTheme),
    EV_LBN_SELCHANGE(idListArticles, CmLbnSelChangeArticle),

    EV_EN_CHANGE(idEditTheme, CmEnChangeTheme),
    EV_EN_CHANGE(idEditArticle, CmEnChangeArticle),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TThemeDialog, TBaseDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_LBN_DBLCLK(idListArticles, CmOk),

    EV_LBN_SELCHANGE(idListThemes, CmLbnSelChangeTheme),
    EV_LBN_SELCHANGE(idListArticles, CmLbnSelChangeArticle),

    EV_EN_CHANGE(idEditTheme, CmEnChangeTheme),
    EV_EN_CHANGE(idEditArticle, CmEnChangeArticle),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TArticleDialog, TBaseDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_LBN_DBLCLK(idListArticles, CmOk),
    EV_LBN_SELCHANGE(idListArticles, CmLbnSelChangeArticle),

    EV_EN_CHANGE(idEditArticle, CmEnChangeArticle),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TChangeLookDialog, TBaseDialog)
    EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TAboutDialog, TBaseDialog)
    EV_WM_TIMER,
END_RESPONSE_TABLE;

// ***************************************************************************
// *** class TTipButton :
// ***************************************************************************
TTipButton :: TTipButton(TResId res, int id, TExFont* tipFn)
            : TButtonGadget(res, id, TButtonGadget::Command, TRUE, TButtonGadget::Up)
{
    tipFont = tipFn;
    SetBorderStyle(TGadget::Recessed);
    SetShadowStyle(TButtonGadget::DoubleShadow);
//    SetAntialiasEdges(FALSE);
//    SetNotchCorners(FALSE);
    ScreenShot = NULL;
    txtTip = NULL;

    char buf[100];
    resModule->LoadString(id, buf, sizeof(buf) ); // из модуля ресурсов.
    txtTip = new char [ strlen(buf)+1 ];
    strcpy(txtTip, buf);

    tipLength = tipFont->SpaceLen*2 +
                tipFont->CountLengthInPixels(txtTip);  // длина подсказки в пикселах.
}
TTipButton :: ~TTipButton()
{
    if (ScreenShot) delete ScreenShot;
    if (txtTip) delete txtTip;
}
void TTipButton :: GetScreenShot(TDC& dc, TRect& r)
{
    if (ScreenShot) ReturnScreenShot();
    rShot = r;
    int w = r.right-r.left+1, h = r.bottom-r.top+1;
        // (+1) потому что если коорд.= (0,0) - (1,1), то высота = 2!
    ScreenShot = new TBitmap(dc, w, h);
    TMemoryDC memDC(dc);
    memDC.SelectObject(*ScreenShot);

    memDC.BitBlt(0, 0, w, h, dc, r.left, r.top, SRCCOPY);

    memDC.RestoreBitmap();
}
void TTipButton :: ReturnScreenShot()
{
    if (ScreenShot == NULL) return;
    BITMAP bm;
    ScreenShot->GetObject(bm);

    TScreenDC dc; // контекст всего экрана
    TMemoryDC memDC(dc); // контекст памяти
    memDC.SelectObject(*ScreenShot); // будем копировать картинку из памяти

    dc.BitBlt(rShot.left, rShot.top, bm.bmWidth, bm.bmHeight,
              memDC, 0, 0, SRCCOPY); // копируем

    memDC.RestoreBitmap();
    delete ScreenShot;
    ScreenShot = NULL;
}
void TTipButton :: LButtonDown(uint modKeys, TPoint& point)
{
    MouseLeave(modKeys, point);
    TButtonGadget :: LButtonDown(modKeys, point);
}
void TTipButton :: MouseEnter(uint modKeys, TPoint& point)
{
    TButtonGadget :: MouseEnter(modKeys, point);
    if (!ShowTips) return;

    TPoint pt(point.x+Bounds.left, 0);
    Window->ClientToScreen(pt);
    pt.y += point.y + 20;   // расстояние между мышкиным курсором и подсказкой

    TRect r(0, 0, tipLength+1, tipFont->chHeight+2*tipFont->chInternal+1);  // учесть тени шириной в 1 пиксел
    r += pt;

    TScreenDC dc;
    GetScreenShot(dc, r);

    dc.SelectObject( *tipFont);/*:: GetStockObject(TIP_HELP_FONT) */
    dc.Rectangle(r);
    dc.MoveTo(r.left+ tipFont->SpaceLen, r.bottom);
    dc.LineTo(r.right, r.bottom);
    dc.LineTo(r.right, r.top+tipFont->chInternal);// тени
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(TColor::Black);
    dc.SetBkColor(::GetSysColor(COLOR_BACKGROUND) ); //COLOR_WINDOW
    dc.TextOut(r.left+tipFont->SpaceLen, r.top+tipFont->chInternal, txtTip);
}
void TTipButton :: MouseLeave(uint modKeys, TPoint& pt)
{
    TButtonGadget::MouseLeave(modKeys, pt);
    if (!ShowTips) return;
    ReturnScreenShot();
}
// ***************************************************************************
// *** class TSmartButton :
// ***************************************************************************
TSmartButton :: TSmartButton(TWindow* parent, int id, LPSTR title,
                     int x, int y, int w, int h, BOOL isDefault)
             : TButton(parent, id, title, x, y, w, h, isDefault)
{
}
void TSmartButton :: SetupWindow()
{
    TButton :: SetupWindow();
    SetWindowFont( (HFONT)::GetStockObject(ANSI_VAR_FONT), TRUE);
}
// ***************************************************************************
// *** class TArticleDialog :
// ***************************************************************************
TArticleDialog :: TArticleDialog(TWindow* parent, TResId id, int* pos, TExFont* tip)
             : TBaseDialog(parent, id, tip, bmpTriangle), ArtList()
{
    oldPos = pos;
}
TArticleDialog :: ~TArticleDialog()
{
}
void TArticleDialog :: SetupWindow()
{
    TBaseDialog::SetupWindow();

    FillArticleList();
}
void TArticleDialog :: FillArticleList()
{
    char buf[256];
    for (int i = 0; i<ArtList.GetItemsNum(); ++i) {
        SendDlgItemMessage(idListArticles, LB_ADDSTRING, 0, (LPARAM)ArtList.GetNthName(i));
    }
    SendDlgItemMessage(idListArticles, LB_SETCURSEL, (WPARAM)*oldPos, 0L);

    SendDlgItemMessage(idListArticles, LB_GETTEXT, (WPARAM)*oldPos, (LPARAM)buf);
    SetDlgItemText(idEditArticle, buf);
}
// -------------------------- commands : ------------------------------------
void TArticleDialog :: CmLbnSelChangeArticle()
{
    char buf[100];
    int index = (int)SendDlgItemMessage(idListArticles, LB_GETCURSEL, 0, 0L);
    if (index == LB_ERR) index=0;
    SendDlgItemMessage(idListArticles, LB_GETTEXT, (WPARAM)index, (LPARAM)buf);
    SetDlgItemText(idEditArticle, buf);
}
void TArticleDialog :: CmOk()
{
    *oldPos = (int)SendDlgItemMessage(idListArticles, LB_GETCURSEL, 0, 0L);
    if (*oldPos == LB_ERR) *oldPos=0;
    int index = ArtList.GetNthNumber(*oldPos);

    Parent->PostMessage(wmNewArticle, (WPARAM)index, 0L);

    TDialog :: CmOk();
}
void TArticleDialog :: CmEnChangeArticle()
{
    char buf[100];
    GetDlgItemText(idEditArticle, buf, sizeof(buf)-1);
    SendDlgItemMessage(idListArticles, LB_SELECTSTRING, -1, (LPARAM)buf);
}
// ***************************************************************************
// *** class TThemeDialog :
// ***************************************************************************
TThemeDialog :: TThemeDialog(TWindow* parent, TResId id, TPositionLists* pos, TExFont* tip)
             : TBaseDialog(parent, id, tip, bmpTriangle), ThemeList()
{
    oldPos = pos;
    thIndex = oldPos->oldThemePos;
    arIndex = oldPos->oldArticlePos;
}
TThemeDialog :: ~TThemeDialog()
{
}
void TThemeDialog :: SetupWindow()
{
    TBaseDialog::SetupWindow();

    FillThemeList();
    FillArticleList();
}
void TThemeDialog :: FillThemeList()
{
    char buf[128];
    for (int i = 0; i<ThemeList.GetItemsNum(); ++i)
        SendDlgItemMessage(idListThemes, LB_ADDSTRING, 0, (LPARAM)ThemeList.GetNthName(i));

    SendDlgItemMessage(idListThemes, LB_SETCURSEL, (WPARAM)thIndex, 0L);

    SendDlgItemMessage(idListThemes, LB_GETTEXT, (WPARAM)thIndex, (LPARAM)buf);
    SetDlgItemText(idEditTheme, buf);
}
void TThemeDialog :: FillArticleList()
{
    SendDlgItemMessage(idListArticles, LB_RESETCONTENT, 0, 0L);

    char buf[128];
    TArticleList* pArticleList = ThemeList.GetNthArtList(thIndex);
    for (int i = 0; i<pArticleList->GetItemsNum(); ++i)
        SendDlgItemMessage(idListArticles, LB_ADDSTRING, 0, (LPARAM)pArticleList->GetNthName(i));

    SendDlgItemMessage(idListArticles, LB_GETTEXT, (WPARAM)arIndex, (LPARAM)buf);
    SetDlgItemText(idEditArticle, buf);
}
// -------------------------- commands : ------------------------------------
void TThemeDialog :: CmOk()
{
    oldPos->oldThemePos = thIndex;//(int)SendDlgItemMessage(idListThemes, LB_GETCURSEL, 0, 0L);
    oldPos->oldArticlePos = arIndex;//(int)SendDlgItemMessage(idListArticles, LB_GETCURSEL, 0, 0L);

    int index = ThemeList.ThArtList->GetNthNumber(arIndex);
    Parent->PostMessage(wmNewArticle, (WPARAM)index, 0L);

    TDialog :: CmOk();
}
// -----------------------------------------------------------------------
void TThemeDialog :: CmEnChangeTheme()
{
    char buf[128];
    GetDlgItemText(idEditTheme, buf, sizeof(buf)-1);
    int index = (int)SendDlgItemMessage(idListThemes, LB_SELECTSTRING, -1, (LPARAM)buf);
    if (index == LB_ERR) index = 0;

    if (thIndex != index) {
        thIndex = index;
        arIndex = 0;

        FillArticleList();
    }
}
void TThemeDialog :: CmLbnSelChangeTheme()
{
    char buf[128];
    int index = (int)SendDlgItemMessage(idListThemes, LB_GETCURSEL, 0, 0L);
    SendDlgItemMessage(idListThemes, LB_GETTEXT, (WPARAM)index, (LPARAM)buf);
    SetDlgItemText(idEditTheme, buf); // -> заставляет посылать сообщ LB_ENCHANGE
}
// ----------------------------------------------------------------------
void TThemeDialog :: CmEnChangeArticle()
{
    char buf[128];
    GetDlgItemText(idEditArticle, buf, sizeof(buf)-1);
    int in = (int)SendDlgItemMessage(idListArticles, LB_SELECTSTRING, -1, (LPARAM)buf);
    if ( in != LB_ERR) arIndex = in;
}
void TThemeDialog :: CmLbnSelChangeArticle()
{
    int index = (int)SendDlgItemMessage(idListArticles, LB_GETCURSEL, 0, 0L);

    char buf[128];
    SendDlgItemMessage(idListArticles, LB_GETTEXT, (WPARAM)index, (LPARAM)buf);
    SetDlgItemText(idEditArticle, buf); // -> заставляет посылать сообщ LB_ENCHANGE
}
// ***************************************************************************
// *** class TChooseDialog :
// ***************************************************************************
TChangeLookDialog :: TChangeLookDialog(TWindow* p, TResId r, TExFont* tip)
                  : TBaseDialog(p, r, tip, bmpTriangle)
{
/*    pAfterRun = pRun;
    pAfterOpen = pOpen;*/
}
void TChangeLookDialog :: SetupWindow()
{
    TBaseDialog :: SetupWindow();

    char ch[128];
    for (int i=0; i<NUM_AR_STRINGS; ++i) {
        resModule->LoadString(stringAR_CHOOSEART+i, ch, sizeof(ch));
        SendDlgItemMessage(idAfterRun, CB_ADDSTRING, 0, (LPARAM) ch);
    }
    for (i=0; i<NUM_AO_STRINGS; ++i) {
        resModule->LoadString(stringAO_WIDEN+i, ch, sizeof(ch));
        SendDlgItemMessage(idAfterOpen, CB_ADDSTRING, 0, (LPARAM) ch);
    }
    for (i=0; i<NUM_BK_STRINGS; ++i) {
        resModule->LoadString(stringBK_FLECK+i, ch, sizeof(ch));
        SendDlgItemMessage(idBackgrounds, CB_ADDSTRING, 0, (LPARAM) ch);
    }
    SendDlgItemMessage(idAfterRun, CB_SETCURSEL, (WPARAM)AfterRun, 0L);
    SendDlgItemMessage(idAfterOpen, CB_SETCURSEL, (WPARAM)AfterOpen, 0L);
    SendDlgItemMessage(idBackgrounds, CB_SETCURSEL, (WPARAM)BKGround, 0L);
    CheckDlgButton(idShowTips, ShowTips == TRUE);
}
void TChangeLookDialog :: CmOk()
{
    AfterRun = (int)SendDlgItemMessage(idAfterRun, CB_GETCURSEL, 0L);
    AfterOpen = (int)SendDlgItemMessage(idAfterOpen, CB_GETCURSEL, 0L);
    ShowTips = (int)IsDlgButtonChecked(idShowTips);
    int old = BKGround;
    BKGround = (int)SendDlgItemMessage(idBackgrounds, CB_GETCURSEL, 0L);
    if (old != BKGround)
        Parent->PostMessage(wmBKChanged, 0, 0L);

    TDialog :: CmOk();
}
// ***************************************************************************
// *** class TKeywordDialog :
// ***************************************************************************
TKeywordDialog :: TKeywordDialog(TWindow* parent, TResId id, TPositionLists* pos, TExFont* tip)
                : TBaseDialog(parent, id, tip, bmpTriangle), KeywordList()
{
    oldPos = pos;
    kwIndex = oldPos->oldThemePos;
    arIndex = oldPos->oldArticlePos;
}
TKeywordDialog :: ~TKeywordDialog()
{
}
void TKeywordDialog :: SetupWindow()
{
    TBaseDialog::SetupWindow();

    FillKeywordList();
    FillArticleList();
}
void TKeywordDialog :: FillKeywordList()
{
    char buf[128];
    for (int i = 0; i<KeywordList.GetItemsNum(); ++i)
        SendDlgItemMessage(idListThemes, LB_ADDSTRING, 0, (LPARAM)KeywordList.GetNthName(i));

    SendDlgItemMessage(idListThemes, LB_SETCURSEL, (WPARAM)kwIndex, 0L);

    SendDlgItemMessage(idListThemes, LB_GETTEXT, (WPARAM)kwIndex, (LPARAM)buf);
    SetDlgItemText(idEditTheme, buf);
}
void TKeywordDialog :: FillArticleList()
{
    SendDlgItemMessage(idListArticles, LB_RESETCONTENT, 0, 0L);

    char buf[128];
    TArticleList* pArticleList = KeywordList.GetNthArtList(kwIndex);
    for (int i = 0; i<pArticleList->GetItemsNum(); ++i)
        SendDlgItemMessage(idListArticles, LB_ADDSTRING, 0, (LPARAM)pArticleList->GetNthName(i));

    SendDlgItemMessage(idListArticles, LB_GETTEXT, (WPARAM)arIndex, (LPARAM)buf);
    SetDlgItemText(idEditArticle, buf);
}
// -------------------------- commands : ------------------------------------
void TKeywordDialog :: CmOk()
{
    oldPos->oldThemePos = kwIndex;//(int)SendDlgItemMessage(idListThemes, LB_GETCURSEL, 0, 0L);
    oldPos->oldArticlePos = arIndex;//(int)SendDlgItemMessage(idListArticles, LB_GETCURSEL, 0, 0L);

    int index = KeywordList.KeyArtList->GetNthNumber(arIndex);
    Parent->PostMessage(wmNewArticle, (WPARAM)index, 0L);

    TDialog :: CmOk();
}
// -----------------------------------------------------------------------
void TKeywordDialog :: CmEnChangeTheme()
{
    char buf[128];
    GetDlgItemText(idEditTheme, buf, sizeof(buf)-1);
    int index = (int)SendDlgItemMessage(idListThemes, LB_SELECTSTRING, -1, (LPARAM)buf);
    if (index == LB_ERR) index = 0;

    if (kwIndex != index) {
        kwIndex = index;
        arIndex = 0;

        FillArticleList();
    }
}
void TKeywordDialog :: CmLbnSelChangeTheme()
{
    char buf[128];
    int index = (int)SendDlgItemMessage(idListThemes, LB_GETCURSEL, 0, 0L);
    SendDlgItemMessage(idListThemes, LB_GETTEXT, (WPARAM)index, (LPARAM)buf);
    SetDlgItemText(idEditTheme, buf); // -> заставляет посылать сообщ LB_ENCHANGE
}
// ----------------------------------------------------------------------
void TKeywordDialog :: CmEnChangeArticle()
{
    char buf[128];
    GetDlgItemText(idEditArticle, buf, sizeof(buf)-1);
    int in = (int)SendDlgItemMessage(idListArticles, LB_SELECTSTRING, 0, (LPARAM)buf);
    if ( in != LB_ERR) arIndex = in;
}
void TKeywordDialog :: CmLbnSelChangeArticle()
{
    int index = (int)SendDlgItemMessage(idListArticles, LB_GETCURSEL, 0, 0L);

    char buf[128];
    SendDlgItemMessage(idListArticles, LB_GETTEXT, (WPARAM)index, (LPARAM)buf);
    SetDlgItemText(idEditArticle, buf); // -> заставляет посылать сообщ LB_ENCHANGE
}
// ***************************************************************************
// *** class TAboutDialog :
// ***************************************************************************
TAboutDialog :: TAboutDialog(TWindow* p, TResId r, LPSTR RN, LPSTR UN, LPSTR left, TExFont* tip)
             : TBaseDialog(p, r, tip, bmpTriangle)
{
    RegNumber = RN;
    UserName = UN;
    InstallLeft = left;
    increment = 1;
    curFrame = 0;
    HINSTANCE hin = GetApplication()->GetInstance();
    fr[0] = new TIcon(hin, icoFrame1);
    fr[1] = new TIcon(hin, icoFrame2);
    fr[2] = new TIcon(hin, icoFrame3);
    fr[3] = new TIcon(hin, icoFrame4);
    fr[4] = new TIcon(hin, icoFrame5);
    fr[5] = new TIcon(hin, icoFrame6);
}
TAboutDialog :: ~TAboutDialog()
{
    KillTimer(1234);
    delete fr[0];
    delete fr[1];
    delete fr[2];
    delete fr[3];
    delete fr[4];
    delete fr[5];
}
void TAboutDialog :: SetupWindow()
{
    TBaseDialog :: SetupWindow();

    SetTimer(1234, TIMER_TICK, NULL);

    char buf [ sizeof(OwnerText)+1 ];
    GetDlgItemText(idTxtOwner, buf, sizeof(OwnerText) );
    sprintf(OwnerText, buf, UserName, RegNumber, InstallLeft );
    SetDlgItemText(idTxtOwner, OwnerText);
}
void TAboutDialog :: EvTimer(uint)
{
    curFrame += increment;
    if (curFrame >=5 || curFrame<=0) increment= -increment;
    HWND hwnd = GetDlgItem(idIconID);
    RECT r;
    ::GetWindowRect(hwnd, &r);
    TPoint p; p.x = r.left; p.y = r.top;
    ScreenToClient(p);
    TClientDC dc(HWindow);
    dc.DrawIcon(p, *fr[curFrame]);
}
// ***************************************************************************
// *** class TDrawingWindow :
// ***************************************************************************
TDrawingWindow :: TDrawingWindow(TWindow* p, HINSTANCE hInst, TModule* m)
               : TWindow(p, NULL, m)
{
    Attr.Style |= WS_POPUP | WS_VISIBLE | WS_DLGFRAME;

    TDib d(hInst, bmpDrawing);
    ChangeDIB(&d);
    dib = new TBitmap(d);

    Attr.H = dib->Height()+2;
    Attr.W = dib->Width()+2;
    Attr.X = (GetSystemMetrics(SM_CXFULLSCREEN) - Attr.W) /2;
    Attr.Y = (GetSystemMetrics(SM_CYFULLSCREEN) - Attr.H) /2;

    diby = (Attr.H-dib->Height())/2 -GetSystemMetrics(SM_CYDLGFRAME);
    dibx = (Attr.W-dib->Width())/2 - GetSystemMetrics(SM_CXDLGFRAME);
}
TDrawingWindow :: ~TDrawingWindow()
{
    if (dib) delete dib;
}
void TDrawingWindow :: SetupWindow()
{
    TWindow :: SetupWindow();

    // делаем окно лежащим всегда наверху
    ::SetWindowPos(HWindow, (HWND)-1, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); // always topmost
}
void TDrawingWindow :: GetWindowClass(WNDCLASS& one)
{
    TWindow :: GetWindowClass(one);
    one.style |= CS_NOCLOSE;
    one.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
}
void TDrawingWindow :: ChangeDIB(TDib* d)
{
    // замена прозрачного цвета
    d->MapColor(TColor::LtGray, ::GetSysColor(COLOR_BTNFACE) );
}
void TDrawingWindow :: Paint(TDC& dc, bool, TRect&)
{
    TMemoryDC memDC(dc);

    memDC.SelectObject(*dib);
    dc.BitBlt(dibx, diby, dib->Width(), dib->Height(), memDC, 0, 0, SRCCOPY);

    memDC.RestoreBitmap();
}

