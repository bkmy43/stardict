#include "gmenu.hpp"

#define min(a,b)    (((a) < (b)) ? (a) : (b))

GMMENU GMmenu;


GMMENU::GMMENU()
{
q=0;
for (int i=0;i<MAX_LOG_ITEMS;i++) MenuLog[i]=-1;
LastCurrent='N';
PullDown='N';
}


int GMMENU::Operate(int mhandler)
/* для ACTIVATE
   возвpат GM_OK или код ошибки  */
{
int r;

if (q==MAX_LOG_ITEMS)
   {
   gerrmes(MANYAMENUS,"GMMENU::Operate(int)",0,COMMON_ERROR);
   return GM_NOK;
   }
r=DrawMenu(mhandler,GM_SAVE);
if (r!=GM_OK) return r;

MenuLog[q]=mhandler;
LastCurrent='Y';
q++;

return GM_OK;
}


int GMMENU::Operate(char param, GMEVENT& e)
/* возвpат
   ASK - handler меню, котоpое считает событие своим
	 или GM_NOK, если таковых нет
   MANAGE: GM_NOK - не удается отработать
	   CLOSE_APPLICATION - меню не может отработать из-за
			      активного приложения
	   n - номер функциональной клавиши, переданной FuncKeyItem-ом
	   GM_OK - отработало
	   GM_ESC - отpаботало на закpытие
*/
{
int r;

switch(param)
   {
   case ASK: return FindOwner(e);

   case MANAGE:
	if (q==0) return GM_NOK;

	r=FindOwner(e);
	if (r==GM_NOK) return GM_NOK;

	if (r==MenuLog[q-1]) /* последнее открытое меню -----------------> */
	   {
	   if (LastCurrent!='Y') return OperateLastNotCurrent(r,e);
			    else return OperateLastCurrent(r,e);
	   } /* <-------------------- последнее открытое меню */
	else return OperateNotLast(r,e);

   default: termes("Case error","GMMENU::Operate(char,GMEVENT)"); return GM_NOK;
   }
}



/********************** GMEVENT::WaitEvent **************************/
GMEVENT& GMEVENT::WaitEvent()
{
int r,xx,yy;

r=Revent(&xx,&yy);

if (r==-1)   SetEvent(E_LEFT,xx,yy);
else if (r==-2)   SetEvent(E_RIGHT,xx,yy);
else SetEvent(r);

return *this;
}


int GMMENU::CloseLastMenu()
{
int upmenu;

if (q==0) termes("No menues to close","GMMENU::OperateLastCurrent");

RestoreMenu(MenuLog[q-1]);
if (GMenus[MenuLog[q-1]]->menucall!=-1)
   {
   upmenu=GMenus[MenuLog[q-1]]->menucall;
   ShowStatus(upmenu,GMenus[upmenu]->curitem);
   GMenus[MenuLog[q-1]]->menucall=-1;
   }
MenuLog[q-1]=-1;
q--;

return GM_OK;
}


/*+++++++++++++++++++++++ PRIVATE ++++++++++++++++++++++++++++++++*/
int GMMENU::OperateNotLast(int mhandler, GMEVENT& e)
{
int it,r;

switch(e.Type())
   {
   case E_LEFT:
      while (MenuLog[q-1]!=mhandler) CloseLastMenu();
      LastCurrent='Y';
      it=WhichItemPosition(mhandler,e.X(),e.Y());
      if (it==-1) return GM_OK;
      r=GMenus[mhandler]->curitem;
      GMenus[mhandler]->curitem=it;
      DrawItem(mhandler,r);
      DrawItem(mhandler,it);
      return ManageItem(mhandler,it);

   case E_KEY:
      it=IfFuncKey(e.X(),mhandler);
      if (it!=-1)  return ManageItem(mhandler,it);
      return GM_NOK;

   default: termes("Case error","GMMENU::OperateNotLast"); return GM_NOK;
   }
}



int GMMENU::OperateLastNotCurrent(int mhandler, GMEVENT& e)
{
int it;

switch(e.Type())
   {
   case E_KEY:	  /* обработка функциональных клавиш */
      it=IfFuncKey(e.X(),mhandler);
      if (it!=-1)  return ManageItem(mhandler,it);
      termes("Error !","GMMENU::OperateLastNotCurrent");
      return GM_NOK;

   case E_LEFT:   LastCurrent='Y'; return GM_OK;

   default: termes("Case error","GMMENU::OperateLastNotCurrent"); return GM_NOK;
   }
}



