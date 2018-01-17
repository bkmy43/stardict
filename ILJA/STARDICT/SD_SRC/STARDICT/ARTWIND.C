/* **************************************************************************
                                 ARTWIND.C
    v2.0
    Содержит реализацию класса TArticleWindow и еще мелочь разную.
    Классы:
        TLink,
        TLinksList,
        TArticleWindow.
    (c) В.Перов.
************************************************************************** */

#include <windows.h>
#include <owl\window.rh>
#include <owl\mdi.rh>
#include <owl\decmdifr.h>
#include <owl\mdi.h>
#include <owl\mdichild.h>
#include <owl\choosefo.h>
#include <owl\scroller.h>
#include <owl\button.h>
#include <owl\buttonga.h>
#include <stdio.h>

#pragma hdrstop

#include "exart.h"

#include "ex.h"
#include "tags.h"
#include "stddef.h"
#include "bd.h"
#include "hst.h"
#include "nbtn.h"
#include "artw.h"
#include "std.h"

// знаечние флага: говорящего о том, что мы обрабатываем ссылку.
#define  tagLinkProcessing            1
// значение флага : говорит о том, что обрабатывается ключевое слово
#define  tagKeywordProcessing         2 

static char redLine[] = "        " ; // spaces
static int  fNoWrap = 0;
static TRect rDraw;
static int MaxX = 0;

DEFINE_RESPONSE_TABLE1(TArticleWindow, TMDIChild)
    EV_WM_MOUSEMOVE,
    EV_WM_KEYDOWN,
    EV_WM_SYSKEYDOWN,
    EV_WM_LBUTTONDOWN,
    EV_WM_RBUTTONDOWN,
    EV_WM_SIZE,
    EV_MESSAGE(wmInvalidate, EvInvalidate),
    EV_COMMAND(cmOpenHere, CmOpenHere),
    EV_COMMAND(cmOpenNew, CmOpenNew),
    EV_COMMAND(IDCANCEL, CmEsc),
    EV_COMMAND(wmMoveBack, ProcessMoveBack),
    EV_COMMAND(wmMoveForward, ProcessMoveForward),
END_RESPONSE_TABLE;

