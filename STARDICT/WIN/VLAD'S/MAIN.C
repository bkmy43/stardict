/* **************************************************************************
                                 MAIN.C
    v2.0
    Содержит реализацию основных оконных классов StarDict и функцию WinMain.
    (c) В.Перов.
************************************************************************** */

#include <windows.h>
#include <owl\window.rh>
#include <owl\framewin.h>
#include <owl\decmdifr.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\mdi.h>
#include <owl\mdichild.h>
#include <owl\mdi.rh>
#include <owl\profile.h>
#include <owl\choosefo.h>
#include <owl\controlg.h>
#include <owl\button.h>
#include <owl\buttonga.h>
#include <owl\combobox.h>
#include <owl\scroller.h>

#include <string.h>

#pragma hdrstop

#include "tags.h"
#include "stddef.h"
#include "std.h"

// Указатель на модуль (dll) языковых ресурсов :
TModule* resModule = NULL;

// Статические элементы класса TTipButton.
// Должны изменяться при изменении системных шрифтов.
int TTipButton :: SpaceLen = 0;
int TTipButton :: tipHeight = 0;
int TTipButton :: lpSizes[256];

// Шрифт для посказок :
#define  TIP_HELP_FONT           ANSI_VAR_FONT
// Отступ сверху и снизу от границ подсказки:
#define  TIP_EXTERNAL            3

DEFINE_RESPONSE_TABLE1(TStarDictClient, TMDIClient)
    EV_COMMAND(cmAbout, CmAbout),
    EV_COMMAND(cmQuitDict, CmQuitDict),
    EV_COMMAND(cmShowContents, CmShowContents),
    EV_COMMAND(cmShowOptions, CmShowOptions),
    EV_COMMAND(cmNewWindow, CmNewWindow),
    EV_COMMAND(cmCascade, CmCascade),
    EV_COMMAND(cmCloseAll, CmCloseAll),

    EV_COMMAND_ENABLE(cmCascade, CeCascade),
    EV_COMMAND_ENABLE(cmCloseAll, CeCloseAll),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TSDecoratedMDIFrame, TDecoratedMDIFrame)
    EV_WM_SIZE,
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TArticleWindow, TMDIChild)
    EV_WM_SIZE,
    EV_MESSAGE(wmInvalidate, EvInvalidate),
END_RESPONSE_TABLE;

