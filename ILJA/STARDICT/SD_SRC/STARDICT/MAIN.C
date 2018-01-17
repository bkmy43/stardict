/* **************************************************************************
                                 MAIN.C
    v2.0
    Содержит реализацию основных оконных классов StarDict и функцию WinMain.
    Классы:
        TStarDictClient,
        TSDecoratedMDIFrame,
        TStarDictApp.
    (c) В.Перов.
************************************************************************** */

#include <windows.h>
#include <owl\window.rh>
#include <owl\mdi.rh>
#include <owl\decmdifr.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\mdi.h>
#include <owl\mdichild.h>
#include <owl\profile.h>
#include <owl\choosefo.h>
#include <owl\scroller.h>
#include <owl\button.h>
#include <owl\buttonga.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#pragma hdrstop

#include "rw.h"
#include "defines.h"
#include "exart.h"

#include "sd_defs.h"
#include "stddef.h"
#include "code.h"
#include "ex.h"
#include "tags.h"
#include "bd.h"
#include "nbtn.h"
#include "hst.h"
#include "artw.h"
#include "std.h"

#define idLocalTimer            1111
#define TIMER_TICK              150
#define TIME_TO_SHOW_LOGO       3000

extern BOOL Windows31;
extern void OpenDataFiles();
extern void CloseDataFiles();

//TDrawingWindow* Drawing = NULL;

char RegNumber[50];
char UserName[100];

int AfterRun = AR_CHOOSEART;
int AfterOpen = AO_WIDEN;
int BKGround = BK_FLECK;
BOOL ShowTips = TRUE;
int InstallLeft = 0;

#if defined (_STD_DEBUG)
static LPSTR chTime = __DATE__ ", " __TIME__ ".DEBUG";
#endif

typedef int filehand;

filehand fhIndex, fhThemes, fhData, fhKeys;

// Шрифт для посказок :
//#define  TIP_HELP_FONT     DEVICE_DEFAULT_FONT
//ANSI_VAR_FONT

// Указатель на модуль (dll) языковых ресурсов :
TModule* resModule = NULL;

BOOL fLangChanged = FALSE;
extern int stdLanguage;// = LANG_RUSSIAN;

int stdCharSet = OEM_CHARSET;

DEFINE_RESPONSE_TABLE1(TStarDictClient, TMDIClient)
    EV_WM_RBUTTONDOWN,
    EV_COMMAND(cmAbout, CmAbout),
    EV_COMMAND(cmAboutProgrammers, CmAboutProgrammers),
    EV_COMMAND(cmQuitDict, CmQuitDict),
    EV_COMMAND(cmShowFonts, CmShowFonts),
    EV_COMMAND(cmChangeLook, CmChangeLook),
    EV_COMMAND(cmMakeEnglish, CmMakeEnglish),
    EV_COMMAND(cmMakeRussian, CmMakeRussian),
    EV_COMMAND(IDCANCEL, CmEsc),
    EV_MESSAGE(wmNewArticle, WmNewArticle),
    EV_MESSAGE(wmBKChanged, WmBKChanged),

    EV_COMMAND(cmShowThemeList, CmShowThemeList),
    EV_COMMAND(cmShowArticleList, CmShowArticleList),
    EV_COMMAND(cmSearchKeyword, CmSearchKeyword),
    EV_COMMAND(wmMoveBack, CmMoveBack),
    EV_COMMAND(wmMoveForward, CmMoveForward),

    EV_COMMAND_ENABLE(wmMoveBack, CeMoveBack),
    EV_COMMAND_ENABLE(wmMoveForward, CeMoveForward),
    EV_COMMAND_ENABLE(cmMakeEnglish-1, CeEnablePopup),
    EV_COMMAND_ENABLE(cmMakeEnglish, CeMakeEnglish),
    EV_COMMAND_ENABLE(cmMakeRussian, CeMakeRussian),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TSDecoratedMDIFrame, TDecoratedMDIFrame)
    EV_WM_SIZE,
    EV_WM_TIMER,
END_RESPONSE_TABLE;

