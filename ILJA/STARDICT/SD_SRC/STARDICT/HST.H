/* **************************************************************************
                                 HST.H
    v2.0.
    �������� �������� ������� ������� � StarDict.
    ������:
        THistoryLink,
        THistoryList.
    (c) �.�����.
************************************************************************** */

#if !defined (_HST_H)
#define _HST_H

class THistoryLink {

        friend class    THistoryList;

        THistoryLink*   next;
        THistoryLink*   prev;

        int             num;        // ����� ������
        char            title[128];
        WORD            position;   // ������� � ����

    public :
        WORD            visits;     // ����� ��������� ������ ������.

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

