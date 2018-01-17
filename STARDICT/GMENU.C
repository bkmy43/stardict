//#define FILENAME_EXIST   // Если объявлен - можно использовать FileNameItem
		   // необходимо подключить LISTDIR.C

#define HELP_EXIST  // Если объявлен - можно использовать Help
		   // необходимо подключить HELP.C
#define DEBUG_MODE // Если объявлен - pаботает в отладочном pежиме

#define RECUR_OPERATION_MODE // Если объявлен - включен pежим pекуpсивного
	      // упpавления меню. Будет включена функция ActivateMenu
//#define OUTER_OPERATION_MODE // Если объявлен - включен pежим внешнего
	      // упpавления меню. Будут подключены файлы GMENU.HPP & GMENU.CPP

#include "gmenu.h"
#include "graph.h"
#include "util.h"
#include "keys.h"
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <alloc.h>
#include <string.h>
#include <ctype.h>
#include <bios.h>


#ifdef FILENAME_EXIST
#include "listdir.h" // этот хэдэр необходим только при использовании
		     // FILENAME-itemов и требует подключения LISTDIR.C
#endif

#ifdef HELP_EXIST
#include "help.h"
#endif

#ifdef DEBUG_MODE
#include <stdio.h>
#define F stdprn
#endif



#define max(a,b)    (((a) > (b)) ? (a) : (b))


void MenuInf(void);
char *GMpath=0;

extern int LINE_HEIGHT,LINE_WIDTH;

struct GMENU *GMenus[MAX_MENUS];
int QGMenus=0;
char *savebuf=0,*statbuf=0;
int AVector[MAX_MENUS],
    CloseCounter,
    KeyBuffer[MAX_MENUS],
    active_menu=0,
    stathandler=0;    /* см. define */
unsigned long memhandler; /*OTL*/
int BUFFER_SIZE=120;

#ifdef OUTER_OPERATION_MODE
#include "gmenu.cpp"
#endif

/******************************/
const ESC_IN_MAIN_MENU = -1;
const QUIT_IN_MAIN_MENU = -2;