// *************************************************************
// *** class TStarDictClient :
// *************************************************************
TStarDictClient :: TStarDictClient(void)
                : TMDIClient(), pos(0, 0), posKeys(0, 0), articlePos(0)
{
    Attr.Style |= WS_TABSTOP;

    pTipFont = pTitleFont = pFont = pLinkFont = pKeywordFont = NULL;
    hcHand = hcUsual = NULL;
    pForeground = NULL;

    ReadProfileFont();
    CreateAllFonts();
}
TStarDictClient :: ~TStarDictClient()
{
    if (pForeground) delete pForeground;
    DestroyCursor(hcHand);
    WriteProfileFont();
    DeleteAllFonts();
}
/*void TStarDictClient :: EvKeyDown(uint key, uint repeatCount, uint flags)
{
    HWND p = GetLastActivePopup();
    if (p)
        ForwardMessage(p, FALSE);
} */
void TStarDictClient :: CreateAllFonts()
{
    DeleteAllFonts();

    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT) );

    TClientDC* dc = new TClientDC(*this);
    int yUnits = dc->GetDeviceCaps(LOGPIXELSY);
    delete dc;

    lf.lfHeight = -::MulDiv(9, yUnits, 72); // Size
    strcpy(lf.lfFaceName, "MS Sans Serif"); // Facename

    lf.lfCharSet = (BYTE) stdCharSet;//OEM_CHARSET;
    lf.lfItalic = lf.lfUnderline = lf.lfStrikeOut = 0;
    lf.lfWeight = FW_NORMAL;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = PROOF_QUALITY;
    lf.lfPitchAndFamily = FF_MODERN; // or FF_DONTCARE

    pTipFont = new TExFont( &lf, TColor::Black );

    CreateFontsDependable();
}
void TStarDictClient :: CreateFontsDependable()
{
    LOGFONT lf = data.LogFont;
    lf.lfWeight = FW_NORMAL;
    pFont = new TExFont(&lf, data.Color);

    // высота шрифта TitleFont ДОЛЖНА быть в два раза больше высоты обычного!!
    lf = data.LogFont;
    lf.lfWeight = FW_SEMIBOLD;
    lf.lfHeight = data.LogFont.lfHeight*2;
    pTitleFont = new TExFont(&lf, data.Color);

    lf = data.LogFont;
    lf.lfWeight = FW_NORMAL;// */FW_HEAVY;
    lf.lfUnderline = TRUE;
    pLinkFont = new TExFont(&lf, ColorLink);

    lf = data.LogFont;
    lf.lfWeight = FW_NORMAL;//FW_SEMIBOLD;//FW_EXTRALIGHT;
    lf.lfItalic = TRUE;
    pKeywordFont = new TExFont(&lf, ColorKeyword);
}
void TStarDictClient :: DeleteAllFonts()
{
    delete pTipFont; pTipFont = NULL;
    DeleteFontsDependable();
}
void TStarDictClient :: DeleteFontsDependable()
{
    delete pFont;
    delete pTitleFont;
    delete pLinkFont;
    delete pKeywordFont;

    pFont = pTitleFont = pLinkFont = pKeywordFont = NULL;
}
void TStarDictClient :: ReadProfileFont()
{
    TProfile pr(txtIniSectionFont, txtIniFileName);
    ShowTips = pr.GetInt(txtIniSectionFont_Tips, 1);
    BKGround = pr.GetInt(txtIniSectionFont_BK, BK_NOTHING);
    if (BKGround<0 || BKGround>NUM_BK_STRINGS-1) BKGround = BK_NOTHING;

    if (BKGround != BK_NOTHING) {
         TDib dib(GetApplication()->GetInstance(), bmpFon+BKGround);
         pForeground = new TBitmap(dib);
    }
    else pForeground = NULL;

    data.DC = 0;
    data.Flags = /*CF_FIXEDPITCHONLY | CF_EFFECTS | */CF_FORCEFONTEXIST |
                 CF_SCREENFONTS | CF_LIMITSIZE;
    data.FontType = SCREEN_FONTTYPE;
    data.SizeMin = 5;
    data.SizeMax = 30;

    char buf[100];

    pr.GetString(txtIniSectionFont_Color, buf, sizeof(buf)-2, "0x0"); // Color
    data.Color = strtoul(buf, NULL, NULL);
    data.Flags |= CF_ANSIONLY | CF_TTONLY;

    memset(&data.LogFont, 0, sizeof(LOGFONT) );
    data.LogFont.lfHeight = pr.GetInt(txtIniSectionFont_Size, -13); // Size
    pr.GetString(txtIniSectionFont_Face, data.LogFont.lfFaceName, LF_FACESIZE, "Courier new"); // Facename
    stdCharSet = data.LogFont.lfCharSet = (BYTE) pr.GetInt(txtIniSectionFont_Charset, OEM_CHARSET);

    data.LogFont.lfItalic = data.LogFont.lfUnderline = data.LogFont.lfStrikeOut = 0;
    data.LogFont.lfWeight = FW_NORMAL;
    data.LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    data.LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    data.LogFont.lfQuality = PROOF_QUALITY;
    data.LogFont.lfPitchAndFamily = FF_MODERN; // or FF_DONTCARE

    pr.GetString(txtIniSectionFont_ColorLink, buf, sizeof(buf)-1, "0x60BF60");
    ColorLink = TColor( strtoul(buf, NULL, NULL));
    pr.GetString(txtIniSectionFont_ColorKeyword, buf, sizeof(buf)-1, "0x0");
    ColorKeyword = TColor(strtoul(buf, NULL, NULL));
}
void TStarDictClient :: WriteProfileFont()
{
    TProfile pr(txtIniSectionFont, txtIniFileName);
    char buf[40] = "0x";
    ultoa(data.Color, buf+2, 16);
    pr.WriteString(txtIniSectionFont_Color,  buf);
    ultoa(ColorLink, buf+2, 16);
    pr.WriteString(txtIniSectionFont_ColorLink,  buf);
    ultoa(ColorKeyword, buf+2, 16);
    pr.WriteString(txtIniSectionFont_ColorKeyword,  buf);

    pr.WriteInt(txtIniSectionFont_Charset, data.LogFont.lfCharSet);
    pr.WriteInt(txtIniSectionFont_Size, data.LogFont.lfHeight);
    pr.WriteString(txtIniSectionFont_Face, data.LogFont.lfFaceName);
    pr.WriteInt(txtIniSectionFont_BK, BKGround);
    pr.WriteInt(txtIniSectionFont_Tips, ShowTips);
}
void TStarDictClient :: SetupWindow()
{
    TMDIClient :: SetupWindow();

    hcUsual = ::LoadCursor(NULL, IDC_ARROW);
    hcHand = GetModule()->LoadCursor(curMSHand);
    resModule->LoadString(stringLinkHere, strLinkHere, MAX_MENU_LEN-1);
    resModule->LoadString(stringLinkNew, strLinkNew, MAX_MENU_LEN-1);
    resModule->LoadString(stringCancel, strCancel, MAX_MENU_LEN-1);
    resModule->LoadString(stringArtCap, strArtCap, MAX_MENU_LEN-1);
//    SetCursor(GetModule(), curStarDict);
}
void TStarDictClient :: GetWindowClass(WNDCLASS& one)
{
    TMDIClient :: GetWindowClass(one);
    one.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
}
TArticleWindow* TStarDictClient :: InitChild()
{
    return new TArticleWindow(*this, ART_FIRST);
}
void TStarDictClient :: Paint(TDC& dc, BOOL b, TRect& r)
{
    TMDIClient :: Paint(dc, b, r);

    if (BKGround == BK_NOTHING || pForeground == NULL) return;

    TSize sz;
    int cx = pForeground->Width(), cy = pForeground->Height();

    TMemoryDC memdc(dc);
    memdc.SelectObject(*pForeground);

    for (int y=0; y<Attr.H; y+=cy)
        for (int x=0; x<Attr.W; x+=cx)
            dc.BitBlt(x, y, cx, cy, memdc, 0, 0, SRCCOPY);

    memdc.RestoreBitmap();
}
void TStarDictClient :: EvRButtonDown(uint, TPoint& point)
{
    ClientToScreen(point);
    TrackRButton(point);
}
void TStarDictClient :: TrackRButton(TPoint& point)
{
    HWND hwnd = GetApplication()->GetMainWindow()->HWindow;
    TMenu mn(hwnd);
    TPopupMenu pm(AutoDelete);
    char buf[100];

    mn.GetMenuString(cmShowArticleList, buf, sizeof(buf)-1, MF_BYCOMMAND);
    pm.AppendMenu(MF_STRING, cmShowArticleList, buf);
    mn.GetMenuString(cmShowThemeList, buf, sizeof(buf)-1, MF_BYCOMMAND);
    pm.AppendMenu(MF_STRING, cmShowThemeList, buf);
    mn.GetMenuString(cmSearchKeyword, buf, sizeof(buf)-1, MF_BYCOMMAND);
    pm.AppendMenu(MF_STRING, cmSearchKeyword, buf);
    pm.AppendMenu(0, MF_SEPARATOR, 0);
    mn.GetMenuString(cmChangeLook, buf, sizeof(buf)-1, MF_BYCOMMAND);
    pm.AppendMenu(MF_STRING, cmChangeLook, buf);
    mn.GetMenuString(cmShowFonts, buf, sizeof(buf)-1, MF_BYCOMMAND);
    pm.AppendMenu(MF_STRING, cmShowFonts, buf);
    pm.AppendMenu(0, MF_SEPARATOR, 0);
    pm.AppendMenu(MF_STRING, IDCANCEL, strCancel);

    pm.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point, 0, hwnd);
}
// ------------------------- commands : --------------------------------
void TStarDictClient :: CeMoveBack(TCommandEnabler& c)
{
    TArticleWindow* ch = (TArticleWindow*)GetActiveMDIChild();

    c.Enable( ch != NULL && ch->IsPrevExists());
}
void TStarDictClient :: CeMoveForward(TCommandEnabler& c)
{
    TArticleWindow* ch = (TArticleWindow*)GetActiveMDIChild();
    c.Enable( ch != NULL && ch->IsNextExists());
}
void TStarDictClient :: CmMoveBack()
{
    TMDIChild* ch = GetActiveMDIChild();
    if (ch)
        ch->PostMessage(wmMoveBack, 0, 0L);
}
void TStarDictClient :: CmMoveForward()
{
    TMDIChild* ch = GetActiveMDIChild();
    if (ch)
        ch->PostMessage(wmMoveForward, 0, 0L);
}
void TStarDictClient :: CeMakeEnglish(TCommandEnabler& c)
{
    c.SetCheck( stdLanguage == LANG_ENGLISH );
}
void TStarDictClient :: CeMakeRussian(TCommandEnabler& c)
{
    c.SetCheck( stdLanguage == LANG_RUSSIAN );
}
void TStarDictClient :: CeEnablePopup(TCommandEnabler& c)
{
    c.Enable(TRUE);
}
void TStarDictClient :: CmChangeLook()
{
    TChangeLookDialog* p = new TChangeLookDialog(this, dlgChangeLook, pTipFont);
    p->Execute();
}
void TStarDictClient :: CmMakeRussian()
{
    if (stdLanguage != LANG_RUSSIAN) {
        stdLanguage = LANG_RUSSIAN;
        fLangChanged = TRUE;
        Parent->PostMessage(WM_QUIT, 0, 0); // второе (это) приложение закрываем
    }
}
void TStarDictClient :: CmMakeEnglish()
{
    if (stdLanguage != LANG_ENGLISH) {
        stdLanguage = LANG_ENGLISH;
        fLangChanged = TRUE;
        Parent->PostMessage(WM_QUIT, 0, 0); // второе (это) приложение закрываем
    }
}
void TStarDictClient :: CmEsc()
{}
void TStarDictClient :: CmSearchKeyword()
{
    TKeywordDialog* p = new TKeywordDialog(this, dlgKeyword, &posKeys, pTipFont);
    p->Execute();
}
void TStarDictClient :: CmShowThemeList()
{
    TThemeDialog* p = new TThemeDialog(this, dlgTheme, &pos, pTipFont);
    p->Execute();
}
void TStarDictClient :: CmShowArticleList()
{
    TArticleDialog* a = new TArticleDialog(this, dlgArticle, &articlePos, pTipFont);
    a->Execute();
}
void TStarDictClient :: CmShowFonts()
{
    if (pFont) data.Flags |= CF_INITTOLOGFONTSTRUCT;
    TChooseFontDialog* d = new TChooseFontDialog(this, data);
    if (d->Execute() == IDOK) {
        DeleteFontsDependable();

        data.LogFont.lfItalic = data.LogFont.lfUnderline = data.LogFont.lfStrikeOut = 0;
        data.LogFont.lfWeight = FW_NORMAL;

        CreateFontsDependable();
        ChildBroadcastMessage(wmInvalidate, 0, 0);
    }
}
void TStarDictClient :: CmQuitDict()
{
    Parent->HandleMessage(WM_COMMAND, CM_EXIT, 0);
}
void TStarDictClient :: CmAbout()
{
    WinHelp(txtHelpFileName, HELP_CONTENTS, 0L);
}
void TStarDictClient :: CmAboutProgrammers()
{
    char buf[15];
    sprintf(buf, "%d", InstallLeft);
    TDialog* dlg = new TAboutDialog(this, dlgAbout, RegNumber, UserName, buf, pTipFont);
    dlg->Execute();
}
LPARAM TStarDictClient :: WmBKChanged(WPARAM, LPARAM)
{
    TBitmap* f = pForeground;

    if (BKGround != BK_NOTHING && BKGround < NUM_BK_STRINGS) {
        TDib dib(GetApplication()->GetInstance(), bmpFon+BKGround);
        pForeground = new TBitmap(dib);
    }
    else pForeground = NULL;

    delete f;
    Invalidate();
    return 0L;
}
LPARAM TStarDictClient :: WmNewArticle(WPARAM num, LPARAM)
{
    TArticleWindow* p = new TArticleWindow(*this, num);
    p->Create();
    if (AfterOpen == AO_WIDEN)
        p->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    return 0;
}
// ********************************************************************
// *** class TSDecoratedMDIFrame :
// ********************************************************************
TSDecoratedMDIFrame :: TSDecoratedMDIFrame(LPCSTR t, TResId id, TStarDictClient& c)
          : TDecoratedMDIFrame(t, id, c, TRUE)
{
    fTimerIsOn = FALSE;
    OldTips = TRUE;
    Drawing = NULL;
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
    curFrame = 0;
    HINSTANCE hin = GetApplication()->GetInstance();
    increment = 1;
    fr[0] = new TIcon(hin, icoFrame1);
    fr[1] = new TIcon(hin, icoFrame2);
    fr[2] = new TIcon(hin, icoFrame3);
    fr[3] = new TIcon(hin, icoFrame4);
    fr[4] = new TIcon(hin, icoFrame5);
    fr[5] = new TIcon(hin, icoFrame6);
}
TSDecoratedMDIFrame :: ~TSDecoratedMDIFrame()
{
    if (fTimerIsOn) KillTimer(idLocalTimer); fTimerIsOn = FALSE;
    WinHelp(txtHelpFileName, HELP_QUIT, 0L);
    TProfile pr(txtIniSectionSize, txtIniFileName);
    pr.WriteInt(txtIniSectionSize_Max, isMaximized);
    pr.WriteInt(txtIniSectionSize_X, Attr.X);
    pr.WriteInt(txtIniSectionSize_Y, Attr.Y);
    pr.WriteInt(txtIniSectionSize_W, Attr.W);
    pr.WriteInt(txtIniSectionSize_H, Attr.H);
    delete fr[0];
    delete fr[1];
    delete fr[2];
    delete fr[3];
    delete fr[4];
    delete fr[5];
    if (Drawing) {
        Drawing->Destroy();
        delete Drawing;
        Drawing = NULL;
    }
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
//#if !defined (_STD_DEBUG)
    ShowDrawing();
//#endif

    SetCurrentIcon();

    // Порядок следующих двух строк важен. Почему - долго объяснять. :)
    if (isMaximized) PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    SetMenu( resModule->LoadMenu(mnuMainMenu) );
    switch (AfterRun) {
        case AR_CHOOSEART   :   PostMessage(WM_COMMAND, cmShowArticleList, 0L); break;
        case AR_CHOOSETH    :   PostMessage(WM_COMMAND, cmShowThemeList, 0L); break;
        case AR_SEARCHKEYWORD:  PostMessage(WM_COMMAND, cmSearchKeyword, 0L); break;
    }

    SetTimer(idLocalTimer, TIMER_TICK, NULL);
    fTimerIsOn = TRUE;
}
void TSDecoratedMDIFrame :: SetCurrentIcon()
{
    if (curFrame > 5 || curFrame < 0) return;
    HICON h = (HICON) *fr[curFrame];
    ::SetClassWord(HWindow, GCW_HICON, (WORD)h);
}
void TSDecoratedMDIFrame :: EvTimer(uint id)
{
    static int count = 0;
    if (id!=idLocalTimer) return;
    count += TIMER_TICK;
    if (Drawing && count >= TIME_TO_SHOW_LOGO) {
        Drawing->Destroy();
        delete Drawing;
        Drawing = NULL;
        ShowTips = OldTips;
        count = 0;
    }
//    if (fTimerIsOn) KillTimer(idLocalTimer);
//    fTimerIsOn = FALSE;
    curFrame += increment;
    if (curFrame >=5 || curFrame<=0) increment= -increment;
    SetCurrentIcon();
}
void TSDecoratedMDIFrame :: ShowDrawing()
{
    OldTips = ShowTips;
    ShowTips = FALSE;
    Drawing = new TDrawingWindow(this, GetApplication()->GetInstance(), ::Module);
    Drawing->Create();
    Drawing->ShowWindow(SW_SHOW);
}
// ********************************************************************
// *** class TStarDictApp :
// ********************************************************************
TStarDictApp :: TStarDictApp(LPSTR t)/*, HINSTANCE hInstance,
                             HINSTANCE hPrevInstance, LPSTR lpCmdLine,
                             int nCmdShow)*/
              : TApplication(t)//, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
{
    TProfile pr(txtIniSectionAppl, txtIniFileName);
    AfterRun = (int)pr.GetInt(txtIniSectionAppl_AR, AR_CHOOSEART);
    AfterOpen = (int)pr.GetInt(txtIniSectionAppl_AO, AO_WIDEN);
    if (AfterRun<0 || AfterRun>NUM_AR_STRINGS-1) AfterRun = AR_NOTHING;
    if (AfterOpen<0 || AfterOpen>NUM_AO_STRINGS-1) AfterOpen = AO_NOTHING;

    stdLanguage = pr.GetInt(txtIniSectionAppl_Lang, LANG_RUSSIAN);
    InstallLeft = pr.GetInt(txtIniSectionAppl_Inst, 1);

    try {
        switch (stdLanguage) {
            case LANG_RUSSIAN :  resModule = new TModule("stdrus.dll", TRUE);
                                 break;

            case LANG_ENGLISH :
            default :            resModule = new TModule("stdeng.dll", TRUE);
                                 break;
        }
    }

    catch (TXInvalidModule) {
        throw TSTDError("V\\" __FILE__, __LINE__, EC_NODLL, 0);
                        /*"Cannot find resource library"*/
    }
    CheckInstallation();
    OpenDataFiles();
    CheckDataFiles();
}
TStarDictApp :: ~TStarDictApp()
{
    TProfile pr(txtIniSectionAppl, txtIniFileName);
    pr.WriteInt(txtIniSectionAppl_Lang, stdLanguage);
    pr.WriteInt(txtIniSectionAppl_AR, AfterRun);
    pr.WriteInt(txtIniSectionAppl_AO, AfterOpen);

    if (resModule) delete resModule; resModule = NULL;

    CloseDataFiles();
}
void TStarDictApp :: InitializeBars(TDecoratedMDIFrame* frame)
{
    controlBar = new TControlBar(frame, TGadgetWindow::Horizontal);
    statusBar = new TStatusBar(frame, TGadget::Recessed, 0);
    TExFont* tip = stdClient->pTipFont;

    controlBar->Insert(*new TTipButton(bmpBack, wmMoveBack, tip));
    controlBar->Insert(*new TTipButton(bmpForw, wmMoveForward, tip));
    controlBar->Insert(*new TSeparatorGadget(20) );

    controlBar->Insert(*new TTipButton(bmpArticles, cmShowArticleList, tip));
    controlBar->Insert(*new TTipButton(bmpThemes, cmShowThemeList, tip));
    controlBar->Insert(*new TTipButton(bmpSearch, cmSearchKeyword, tip));
    controlBar->Insert(*new TSeparatorGadget(20) );

    controlBar->Insert(*new TTipButton(bmpAbout, cmAbout, tip));
    controlBar->Insert(*new TTipButton(bmpExit, cmQuitDict, tip));

    controlBar->SetHintMode(TGadgetWindow :: EnterHints);

    char lpstr[30];
    resModule->LoadString(stringStatus, lpstr, sizeof(lpstr)-1);
    statusBar->Insert(
        *(sbtxtStatus = new TTextGadget(0, TGadget::Recessed, TTextGadget::Left, strlen(lpstr)-5, lpstr)), TGadgetWindow::Before );
#if defined (_STD_DEBUG)
    statusBar->Insert(
        *new TTextGadget(0, TGadget::Recessed, TTextGadget::Left, 25, chTime), TGadgetWindow::Before );
#endif
}
void TStarDictApp :: InitMainWindow()
{
    DWORD v = GetVersion();
    int not_nt = ((HIWORD(v) & 0x8000) != 0);
    WORD win_ver = LOWORD(v);

    BYTE major = LOBYTE(win_ver);
    BYTE minor = HIBYTE(win_ver);
    if ( not_nt && major<=3 && minor<95 )
         Windows31 = TRUE;
    else Windows31 = FALSE;

#if defined (_STD_DEBUG)
//    char buff[256];
//    sprintf(buff, "You are not in Windows NT: %d,\nHIWORD=%X \tLOWORD=%X\nWindows 3.1 = %d, \tversion %d.%02d",
//                   not_nt, HIWORD(v), LOWORD(v), Windows31, major, minor);
//    MessageBox(NULL, buff, "Debug message", MB_OK | MB_ICONINFORMATION );
#endif

    EnableCtl3d(TRUE);

    TSDecoratedMDIFrame* frame =
          new TSDecoratedMDIFrame("Star Dictionary",
                                  0, // установим меню позже.
                                  *(stdClient = new TStarDictClient));

    InitializeBars(frame);
    if (controlBar) frame->Insert(*controlBar, TDecoratedFrame :: Top);
    if (statusBar) frame->Insert(*statusBar, TDecoratedFrame :: Bottom);

    SetMainWindow(frame);

    MainWindow->SetIcon(this, icoStarDict);
    MainWindow->SetCursor(NULL, IDC_ARROW);
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

        HAccTable = LoadAccelerators(accMainMenu);
        randomize();
    }
}
void TStarDictApp :: CheckInstallation()
{
    // in section [Compatibility]
    // in key VBVI
    // i wait for exact string 0x0111
    char buffer[250];
    char section[40] = "";
    char key[40] = "";
    char ideal[40] = "";

    strcat(section, "Com");  strcat(key, "V"); strcat(ideal, "0x");
    strcat(section, "pat");  strcat(key, "B"); strcat(ideal, "01");
    strcat(section, "ibi");  strcat(key, "V"); strcat(ideal, "11");
    strcat(section, "lity"); strcat(key, "I");

    ::GetProfileString(section, key, "", buffer, sizeof(buffer));
    int res = strcmpi(buffer, ideal);
    if (res != 0 )
        throw TSTDError("", __LINE__, EC_NOLICENSE, 0);

    CheckNames();
}
void TStarDictApp :: CheckNames()
{
    char UNcoded[100];
    char UNtest[100];
    TProfile pr(txtIniSectionAppl, txtIniFileName);

    pr.GetString(txtIniSectionAppl_UN, UserName, sizeof(UserName), "");
    pr.GetString(txtIniSectionAppl_UNcoded, UNcoded, sizeof(UNcoded), "***");

    if (strlen(UserName) < CODE_MIN_STRINGLEN)
        throw TSTDError("", __LINE__, EC_WRONGUSERNAME, 0);

    CodeUserName(UNtest, UserName, sizeof(UNtest));
    if ( strcmp(UNtest, UNcoded) != 0 )
        throw TSTDError("", __LINE__, EC_WRONGUSERNAME, 0);

    pr.GetString(txtIniSectionAppl_RN, RegNumber, sizeof(RegNumber), "");
    pr.GetString(txtIniSectionAppl_RNcoded, UNcoded, sizeof(UNcoded), "***");

    if (strlen(RegNumber) < CODE_MIN_STRINGLEN)
        throw TSTDError("", __LINE__, EC_WRONGREGNUM, 0);

    CodeRegNum(UNtest, RegNumber, sizeof(UNtest));
    if (strcmp(UNtest, UNcoded) != 0 )
        throw TSTDError("", __LINE__, EC_WRONGREGNUM, 0);
}
// **************************************************************************
int OwlMain(int a, char* b[])
{
    TStarDictApp* ap = NULL;
    fLangChanged = TRUE;
    int result;
    if (a>1 && strcmpi(b[1], "-showtriangles")==0 )
         fShowTriangles = TRUE;
    else fShowTriangles = FALSE;

//#if defined(_STD_DEBUG)
//    if (a>1 && strcmpi(b[1], "-registervbvi")==0 )
//        ::WriteProfileString("Compatibility", "VBVI", "0x0111");
//#endif

    try {
        while (fLangChanged) {
            fLangChanged = FALSE;

            ap = new TStarDictApp("StarDictApp");
            result = ap->Run();

            delete ap; ap = NULL;
        }
    }
    catch (TSTDError s) {
        s.ProcessException();
    }
    if (ap) delete ap; ap = NULL;
    if (resModule) delete resModule; resModule=NULL;

    return result;
}

