/* *************************************************************************
                                BASEDLG.C
    v2.0
    Реализация базовых классов.
    Классы:
        TExFont,
        TBaseDialog.
************************************************************************* */

#include <windows.h>
#include <owl\gadgetwi.h>
#include <owl\button.h>
#include <owl\buttonga.h>
#include <owl\dialog.h>
#include <stdio.h>

#pragma hdrstop

#include "sd_defs.h"
//#include "stddef.h"
#include "bd.h"

extern TModule* resModule;

BOOL fShowTriangles = FALSE;
BOOL Windows31 = TRUE;

DEFINE_RESPONSE_TABLE1(TBaseDialog, TDialog)
    EV_WM_PAINT,
    EV_COMMAND(idHelp, CmHelp),
END_RESPONSE_TABLE;

// ***************************************************************************
// *** class TExFont :
// ***************************************************************************
TExFont :: TExFont(LOGFONT* logfont, TColor c)
         : TFont(logfont)
{
    Color = c;
    InitAllWithFonts();
}
void TExFont :: InitAllWithFonts()
{
// Должны изменяться при изменении системных шрифтов.
// !+ Пока автоматическое изменение не реализовано.
    SpaceLen = chHeight = chExternal = chInternal = 0;

    TScreenDC vlad_dc;
    TMemoryDC dc(vlad_dc);
    dc.SetTextAlign(TA_UPDATECP);
    dc.SelectObject(*this);

    TEXTMETRIC n;
    dc.GetTextMetrics(n);
    chInternal = n.tmInternalLeading;
    chExternal = n.tmExternalLeading;
    chHeight = n.tmHeight;// + chExternal;//TIP_EXTERNAL*2;
    chMaxWidth = n.tmMaxCharWidth;
    chAveWidth = n.tmAveCharWidth;

    OUTLINETEXTMETRIC tm;
    dc.GetOutlineTextMetrics(sizeof(OUTLINETEXTMETRIC), tm);
    chAscent = tm.otmAscent;
    chDescent = tm.otmDescent;

    dc.GetCharWidth(0, 255, lpSizes); // массив ширин символов выбранного шрифта.
    memset(lpSizes, 0, 32*sizeof(lpSizes[0]));
    SpaceLen = lpSizes[ 32 ]; // ширина пробела.
}
int TExFont :: CountLengthInPixels(LPSTR txt)
{
    LPBYTE temp = (LPBYTE)txt; // главное - чтобы temp указывал на unsigned char!
    int result =0;
    while (*temp != '\0')
        result += lpSizes[ *temp++ ];

    return result;
}
// ***************************************************************************
// *** class TBaseDialog :
// ***************************************************************************
TBaseDialog :: TBaseDialog(TWindow* parent, TResId id, TExFont* tip, TResId bmp, int fUseGlobalModule)
             : TDialog(parent, id, (fUseGlobalModule) ? (::Module) : (resModule) )
{
    tipFont = tip;
    if (fShowTriangles && bmp!=TResId(0)) {
       TDib d(GetApplication()->GetInstance(), bmp);
       d.MapColor(TColor::LtGray, ::GetSysColor(COLOR_BTNFACE) );
       triangle = new TBitmap(d);
    }
    else triangle = NULL;
}
TBaseDialog :: ~TBaseDialog()
{
    if (triangle) delete triangle;
}
void TBaseDialog :: SetupWindow()
{
    TDialog::SetupWindow();
    if (Windows31 && tipFont)
          ChildBroadcastMessage(WM_SETFONT,
                                (WPARAM)HFONT(*tipFont),//(WPARAM) GetStockObject(ANSI_VAR_FONT),
                                MAKELPARAM(TRUE, 0)  );

    // а теперь установим окошко ровно посередине:
    TRect r;
    GetWindowRect(r);
    SetWindowPos(NULL, (GetSystemMetrics(SM_CXFULLSCREEN) - (r.right-r.left) ) /2,
                       (GetSystemMetrics(SM_CYFULLSCREEN) - (r.bottom-r.top) ) /2,
                       0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
void TBaseDialog :: EvPaint()
{
    if (triangle) {
        TPaintDC* dc = new TPaintDC(HWindow);

        TMemoryDC memDC(*dc);

        memDC.SelectObject(*triangle);
        dc->BitBlt(0, 0, triangle->Width(), triangle->Height(), memDC, 0, 0, SRCCOPY);

        memDC.RestoreBitmap();
        delete dc;
    }

    TDialog :: EvPaint();
}
void TBaseDialog :: CmHelp()
{
    int id =(int)(uint32)Attr.Name;
    WinHelp(txtHelpFileName, HELP_CONTEXT, (DWORD)id);
}