// ***************************************************************************
// *** class TTipButton :
// ***************************************************************************
// Статическая функция инициализации. Вызывается до создания элементов класса TTipButton.
void TTipButton :: InitAllWithFonts()
{
    TScreenDC dc;
    dc.SelectObject( (HFONT)::GetStockObject( TIP_HELP_FONT) );

    TEXTMETRIC tm;
    dc.GetTextMetrics(tm);
    TTipButton::tipHeight = tm.tmHeight + TIP_EXTERNAL*2;

    dc.GetCharWidth(32, 255, lpSizes+32); // массив ширин символов выбранного шрифта.
    TTipButton::SpaceLen = lpSizes[ 32 ]; // ширина пробела.
}
TTipButton :: TTipButton(TResId res, int id)
            : TButtonGadget(res, id, TButtonGadget::Command, TRUE, TButtonGadget::Up)
{
    SetBorderStyle(TGadget::Embossed);
    ScreenShot = NULL;
    txtTip = NULL;

    char buf[100];
    resModule->LoadString(id, buf, sizeof(buf) ); // из модуля ресурсов.
    txtTip = new char [ strlen(buf)+1 ];
    strcpy(txtTip, buf);

    tipLength = CountLengthInPixels();  // длина подсказки в пикселах.
}
TTipButton :: ~TTipButton()
{
    if (ScreenShot) delete ScreenShot;
    if (txtTip) delete txtTip;
}
int TTipButton :: CountLengthInPixels()
{
    LPBYTE temp = (LPBYTE)txtTip; // главное - чтобы temp указывал на unsigned char!
    int result =0;
    while (*temp != '\0')
        result += TTipButton::lpSizes[ (unsigned) *temp++ ];

    result += TTipButton::SpaceLen *2;
    return result;
}
void TTipButton :: GetScreenShot(TDC& dc, TRect& r)
{
    if (ScreenShot) ReturnScreenShot();
    rShot = r;
    int w = r.right-r.left+1, h = r.bottom-r.top+1;
    ScreenShot = new TBitmap(dc, w, h);
    TMemoryDC memDC(dc);
    memDC.SelectObject(*ScreenShot);

    memDC.BitBlt(0, 0, w, h, dc, r.left, r.top, SRCCOPY);

    memDC.RestoreBitmap();
}
void TTipButton :: ReturnScreenShot()
{
    if (ScreenShot == NULL) return;
    TScreenDC dc;
    TMemoryDC memDC(dc);
    memDC.SelectObject(*ScreenShot);

    dc.BitBlt(rShot.left, rShot.top, ScreenShot->Width(), ScreenShot->Height(),
              memDC, 0, 0, SRCCOPY);

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

    TPoint pt((Bounds.right+Bounds.left)/2, 0); // ровно посередине
    Window->ClientToScreen(pt);
    pt.y += point.y + 20;   // расстояние между мышкиным курсором и подсказкой

    TRect r(0, 0, tipLength, tipHeight);
    r += pt;

    TScreenDC dc;
    GetScreenShot(dc, r);

    dc.SelectObject( (HFONT) :: GetStockObject(TIP_HELP_FONT) );
    dc.Rectangle(r);
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(TColor::Black);
    dc.TextOut(r.left+SpaceLen, r.top+TIP_EXTERNAL, txtTip);
}
void TTipButton :: MouseLeave(uint modKeys, TPoint& pt)
{
    TButtonGadget::MouseLeave(modKeys, pt);
    ReturnScreenShot();
}
// **********************************************************************************
void TExArticle::CreateString()
{
    article = new char[80*20];
    LPSTR art = article;
    *art ++ = TAG_PARAGRAPH;
    strcpy(art, "Example. Just an example. Never mind this, I only "
                "wanted to look at the result of my exeptioning, so "
                "you're welcome here for wealthy critic. Saying one more - "
                "don't think how to interpret this delirium...");
}
// *************************************************************
// *** class TArticleWindow :
// *************************************************************
TArticleWindow :: TArticleWindow(TStarDictClient& parent, TExArticle* artLink)
            : TMDIChild(parent, NULL)
{
    if (artLink) pArticle = artLink;
    else {
        pArticle = new TExArticle;
    }
}
TArticleWindow :: ~TArticleWindow()
{
    if (pArticle) delete pArticle;
}
void TArticleWindow :: SetupWindow()
{
    if (pArticle) SetCaption(pArticle->Title);
             else SetCaption("Без имени");

    Scroller = new TScroller(this, 10, 10, 20, 20);
    ShowScrollBar(SB_VERT, TRUE);
}
void TArticleWindow :: GetWindowClass(WNDCLASS& one)
{
    TMDIChild :: GetWindowClass(one);
    one.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    one.style = 0; //CS_HREDRAW | CS_VREDRAW;
}
void TArticleWindow :: Paint(TDC& dc, BOOL, TRect& )
{
    TStarDictClient* p = TYPESAFE_DOWNCAST(Parent, TStarDictClient);

    dc.SelectObject( *p->pFont);
    dc.SetTextColor( p->data.Color);
    dc.DrawText(pArticle->article, -1 /* whole */, GetClientRect(), DT_WORDBREAK | DT_LEFT);
}
void TArticleWindow :: EvSize(uint sizeType, TSize& size)
{
    TMDIChild::EvSize(sizeType, size);
    if (sizeType == SIZE_MAXIMIZED || sizeType == SIZE_RESTORED)
        Invalidate(TRUE);
}
LPARAM TArticleWindow :: EvInvalidate(WPARAM, LPARAM)
{
    if (!IsIconic()) Invalidate();
    return 0;
}
// *************************************************************
// *** class TStarDictClient :
// *************************************************************
TStarDictClient :: TStarDictClient(void)
                : TMDIClient()
{
    ReadProfileFont();
    pFont = new TFont(&data.LogFont);

    isWindowOpened = FALSE;
}
TStarDictClient :: ~TStarDictClient()
{
    WriteProfileFont();
    delete pFont;
}
void TStarDictClient :: ReadProfileFont()
{
    data.DC = 0;
    data.Flags = CF_FIXEDPITCHONLY | CF_EFFECTS | CF_FORCEFONTEXIST | CF_SCREENFONTS;
    data.FontType = SCREEN_FONTTYPE;
    data.SizeMin = 9;
    data.SizeMax = 17;

    TProfile pr(txtIniSectionFont, txtIniFileName);
    char buf[100];

    pr.GetString(txtIniSectionFont_Color, buf, sizeof(buf)-2, "0x0"); // Color
    data.Color = strtoul(buf, NULL /* указатель на ошибку */, NULL /* выбор стистемы автоматически*/);

    memset(&data.LogFont, 0, sizeof(LOGFONT) );
    data.LogFont.lfHeight = pr.GetInt(txtIniSectionFont_Size, -13); // Size
    pr.GetString(txtIniSectionFont_Face, data.LogFont.lfFaceName, LF_FACESIZE, "Courier New"); // Facename
    data.LogFont.lfCharSet = (BYTE) pr.GetInt(txtIniSectionFont_Charset, OEM_CHARSET);

    data.LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    data.LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    data.LogFont.lfQuality = PROOF_QUALITY;
    data.LogFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN; // or FF_DONTCARE
}
void TStarDictClient :: WriteProfileFont()
{
    TProfile pr(txtIniSectionFont, txtIniFileName);
    char buf[40] = "0x";
    ultoa(data.Color, buf+2, 16);

    pr.WriteString(txtIniSectionFont_Color,  buf);
    pr.WriteInt(txtIniSectionFont_Charset, data.LogFont.lfCharSet);
    pr.WriteInt(txtIniSectionFont_Size, data.LogFont.lfHeight);
    pr.WriteString(txtIniSectionFont_Face, data.LogFont.lfFaceName);
}
void TStarDictClient :: SetupWindow()
{
    TMDIClient :: SetupWindow();
}
void TStarDictClient :: GetWindowClass(WNDCLASS& one)
{
    TMDIClient :: GetWindowClass(one);
    one.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
}
TArticleWindow* TStarDictClient :: InitChild()
{
    return new TArticleWindow(*this, NULL);
}
// ------------------------- commands : --------------------------------
void TStarDictClient :: CmShowOptions()
{
    if (pFont) data.Flags |= CF_INITTOLOGFONTSTRUCT;
    TChooseFontDialog* d = new TChooseFontDialog(this, data);
    if (d->Execute() == IDOK) {
        delete pFont;
        pFont = new TFont(&data.LogFont);
        ChildBroadcastMessage(wmInvalidate, 0, 0);
    }
}
void TStarDictClient :: CmQuitDict()
{
    Parent->HandleMessage(WM_COMMAND, CM_EXIT, 0);
}
void TStarDictClient :: CmCascade()
{
    CascadeChildren();
}
void TStarDictClient :: CmCloseAll()
{
    CloseChildren();
    isWindowOpened = FALSE;
}
void TStarDictClient :: CmAbout()
{
    MessageBox("This is created.", "Title", MB_OK | MB_ICONINFORMATION);
}
void TStarDictClient :: CmShowContents()
{
}
void TStarDictClient :: CmNewWindow()
{
    isWindowOpened = TRUE;
    TArticleWindow* p = new TArticleWindow(*this, NULL);
    // объект TArticleWindow* не нужно удалять самостоятельно - при закрытии
    // окна автоматически выполняется delete.
    p->Create();
}
void TStarDictClient :: CeCascade(TCommandEnabler& ce)
{
  // Enable CmCascade if not dirty
  ce.Enable(isWindowOpened);
}
void TStarDictClient :: CeCloseAll(TCommandEnabler& ce)
{
  ce.Enable(isWindowOpened);
} 
// ********************************************************************
// *** class TSDecoratedMDIFrame :
// ********************************************************************
TSDecoratedMDIFrame :: TSDecoratedMDIFrame(LPCSTR t, TResId id, TStarDictClient& c)
          : TDecoratedMDIFrame(t, id, c, TRUE)
{
    MergeModule = resModule; // модуль ресурсов, откуда берутся подсказки.

    RECT r;
    ::GetWindowRect(GetDesktopWindow(), &r);
    int w = (r.right-r.left)*4/5,
        h = (r.bottom-r.top)*4/5,
        x = (r.right-r.left - w)/2,
        y = (r.bottom-r.top - h)/2;

    TProfile pr(txtIniSectionSize, txtIniFileName);
    isMaximized = pr.GetInt(txtIniSectionSize_Max, 0);
    Attr.X = pr.GetInt(txtIniSectionSize_X, x);
    Attr.Y = pr.GetInt(txtIniSectionSize_Y, y);
    Attr.W = pr.GetInt(txtIniSectionSize_W, w);
    Attr.H = pr.GetInt(txtIniSectionSize_H, h);
}
TSDecoratedMDIFrame :: ~TSDecoratedMDIFrame()
{
    TProfile pr(txtIniSectionSize, txtIniFileName);

    pr.WriteInt(txtIniSectionSize_Max, isMaximized);
    pr.WriteInt(txtIniSectionSize_X, Attr.X);
    pr.WriteInt(txtIniSectionSize_Y, Attr.Y);
    pr.WriteInt(txtIniSectionSize_W, Attr.W);
    pr.WriteInt(txtIniSectionSize_H, Attr.H);
}
void TSDecoratedMDIFrame :: EvSize(uint sizeType, TSize& size)
{
    if (sizeType == SIZE_MAXIMIZED) isMaximized = 1;
    else if (sizeType == SIZE_RESTORED) isMaximized = 0;

    TDecoratedMDIFrame :: EvSize(sizeType, size);
}
void TSDecoratedMDIFrame :: SetupWindow()
{
    TDecoratedMDIFrame :: SetupWindow();

    // Порядок след. двух строк важен. Почему - долго объяснять. :)
    if (isMaximized) ::PostMessage(HWindow, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    SetMenu( resModule->LoadMenu(mnuMainMenu) );
}
// ********************************************************************
// *** class TStarDictApp :
// ********************************************************************
void TStarDictApp :: InitializeBars(TDecoratedMDIFrame* frame)
{
    controlBar = new TControlBar(frame, TGadgetWindow::Horizontal);
    statusBar = new TStatusBar(frame, TGadget::Recessed, 0);
    char lpstr[30];

    resModule->LoadString(stringAbout, lpstr, sizeof(lpstr));
    controlBar->Insert(*new TTipButton(bmpExit, cmQuitDict));
    controlBar->Insert(*new TSeparatorGadget(10) );
    controlBar->Insert(*new TTipButton(bmpAbout, cmAbout));
    controlBar->Insert(*new TTipButton(bmpContents, cmShowContents));

    controlBar->SetHintMode(TGadgetWindow :: EnterHints);

    resModule->LoadString(stringStatus, lpstr, sizeof(lpstr));
    statusBar->Insert(
        *(sbtxtStatus = new TTextGadget(0, TGadget::Recessed, TTextGadget::Left, 20, lpstr)), TGadgetWindow::Before );
}
void TStarDictApp :: InitMainWindow()
{
    EnableCtl3d(TRUE);

    TTipButton :: InitAllWithFonts();  // перед вызовом конструкторов TTipButton.

    TSDecoratedMDIFrame* frame =
          new TSDecoratedMDIFrame("Star Dictionary",
                                  0, // установим меню позже.
                                  *(stdClient = new TStarDictClient));

    InitializeBars(frame);
    if (controlBar) frame->Insert(*controlBar, TDecoratedFrame :: Top);
    if (statusBar) frame->Insert(*statusBar, TDecoratedFrame :: Bottom);

    SetMainWindow(frame);

    MainWindow->SetIcon(this, icoStarDict);
    MainWindow->SetCursor(this, curStarDict);

    MainWindow->EnableKBHandler();
}
void TStarDictApp :: InitInstance()
{
    if (hPrevInstance) { // вытаскиваем на поверхность предыдущее приложение
            HWND hwnd = FindWindow(txtMainWindowClassName, NULL/*заголовок не важен*/);
            if (hwnd) {
                hwnd = GetLastActivePopup(hwnd);
                BringWindowToTop( hwnd );
                ShowWindow(hwnd, SW_RESTORE);
            }
            PostAppMessage(GetCurrentTask(), WM_QUIT, 0, 0); // второе (это) приложение закрываем
    }
    else {
        TApplication :: InitInstance();
//        HAccTable = LoadAccelerators(idAccel);
    }
}
// **************************************************************************
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
                   lpCmdLine, int nCmdShow)
{
    resModule = new TModule("stdrus.dll"/*, TRUE*/);

    TStarDictApp ap("StarDictApp", hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    int result = ap.Run();

    delete resModule;
    return result;
}

