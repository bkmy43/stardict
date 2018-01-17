/* **************************************************************************
                                 HST.H
    v2.0.
    Содержит описание классов истории в StarDict.
    Классы:
        THistoryLink,
        THistoryList.
    (c) В.Перов.
************************************************************************** */

#if !defined (_HST_H)
#define _HST_H

class THistoryLink {

        friend class    THistoryList;

        THistoryLink*   next;
        THistoryLink*   prev;

        int             num;        // номер статьи
        char            title[128];
        WORD            position;   // позиция в окне

    public :
        WORD            visits;     // число песещений данной ссылки.

        THistoryLink(int num, LPSTR title, WORD position, WORD vis);
        int     GetArtNum()    { return num; }
        LPSTR   GetArtName()   { return title; }
};


class THistoryList {

        void        SortHistoryList();
        void        DeleteLinkWithMinVisits();

        THistoryLink*   head;
        THistoryLink*   curHistoryPos;

    public :

        THistoryList();
        ~THistoryList();

        THistoryLink*   GetNextArticle();//THistoryLink* cur);
        THistoryLink*   GetPrevArticle();//THistoryLink* cur);
        BOOL            IsNextExists();//THistoryLink* cur);
        BOOL            IsPrevExists();//THistoryLink* cur);

        THistoryLink*   AddArticleToHistory(TExArticle& art, WORD pos);
};

#endif