// *************************************************************
// *** class TLink & TLinksList :
// *************************************************************
TLink :: TLink(TRect& a, int num)
{
    r = a;
    numberOfArt = num;
    next = NULL;
}
BOOL TLink :: IsPointInside(TPoint& p)
{
    return r.Contains(TPoint(p.x, p.y));
}
BOOL TLink :: IsLinkInsideRect(TRect& rt)
{
    return rt.Contains(r);
}
// -------------------
TLinksList :: TLinksList()
{
    head = NULL;
}
TLinksList :: ~TLinksList()
{
    ClearList();
}
void TLinksList :: ClearList()
{
    TLink* t=head;
    while ( t != NULL) {
        head = head->next;
        delete t;
        t = head;
    }
}
void TLinksList :: AddLink(TLink* link)
{
    link->next = NULL;
    if (head == NULL) {
        head = link;
    }
    else {
        TLink* cur=head;
        for (; cur->next; cur=cur->next);
        cur->next=link;
    }
}
void TLinksList :: AddLink(TRect& r, int num)
{
    TLink* link = new TLink(r, num);
    AddLink(link);
}
TLink* TLinksList :: IsPointInsideLink(TPoint& p, int YPos)
{
    TPoint pp(p.x, p.y+YPos);
    for (TLink* cur=head; cur; cur=cur->next)
        if (cur->IsPointInside(pp)) return cur;

    return NULL;
}
TLink* TLinksList :: GetFirstLinkInsideRect(TRect& r, int YPos)
{
    TRect rt(r.left, r.top+YPos, r.right, r.bottom+YPos);
    for (TLink* cur=head; cur; cur=cur->next)
        if (cur->IsLinkInsideRect(rt)) return cur;

    return NULL;
}
// *************************************************************
// *** class TArticleWindow :
// *************************************************************
TArticleWindow :: TArticleWindow(TStarDictClient& parent, int num)
            : TMDIChild(parent, NULL), linksList(),
              Article(num), History()
{
    stdParent = &parent;
    pCurrentFont = NULL;
    fHandCursor = FALSE;
    curLink = tabLink = NULL;
    fRButtonPressed = FALSE;
    Attr.Style |= WS_VSCROLL | WS_HSCROLL;
    FontsToDelete = 0;
    TopOfStack = 0;
    History.AddArticleToHistory(Article, 0);
//    EnableKBHandler();
}
TArticleWindow :: ~TArticleWindow()
{
    if (FontsToDelete != 0 || TopOfStack != 0) {
        TSTDError s("V\\" __FILE__, __LINE__, EC_ERRORFONTTAG);
        s.ProcessException();
    }
    stdParent->ChildBroadcastMessage(wmInvalidate, 0, 0);
}
void TArticleWindow :: SetupWindow()
{
    SetArticleCaption();

    Scroller = new TScroller(this, stdParent->pFont->SpaceLen, stdParent->pFont->chHeight, 100, 100);
    ShowScrollBar(SB_BOTH, TRUE);
    ::SetClassWord(HWindow, GCW_HCURSOR, (WORD)stdParent->hcUsual);
    ::SetCursor(stdParent->hcUsual);

// !+ Проверить - не забираются ли ресурсы при очень частом чтении иконки из модуля
    SetIcon(GetModule(), icoArticle);
}
void TArticleWindow :: GetWindowClass(WNDCLASS& one)
{
    TMDIChild :: GetWindowClass(one);
    one.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
}
void TArticleWindow :: SetArticleCaption()
{
    char out[256];
    wsprintf(out, "%s \"%s\"", stdParent->strArtCap, Article.GetArtName());
    SetCaption(out);
}
void TArticleWindow :: EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
    if ( (flags & ( 1 << 13)) == 0 ) { // Alt is not pressed
        TMDIChild :: EvSysKeyDown(key, repeatCount, flags);
        return;
    }
    TPoint p;
    TArticleWindow :: GetCursorPos(p);
    switch (key) {

        case VK_F10     :       fRButtonPressed = TRUE;
                                stdParent->TrackRButton(p);
                                break;

        default         :       TMDIChild :: EvSysKeyDown(key, repeatCount, flags);
                                break;
    }

}
/*void TArticleWindow :: ProcessTab()
{
    TRect r = GetWindowRect();
//    TRect shift(0, (int)Scroller->YPos, 0, (int)Scroller->YPos);
    int YPos = (int)Scroller->YPos;

    TLink* first = linksList.GetFirstLinkInsideRect(r, YPos);
    if (first == NULL) return;

    TLink* old = tabLink;
    if (tabLink == NULL)
        tabLink = first;
    else {
        tabLink=tabLink->Next();
        if (tabLink == NULL || !tabLink->IsLinkInsideRect(r) ) tabLink = first;
    }

    TClientDC dc(HWindow);
    if (old)
        dc.DrawFocusRect(old->r.left, old->r.top+YPos, old->r.right, old->r.bottom+YPos);
    dc.DrawFocusRect(tabLink->r.left, tabLink->r.top+YPos, tabLink->r.right, tabLink->r.bottom+YPos);
}*/
void TArticleWindow :: DoChangeArticle(int num)
{
    try {
        Article.ChangeArt(num);
    }
    catch (TSTDError s) {
        s.ProcessException();
    }
}
void TArticleWindow :: ProcessMoveForward()
{
    if (!History.IsNextExists()) return;

    DoChangeArticle(History.GetNextArticle()->GetArtNum());
    OpenArticle();
}
void TArticleWindow :: ProcessMoveBack()
{
    if (! History.IsPrevExists()) return;

    DoChangeArticle(History.GetPrevArticle()->GetArtNum());
    OpenArticle();
}
void TArticleWindow :: EvKeyDown(uint key, uint repeatCount, uint flags)
{
    switch (key) {

        case VK_DOWN  :         Scroller->ScrollBy(0, 1); break;
        case VK_UP    :         Scroller->ScrollBy(0, -1); break;
        case VK_NEXT  :         Scroller->ScrollBy(0, Scroller->YPage); break;
        case VK_PRIOR :         Scroller->ScrollBy(0, -Scroller->YPage); break;
        case VK_HOME  :         Scroller->ScrollTo(0, 0); break;
        case VK_END   :         Scroller->ScrollTo(0, Scroller->YRange); break;
        case VK_RIGHT :         Scroller->ScrollBy(10, 0); break;
        case VK_LEFT  :         Scroller->ScrollBy(-10, 0); break;

//        case VK_TAB   :         ProcessTab(); break;

        default       :         TMDIChild :: EvKeyDown(key, repeatCount, flags); break;
    }
    SetCursorToUsual();
}
void TArticleWindow :: EvRButtonDown(uint , TPoint& point)
{
    fRButtonPressed = TRUE;
    ClientToScreen(point);
    if (curLink != NULL ) {
        ::SetClassWord(HWindow, GCW_HCURSOR, (WORD)stdParent->hcUsual);
        ::SetCursor(stdParent->hcUsual);
        TPopupMenu popupMenu(AutoDelete);
#if defined (_STD_DEBUG)
        char buf[256];
        sprintf(buf, "%s: link No %d", stdParent->strLinkHere, curLink->GetNumberOfArt() );
        popupMenu.AppendMenu(MF_STRING, cmOpenHere, buf);
#else
        popupMenu.AppendMenu(MF_STRING, cmOpenHere, stdParent->strLinkHere);
#endif
        popupMenu.AppendMenu(MF_STRING, cmOpenNew, stdParent->strLinkNew);
        popupMenu.AppendMenu(0, MF_SEPARATOR, 0);
        popupMenu.AppendMenu(MF_STRING, IDCANCEL, stdParent->strCancel);

        popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
                                 point, 0, HWindow);
    }
    else stdParent->TrackRButton(point);
}
void TArticleWindow :: EvLButtonDown(uint , TPoint&)
{
    if (curLink != NULL && !fRButtonPressed) {
         CmOpenHere();
         SetCursorToUsual();
    }
    else fRButtonPressed = FALSE;
}
void TArticleWindow :: SetCursorToUsual()
{
    fHandCursor = FALSE;
// эти двес строки меняют курсор. Хотя на первый взгляд кажется, что достанточно
// ::SetCursor - но этого очень даже мало.
    ::SetClassWord(HWindow, GCW_HCURSOR, (WORD)stdParent->hcUsual);
    ::SetCursor(stdParent->hcUsual);
}
void TArticleWindow :: SetCursorToHand()
{
    fHandCursor = TRUE;
    ::SetClassWord(HWindow, GCW_HCURSOR, (WORD)stdParent->hcHand);
    ::SetCursor(stdParent->hcHand);
}
void TArticleWindow :: EvMouseMove(uint , TPoint& p)
{
    curLink = linksList.IsPointInsideLink(p, (int)(Scroller->YPos*Scroller->YUnit));
    if (curLink && fHandCursor==FALSE)
        SetCursorToHand();
    else if ( curLink==NULL && fHandCursor)
        SetCursorToUsual();
}
void TArticleWindow :: EvSize(uint sizeType, TSize& size)
{
    TMDIChild::EvSize(sizeType, size);
    linksList.ClearList();
    if (sizeType == SIZE_MAXIMIZED || sizeType == SIZE_RESTORED) {
        Attr.H = size.cy;
        Attr.W = size.cx;
        Invalidate(TRUE);
    }
}
LPARAM TArticleWindow :: EvInvalidate(WPARAM, LPARAM)
{
    Scroller->SetUnits(stdParent->pFont->SpaceLen, stdParent->pFont->chHeight);
    linksList.ClearList();
    Invalidate(TRUE);
    fRButtonPressed = FALSE;
    curLink = NULL;
    return 0;
}
// ----------------------------------------- Commands :
void TArticleWindow :: CmEsc()
{
    fRButtonPressed = FALSE;
}
void TArticleWindow :: CmOpenHere()
{
    DoChangeArticle(curLink->GetNumberOfArt());
    History.AddArticleToHistory(Article, 0);
    OpenArticle();
}
void TArticleWindow :: OpenArticle()
{
    linksList.ClearList();

    SetArticleCaption();
    Scroller->ScrollTo(0, 0);
    Invalidate(TRUE);
    SetCursorToUsual();

    curLink = NULL;
    fRButtonPressed = FALSE;
}
void TArticleWindow :: CmOpenNew()
{
    stdParent->PostMessage(wmNewArticle, (WPARAM)curLink->GetNumberOfArt(), 0L);
    fRButtonPressed = FALSE;
}
// ****************************************************************** Paint :
void TArticleWindow :: Paint(TDC& dc, BOOL, TRect& r)
{
    linksList.ClearList();
    rDraw = r;
    rDraw.left = 0;
    rDraw.right = max(Attr.W, rDraw.right);
    MaxX = 0;
    dc.SetMapMode(MM_TEXT);
    NumberOfLines = 0;
    DispatchString(dc, Article.GetArtBody());
    TPoint p;
    dc.GetCurrentPosition(p);
    NumberOfLines = p.y / stdParent->pFont->chHeight + 1;
    int t = (MaxX - Attr.W + stdParent->pFont->chAveWidth)/stdParent->pFont->SpaceLen;
    Scroller->SetRange(t, NumberOfLines-Scroller->YPage );
}
// ------------------------------------------------------- tag processing :
TArticleTags TArticleWindow :: FindFirstTag(LPSTR art, WORD& len)
{
    LPBYTE cur = (LPBYTE)art;
    while (*cur++ >= TAG_NONEXISTTAG ) ;
    len = (WORD)(cur - (LPBYTE)art)-1;
    return (TArticleTags)*(cur-1);
}
void TArticleWindow :: DoEndOfLine(TDC& dc)
{
    ++NumberOfLines;
    int mm = pCurrentFont->chHeight;
    TExFont* old = pCurrentFont;
    if ( CurrentMode & tagLinkProcessing ) {
        DoLinkEnd(dc);
        CurrentMode |= tagLinkProcessing;
    }

    TPoint p;
    dc.GetCurrentPosition(p);
    p.x = MarginLeft;
    if (mm<old->chHeight)
         p.y += old->chHeight;
    else p.y += mm;
    dc.MoveTo(p);

    if ( CurrentMode & tagLinkProcessing )
        DoLinkBegin(dc, NULL, TRUE);
}
LPSTR TArticleWindow :: WordOut(TDC& dc, LPSTR art)
{
    char buf[256];
    int i=0;
    while (i<sizeof(buf) && ((BYTE)*art)>=TAG_NONEXISTTAG && !IsCharAlphaNumeric(*art) )
        buf[i++] = *art++;

    while (i<sizeof(buf) && IsCharAlphaNumeric(*art))
        buf[i++] = *art++;

    while (i<sizeof(buf) && ((BYTE)*art)>=TAG_NONEXISTTAG && ( (BYTE)*art != '\x20' ))
        buf[i++] = *art++;

    while (i<sizeof(buf) && ( (BYTE)*art == '\x20' ))
        buf[i++] = *art++;

    buf[i] = '\0';
    int len = pCurrentFont->CountLengthInPixels(buf);

    TRect r = rDraw.InflatedBy(0, pCurrentFont->chHeight);
    TPoint p;
    dc.GetCurrentPosition(p);
    if (fNoWrap == 0 && p.x + len > Attr.W-GetSystemMetrics(SM_CXBORDER)*2 - MarginLeft ) {
            DoEndOfLine(dc);
            dc.GetCurrentPosition(p);
//            len = pCurrentFont->CountLengthInPixels(buf);
    }

    if (r.Contains(p)) {
         dc.TextOut(0, 0, buf, strlen(buf));
         dc.GetCurrentPosition(p);
    }
    else {
        p.x += len;
        dc.MoveTo(p);
    }
    MaxX = max (p.x, MaxX);
    return art;
}
void TArticleWindow :: WrappedTextOut(TDC& dc, int , int , LPSTR art, int len)
{
    LPSTR str=art;
    while (art-str<len)
         art = WordOut(dc, art); // печатается слово
}
void TArticleWindow :: DoParagraph(TDC& dc, BYTE prop)
{
    if ( prop & TXP_REDLINE) {
        TRect r = rDraw.InflatedBy(0, pCurrentFont->chHeight + pCurrentFont->chExternal);
        TPoint p;
        dc.GetCurrentPosition(p);
        if (p.y >0) {
            DoEndOfLine(dc);
            dc.GetCurrentPosition(p);
        }
        if (r.Contains(p))
            dc.TextOut(0, 0, redLine, sizeof(redLine)-1 );
        else {
            p.x += pCurrentFont->CountLengthInPixels(redLine);
            dc.MoveTo(p);
        }
        return;
    }
}
void TArticleWindow :: DoLinkBegin(TDC& dc, LPSTR* art, BOOL skipLinkNum)
{
    CurrentMode |= tagLinkProcessing;
    TPoint p;
    dc.GetCurrentPosition(pBegin);
    pBegin.y -= pCurrentFont->chAscent;
    if (!skipLinkNum) {
        curLinkNum = * ((int*) *art);
        *art += sizeof(int);
    }

    PushPreviousFontAndChooseThis(
            dc,
            stdParent->pLinkFont
    );
}
void TArticleWindow :: DoLinkEnd(TDC& dc)
{
    TPoint p;
    dc.GetCurrentPosition(pEnd);
    pEnd.y -= pCurrentFont->chDescent;//pCurrentFont->chMacAscent;//chHeight-1;

    linksList.AddLink(TRect(pBegin, pEnd), curLinkNum);

    PopTopFontAndChooseNext(dc);
    CurrentMode &= (~tagLinkProcessing);
}
void TArticleWindow :: DrawTitle(TDC& dc)
{
    TPoint p;
    dc.GetCurrentPosition(p);
    p.x = MarginLeft;
    p.y = pCurrentFont->chHeight/*chAscent*/ + pCurrentFont->chExternal;//chHeight
    dc.MoveTo(p);

    dc.SetTextColor(pCurrentFont->Color);
    dc.SelectObject(*pCurrentFont);
    WrappedTextOut(dc, 0, 0, Article.GetArtName(), strlen(Article.GetArtName()));

    dc.GetCurrentPosition(p);
    p.x = MarginLeft;
    p.y -= pCurrentFont->chDescent;//chHeight
    dc.MoveTo(p);
}
void TArticleWindow :: DoFontBegin(TDC& dc, LPSTR& part)
{
    BYTE flags = * ((BYTE*)part);
    part += sizeof(BYTE);

    int increment = (flags & (~0xE0)); // биты 0..4
    if (increment >0) ++NumberOfLines;
    LOGFONT lf;
    pCurrentFont->GetObject(lf);
    if ( flags & FNP_BOLD) lf.lfWeight = FW_BOLD;
    if ( flags & FNP_ITALIC) lf.lfItalic = 1;
    if ( flags & FNP_UNDERLINED ) lf.lfUnderline = 1;

    lf.lfHeight -= increment;

    PushPreviousFontAndChooseThis(
            dc,
            new TExFont(&lf, stdParent->pFont->Color)
    );
    ++FontsToDelete;
}
void TArticleWindow :: DoFontEnd(TDC& dc)
{
    TExFont* pf = PopTopFontAndChooseNext(dc);
    if (pf) delete pf;
    if (FontsToDelete <= 0) {
        TSTDError s("V\\" __FILE__, __LINE__, EC_ERRORFONTTAG);
        s.ProcessException();
    }
    --FontsToDelete;
}
void TArticleWindow :: DoKeywordBegin(TDC& dc)
{
    if ( (CurrentMode & tagLinkProcessing) == 0) {
        CurrentMode |= tagKeywordProcessing;
        PushPreviousFontAndChooseThis(
            dc,
            stdParent->pKeywordFont
        );
    }
}
void TArticleWindow :: DoKeywordEnd(TDC& dc)
{
    if (CurrentMode & tagKeywordProcessing) {
        CurrentMode &= (~tagKeywordProcessing);
        PopTopFontAndChooseNext(dc);
    }
}
TExFont* TArticleWindow :: PopTopFontAndChooseNext(TDC& dc)
{
    if (TopOfStack <= 0) {
        TSTDError s("V\\" __FILE__, __LINE__, EC_ERRORFONTTAG);
        s.ProcessException();
    }

    TExFont* cur = pCurrentFont;
    pCurrentFont = FontsStack[--TopOfStack];
    dc.SelectObject(*pCurrentFont);
    dc.SetTextColor(pCurrentFont->Color);

    return cur;
}
void TArticleWindow :: PushPreviousFontAndChooseThis(TDC& dc, TExFont* pf)
{
    if (TopOfStack > FONTS_STACK_SIZE) {
        TSTDError s("V\\" __FILE__, __LINE__, EC_ERRORFONTTAG);
        s.ProcessException();
    }
    FontsStack[TopOfStack++] = pCurrentFont;

    pCurrentFont = pf;
    dc.SelectObject(*pCurrentFont);
    dc.SetTextColor(pCurrentFont->Color);
}
void TArticleWindow :: DoTabulation(TDC& dc, LPSTR& art)
{
    TPoint p;
    BYTE type = (BYTE) *art++;
    int num = TAG_NUM_SPACES_BIG;
    if (type==0) num = TAG_NUM_SPACES_LITTLE;

    dc.GetCurrentPosition(p);

    int splen = stdParent->pFont->SpaceLen;
    p.x = (p.x / (splen* num ) +1) * splen* num  /*TAG_NUM_SPACES*/;

    dc.MoveTo(p);
}
void TArticleWindow :: DispatchString(/*TStarDictClient& parent, */TDC& dc, LPSTR art)
{
    CurrentMode = 0;
    dc.SetTextAlign(TA_UPDATECP | TA_LEFT | TA_BASELINE/*TA_TOP*/);
    dc.SetBkColor(::GetSysColor(COLOR_WINDOW) );

    pCurrentFont = stdParent->pTitleFont;
    MarginLeft = pCurrentFont->SpaceLen/2;

    DrawTitle(dc);

    pCurrentFont = stdParent->pFont;
    dc.SelectObject(*pCurrentFont);
    dc.SetTextColor(pCurrentFont->Color);

    fNoWrap = 0;

    TArticleTags tag = TAG_NONEXISTTAG;
    while (tag != TAG_EOFBODY && art != NULL && *art != '\0') {

        WORD len = 0;
        tag = FindFirstTag(art, len);
        if (len>0) WrappedTextOut(dc, 0, 0, art, len);
        art += (len+1);

        switch (tag) {

            case TAG_BITMAP         :       art += sizeof(TBitmap*); break;
            case TAG_PARAGRAPH      :       DoParagraph(dc, (BYTE)*art++); break;
            case TAG_ENDOFLINE      :       DoEndOfLine(dc); break;
            case TAG_TABULATION     :       DoTabulation(dc, art); break;

            case TAG_KEYWORDBEGIN   :       DoKeywordBegin(dc); break;
            case TAG_KEYWORDEND     :       DoKeywordEnd(dc); break;

            case TAG_FONTBEGIN      :       DoFontBegin(dc, art); break;
            case TAG_FONTEND        :       DoFontEnd(dc); break;

            case TAG_LINKBEGIN      :       DoLinkBegin(dc, &art); break;
            case TAG_LINKEND        :       DoLinkEnd(dc); break;

            case TAG_NOWRAPBEGIN    :       ++fNoWrap; break;
            case TAG_NOWRAPEND      :       if (fNoWrap>0) --fNoWrap;
                                            else fNoWrap =0;
                                            break;

        }
    }
    dc.RestoreFont();
    if (art == NULL) {
        TSTDError s("V\\" __FILE__, __LINE__, EC_NOSUCHARTICLE, 0);
        s.ProcessException();
        GetApplication()->GetMainWindow()->PostMessage(WM_COMMAND, CM_EXIT, 0L);
    }
}

