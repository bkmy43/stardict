/* **************************************************************************
                                HISTORY.C
    v2.0.
    Содержит реализацию истории в StarDict.
    Классы:
        THistoryLink,
        THistoryList.
    (c) В.Перов.
************************************************************************** */

#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma hdrstop

#include "exart.h"
#include "hst.h"

// максимальная длина списка ссылок (истории):
#define   HIST_MAX_LENGTH      30

// **********************************************************************
// *** class THistoryLink :
// **********************************************************************
THistoryLink :: THistoryLink(int n, LPSTR t, WORD pos, WORD vis)
{
    next = prev = NULL;
    num = n;
    position = pos;
    visits = vis;
    strcpy(title, t);
}
// **********************************************************************
// *** class THistoryList :
// **********************************************************************
THistoryList :: THistoryList()
{
    head = NULL;
    curHistoryPos = NULL;
}
THistoryList :: ~THistoryList()
{
    THistoryLink* cur = head;
    while (cur) {
        THistoryLink* tmp = cur;
        cur=cur->next;
        delete tmp;
    }
}
THistoryLink* THistoryList :: GetNextArticle(/*THistoryLink* c*/)
{
    if (curHistoryPos == NULL) return NULL;
    return (curHistoryPos=curHistoryPos->next);
}
THistoryLink* THistoryList :: GetPrevArticle(/*THistoryLink* c*/)
{
    if (curHistoryPos == NULL) return NULL;
    return (curHistoryPos=curHistoryPos->prev);
}
BOOL THistoryList :: IsNextExists(/*THistoryLink* c*/)
{
    if (curHistoryPos == NULL) return FALSE;
    else return ( curHistoryPos->next != NULL);
}
BOOL THistoryList :: IsPrevExists(/*THistoryLink* c*/)
{
    if (curHistoryPos == NULL) return FALSE;
    else return ( curHistoryPos->prev != NULL);
}
void THistoryList :: SortHistoryList()
{
}
void THistoryList :: DeleteLinkWithMinVisits()
{
    if (head == NULL) return;
    THistoryLink* p = head;
    int vis = 32000;
    for (THistoryLink* cur=head; cur!=NULL; cur=cur->next)
        if (cur->visits<vis) {
            p=cur;
            vis=cur->visits;
        }
    if (p->prev && p->next) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
    }
    else if (p->prev) {
        p->prev->next = NULL;
        delete p;
    }
    else if (p->next) {
        p->next->prev = NULL;
        delete p;
    }
}
THistoryLink* THistoryList :: AddArticleToHistory(TExArticle& art, WORD pos)
{
    if (head == NULL) {
        curHistoryPos = head =
              new THistoryLink(art.GetArtNum(), art.GetArtName(), pos, 0);

        return head;
    }
    else {
        THistoryLink* cur = new THistoryLink(art.GetArtNum(), art.GetArtName(), pos,
                                             curHistoryPos->visits+1);
        for (THistoryLink* p = head; p!=curHistoryPos && p!=NULL; p=p->next);
        if (p->next) {
            cur->next = p->next;
            p->next->prev=cur;
        }
        else cur->next = NULL;

        p->next = cur;
        cur->prev = p;
        curHistoryPos = cur;

        int count=0;
        for (cur=head; cur!=NULL; ++count, cur=cur->next);
        for (int i=0; i<count-HIST_MAX_LENGTH; ++i)
            DeleteLinkWithMinVisits();

        SortHistoryList();
        return cur;
/*        for (THistoryLink* p = head; p->next; p=p->next, ++count);
        p->next = cur;
        cur->prev = p;
        curHistoryPos = cur;

        if (count> HIST_MAX_LENGTH) {
            cur = head;
            head = head->next;
            cur->next = NULL;
            delete cur;
        }

        SortHistoryList();
        return cur;*/
    }
}