int GMMENU::OperateLastCurrent(int mhandler, GMEVENT& e)
{
int r,it,up;

switch(e.Type())
   {
   case E_KEY:
      /* обработка функциональных клавиш */
      it=IfFuncKey(e.X(),mhandler);
      if (it!=-1)  return ManageItem(mhandler,it);

      /* обработка гоpячих клавиш */
      it=IfHighLet(e.X(),mhandler);
      if (it!=-1)  return ManageItem(mhandler,it);

      /* выход по альтеpнативной клавише */
      if (GMenus[mhandler]->exitkey==e.X()) { CloseLastMenu(); return GM_ESC; }

      switch(e.X())
	 { /* switch по клавишам */
	 case DOWN:
	    if (GMenus[mhandler]->menutype=='V')
	       {
	       it=FindNewItem(mhandler,GMenus[mhandler]->curitem,NEXT);
	       if (it==GM_NOK) return GM_NOK;
	       return (MoveItemBar(mhandler,GMenus[mhandler]->curitem,it));
	       }
	    else return ManageItem(mhandler,GMenus[mhandler]->curitem);

	    case UP:
	       it=FindNewItem(mhandler,GMenus[mhandler]->curitem,PREV);
	       if (it==GM_NOK) return GM_NOK;
	       return (MoveItemBar(mhandler,GMenus[mhandler]->curitem,it));

	    case HOME:
	       it=FindNewItem(mhandler,GMenus[mhandler]->curitem,FIRST);
	       if (it==GM_NOK) return GM_NOK;
	       return (MoveItemBar(mhandler,GMenus[mhandler]->curitem,it));

	    case END:
	       it=FindNewItem(mhandler,GMenus[mhandler]->curitem,LAST);
	       if (it==GM_NOK) return GM_NOK;
	       return (MoveItemBar(mhandler,GMenus[mhandler]->curitem,it));

	    case RIGHT:
	       if (GMenus[mhandler]->menutype=='V')
		  {
		  up=MenuLog[q-2];
		  if (q<2
		     || GMenus[up]->menutype!='H'
		     || GMenus[up]->GItems[GMenus[up]->curitem]->itemtype
						!=MENU_ITEM) return GM_NOK;
		  it=FindNewItem(up,GMenus[up]->curitem,NEXT);
		  if (it==GM_NOK) return GM_NOK;
		  CloseLastMenu();
		  MoveItemBar(up,GMenus[up]->curitem,it);
		  if (GMenus[up]->GItems[it]->itemtype==MENU_ITEM)
			 return ManageItem(up,it);
		  else PullDown='Y';
		  return GM_OK;
		  }
	       else
		  {
		  it=FindNewItem(mhandler,GMenus[mhandler]->curitem,NEXT);
		  if (it==GM_NOK) return GM_NOK;
		  r=MoveItemBar(mhandler,GMenus[mhandler]->curitem,it);
		  if (r==GM_OK && PullDown=='Y'
		    && GMenus[mhandler]->GItems[it]->itemtype==MENU_ITEM)
		    return ManageItem(mhandler,it);
		    else return r;
		  }

	    case LEFT:
	       if (GMenus[mhandler]->menutype=='V')
		  {
		  up=MenuLog[q-2];
		  if (q<2
		     || GMenus[up]->menutype!='H'
		     || GMenus[up]->GItems[GMenus[up]->curitem]->itemtype
						!=MENU_ITEM) return GM_NOK;
		  it=FindNewItem(up,GMenus[up]->curitem,PREV);
		  if (it==GM_NOK) return GM_NOK;
		  CloseLastMenu();
		  MoveItemBar(up,GMenus[up]->curitem,it);
		  if (GMenus[up]->GItems[it]->itemtype==MENU_ITEM)
			 return ManageItem(up,it);
		  else PullDown='Y';
		  return GM_OK;
		  }
	       else
		  {
		  it=FindNewItem(mhandler,GMenus[mhandler]->curitem,PREV);
		  if (it==GM_NOK) return GM_NOK;
		  r=MoveItemBar(mhandler,GMenus[mhandler]->curitem,it);
		  if (r==GM_OK && PullDown=='Y'
		    && GMenus[mhandler]->GItems[it]->itemtype==MENU_ITEM)
		    return ManageItem(mhandler,it);
		    else return r;
		  }

	 case ENTER:
	    return ManageItem(mhandler,GMenus[mhandler]->curitem);

	 case ESC:
	    if (GMenus[mhandler]->exitkey!=ESC) return GM_NOK;
	    CloseLastMenu();
	    return GM_ESC;

	 default: termes("Case error #1","GMMENU::OperateLastCurrent"); return GM_NOK;
	 } /* switch по клавишам */

   case E_LEFT:
      it=WhichItemPosition(mhandler,e.X(),e.Y());
      if (it==-1) return GM_NOK;
      r=GMenus[mhandler]->curitem;
      GMenus[mhandler]->curitem=it;
      DrawItem(mhandler,r);
      DrawItem(mhandler,it);
      return ManageItem(mhandler,it);

   case E_RIGHT:
      if (GMenus[mhandler]->exitkey!=ESC) return GM_NOK;
      CloseLastMenu();
      return GM_OK;

   default: termes("Case error #2","GMMENU::OperateLastCurrent"); return GM_NOK;
   }
}