extern int MainMenuLeaveMode;
/******************************/
/**************************** InitMenu ***********************************/
int InitMenu(int *menuhandler,
	     int left, int top, char menutype, int lang,
	     int bordtype, char *bordname0, char *bordname1, char *bordname2,
	     int otstup, int interval, int freq,
	     int groundcolor, int groundbordcolor, int bordcolor,
	     int barcolor, int highbarcolor, int hidebarcolor,
	     int textcolor, int hightextcolor, int hidetextcolor,
	     int hidehightextcolor, int highletcolor)
{
/***************** OTL ***************/
#ifdef DEBUG_MODE
if (QGMenus==0) memhandler=farcoreleft();
#endif
/*************************************/

if (QGMenus==MAX_MENUS)
   { gerrmes(MANYMENUS,"InitMenu",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[QGMenus]=(struct GMENU *)farmalloc(sizeof(struct GMENU));
if (GMenus[QGMenus]==NULL)
   { gerrmes(NOMEMENOUGH,"InitMenu",0,COMMON_ERROR); return(GM_NOT_MEM);  }
GMenus[QGMenus]->top=top;
GMenus[QGMenus]->left=left;
GMenus[QGMenus]->menutype=menutype;
GMenus[QGMenus]->lang=lang,
GMenus[QGMenus]->bordtype=bordtype;
GMenus[QGMenus]->bordname0=bordname0;
GMenus[QGMenus]->bordname1=bordname1;
GMenus[QGMenus]->bordname2=bordname2;
switch(lang)
   {
   case 0:  GMenus[QGMenus]->bordname=bordname0;  break;
   case 1:  GMenus[QGMenus]->bordname=bordname1;  break;
   case 2:  GMenus[QGMenus]->bordname=bordname2;  break;
   }
GMenus[QGMenus]->otstup=otstup;
GMenus[QGMenus]->interval=interval;
GMenus[QGMenus]->freq=freq;
GMenus[QGMenus]->groundcolor=groundcolor;
GMenus[QGMenus]->groundbordcolor=groundbordcolor;
GMenus[QGMenus]->bordcolor=bordcolor;
GMenus[QGMenus]->barcolor=barcolor;
GMenus[QGMenus]->highbarcolor=highbarcolor;
GMenus[QGMenus]->hidebarcolor=hidebarcolor;
GMenus[QGMenus]->textcolor=textcolor;
GMenus[QGMenus]->hightextcolor=hightextcolor;
GMenus[QGMenus]->hidetextcolor=hidetextcolor;
GMenus[QGMenus]->hidehightextcolor=hidehightextcolor;
GMenus[QGMenus]->highletcolor=highletcolor;
GMenus[QGMenus]->str_color=15;
GMenus[QGMenus]->str_groundcolor=1;
GMenus[QGMenus]->str_curscolor=12;
GMenus[QGMenus]->qitems=0;
GMenus[QGMenus]->curitem=0;
GMenus[QGMenus]->savepos=-777;
GMenus[QGMenus]->savesize=0l;
GMenus[QGMenus]->savehandler=0;
GMenus[QGMenus]->menucall=-1;
GMenus[QGMenus]->statcolor=0;
GMenus[QGMenus]->statgroundcolor=15;
GMenus[QGMenus]->exitkey=ESC;

*menuhandler=QGMenus;
QGMenus++;

return(InitSaveBuf());
}


/*************************** DestroyMenu ********************************/
void DestroyMenu(int menuhandler)
{
int i;

for ( i=0 ; i< GMenus[menuhandler]-> qitems ; i++ )
   {
   switch( GMenus[menuhandler]->GItems[i]->itemtype )
      {
      case YES_NO_ITEM:
      case SWITCH_ITEM:
      case FILENAME_ITEM:
	   farfree( GMenus[menuhandler]->GItems[i]->str );
	   break;
      case STRING_ITEM:
	   farfree( GMenus[menuhandler]->GItems[i]->str );
	   /* farfree( GMenus[menuhandler]->GItems[i]->string ); */
	   break;
      }
   farfree( GMenus[menuhandler]->GItems[i] );
   }
farfree(GMenus[menuhandler]);
GMenus[menuhandler]=DESTROYED_MENU;
QGMenus--;
}


/************************* DestroyAllMenus *******************************/
void DestroyAllMenus(void)
{
int i,q;
unsigned long m; /*OTL*/
q=QGMenus;
for (i=0;i<q;i++)  if (GMenus[i]!=DESTROYED_MENU) DestroyMenu(i);
if (savebuf!=0) farfree(savebuf);
if (stathandler!=STAT_NOTINIT) farfree(statbuf);
if (GMpath!=0) farfree(GMpath);

/***************** OTL ***************/
#ifdef DEBUG_MODE
   m=farcoreleft();
   if (m!=memhandler) for (i=0;i<10;i++) badkey();
#endif
/*************************************/
}


/************************** InitMenuItem ********************************/
int InitMenuItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int newmenuhandler, int (* funchandler)(), int helpitem)
{
int q;

q=GMenus[menuhandler]->qitems;
if (q==MAX_ITEMS)
   { gerrmes(MANYITEMS,"InitMenuItem",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[menuhandler]->GItems[q]=(struct GITEM *)farmalloc(sizeof(struct GITEM));
if (GMenus[menuhandler]->GItems[q]==NULL)
   { gerrmes(NOMEMENOUGH,"InitMenuItem",0,COMMON_ERROR); return(GM_NOT_MEM);  }


(GMenus[menuhandler]->GItems[q])->itemtype=MENU_ITEM;
(GMenus[menuhandler]->GItems[q])->hideitem='N';

(GMenus[menuhandler]->GItems[q]) -> str0 = str0;
(GMenus[menuhandler]->GItems[q]) -> str1 = str1;
(GMenus[menuhandler]->GItems[q]) -> str2 = str2;
switch(GMenus[menuhandler]->lang)
   {
   case 0: (GMenus[menuhandler]->GItems[q]) -> str = str0;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet0;
	   break;
   case 1: (GMenus[menuhandler]->GItems[q]) -> str = str1;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet1;
	   break;
   case 2: (GMenus[menuhandler]->GItems[q]) -> str = str2;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet2;
	   break;
   }
GMenus[menuhandler]->GItems[q]->hlet=SetHighLet(menuhandler,q);

(GMenus[menuhandler]->GItems[q]) -> menuhandler = newmenuhandler;
(GMenus[menuhandler]->GItems[q]) -> menuclose = 0;
(GMenus[menuhandler]->GItems[q]) -> helpitem  = helpitem;
(GMenus[menuhandler]->GItems[q]) -> statstr = 0;
(GMenus[menuhandler]->GItems[q]) -> funchandler = funchandler;

(GMenus[menuhandler]->GItems[q]) -> length=
		 strlen( (GMenus[menuhandler]->GItems[q]) -> str );

GMenus[menuhandler]->qitems++;
return(GMenus[menuhandler]->qitems-1);
}


/************************** InitYesNoItem ********************************/
int InitYesNoItem(int menuhandler,
		  char *str0, char *str1, char *str2,
		  int hlet0, int hlet1, int hlet2,
		  int *yesnohandler, int menuclose, int helpitem)
{
int i,q,l,r;

q=GMenus[menuhandler]->qitems;
if (q==MAX_ITEMS)
   { gerrmes(MANYITEMS,"InitYesNoItem",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[menuhandler]->GItems[q]=(struct GITEM *)farmalloc(sizeof(struct GITEM));
if (GMenus[menuhandler]->GItems[q]==NULL)
   { gerrmes(NOMEMENOUGH,"InitYesNoItem",1,COMMON_ERROR); return(GM_NOT_MEM);  }

(GMenus[menuhandler]->GItems[q])->itemtype=YES_NO_ITEM;
(GMenus[menuhandler]->GItems[q])->hideitem='N';

(GMenus[menuhandler]->GItems[q]) -> str0 = str0;
(GMenus[menuhandler]->GItems[q]) -> str1 = str1;
(GMenus[menuhandler]->GItems[q]) -> str2 = str2;

l=max(strlen(str0),strlen(str1));
l=max(l,strlen(str2));
(GMenus[menuhandler]->GItems[q]) -> str=(char *)farmalloc(l+5);
if ( (GMenus[menuhandler]->GItems[q]) -> str == NULL)
   { gerrmes(NOMEMENOUGH,"InitYesNoItem",2,COMMON_ERROR); return(GM_NOT_MEM); }
for (i=0;i<l+5;i++) (GMenus[menuhandler]->GItems[q]) -> str[i]=' ';
(GMenus[menuhandler]->GItems[q]) -> str[l+4]=0;

switch(GMenus[menuhandler]->lang)
   {
   case 0: memcpy((GMenus[menuhandler]->GItems[q])->str, str0, strlen(str0)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet0;
	   break;
   case 1: memcpy((GMenus[menuhandler]->GItems[q])->str, str1, strlen(str1)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet1;
	   break;
   case 2: memcpy((GMenus[menuhandler]->GItems[q])->str, str2, strlen(str2)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet2;
	   break;
   }
GMenus[menuhandler]->GItems[q]->hlet=SetHighLet(menuhandler,q);

(GMenus[menuhandler]->GItems[q]) -> mainhandler = yesnohandler;
(GMenus[menuhandler]->GItems[q]) -> menuclose = menuclose;
(GMenus[menuhandler]->GItems[q]) -> helpitem  = helpitem;
(GMenus[menuhandler]->GItems[q]) -> statstr = 0;

(GMenus[menuhandler]->GItems[q]) -> length=l+4;

r=YesNoSet((GMenus[menuhandler]->GItems[q])->str,yesnohandler,
				    GMenus[menuhandler]->lang);

if (r==GM_OK)
   {
   (GMenus[menuhandler]->qitems)++;
   return(GMenus[menuhandler]->qitems-1);
   }
else return(r);
}


/************************** InitSwitchItem *******************************/
int InitSwitchItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int *curstr, int menuclose, int helpitem)
{
int q;

q=GMenus[menuhandler]->qitems;
if (q==MAX_ITEMS)
   { gerrmes(MANYITEMS,"InitSwitchItem",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[menuhandler]->GItems[q]=(struct GITEM *)farmalloc(sizeof(struct GITEM));
if (GMenus[menuhandler]->GItems[q]==NULL)
   { gerrmes(NOMEMENOUGH,"InitSwitchItem",0,COMMON_ERROR); return(GM_NOT_MEM);  }


(GMenus[menuhandler]->GItems[q])->itemtype=SWITCH_ITEM;
(GMenus[menuhandler]->GItems[q])->hideitem='N';

switch(GMenus[menuhandler]->lang)
   {
   case 0: (GMenus[menuhandler]->GItems[q]) -> hlet = hlet0;       break;
   case 1: (GMenus[menuhandler]->GItems[q]) -> hlet = hlet1;       break;
   case 2: (GMenus[menuhandler]->GItems[q]) -> hlet = hlet2;       break;
   }

(GMenus[menuhandler]->GItems[q]) -> str0 = str0;
(GMenus[menuhandler]->GItems[q]) -> str1 = str1;
(GMenus[menuhandler]->GItems[q]) -> str2 = str2;
(GMenus[menuhandler]->GItems[q]) -> menuclose = menuclose;
(GMenus[menuhandler]->GItems[q]) -> helpitem  = helpitem;
(GMenus[menuhandler]->GItems[q]) -> statstr = 0;
(GMenus[menuhandler]->GItems[q]) -> strlength = 0;
(GMenus[menuhandler]->GItems[q]) -> mainhandler = curstr;
(GMenus[menuhandler]->GItems[q]) -> menuhandler = 0;
	       /* инициализация, а вообще в нем кол-во строк */
(GMenus[menuhandler]->qitems)++;
return(GMenus[menuhandler]->qitems-1);
}


/************************* AddStringToItem ******************************/
int AddStringToItem(int menuhandler, char *str0, char *str1, char *str2)
    /*  вызывать функцию непосредственно после инициализации item-а*/
{
int q,n;

q=GMenus[menuhandler]->qitems-1;
n=(GMenus[menuhandler]->GItems[q])->menuhandler;
(GMenus[menuhandler]->GItems[q])->menurepaint=-1;
(GMenus[menuhandler]->GItems[q])->strings[n].str0 = str0;
(GMenus[menuhandler]->GItems[q])->strings[n].str1 = str1;
(GMenus[menuhandler]->GItems[q])->strings[n].str2 = str2;
(GMenus[menuhandler]->GItems[q])->menuhandler++;
(GMenus[menuhandler]->GItems[q])->strlength=
	max((GMenus[menuhandler]->GItems[q])->strlength,strlen(str0));
(GMenus[menuhandler]->GItems[q])->strlength=
	max((GMenus[menuhandler]->GItems[q])->strlength,strlen(str1));
(GMenus[menuhandler]->GItems[q])->strlength=
	max((GMenus[menuhandler]->GItems[q])->strlength,strlen(str2));

return GM_OK;
}


/*********************** AddLastStringToItem *****************************/
int AddLastStringToItem(int menuhandler, char *str0, char *str1, char *str2)
    /*  вызывать в качестве последней AddStringToItem */
{
int i,q,l,r;

q=GMenus[menuhandler]->qitems-1;
AddStringToItem(menuhandler,str0,str1,str2);
(GMenus[menuhandler]->GItems[q])->menurepaint=0;
l=max(strlen( (GMenus[menuhandler]->GItems[q]) -> str0 ),
      strlen( (GMenus[menuhandler]->GItems[q]) -> str1 ) );
l=max(strlen( (GMenus[menuhandler]->GItems[q]) -> str2 ), l );
l+=(GMenus[menuhandler]->GItems[q]) -> strlength;
(GMenus[menuhandler]->GItems[q]) -> length = l;
(GMenus[menuhandler]->GItems[q]) -> str=(char *)farmalloc(l+1);
if ( (GMenus[menuhandler]->GItems[q]) -> str == NULL )
   { gerrmes(NOMEMENOUGH,"AddLastStringToItem",0,COMMON_ERROR); return(GM_NOT_MEM);  }
for (i=0;i<l+1;i++) (GMenus[menuhandler]->GItems[q]) -> str[i]=' ';
(GMenus[menuhandler]->GItems[q]) -> str[l]=0;
switch(GMenus[menuhandler]->lang)
   {
   case 0: memcpy((GMenus[menuhandler]->GItems[q])->str,
		   (GMenus[menuhandler]->GItems[q])->str0,
		   strlen( (GMenus[menuhandler]->GItems[q])->str0) ) ;
	   break;
   case 1: memcpy((GMenus[menuhandler]->GItems[q])->str,
		   (GMenus[menuhandler]->GItems[q])->str1,
		   strlen( (GMenus[menuhandler]->GItems[q])->str1) ) ;
	   break;
   case 2: memcpy((GMenus[menuhandler]->GItems[q])->str,
		   (GMenus[menuhandler]->GItems[q])->str2,
		   strlen( (GMenus[menuhandler]->GItems[q])->str2) ) ;
	   break;
   }
r=SwitchSet( GMenus[menuhandler]->GItems[q]->str,
	   & (GMenus[menuhandler]->GItems[q]->
	       strings[ *(GMenus[menuhandler]->GItems[q]->mainhandler) ] ),
	   GMenus[menuhandler]->GItems[q]->strlength,
	   GMenus[menuhandler]->lang);

GMenus[menuhandler]->GItems[q]->hlet=SetHighLet(menuhandler,q);

return(r);
}


/************************** InitFuncItem ********************************/
int InitFuncItem(int menuhandler,
		  char *str0, char *str1, char *str2,
		  int hlet0, int hlet1, int hlet2,
		  int (* funchandler)(),
		  int menuclose, int menurepaint, int helpitem)
{
int q;

q=GMenus[menuhandler]->qitems;
if (q==MAX_ITEMS)
   { gerrmes(MANYITEMS,"InitFuncItem",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[menuhandler]->GItems[q]=(struct GITEM *)farmalloc(sizeof(struct GITEM));
if (GMenus[menuhandler]->GItems[q]==NULL)
   { gerrmes(NOMEMENOUGH,"InitFuncItem",0,COMMON_ERROR); return(GM_NOT_MEM); }

(GMenus[menuhandler]->GItems[q])->itemtype=FUNC_ITEM;
(GMenus[menuhandler]->GItems[q])->hideitem='N';

(GMenus[menuhandler]->GItems[q]) -> str0 = str0;
(GMenus[menuhandler]->GItems[q]) -> str1 = str1;
(GMenus[menuhandler]->GItems[q]) -> str2 = str2;

switch(GMenus[menuhandler]->lang)
   {
   case 0: (GMenus[menuhandler]->GItems[q]) -> str = str0;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet0;
	   break;
   case 1: (GMenus[menuhandler]->GItems[q]) -> str = str1;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet1;
	   break;
   case 2: (GMenus[menuhandler]->GItems[q]) -> str = str2;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet2;
	   break;
   }
GMenus[menuhandler]->GItems[q]->hlet=SetHighLet(menuhandler,q);

(GMenus[menuhandler]->GItems[q]) -> funchandler = funchandler;
(GMenus[menuhandler]->GItems[q]) -> menuclose = menuclose;
(GMenus[menuhandler]->GItems[q]) -> menurepaint = menurepaint;
(GMenus[menuhandler]->GItems[q]) -> helpitem  = helpitem;
(GMenus[menuhandler]->GItems[q]) -> statstr = 0;

(GMenus[menuhandler]->GItems[q]) -> length=
		 strlen( (GMenus[menuhandler]->GItems[q]) -> str );

(GMenus[menuhandler]->qitems)++;
return(GMenus[menuhandler]->qitems-1);
}


/************************** InitStringItem ******************************/
int InitStringItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 char *string, int strlength, int strtype,
		 int menuclose, int helpitem)
{
int i,q,l;

if (GMenus[menuhandler]->menutype=='H')
		       gerrmes(BAD_TYPE,"InitStringItem",0,SYSTEM_ERROR);

q=GMenus[menuhandler]->qitems;
if (q==MAX_ITEMS)
   { gerrmes(MANYITEMS,"InitStringItem",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[menuhandler]->GItems[q]=(struct GITEM *)farmalloc(sizeof(struct GITEM));
if (GMenus[menuhandler]->GItems[q]==NULL)
   { gerrmes(NOMEMENOUGH,"InitStringItem",1,COMMON_ERROR); return(GM_NOT_MEM);  }

(GMenus[menuhandler]->GItems[q])->itemtype=STRING_ITEM;
(GMenus[menuhandler]->GItems[q])->hideitem='N';

(GMenus[menuhandler]->GItems[q]) -> str0 = str0;
(GMenus[menuhandler]->GItems[q]) -> str1 = str1;
(GMenus[menuhandler]->GItems[q]) -> str2 = str2;

l=max(strlen(str0),strlen(str1));
l=max(l,strlen(str2));
l+=strlength+2;
(GMenus[menuhandler]->GItems[q]) -> strlength = strlength;
(GMenus[menuhandler]->GItems[q]) -> str=(char *)farmalloc(l);
if ((GMenus[menuhandler]->GItems[q]) -> str == NULL)
   { gerrmes(NOMEMENOUGH,"InitStringItem",2,COMMON_ERROR); return(GM_NOT_MEM); }
/*
(GMenus[menuhandler]->GItems[q]) -> string=
   (char *)farmalloc((GMenus[menuhandler]->GItems[q]) -> strlength);
if ((GMenus[menuhandler]->GItems[q]) -> string == NULL)
   { gerrmes(NOMEMENOUGH,"InitStringItem",3,COMMON_ERROR); return(GM_NOT_MEM); }
strcpy((GMenus[menuhandler]->GItems[q]) -> string,string);
*/
(GMenus[menuhandler]->GItems[q]) -> string = string; /**/

for (i=0;i<l;i++) (GMenus[menuhandler]->GItems[q]) -> str[i]=' ';
(GMenus[menuhandler]->GItems[q]) -> str[l-1]=0;
AddStringToStringItem((GMenus[menuhandler]->GItems[q]) ->str,string,strlength);


switch(GMenus[menuhandler]->lang)
   {
   case 0: memcpy((GMenus[menuhandler]->GItems[q])->str, str0, strlen(str0)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet0;
	   break;
   case 1: memcpy((GMenus[menuhandler]->GItems[q])->str, str1, strlen(str1)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet1;
	   break;
   case 2: memcpy((GMenus[menuhandler]->GItems[q])->str, str1, strlen(str1)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet2;
	   break;
   }
GMenus[menuhandler]->GItems[q]->hlet=SetHighLet(menuhandler,q);

(GMenus[menuhandler]->GItems[q]) -> menuhandler = strtype;
(GMenus[menuhandler]->GItems[q]) -> strlength = strlength;
(GMenus[menuhandler]->GItems[q]) -> menuclose = menuclose;
(GMenus[menuhandler]->GItems[q]) -> helpitem  = helpitem;
(GMenus[menuhandler]->GItems[q]) -> statstr = 0;

(GMenus[menuhandler]->GItems[q]) -> length=
		 strlen( (GMenus[menuhandler]->GItems[q]) -> str );

(GMenus[menuhandler]->qitems)++;
return(GMenus[menuhandler]->qitems-1);
}

/************************ InitFilenameItem ******************************/
#ifdef FILENAME_EXIST
int InitFilenameItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 char *mask, char *filename, int filemask,
		 int menuclose, int helpitem)
{
int i,q,l,ll;

q=GMenus[menuhandler]->qitems;
if (q==MAX_ITEMS)
   { gerrmes(MANYITEMS,"InitFilenameItem",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[menuhandler]->GItems[q]=(struct GITEM *)farmalloc(sizeof(struct GITEM));
if (GMenus[menuhandler]->GItems[q]==NULL)
   { gerrmes(NOMEMENOUGH,"InitFilenameItem",1,COMMON_ERROR); return(GM_NOT_MEM);  }

ll=(filemask==GM_EXTENSION) ? 14 : 9 ;
(GMenus[menuhandler]->GItems[q])->itemtype=FILENAME_ITEM;
(GMenus[menuhandler]->GItems[q])->hideitem='N';

(GMenus[menuhandler]->GItems[q]) -> str0 = str0;
(GMenus[menuhandler]->GItems[q]) -> str1 = str1;
(GMenus[menuhandler]->GItems[q]) -> str2 = str2;

l=max(strlen(str0),strlen(str1));
l=max(l,strlen(str2));
(GMenus[menuhandler]->GItems[q]) -> str=(char *)farmalloc(l+ll);
if ( (GMenus[menuhandler]->GItems[q]) -> str == NULL)
   { gerrmes(NOMEMENOUGH,"InitFilenameItem",2,COMMON_ERROR); return(GM_NOT_MEM); }
for (i=0;i<l+ll;i++) (GMenus[menuhandler]->GItems[q]) -> str[i]=' ';
(GMenus[menuhandler]->GItems[q]) -> str[l+ll-1]=0;

switch(GMenus[menuhandler]->lang)
   {
   case 0: memcpy((GMenus[menuhandler]->GItems[q])->str, str0, strlen(str0)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet0;
	   break;
   case 1: memcpy((GMenus[menuhandler]->GItems[q])->str, str1, strlen(str1)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet1;
	   break;
   case 2: memcpy((GMenus[menuhandler]->GItems[q])->str, str2, strlen(str2)) ;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet2;
	   break;
   }
GMenus[menuhandler]->GItems[q]->hlet=SetHighLet(menuhandler,q);
SetFilename((GMenus[menuhandler]->GItems[q])->str,filename,filemask);

//memcpy((GMenus[menuhandler]->GItems[q]) -> str+l-strlen(filename),
//       filename,strlen(filename));

(GMenus[menuhandler]->GItems[q]) -> mask = mask;
(GMenus[menuhandler]->GItems[q]) -> string = filename;
(GMenus[menuhandler]->GItems[q]) -> menuhandler = filemask;
(GMenus[menuhandler]->GItems[q]) -> menuclose = menuclose;
(GMenus[menuhandler]->GItems[q]) -> helpitem  = helpitem;
(GMenus[menuhandler]->GItems[q]) -> statstr = 0;

(GMenus[menuhandler]->GItems[q]) -> length=l+ll;

GMenus[menuhandler]->qitems++;
return(GM_OK);
}
#endif


/************************** InitFuncItem ********************************/
#ifdef OUTER_OPERATION_MODE
int InitFuncKeyItem(int menuhandler,
		    char *str0, char *str1, char *str2,
		    int hlet0, int hlet1, int hlet2,
		    int funckey, int menuopen, int helpitem)
{
int q;

q=GMenus[menuhandler]->qitems;
if (q==MAX_ITEMS)
   { gerrmes(MANYITEMS,"InitFuncKeyItem",0,COMMON_ERROR); return(GM_OVERFLOW); }
GMenus[menuhandler]->GItems[q]=(struct GITEM *)farmalloc(sizeof(struct GITEM));
if (GMenus[menuhandler]->GItems[q]==NULL)
   { gerrmes(NOMEMENOUGH,"InitFuncKeyItem",0,COMMON_ERROR); return(GM_NOT_MEM); }

(GMenus[menuhandler]->GItems[q])->itemtype=FUNCKEY_ITEM;
(GMenus[menuhandler]->GItems[q])->hideitem='N';

(GMenus[menuhandler]->GItems[q]) -> str0 = str0;
(GMenus[menuhandler]->GItems[q]) -> str1 = str1;
(GMenus[menuhandler]->GItems[q]) -> str2 = str2;

switch(GMenus[menuhandler]->lang)
   {
   case 0: (GMenus[menuhandler]->GItems[q]) -> str = str0;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet0;
	   break;
   case 1: (GMenus[menuhandler]->GItems[q]) -> str = str1;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet1;
	   break;
   case 2: (GMenus[menuhandler]->GItems[q]) -> str = str2;
	   (GMenus[menuhandler]->GItems[q]) -> hlet = hlet2;
	   break;
   }
GMenus[menuhandler]->GItems[q]->hlet=SetHighLet(menuhandler,q);

(GMenus[menuhandler]->GItems[q]) -> menuhandler = funckey;
(GMenus[menuhandler]->GItems[q]) -> menuclose = menuopen;
(GMenus[menuhandler]->GItems[q]) -> helpitem  = helpitem;
(GMenus[menuhandler]->GItems[q]) -> statstr = 0;

(GMenus[menuhandler]->GItems[q]) -> length=
		 strlen( (GMenus[menuhandler]->GItems[q]) -> str );

(GMenus[menuhandler]->qitems)++;
return(GMenus[menuhandler]->qitems-1);
}
#endif


/************************ SetEnterStringColors **************************/
void SetEnterStringColors(int menuhandler,
		     int color, int groundcolor, int curscolor)
{
GMenus[menuhandler]->str_color=color;
GMenus[menuhandler]->str_groundcolor=groundcolor;
GMenus[menuhandler]->str_curscolor=curscolor;
}


/**************************** MoveMenu **********************************/
int MoveMenu(int menuhandler, int left, int top)
   /* устанавливает координаты меню
   неприменима к активному меню
   возвращает GR_OK или
	      GR_NOK, если меню активное */
{
int i;
for (i=0;i<MAX_MENUS && AVector[i]!=-1;i++)
     if (AVector[i]==menuhandler)
     { gerrmes(ALREADYACTIVE,"MoveMenu",0,COMMON_ERROR); return -1; }
GMenus[menuhandler]->left=left;
GMenus[menuhandler]->top=top;
return(GM_OK);
}

/**************************** HideItem **********************************/
void HideItem(int menuhandler, int itemhandler)
{
if (GMenus[menuhandler]==DESTROYED_MENU  ||
    itemhandler>GMenus[menuhandler]->qitems-1 ||
    itemhandler<0) gerrmes(BADITEMNUM,"HideItem",0,SYSTEM_ERROR);
GMenus[menuhandler]->GItems[itemhandler]->hideitem='H';
}

/*************************** RebirthItem ********************************/
void RebirthItem(int menuhandler, int itemhandler)
{
if (GMenus[menuhandler]==DESTROYED_MENU  ||
    itemhandler>GMenus[menuhandler]->qitems-1 ||
    itemhandler<0) gerrmes(BADITEMNUM,"HideItem",0,SYSTEM_ERROR);
GMenus[menuhandler]->GItems[itemhandler]->hideitem='N';
}


/*************************** SwitchReset ********************************/
int SwitchReset(int menuhandler, int itemhandler)
{
int r;

if (GMenus[menuhandler]->GItems[itemhandler]->itemtype != SWITCH_ITEM)
		     gerrmes(BADITEMTYPE,"SwitchReset",0,SYSTEM_ERROR);

r=SwitchSet( GMenus[menuhandler]->GItems[itemhandler]->str,
	   & (GMenus[menuhandler]->GItems[itemhandler]->
	       strings[ *(GMenus[menuhandler]->GItems[itemhandler]->mainhandler) ] ),
	   GMenus[menuhandler]->GItems[itemhandler]->strlength,
	   GMenus[menuhandler]->lang);
return r;
}


void SetExitKey(int menuhandler, int key)
{
GMenus[menuhandler]->exitkey=key;
}

/***************************************************************************/
/***************************************************************************/

/*********************** AddStringToStringItem ***************************/
int AddStringToStringItem(char *str, char *string, int strlength)
{
int i;

if (strlen(str)<strlength || strlen(string)>strlength)
 { gerrmes(BADSTRLEN,"AddStringToStringItem",0,COMMON_ERROR); return(GM_NOK);}
for (i=0;i<strlength;i++) str[strlen(str)-i-1]=' ';
strcpy(str + strlen(str) - strlen(string), string);
return (GM_OK);
}



/**************************** DrawMenu ***********************************/
int DrawMenu(int menuhandler, int saveoption)
{
int i,top,left,right,bottom,length=0,bordxlength,bordylength,ret;

for (i=0;i<GMenus[menuhandler]->qitems;i++)
   {
   if (
   GMenus[menuhandler]->GItems[i]->itemtype==SWITCH_ITEM  &&
	GMenus[menuhandler]->GItems[i]->menurepaint==-1 )
			gerrmes(NOLASTSTRING,"DrawMenu",0,SYSTEM_ERROR);
   length=max(length, (GMenus[menuhandler]->GItems[i]) -> length );
   }

GetBordLength(menuhandler,&bordxlength,&bordylength);

left=GMenus[menuhandler]->left;
top=GMenus[menuhandler]->top;

if (GMenus[menuhandler]->menutype=='V')
   {
   right= left+
	  bordxlength * 2 +
	  GMenus[menuhandler]->otstup * 2 +
	  length*LINE_WIDTH;
   bottom=top+
	  bordylength * 2 +
	  GMenus[menuhandler]->interval *  GMenus[menuhandler]->qitems +
	  LINE_HEIGHT * GMenus[menuhandler]->qitems ;
	  }
else
   {
   right= left+
	  bordxlength * 2 +
	  GMenus[menuhandler]->interval * GMenus[menuhandler]->qitems +
	  GetStrSumWidth(menuhandler,GMenus[menuhandler]->qitems)*LINE_WIDTH;
   bottom=top+
	  bordylength * 2 +
	  GMenus[menuhandler]->otstup * 2 +
	  LINE_HEIGHT ;
   }
GMenus[menuhandler]->right=right;
GMenus[menuhandler]->bottom=bottom;

Rmouse_hide();
if (saveoption==GM_SAVE && savebuf!=NULL && GMenus[menuhandler]->savehandler==0)
	   {
	   if ((long)Rimagesize(left,top,right,bottom)>(long)BUFFER_SIZE*1000l)
			     gerrmes(LOWBUFFER,"DrawMenu",0,SYSTEM_ERROR);
	   else
	      {
	      ret=SaveMenu(&(GMenus[menuhandler]->savepos),
		  &(GMenus[menuhandler]->savesize),
		  &(GMenus[menuhandler]->savehandler),
		  left,top,right,bottom);
	      if (ret!=GM_OK)
		  { gerrmes(CANTSAVE,"DrawMenu",0,COMMON_ERROR); return(ret); }
	      }
	   }

Rbarc(left,top,right,bottom,1,GMenus[menuhandler]->groundbordcolor);
Rbarc(left+bordxlength,top+bordylength,right-bordxlength,bottom-bordxlength,
      1,GMenus[menuhandler]->groundcolor);

switch(GMenus[menuhandler]->bordtype)
   {
   case NO_BORD:     break;
   case DOUBLE_BORD:
	Rramc(left+14,top+14,right-14,bottom-14,GMenus[menuhandler]->bordcolor);
   case SINGLE_BORD:
	Rramc(left+8,top+6,right-8,bottom-6,GMenus[menuhandler]->bordcolor);
	    break;
   }

if (GMenus[menuhandler]->bordtype!=REDUCED_BORD && GMenus[menuhandler]->bordname!=0)
   DrawBordStr(left,top,right,bordylength,GMenus[menuhandler]->groundbordcolor,
	       GMenus[menuhandler]->bordcolor,GMenus[menuhandler]->bordname);

for (i=0;i<GMenus[menuhandler]->qitems;i++)  DrawItem(menuhandler,i);
Rmouse_show();
return(GM_OK);
}


int DrawItem(int menuhandler, int itemhandler)
{
int l,t,r,b,ret;

DefineItemPosition(menuhandler,itemhandler,&l,&t,&r,&b);
Rmouse_hide();

if (GMenus[menuhandler]->GItems[itemhandler]->hideitem=='H')
   {
   Rbarc(l,t-8,r,b+4,1,GMenus[menuhandler]->hidebarcolor);
   Rcol(GMenus[menuhandler]->hidetextcolor);
   Rtext_with_high_let(l,t,GMenus[menuhandler]->GItems[itemhandler]->str,
	GMenus[menuhandler]->GItems[itemhandler]->hlet,
	GMenus[menuhandler]->highletcolor);
   if (GMenus[menuhandler]->curitem!=itemhandler)
      {
      Rbarc(l,t-8,r,b+4,1,GMenus[menuhandler]->hidebarcolor);
      Rcol(GMenus[menuhandler]->hidetextcolor);
      }
   else
      {
      Rbarc(l,t-8,r,b+4,1,GMenus[menuhandler]->highbarcolor);
      if (KeyBuffer[0]==-1) ShowStatus(menuhandler,itemhandler);
      Rcol(GMenus[menuhandler]->hidehightextcolor);
      }
   Rtext(l,t,GMenus[menuhandler]->GItems[itemhandler]->str);
   ret=GM_OK;
   }
else
   {
   if (GMenus[menuhandler]->curitem!=itemhandler)
      {
      Rbarc(l,t-8,r,b+4,1,GMenus[menuhandler]->barcolor);
      Rcol(GMenus[menuhandler]->textcolor);
      }
   else
      {
      Rbarc(l,t-8,r,b+4,1,GMenus[menuhandler]->highbarcolor);
      if (KeyBuffer[0]==-1) ShowStatus(menuhandler,itemhandler);
      Rcol(GMenus[menuhandler]->hightextcolor);
      }
   ret= Rtext_with_high_let(l,t,GMenus[menuhandler]->GItems[itemhandler]->str,
      GMenus[menuhandler]->GItems[itemhandler]->hlet,
      GMenus[menuhandler]->highletcolor);
   }
if (ret!=GM_OK) gerrmes(CANTOUTTEXT,"DrawItem",0,SYSTEM_ERROR);
Rmouse_show();
return(GM_OK);
}


/********************* DefineItemPosition ******************************/
int DefineItemPosition(int menuhandler, int itemhandler,
		       int *l, int *t, int *r, int *b)
{
int bordxlength,bordylength;

GetBordLength(menuhandler,&bordxlength,&bordylength);

if (GMenus[menuhandler]->menutype=='V')
  {
  *l=GMenus[menuhandler]->left + bordxlength + GMenus[menuhandler]->otstup;
  *t=GMenus[menuhandler]->top  + bordylength + GMenus[menuhandler]->interval/2 +
    (GMenus[menuhandler]->interval+LINE_HEIGHT) * itemhandler;
  *r=GMenus[menuhandler]->right - bordxlength - GMenus[menuhandler]->otstup;
  *b=GMenus[menuhandler]->top   + bordylength+ GMenus[menuhandler]->interval/2 +
    (GMenus[menuhandler]->interval+LINE_HEIGHT ) * itemhandler + LINE_HEIGHT;
  }
else
  {
  *l=GMenus[menuhandler]->left + bordxlength + GMenus[menuhandler]->interval/2 +
    GMenus[menuhandler]->interval * itemhandler +
    GetStrSumWidth(menuhandler,itemhandler)*LINE_WIDTH;
  *t=GMenus[menuhandler]->top + bordylength +GMenus[menuhandler]->otstup;
  *r=*l + GMenus[menuhandler]->GItems[itemhandler]->length * LINE_WIDTH;
  *b=GMenus[menuhandler]->bottom - bordylength - GMenus[menuhandler]->otstup;
  }
return (GM_OK);
}


/********************** WhichItemPosition ******************************/
int WhichItemPosition(int menuhandler, int x, int y)
    /* определяет в какой ITEM данного меню попадают координаты x,y
       используется в MouseOperate
       возвращает номер ITEMа или -1 */
{
int i,l,t,r,b;

for (i=0;i<GMenus[menuhandler]->qitems;i++)
   {
   DefineItemPosition(menuhandler,i,&l,&t,&r,&b);
   if (x>=l && x<=r && y>=t && y<=b
      && GMenus[menuhandler]->GItems[i]->hideitem!='H') return i;
   }
return -1;
}


/************************ DrawBordStr ********************************/
int DrawBordStr(int left, int top, int right, int bordylength,
		int bordgroundcolor, int bordcolor, char *str)
{
int length;
length=strlen(str)*LINE_WIDTH;
Rbarc((left+right-length)/2,top,(left+right+length)/2,top+bordylength-3,1,
       bordgroundcolor);
Rtextc((left+right-length)/2,top+(bordylength-LINE_HEIGHT)/2,str,bordcolor);
return(GM_OK);
}

/************************ GetStrSumWidth *****************************/
int GetStrSumWidth(int menuhandler, int item)
{
int i,length=0;
for (i=0;i<item;i++)  length+=GMenus[menuhandler]->GItems[i]->length;
return(length);
}

/************************ GetBordLength *****************************/
int GetBordLength(int menuhandler, int *bordxlength, int *bordylength)
{
switch(GMenus[menuhandler]->bordtype)
   {
   case NO_BORD:       *bordxlength=19;        *bordylength=17;      break;
   case SINGLE_BORD:   *bordxlength=17+10;     *bordylength=17+7;    break;
   case DOUBLE_BORD:   *bordxlength=17+10;     *bordylength=17+7;    break;
   case REDUCED_BORD:  *bordxlength=0;         *bordylength=0;       break;
   }
return (GM_OK);
}


/************************ ActivateMenu *****************************/
#ifdef RECUR_OPERATION_MODE
int ActivateMenu(int menuhandler)
{
int i,c,cur,r=GM_OK,uprepmenu,l,k,mhandler,left,top,right,bottom;

for (i=0;AVector[i]!=-1;i++)
 if (AVector[i]==menuhandler)
    {
    gerrmes(ALREADYACTIVE,"ActivateMenu",0,COMMON_ERROR);
    return(GM_OK);
    }
AVector[active_menu++]=menuhandler;
DrawMenu(menuhandler,GM_SAVE);

ret:
cur=GMenus[menuhandler]->curitem;
if (CloseCounter!=0) { AddKeyToBuf(ESC); CloseCounter--; }
c=GetEvent();
switch(c)
   {
   case UP:
	 if (GMenus[menuhandler]->menutype=='V')
	    {
	    GMenus[menuhandler]->curitem=(cur==0)
		 ? GMenus[menuhandler]->qitems-1 : cur-1 ;
	    menusound(menuhandler);
	    DrawItem(menuhandler,cur);
	    DrawItem(menuhandler,GMenus[menuhandler]->curitem);
	    }
	  break;
   case DOWN:
	 if (GMenus[menuhandler]->menutype=='V')
	    {
	    GMenus[menuhandler]->curitem=(cur==GMenus[menuhandler]->qitems-1)
		  ? 0 : cur+1 ;
	    menusound(menuhandler);
	    DrawItem(menuhandler,cur);
	    DrawItem(menuhandler,GMenus[menuhandler]->curitem);
	    }
	 else if (GMenus[menuhandler]->GItems[cur]->itemtype==MENU_ITEM)
	    {
	    AddKeyToBuf(ENTER);
	    goto ret;
	    }
	 break;
	  /*************/
   case LEFT:
	 if (GMenus[menuhandler]->menutype=='H')
	    {
	    GMenus[menuhandler]->curitem=(cur==0)
		 ? GMenus[menuhandler]->qitems-1 : cur-1 ;
	    menusound(menuhandler);
	    DrawItem(menuhandler,cur);
	    DrawItem(menuhandler,GMenus[menuhandler]->curitem);
	    }
	 else
	    {
	    mhandler=GetUpperMenuHandler(menuhandler,1);
	    if (mhandler!=-1 && GMenus[mhandler]->menutype=='H')
	       {
	       AddKeyToBuf(ESC);
	       AddKeyToBuf(LEFT);
	       AddKeyToBuf(MENU_ENTER);
	       }
	    }
	  break;
   case RIGHT:
	 if (GMenus[menuhandler]->menutype=='H')
	    {
	    GMenus[menuhandler]->curitem=(cur==GMenus[menuhandler]->qitems-1)
		  ? 0 : cur+1 ;
	    menusound(menuhandler);
	    DrawItem(menuhandler,cur);
	    DrawItem(menuhandler,GMenus[menuhandler]->curitem);
	    }
	 else
	    {
	    mhandler=GetUpperMenuHandler(menuhandler,1);
	    if (mhandler!=-1 && GMenus[mhandler]->menutype=='H')
	       {
	       AddKeyToBuf(ESC);
	       AddKeyToBuf(RIGHT);
	       AddKeyToBuf(MENU_ENTER);
	       }
	    }
	 break;
   case HOME:
	  GMenus[menuhandler]->curitem=0;
	  menusound(menuhandler);
	  DrawItem(menuhandler,cur);
	  DrawItem(menuhandler,GMenus[menuhandler]->curitem);
	  break;
   case END:
	  GMenus[menuhandler]->curitem=GMenus[menuhandler]->qitems-1;
	  menusound(menuhandler);
	  DrawItem(menuhandler,cur);
	  DrawItem(menuhandler,GMenus[menuhandler]->curitem);
	  break;
   case ENTER:  case MENU_ENTER:
	if (GMenus[menuhandler]->GItems[cur]->hideitem=='H') break;
	if (c==MENU_ENTER &&
	    GMenus[menuhandler]->GItems[cur]->itemtype!=MENU_ITEM)
	       {
	       ShowStatus(menuhandler,cur);
	       break;
	       }
	switch(GMenus[menuhandler]->GItems[cur]->itemtype)
	   {
	   case MENU_ITEM:
		GMenus[GMenus[menuhandler]->GItems[cur]->menuhandler]->menucall=menuhandler;
		if (GMenus[menuhandler]->GItems[cur]->funchandler!=NULL)
		   r=(*(GMenus[menuhandler]->GItems[cur]->funchandler))();
		ActivateMenu(GMenus[menuhandler]->GItems[cur]->menuhandler);
		GMenus[GMenus[menuhandler]->GItems[cur]->menuhandler]->menucall=-1;
		ShowStatus(menuhandler,cur);
		break;
	   case YES_NO_ITEM:
		*(GMenus[menuhandler]->GItems[cur]->mainhandler)=
		  (*(GMenus[menuhandler]->GItems[cur]->mainhandler)==YES) ?
		   NO : YES ;
		r=YesNoSet(GMenus[menuhandler]->GItems[cur]->str,
			 GMenus[menuhandler]->GItems[cur]->mainhandler,
			 GMenus[menuhandler]->lang);
		if (r!=GM_OK) return (r);
		DrawItem(menuhandler,cur);
		break;
	   case SWITCH_ITEM:
		*(GMenus[menuhandler]->GItems[cur]->mainhandler)=
		  ( *(GMenus[menuhandler]->GItems[cur]->mainhandler)==
		     GMenus[menuhandler]->GItems[cur]->menuhandler - 1 ) ?
		   0 : *(GMenus[menuhandler]->GItems[cur]->mainhandler)+1 ;
		r=SwitchSet( GMenus[menuhandler]->GItems[cur]->str,
			   & (GMenus[menuhandler]->GItems[cur]->
		    strings[ *(GMenus[menuhandler]->GItems[cur]->mainhandler) ] ),
		    GMenus[menuhandler]->GItems[cur]->strlength,
		    GMenus[menuhandler]->lang);
		if (r!=GM_OK) return (r);
		DrawItem(menuhandler,cur);
		break;
	   case FUNC_ITEM:
		uprepmenu=RestoreLevels(menuhandler,
			    GMenus[menuhandler]->GItems[cur]->menurepaint);
		Rmouse_hide();
		RestoreStatusScreen();
		r=(*(GMenus[menuhandler]->GItems[cur]->funchandler))();
		mhandler=GetUpperMenuHandler(menuhandler,
			   GMenus[menuhandler]->GItems[cur]->menuclose);
		if (mhandler!=-1 && uprepmenu!=-1)
				      SaveLevels(uprepmenu,mhandler);
		Rmouse_show();
		break;
	   case STRING_ITEM:
		DefineItemPosition(menuhandler,cur,&left,&top,&right,&bottom);
		Rmouse_hide();
		REnterString(GMenus[menuhandler]->GItems[cur]->string,
		  right-(GMenus[menuhandler]->GItems[cur]->strlength) *
						      (LINE_WIDTH+4)-3,
		  top-4,
		  GMenus[menuhandler]->GItems[cur]->strlength,
		  LINE_WIDTH+4,
		  GMenus[menuhandler]->str_color,
		  GMenus[menuhandler]->str_groundcolor,
		  GMenus[menuhandler]->str_curscolor,
		  GMenus[menuhandler]->GItems[cur]->menuhandler,YES);
		AddStringToStringItem((GMenus[menuhandler]->GItems[cur]) ->str,
		  (GMenus[menuhandler]->GItems[cur]) ->string,
		  (GMenus[menuhandler]->GItems[cur]) ->strlength);
		Rmouse_show();
		DrawMenu(menuhandler,GM_NOSAVE);
		break;
	   #ifdef FILENAME_EXIST
	   case FILENAME_ITEM:
		GMpath[0]=0;
		DefineItemPosition(menuhandler,cur,&left,&top,&right,&bottom);
		r=ListDir(GMenus[menuhandler]->GItems[cur]->mask,GMpath,
			  GMenus[menuhandler]->GItems[cur]->string,
			  left,bottom);
		if (r!=0) DrawMenu(menuhandler,GM_NOSAVE);
		SetFilename(GMenus[menuhandler]->GItems[cur]->str,
			  GMenus[menuhandler]->GItems[cur]->string,
			  GMenus[menuhandler]->GItems[cur]->menuhandler);
		DrawItem(menuhandler,cur);
		break;
	   #endif
	   }
	if (GMenus[menuhandler]->GItems[cur]->itemtype!=MENU_ITEM)
	    CloseCounter=GMenus[menuhandler]->GItems[cur]->menuclose;
	break;
#ifdef HELP_EXIST
   case F1:
	if (GMenus[menuhandler]->GItems[cur]->helpitem != -1)
	HShowHelp(GMenus[menuhandler]->GItems[cur]->helpitem);
	Rmouse_show();
	break;
#endif
   case ESC:
	if (AVector[0]==-1)
	   {
	   CloseCounter=0;
	   return(r);
	   }
	if (GMenus[menuhandler]->exitkey!=ESC)
	/********** ---------> */
	   {
	   AddKeyToBuf(END);   /*later*/
	   AddKeyToBuf(ENTER);
	   MainMenuLeaveMode = ESC_IN_MAIN_MENU;
	   break;
	   }
	/* <-------- ***********/
	r=RestoreMenu(menuhandler);
	AVector[--active_menu]=-1;
	return(r);
   default:
	if (GMenus[menuhandler]->exitkey==c)
	   {
	   r=RestoreMenu(menuhandler);
	   AVector[--active_menu]=-1;
	   return(r);
	   }

	if (c>=9900 && c<=9999)
	   {
	   l=c-9900;
	   if (l>=GMenus[menuhandler]->qitems || l<0)
	       gerrmes(BADITEMNUM,"ActivateMenu",0,SYSTEM_ERROR);
	   GMenus[menuhandler]->curitem=l;
	   DrawItem(menuhandler,cur);
	   DrawItem(menuhandler,GMenus[menuhandler]->curitem);
	   goto ret;
	   }
	l=IfHighLet(c,menuhandler);
	if (l!=-1)
	   {
	   k=cur;
	   GMenus[menuhandler]->curitem=l;
	   DrawItem(menuhandler,k);
	   DrawItem(menuhandler,l);
	   AddKeyToBuf(ENTER);
	   goto ret;
	   }
   }
goto ret;
}
#endif /* RECUR_OPERATION_MODE */

/************************** YesNoSet *******************************/
int YesNoSet(char *str, int *yesnohandler, int lang)
{
char *yes[]={" Yes","  Да","  Ja"},*no[]={"  No"," Нет","Nein"};

if (strlen(str)<4)
 { gerrmes(BADSTRLEN,"YesNoSet",0,COMMON_ERROR); return(GM_NOK);}

if (*yesnohandler==YES) strcpy(str+strlen(str)-4,yes[lang]);
   else strcpy(str+strlen(str)-4,no[lang]);

return (GM_OK);
}

/************************** SwitchSet *******************************/
int SwitchSet(char *str, struct STRINGS *strings, int strlength, int lang)
{
char *s;
int i;

switch(lang)
   {
   case 0:  s=strings->str0;   break;
   case 1:  s=strings->str1;   break;
   case 2:  s=strings->str2;   break;
   }
if (strlen(s)>strlength || strlen(s)>strlen(str) )
 { gerrmes(BADSTRLEN,"SwitchSet",0,COMMON_ERROR); return(GM_NOK);}

for (i=0;i<strlength;i++) str[strlen(str)-strlength+i]=' ';
memcpy(str+strlen(str)-strlength,s,strlen(s));
return (GM_OK);
}

/************************** SetFilename *******************************/
#ifdef FILENAME_EXIST
void SetFilename(char *str, char *path, int filemask)
{
int i,l,ll,length;
char filename[14],*s;

ll=(filemask==GM_EXTENSION) ? 14 : 9 ;

length=strlen(str);
for (i=length-1;i>=0;i--)
    if (path[i]=='\\') { strcpy(filename,path+i+1); break; }
      else if (i==0) { strcpy(filename,path); break; }
if (filemask==GM_NOEXTENSION && (s=strchr(filename,'.')) !=NULL ) *s=0;
l=strlen(filename);
if (l>14 || l>8 && filemask==GM_NOEXTENSION)
   gerrmes(BADNAMELEN,"SetFilename",0,SYSTEM_ERROR);

for (i=0;i<ll;i++) str[length-1-i]=' ';
memcpy(str+length-l,filename,l);
}
#endif

/************************** InitSaveBuf *******************************/
int InitSaveBuf(void)
{
int i,j;
char filenam[]={"c:\\tmp  .swp"};
struct ffblk ff;

if (savebuf!=0) return 0;
savebuf=(char *)farmalloc((unsigned long)BUFFER_SIZE*1000l);
if (savebuf==NULL) { gerrmes(NOSAVEMEM,"InitSaveBuf",1,COMMON_ERROR); return GM_NOK; }

statbuf=(char *)farmalloc(
  (unsigned long)Rimagesize(STAT_LEFT,STAT_TOP,STAT_RIGHT,STAT_BOTTOM) );
if (statbuf==NULL)
   {
   gerrmes(NOSAVEMEM,"InitSaveBuf",2,COMMON_ERROR);
   stathandler=STAT_NOTINIT;
   }
 else stathandler=STAT_NOTACTIVE;

GMpath=(char *)farmalloc((unsigned long)MAXPATH);
if (GMpath==NULL) { gerrmes(NOSAVEMEM,"InitSaveBuf",3,COMMON_ERROR); return GM_NOK; }

for (i=0;i<MAX_MENUS;i++)
    {
    AVector[i]=-1;
    KeyBuffer[i]=-1;
    filenam[6]=i/10+48;
    filenam[7]=i%10+48;
    for (j=0;j<=LASTDRIVE-'C';j++)
       {
       filenam[0]='C'+j;
       if (findfirst(filenam,&ff,FA_ARCH)==0) unlink(filenam);
       }
    }
return GM_OK;
}


/************************** SaveMenu *******************************/
int SaveMenu(signed long *savepos, long *savesize, int *savehandler,
	     int left, int top, int right, int bottom)
{
int menuhandler,n;
signed long begin,end;

*savesize=(long)Rimagesize(left,top,right,bottom);
ret:
menuhandler=LastSaveBufCoord(&begin,&end);
if (menuhandler==-1 || (long)BUFFER_SIZE*1000l-end>*savesize)
   {
   Rgetimage(left,top,right,bottom,savebuf+end);
   *savepos=end;
   *savehandler=1;
   }
else
   {
   n=SaveBufferOnDisk(menuhandler);
   if (n!=GM_OK) return(GM_NOK);
   goto ret;
   }
return(GM_OK);
}


/************************* RestoreMenu *****************************/
int RestoreMenu(int menuhandler)
{
int rhandler,r,e;
signed long begin,end,pos;
char filenam[30];

if (GMenus[menuhandler]->savehandler==0) return(1);
Rmouse_hide();
restore_met:
if (GMenus[menuhandler]->savepos>=0)
   {
   Rputimage(GMenus[menuhandler]->left,GMenus[menuhandler]->top,
		savebuf+GMenus[menuhandler]->savepos,0);
   GMenus[menuhandler]->savehandler=0;
   MemoryMove(GMenus[menuhandler]->savepos,0);
   }
else
   {
   ret:
   rhandler=LastSaveBufCoord(&begin,&end);
   if (rhandler!=-1)
      {
      if ((long)BUFFER_SIZE*1000l-end<GMenus[menuhandler]->savesize)
	 {
	 pos=GMenus[r]->savepos;      /* r ? */
	 r=SaveBufferOnDisk(rhandler);
	 if (r!=GM_OK)
	    { gerrmes(CANTSAVE,"RestoreMenu",0,COMMON_ERROR); return r; }
	 MemoryMove(pos,0);
	 goto ret;
	 }
      }
   r=TakeSwapFileName(menuhandler,"tmp  .swp",3,filenam);
   if (r!=GM_OK)
	{ gerrmes(CANTFIND,"RestoreMenu",0,COMMON_ERROR); return(GM_NOK); }
   e=readmem(filenam,savebuf+end);
    if (e==CANT_OPEN)
	{ gerrmes(OPEN_ERROR,"RestoreMenu",0,COMMON_ERROR); return(GM_NOK); }
    else if (e==CANT_READ)
	{ gerrmes(READ_ERROR,"RestoreMenu",0,COMMON_ERROR);  return(GM_NOK); }
   unlink(filenam);
   GMenus[menuhandler]->savepos=end;
   goto restore_met;
   }
Rmouse_show();
RestoreStatusScreen();
return(GM_OK);
}


/************************* MemoryMove *****************************/
int MemoryMove(signed long srcpos, signed long srcsize)
{
int mhandler;
signed long pos,size;

pos=srcpos;
size=srcsize;

ret:
mhandler=FindNextMenu(pos);
if (mhandler!=-1)
   {
   memmove(savebuf+pos+size,savebuf+GMenus[mhandler]->savepos,
			    GMenus[mhandler]->savesize);
   GMenus[mhandler]->savepos=pos+size;
   pos+=size;
   size=GMenus[mhandler]->savesize;
   goto ret;
   }

return(GM_OK);
}

/************************ FindNextMenu ****************************/
int FindNextMenu(signed long menupos)
/* находит следующее более позднее меню в буфере;
   возвращает handler, если нет - то -1 */
{
int i,hand=-1;
signed long pos;

pos=(long)BUFFER_SIZE*1000l;

for (i=0;i<MAX_MENUS;i++)
   if ( GMenus[i]!=DESTROYED_MENU && GMenus[i]->savepos<pos &&
	GMenus[i]->savepos>menupos && GMenus[i]->savehandler==1)   hand=i;
return(hand);
}


/*********************** LastSaveBufCoord *************************/
int LastSaveBufCoord(signed long *begin, signed long *end)
/* если ничего не сохранялось, возвращает -1, иначе указатель на меню,
   сохраненное последним  */
{
int i,ret=-1;
*begin=-1;
*end=0;

for (i=0;i<MAX_MENUS;i++)
   if (GMenus[i]!=DESTROYED_MENU && GMenus[i]->savepos>=0 &&
       GMenus[i]->savehandler==1  && GMenus[i]->savepos>*begin)
      {
      *begin=GMenus[i]->savepos;
      *end=*begin+GMenus[i]->savesize;
      ret=i;
      }
return(ret);
}


/*********************** SaveBufferOnDisk *************************/
int SaveBufferOnDisk(int menuhandler)
{
signed long size;
int drive,r,e;
char filenam[30];

size=GMenus[menuhandler]->savesize;
drive=FindFreeDrive(size);
if (drive==0) return(GM_NOK);
r=GetSwapFileName("tmp  .swp",drive,3,filenam);
e=writemem(filenam,savebuf+GMenus[menuhandler]->savepos,size);
 if (e==CANT_CREATE)
   { gerrmes(CREAT_ERROR,"SaveBufferOnDisk",0,COMMON_ERROR);  return(GM_NOK); }
 else if (e==CANT_WRITE)
   { gerrmes(WRITE_ERROR,"SaveBufferOnDisk",0,COMMON_ERROR);  return(GM_NOK);  }
GMenus[menuhandler]->savepos=-r;
GMenus[menuhandler]->savesize=0;
return(GM_OK);
}

/*********************** GetSwapFileName **************************/
int GetSwapFileName(char *mask, int drive, int firstlet, char *filenam)
{
int i;
struct ffblk ff;

filenam[0]=drive+'A'-1;
filenam[1]=':';
filenam[2]='\\';
strcpy(filenam+3,mask);
for (i=1;i<99;i++)
   {
   filenam[3+firstlet]=i/10+48;
   filenam[4+firstlet]=i%10+48;
   if (findfirst(filenam,&ff,FA_ARCH)==-1) return i;
   }
return(0);
}

/*********************** TakeSwapFileName **************************/
int TakeSwapFileName(int menuhandler, char *mask, int firstlet, char *filenam)
{
int i,n;
struct ffblk ff;

if (GMenus[menuhandler]->savepos>=0) return(GM_NOK);
n=-GMenus[menuhandler]->savepos;

strcpy(filenam+3,mask);
filenam[3+firstlet]=n/10+48;
filenam[4+firstlet]=n%10+48;

for (i=2;i<=LASTDRIVE-'A';i++)
   {
   filenam[0]=i+'A';
   filenam[1]=':';
   filenam[2]='\\';
   if (findfirst(filenam,&ff,FA_ARCH)==0) return (GM_OK);
   }
return(GM_NOK);
}


/*********************** RestoreLevels **************************/
int RestoreLevels(int menuhandler, int levels)
/* возвращает handler последнего востановленного меню
	  или -1, если ничего не восстанавливалось */
{
int i,nextmenu,mhandler,r;

mhandler=(levels==0) ? -1 : menuhandler;

for (i=0;i<levels;i++)
   {
   r=RestoreMenu(mhandler);
   if (r!=GM_OK)
     { gerrmes(CANTRESTORE,"RestoreLevels",0,COMMON_ERROR); return(r); }
   nextmenu=GMenus[mhandler]->menucall;
   if (nextmenu==-1) return mhandler;
   mhandler=nextmenu;
   }
return(mhandler);
}

/************************* SaveLevels **************************/
int SaveLevels(int srcmenuhandler, int destmenuhandler)
{
int mhandler,r;

mhandler=srcmenuhandler;

while(1==1)
   {
   r=DrawMenu(mhandler,GM_SAVE);
   if (r!=GM_OK)
     { gerrmes(CANTDRAW,"SaveLevels",mhandler,COMMON_ERROR); return(r); }
   if (mhandler==destmenuhandler) break;
   mhandler=GMenus[mhandler]->GItems[GMenus[mhandler]->curitem]->menuhandler;
   }
return(GM_OK);
}


/********************** GetUpperMenuHandler ********************/
#ifdef RECUR_OPERATION_MODE
int GetUpperMenuHandler(int menuhandler, int qlevels)
{
int i;
for (i=0; i<MAX_MENUS && AVector[i]!=menuhandler ;i++);
if (i<qlevels) return(-1);
return(AVector[i-qlevels]);
}
#endif

/************************* IfHighLet ***************************/
int IfHighLet(int let, int menuhandler)
/* возвращает номер ITEMа или -1, если эта буква в меню не задействована */
{
int i;
for (i=0;i<GMenus[menuhandler]->qitems;i++)
   {
   if ( ftoupper(let) == ftoupper( GMenus[menuhandler]->GItems[i]->
	str[GMenus[menuhandler]->GItems[i]->hlet] ) &&
	GMenus[menuhandler]->GItems[i]->hideitem!='H' )   return i;
   }
return -1;
}

/************************* SetHighLet ***************************/
int SetHighLet(int menuhandler, int itemhandler)
{
int i;

for (i=0;i<strlen(GMenus[menuhandler]->GItems[itemhandler]->str);i++)
   if ( ftoupper(GMenus[menuhandler]->GItems[itemhandler]->hlet) ==
     ftoupper(GMenus[menuhandler]->GItems[itemhandler]->str[i]) ) return(i);
return(-1);
}


/************************* AddKeyToBuf **************************/
#ifdef RECUR_OPERATION_MODE
void AddKeyToBuf(int c)
{
int i;
for (i=0;i<MAX_MENUS && KeyBuffer[i]!=-1;i++);
if (i==MAX_MENUS) gerrmes(KEY_BUFFER,"AddKeyToBuf",0,SYSTEM_ERROR);
   else KeyBuffer[i]=c;
}
#endif

/************************* GetKeyToBuf **************************/
#ifdef RECUR_OPERATION_MODE
int GetKeyFromBuf()
{
int i,c;
c=KeyBuffer[0];
for (i=0;i<MAX_MENUS-1;i++) KeyBuffer[i]=KeyBuffer[i+1];
return(c);
}
#endif

/*************************** GetEvent **************************/
#ifdef RECUR_OPERATION_MODE
int GetEvent()
{
int c,x,y;

ret:
c=GetKeyFromBuf();
if (c==-1)
   {
   while(1==1)
      {
      if (bioskey(1)!=0) break;
      if (Rmouse_status(&x,&y)==1 && MouseOperate(x,y)==1) goto ret;
      }
   c=getsym();
   }
return(c);
}
#endif

/************************* MouseOperate *************************/
#ifdef RECUR_OPERATION_MODE
int MouseOperate(int x, int y)
{
int i,j,k,rx,ry,mhandler,mhand,l,t,r,b;

rx=x; ry=y;
Rbackconvert(&rx,&ry);
mhandler=AVector[active_menu-1];
k=WhichItemPosition(mhandler,rx,ry);
if (k!=-1)
   {
   AddKeyToBuf(9900+k);
   AddKeyToBuf(ENTER);
   return 1;
   }

for (i=0;i<MAX_MENUS;i++)
   {
   mhand=GetUpperMenuHandler(mhandler,i+1);
   if (mhand==-1) return 0;
   l=GMenus[mhand]->left;
   t=GMenus[mhand]->top;
   r=GMenus[mhand]->right;
   b=GMenus[mhand]->bottom;
   if (rx>=l && rx<=r && ry>=t && ry<=b)
      {
      for (j=0;j<=i;j++) AddKeyToBuf(ESC);
      k=WhichItemPosition(mhand,rx,ry);
      if (k!=-1)
	 {
	 AddKeyToBuf(9900+k);
	 AddKeyToBuf(ENTER);
	 }
      return 1;
      }
   }

return(1);
}
#endif



/*********************** SetSaveBufferSize **********************/
void SetSaveBufferSize(int percents, int minimum)
{
unsigned long l;
l=coreleft();
BUFFER_SIZE=(int)(l*percents/100000l);
if (BUFFER_SIZE<minimum) BUFFER_SIZE=minimum;
}


/************************ SetStatusColor ***********************/
void SetStatusColor(int menuhandler, int col, int bkcol)
{
GMenus[menuhandler]->statcolor=col;
GMenus[menuhandler]->statgroundcolor=bkcol;
}


/************************** AddStatus **************************/
void AddStatus(int menuhandler, int itemhandler,
	      char *str0, char *str1, char *str2)
{
switch(GMenus[menuhandler]->lang)
   {
   case 0: GMenus[menuhandler]->GItems[itemhandler]->statstr=str0; break;
   case 1: GMenus[menuhandler]->GItems[itemhandler]->statstr=str1; break;
   case 2: GMenus[menuhandler]->GItems[itemhandler]->statstr=str2; break;
   }
}


/************************* ShowStatus **************************/
int ShowStatus(int menuhandler, int itemhandler)
{
switch(stathandler)
   {
   case STAT_NOTINIT:  return(GM_OK);
   case STAT_NOTACTIVE:
	if ( GMenus[menuhandler]->GItems[itemhandler]->statstr == 0)
					     return(GM_OK);
	SaveStatusScreen();
	Rbarc(STAT_LEFT,STAT_TOP,STAT_RIGHT,STAT_BOTTOM,1,
	       GMenus[menuhandler]->statgroundcolor);
	Rtextc(STAT_LEFT+8,STAT_TOP+4,
	       GMenus[menuhandler]->GItems[itemhandler]->statstr,
	       GMenus[menuhandler]->statcolor);
	break;
   case STAT_ACTIVE:
	if ( GMenus[menuhandler]->GItems[itemhandler]->statstr == 0)
		RestoreStatusScreen();
	 else
	   {
	   Rbarc(STAT_LEFT,STAT_TOP,STAT_RIGHT,STAT_BOTTOM,1,
	      GMenus[menuhandler]->statgroundcolor);
	   Rtextc(STAT_LEFT+8,STAT_TOP+4,
	      GMenus[menuhandler]->GItems[itemhandler]->statstr,
	      GMenus[menuhandler]->statcolor);
	    }
	 break;
   }
return(GM_OK);
}


/********************** SaveStatusScreen ***********************/
int SaveStatusScreen()
{
if (stathandler!=STAT_NOTACTIVE) return(GM_NOK);
Rgetimage(STAT_LEFT,STAT_TOP,STAT_RIGHT,STAT_BOTTOM,statbuf);
stathandler=STAT_ACTIVE;
return(GM_OK);
}

/********************* RestoreStatusScreen *********************/
int RestoreStatusScreen()
{
if (stathandler!=STAT_ACTIVE) return(GM_NOK);
Rputimage(STAT_LEFT,STAT_TOP,statbuf,0);
stathandler=STAT_NOTACTIVE;
return(GM_OK);
}


/*********************** MoveItemBar ***************************/
#ifdef OUTER_OPERATION_MODE
int MoveItemBar(int mhandler, int olditem, int newitem)
/* возвpат - GM_OK пpи успехе или GM_NOK пpи обломе */
{
if (newitem<0 || newitem >= GMenus[mhandler]->qitems )
    gerrmes(BADITEMNUM,"MoveItemBar",0,SYSTEM_ERROR);
if (olditem==newitem) return GM_NOK;
if  (GMenus[mhandler]->GItems[newitem]->hideitem=='H') return GM_NOK;
GMenus[mhandler]->curitem=newitem;
DrawItem(mhandler,olditem);
DrawItem(mhandler,newitem);
return GM_OK;
}
#endif

/*********************** FindNewItem ***************************/
#ifdef OUTER_OPERATION_MODE
int FindNewItem(int mhandler, int olditem, int rezhim)
/* возвpат - номеp найденного itemа или GM_NOK пpи обломе */
{
int i;

switch(rezhim)
   {
   case NEXT:
      for (i=olditem+1;i<GMenus[mhandler]->qitems;i++)
	  if (GMenus[mhandler]->GItems[i]->hideitem!='H') return i;
      for (i=0;i<olditem;i++)
	  if (GMenus[mhandler]->GItems[i]->hideitem!='H') return i;
      return GM_NOK;
   case PREV:
      for (i=olditem-1;i>=0;i--)
	  if (GMenus[mhandler]->GItems[i]->hideitem!='H') return i;
      for (i=GMenus[mhandler]->qitems-1;i>olditem;i--)
	  if (GMenus[mhandler]->GItems[i]->hideitem!='H') return i;
      return GM_NOK;
   case FIRST:
      for (i=0;i<olditem;i++)
	  if (GMenus[mhandler]->GItems[i]->hideitem!='H') return i;
      return GM_NOK;
   case LAST:
      for (i=GMenus[mhandler]->qitems-1;i>olditem;i--)
	  if (GMenus[mhandler]->GItems[i]->hideitem!='H') return i;
      return GM_NOK;
   default: termes("Case error","FindNewItem"); return GM_NOK;
   }
}
#endif


/************************ IfFuncKey ****************************/
#ifdef OUTER_OPERATION_MODE
int IfFuncKey(int key, int menuhandler)
/* возвращает номер ITEMа или -1, если эта клавиша в меню не задействована */
{
int i;
for (i=0;i<GMenus[menuhandler]->qitems;i++)
   {
   if ( GMenus[menuhandler]->GItems[i]->itemtype == FUNCKEY_ITEM &&
	GMenus[menuhandler]->GItems[i]->menuhandler == key &&
	GMenus[menuhandler]->GItems[i]->hideitem!='H')   return i;
   }
return -1;
}
#endif


void menusound(int menuhandler)
{
if (GMenus[menuhandler]->freq!=0)
   msound(GMenus[menuhandler]->freq);
}

void badkey()
{
sound(1000);
delay(50);
nosound();
}

void msound(int fr)
{
sound(fr);
delay(10);
nosound();
}

/*
int ftoupper(int c)
{
if (c>='a' && c<='z') return(toupper(c));
if (c>='а' && c<='п') return(c-32);
if (c>='р' && c<='я') return(c-80);
return(c);
}
*/

#ifdef DEBUG_MODE
void MenuInf(void)
{
int i;

for (i=0;i<MAX_MENUS;i++)
   if (GMenus[i]!=DESTROYED_MENU)
      {
      fprintf(F,"\nN  %d   позиция  %ld   размер   %ld  savehandler  %d",
	     i,GMenus[i]->savepos,GMenus[i]->savesize,GMenus[i]->savehandler);
      }
}
#endif
