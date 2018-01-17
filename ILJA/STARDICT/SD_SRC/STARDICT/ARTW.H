/* **************************************************************************
                                 ARTW.H
    v2.0
    Содержит определение класса TArticleWindow и все такое прочее.
    Классы:
        TLink,
        TLinksList,
        TArticleWindow.
    (c) В.Перов.
************************************************************************** */

#if !defined (_ARTW_H)
#define _ARTW_H

#define   FONTS_STACK_SIZE              10

//extern  THistoryList*   pHistory;
class TStarDictClient;

class TLink {

        friend  class TLinksList;

        int     numberOfArt;
        TLink*  next;

    public :
        TRect   r;

        TLink(TRect&, int num);
        BOOL    IsPointInside(TPoint&);
        BOOL    IsLinkInsideRect(TRect&);
        int     GetNumberOfArt() { return numberOfArt; }
        TLink*  Next() { return next; }
};

class TLinksList {

        TLink*  head;

    public :
        TLinksList();
        virtual ~TLinksList();
        void    ClearList();

        void    AddLink(TLink* newLink);
        void    AddLink(TRect& r, int numberOfArt);

        TLink*  IsPointInsideLink(TPoint&, int);
        TLink*  GetFirstLinkInsideRect(TRect&, int);
};

class TArticleWindow : public TMDIChild { // Окно, содержащее текст статьи.

        void      PushPreviousFontAndChooseThis(TDC&, TExFont*);
        TExFont*  PopTopFontAndChooseNext(TDC&);

        TArticleTags  FindFirstTag(LPSTR, WORD& len);

        void      DispatchString(/*TStarDictClient& parent,*/TDC& dc, LPSTR art);
        void      WrappedTextOut(TDC& dc, int x, int y, LPSTR, int len);
        LPSTR     WordOut(TDC& dc, LPSTR);
        void      SetArticleCaption();
        void      DrawTitle(TDC&);
//        void      ProcessTab();
        void      DoChangeArticle(int);
        void      OpenArticle();
        void      SetCursorToUsual();
        void      SetCursorToHand();

        void      DoEndOfLine(TDC& dc);
        void      DoParagraph(TDC&, BYTE);
        void      DoLinkBegin(TDC&, LPSTR*, BOOL skipLinkNum = FALSE);
        void      DoLinkEnd(TDC&);
        void      DoFontBegin(TDC&, LPSTR&);
        void      DoFontEnd(TDC&);
        void      DoKeywordBegin(TDC&);
        void      DoKeywordEnd(TDC&);
        void      DoTabulation(TDC&, LPSTR&);  

        TExFont*        pCurrentFont;
        TStarDictClient* stdParent;
        TLinksList      linksList;
        TPoint          pBegin, pEnd;
        int             curLinkNum;
        BYTE            CurrentMode; // показывает, какой тэг обрабатывается
        BOOL            fHandCursor, fRButtonPressed;
        TLink*          curLink;
        TLink*          tabLink;
        int             NumberOfLines;
        int             MarginLeft;
        int             FontsToDelete;

        int             TopOfStack;
        TExFont*        FontsStack[FONTS_STACK_SIZE];

    public :
        TExArticle      Article;
        THistoryList    History;

        TArticleWindow(TStarDictClient& parent, int num);
        virtual ~TArticleWindow();

        void    GetWindowClass(WNDCLASS&);
        void    SetupWindow();
        virtual void Paint(TDC&, BOOL, TRect&);
//        virtual BOOL PreProcessMsg(MSG& m);

        void    EvSize(uint, TSize&);
        void    EvMouseMove(uint, TPoint&);
        void    EvLButtonDown(uint, TPoint&);
        void    EvRButtonDown(uint, TPoint&);
        void    EvKeyDown(uint key, uint repeatCount, uint flags);
        void    EvSysKeyDown(uint key, uint repeatCount, uint flags);

        LPARAM  EvInvalidate(WPARAM, LPARAM);
        void    CmEsc();
        void    CmOpenHere();
        void    CmOpenNew();
        void    ProcessMoveBack();
        void    ProcessMoveForward();

        BOOL      IsPrevExists()
            { return History.IsPrevExists(); }
        BOOL      IsNextExists()
            { return History.IsNextExists();  }

        DECLARE_RESPONSE_TABLE(TArticleWindow);
};

#endif