int GMMENU::ManageItem(int mhandler, int itemhandler)
{
int r,cur,uprepmenu,uphandler,qclose;

PullDown='N';
if (GMenus[mhandler]->GItems[itemhandler]->hideitem=='H') return GM_NOK;
GMenus[mhandler]->curitem=itemhandler;
cur=GMenus[mhandler]->curitem;

switch(GMenus[mhandler]->GItems[cur]->itemtype)
   {
   case MENU_ITEM:

      if (q==MAX_LOG_ITEMS-1)
	 { gerrmes(MANYAMENUS,"GMMENU::Operate(int)",0,SYSTEM_ERROR); return GM_NOK; }
      GMenus[GMenus[mhandler]->GItems[cur]->menuhandler]->menucall=mhandler;
      if (GMenus[mhandler]->GItems[cur]->funchandler!=NULL)
	 r=(*(GMenus[mhandler]->GItems[cur]->funchandler))();
      // GMenus[GMenus[mhandler]->GItems[cur]->menuhandler]->menucall=-1;
      MenuLog[q]=GMenus[mhandler]->GItems[cur]->menuhandler;
      DrawMenu(MenuLog[q],GM_SAVE);
      ShowStatus(MenuLog[q],GMenus[MenuLog[q]]->curitem);
      q++;
      break;

   case FUNC_ITEM:

      uprepmenu=RestoreLevels(mhandler,
		GMenus[mhandler]->GItems[cur]->menurepaint);
      Rmouse_hide();
      RestoreStatusScreen();
      r=(*(GMenus[mhandler]->GItems[cur]->funchandler))();
      uphandler=(q-1 < GMenus[mhandler]->GItems[cur]->menuclose)  ? -1
		: MenuLog[q-1-GMenus[mhandler]->GItems[cur]->menuclose] ;
      if (uphandler!=-1 && uprepmenu!=-1)    SaveLevels(uprepmenu,uphandler);
      Rmouse_show();
      break;

   case	YES_NO_ITEM:

      *(GMenus[mhandler]->GItems[cur]->mainhandler)=
	  (*(GMenus[mhandler]->GItems[cur]->mainhandler)==YES) ? NO : YES ;
      r=YesNoSet(GMenus[mhandler]->GItems[cur]->str,
	  GMenus[mhandler]->GItems[cur]->mainhandler,GMenus[mhandler]->lang);
      if (r!=GM_OK) return GM_NOK;
      DrawItem(mhandler,cur);
      break;

   case	SWITCH_ITEM:

      *(GMenus[mhandler]->GItems[cur]->mainhandler)=
	   ( *(GMenus[mhandler]->GItems[cur]->mainhandler)==
	      GMenus[mhandler]->GItems[cur]->menuhandler - 1 ) ?
	    0 : *(GMenus[mhandler]->GItems[cur]->mainhandler)+1 ;

      r=SwitchSet( GMenus[mhandler]->GItems[cur]->str,
	   & (GMenus[mhandler]->GItems[cur]->
	     strings[ *(GMenus[mhandler]->GItems[cur]->mainhandler) ] ),
	  GMenus[mhandler]->GItems[cur]->strlength, GMenus[mhandler]->lang);

      if (r!=GM_OK) return GM_NOK;
      DrawItem(mhandler,cur);
      break;

   case STRING_ITEM:

      int left,top,right,bottom;
      DefineItemPosition(mhandler,cur,&left,&top,&right,&bottom);
      Rmouse_hide();

      REnterString(GMenus[mhandler]->GItems[cur]->string,
	right-(GMenus[mhandler]->GItems[cur]->strlength) * (LINE_WIDTH+4)-3,
	top-4,   GMenus[mhandler]->GItems[cur]->strlength,
	LINE_WIDTH+4,  GMenus[mhandler]->str_color,
	GMenus[mhandler]->str_groundcolor,
	GMenus[mhandler]->str_curscolor,
	GMenus[mhandler]->GItems[cur]->menuhandler,YES);

      AddStringToStringItem((GMenus[mhandler]->GItems[cur]) ->str,
	   (GMenus[mhandler]->GItems[cur]) ->string,
	   (GMenus[mhandler]->GItems[cur]) ->strlength);

      Rmouse_show();
      DrawMenu(mhandler,GM_NOSAVE);
      break;

   case FUNCKEY_ITEM:
      r=GMenus[mhandler]->GItems[cur]->menuhandler;
      break;


#ifdef FILENAME_EXIST
   case FILENAME_ITEM:
      GMpath[0]=0;
      DefineItemPosition(mhandler,cur,&left,&top,&right,&bottom);
      r=ListDir(GMenus[mhandler]->GItems[cur]->mask,GMpath,
		  GMenus[mhandler]->GItems[cur]->string,
		  left,bottom);
      if (r!=0) DrawMenu(mhandler,GM_NOSAVE);
      SetFilename(GMenus[mhandler]->GItems[cur]->str,
		  GMenus[mhandler]->GItems[cur]->string,
		  GMenus[mhandler]->GItems[cur]->menuhandler);
      DrawItem(mhandler,cur);
      break;
#endif

   default: termes("Case error","GMMENU::ManageItem"); return GM_NOK;
   }

/* для FUNCKEY ITEMа оставим открытыми указанное количество меню */
if (GMenus[mhandler]->GItems[cur]->itemtype==FUNCKEY_ITEM)
   {
   if (GMenus[mhandler]->GItems[cur]->menuclose!=-1)
      {
      qclose=q-GMenus[mhandler]->GItems[cur]->menuclose;
      for (int i=0 ; i<qclose ; i++)  CloseLastMenu();
      }
   return r;
   }

/* закpоем указанное количество меню */
if (GMenus[mhandler]->GItems[cur]->itemtype!=MENU_ITEM)
   {
   qclose=min(q,GMenus[mhandler]->GItems[cur]->menuclose);
   for (int i=0 ; i<qclose ; i++)  CloseLastMenu();
   }

return GM_OK;
}



int GMMENU::IfMenuIsOwner(int mhandler, char menustatus, GMEVENT& e)
/*  возвpат GM_OK  - если меню menuhandler считает событие e своим,
	    GM_NOK - в дpугом случае
    вызывается в FindOwner   */
{
int r;

if (GMenus[mhandler]==0)
   gerrmes(BADMENUNUM,"GMMENU::IfMenuIsOwner",0,SYSTEM_ERROR);

switch(e.Type())
   {
   case E_KEY:
	switch(e.X())
	   {

	   case UP:
	     if (menustatus!=MS_CURRENT) return GM_NOK;
	     if (GMenus[mhandler]->menutype=='H') return GM_NOK;
	     return GM_OK;

	   case DOWN:
	     if (menustatus!=MS_CURRENT) return GM_NOK;
	     if (GMenus[mhandler]->menutype=='H'
	       && GMenus[mhandler]->GItems[GMenus[mhandler]->curitem]->
		  itemtype!=MENU_ITEM) return GM_NOK;
	     return GM_OK;

	   case RIGHT:
	   case LEFT:
	   case HOME:
	   case END:
	   case ENTER:
	      if (menustatus!=MS_CURRENT) return GM_NOK; else return GM_OK;
	   case ESC:
	      if (menustatus!=MS_CURRENT || GMenus[mhandler]->exitkey!=ESC)
		 return GM_NOK; else return GM_OK;

	   default:
	      if (menustatus==MS_CURRENT && GMenus[mhandler]->exitkey==e.X())
		 return GM_OK;

	      if (menustatus==MS_CURRENT && IfHighLet(e.X(),mhandler)!=-1)
		 return GM_OK;

	      if (IfFuncKey(e.X(),mhandler)!=-1) return GM_OK;

	      return GM_NOK;
	   }
   case E_LEFT:
       if (menustatus==MS_CURRENT)
	  {
	  r=WhichItemPosition(mhandler,e.X(),e.Y());
	  if (r==-1) return GM_NOK;  else return GM_OK;
	  }
       else if (menustatus==MS_VISUAL
	     && e.X()>=GMenus[mhandler]->left && e.X()<=GMenus[mhandler]->right
	     && e.Y()>=GMenus[mhandler]->top && e.Y()<=GMenus[mhandler]->bottom)
		return GM_OK;
       else return GM_NOK;
   case E_RIGHT:
       if (menustatus==MS_CURRENT) return GM_OK;
       else return GM_NOK;
   default: termes("Case error","GMMENU::IfMenuIsOwner"); return GM_NOK;
   }
}


int GMMENU::FindOwner(GMEVENT& e)
/* возвpат - handler меню, котоpое считает событие своим
	  или GM_NOK, если таковых нет
   вызывается в Operate
*/
{
if (q==0) return GM_NOK;

if (LastCurrent=='Y')
   if (IfMenuIsOwner(MenuLog[q-1],MS_CURRENT,e)!=GM_NOK) return MenuLog[q-1];
else
   if (IfMenuIsOwner(MenuLog[q-1],MS_VISUAL,e)!=GM_NOK) return MenuLog[q-1];

for (int i=q-2;i>=0;i--)
   if (IfMenuIsOwner(MenuLog[i],MS_VISUAL,e)!=GM_NOK) return MenuLog[i];

for (i=0;i<QGMenus;i++)
   if (IfMenuIsOwner(i,MS_PASSIVE,e)!=GM_NOK) return i;

return GM_NOK;
}



