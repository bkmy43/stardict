#include "graph.h"
#include "help.h"
#include "keys.h"
#include <graphics.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <string.h>


extern int LINE_WIDTH,LINE_HEIGHT;

struct HELPITEM HelpItem={NULL};

/*
char inner[29];

void main()
{
Ropen(GM_EGA);
Rmouse_set();

HInitHelp("helpr.stt",300,200,30,35,10,BLACK,CYAN,RED,WHITE,BLUE,BLUE,LIGHTRED);
HShowHelp(1);
HCloseHelp();
Rclose();
}
*/


int HInitHelp(char *filename, int left, int top, int interval, int length,
	      int qstr,
	      int textcolor, int groundcolor, int entrycolor,
	      int highentrycolor, int highentrygroundcolor,
	      int bordcolor, int hotkeycolor)
{
char s[MAX_HELPSTRINGLENGTH],*ss;
int i,itemnum;
long l=0;

HelpItem.left=left;
HelpItem.top=top;
HelpItem.interval=interval;
HelpItem.length=length;
HelpItem.qstr=qstr;
HelpItem.textcolor=textcolor;
HelpItem.groundcolor=groundcolor;
HelpItem.entrycolor=entrycolor;
HelpItem.highentrycolor=highentrycolor;
HelpItem.highentrygroundcolor=highentrygroundcolor;
HelpItem.bordcolor=bordcolor;
HelpItem.hotkeycolor=hotkeycolor;
HelpItem.qentries=0;
HelpItem.nstr=0;
for (i=0;i<MAX_HELPSTRINGS;i++) HelpItem.str[i]=0;
for (i=0;i<MAX_MEMITEMS;i++) HelpItem.mem[i]=-1;

HelpItem.right=left+(length+4)*LINE_WIDTH;
HelpItem.bottom=top+(qstr+2)*interval;


HelpItem.f=fopen(filename,"rt");        /* создаем индекс */
if (HelpItem.f==NULL)
   {
   gerrmes(OPEN_ERROR,"HInitHelp",0,COMMON_ERROR);
   return -1;
   }

HelpItem.index=(signed long *)farmalloc(MAX_HELPITEMS*4);
if (HelpItem.index==NULL)
   {
   gerrmes(NOMEM,"HInitHelp",0,COMMON_ERROR);
   fclose(HelpItem.f);
   HelpItem.f=NULL;
   return -1;
   }

for (i=0;i<MAX_HELPITEMS;i++) HelpItem.index[i]=-1;

while(1==1)
   {
   ss=fgets(s,MAX_HELPSTRINGLENGTH,HelpItem.f);
   if (ss==NULL) break;
   if (*s==ITEM_MARKER)
      {
      itemnum=(s[1]-48)*100 + (s[2]-48)*10 + s[3]-48;
      if (itemnum<0 || itemnum>=MAX_HELPITEMS)
	 gerrmes(H_BADITEMNUM,"HInitHelp",0,SYSTEM_ERROR);
      HelpItem.index[itemnum]=l;
      }
   l+=strlen(s)+1;         /* проверка на длину строки (length) */
   }


return 0;
}

void HCloseHelp()
{
farfree(HelpItem.index);
fclose(HelpItem.f);
HelpItem.f=NULL;
}

void HShowHelp(int n)
{
int i,c,mx,my,cur=0,curpage=0,curmem=0;
char *buf;

if (HelpItem.f==NULL) return;
HLoadHelp(n);

buf=(char *)farmalloc(Rimagesize(HelpItem.left,HelpItem.top,HelpItem.right,
		      HelpItem.bottom) );
if (buf==NULL)
   {
   gerrmes(H_NOMEM,"HShowHelp",0,COMMON_ERROR);
   return;
   }


Rmouse_hide();
Rgetimage(HelpItem.left,HelpItem.top,HelpItem.right,HelpItem.bottom,buf);

mor:
HelpItem.qentries=0;
HShowHelpPage(curpage);
HShowEntry(0,'H');

ret:
Rmouse_show();
c=Revent(&mx,&my);
Rmouse_hide();
switch(c)
   {
   case DOWN: case RIGHT:
      if (HelpItem.qentries<=1) break;
      HShowEntry(cur,'N');
      cur=(cur==HelpItem.qentries-1) ? 0 : cur+1 ;
      HShowEntry(cur,'H');
      break;
   case UP: case LEFT:
      if (HelpItem.qentries<=1) break;
      HShowEntry(cur,'N');
      cur=(cur==0) ? HelpItem.qentries-1 : cur-1 ;
      HShowEntry(cur,'H');
      break;
   case ENTER:
      menter:
      if (HelpItem.qentries<=0) break;
      for (i=0;i<HelpItem.nstr;i++)
	 {
	 farfree(HelpItem.str[i]);
	 HelpItem.str[i]=0;
	 }
      HelpItem.nstr=0;
      HAddMem(HelpItem.HelpEntry[cur].itemnum);
      HLoadHelp(HelpItem.HelpEntry[cur].itemnum);
      curmem=0;
      curpage=0;
      cur=0;
      goto mor;
   case PGDN:
      mpgdn:
      if (HelpItem.qpages==1) break;
      if (curpage==HelpItem.qpages-1) break;
      cur=0;
      curpage++;
      goto mor;
   case PGUP:
      mpgup:
      if (HelpItem.qpages==1) break;
      if (curpage==0) break;
      cur=0;
      curpage--;
      goto mor;
   case -1:
      if (my>HelpItem.bottom-LINE_HEIGHT-6 && my<HelpItem.bottom)
	 {
	 if ( mx>HelpItem.right-13*LINE_WIDTH
	   && mx<HelpItem.right-7*LINE_WIDTH)  goto mpgup;
	 if ( mx>HelpItem.right-7*LINE_WIDTH
	   && mx<HelpItem.right-1*LINE_WIDTH)  goto mpgdn;
	 if ( mx>HelpItem.left+8*LINE_WIDTH
	   && mx<HelpItem.left+15*LINE_WIDTH)  goto malt;
	 if ( mx>HelpItem.left+2*LINE_WIDTH
	   && mx<HelpItem.left+7*LINE_WIDTH)   goto mend;
	 }
   for (i=0;i<HelpItem.qentries;i++)
       if (mx>HelpItem.HelpEntry[i].left && mx<HelpItem.HelpEntry[i].right &&
	   my>HelpItem.HelpEntry[i].top && my<HelpItem.HelpEntry[i].bottom)
	   {  cur=i;  goto menter;  }
      break;
   case ALT_F1:
      malt:
      if (curmem!=MAX_MEMITEMS && HelpItem.mem[curmem+1]!=-1) curmem++;
      for (i=0;i<HelpItem.nstr;i++)
	 {
	 farfree(HelpItem.str[i]);
	 HelpItem.str[i]=0;
	 }
      HelpItem.nstr=0;
      HLoadHelp(HelpItem.mem[curmem]);
      curpage=0;
      cur=0;
      goto mor;
   case ESC:
      goto mend;
   }
goto ret;

mend:
for (i=0;i<HelpItem.nstr;i++)
   {
   farfree(HelpItem.str[i]);
   HelpItem.str[i]=0;
   }
HelpItem.nstr=0;
Rputimage(HelpItem.left,HelpItem.top,buf,0);
farfree(buf);
Rmouse_show();
}


void  HLoadHelp(int n)
{
int i;
char *s;

for (i=0;i<MAX_HELPSTRINGS && HelpItem.str[i]!=0;i++);
if (i==MAX_HELPSTRINGS) gerrmes(H_LONG,"LoadHelp",n,SYSTEM_ERROR);

HAddMem(n);

fseek(HelpItem.f,HelpItem.index[n],SEEK_SET);
while(1)
   {
   HelpItem.str[HelpItem.nstr]=(char *)farmalloc(MAX_HELPSTRINGLENGTH+1);
   if (HelpItem.str[HelpItem.nstr]==NULL)  gerrmes(H_NOMEM,"HLoadHelp",0,SYSTEM_ERROR);

   s=fgets(HelpItem.str[HelpItem.nstr],MAX_HELPSTRINGLENGTH,HelpItem.f);
   if ( s==NULL
     || s[0]==ITEM_MARKER &&  (s[1]-48)*100+(s[2]-48)*10+s[3]-48 != n )
      {
      farfree(HelpItem.str[HelpItem.nstr]);
      HelpItem.str[HelpItem.nstr]=0;
      HelpItem.qpages=(HelpItem.nstr%HelpItem.qstr!=0)
		    ? HelpItem.nstr/HelpItem.qstr+1
		    : HelpItem.nstr/HelpItem.qstr;
      return;
      }
   else
      {
      s=strchr(HelpItem.str[HelpItem.nstr],'\n');
      if (s!=NULL) *s=0;
      HelpItem.nstr++;
      }
   }
}


void HShowHelpPage(int pagenum)
{
int i;

Rbarc(HelpItem.left,HelpItem.top,HelpItem.right,HelpItem.bottom,SOLID_FILL,
      HelpItem.groundcolor);
Rramc(HelpItem.left+8,HelpItem.top+10,HelpItem.right-8,HelpItem.bottom-10,
      HelpItem.bordcolor);
for (i=0;i<HelpItem.qstr;i++)
    HShowHelpString(HelpItem.left+LINE_WIDTH*2,
    HelpItem.top+HelpItem.interval*(i+1),
    HelpItem.str[HelpItem.qstr*pagenum+i]);
if (pagenum!=0)
   {
   Rbarc(HelpItem.right-13*LINE_WIDTH,HelpItem.bottom-LINE_HEIGHT-6,
	 HelpItem.right-7*LINE_WIDTH,HelpItem.bottom,SOLID_FILL,
	 HelpItem.groundcolor);
   Rtextc(HelpItem.right-12*LINE_WIDTH,HelpItem.bottom-LINE_HEIGHT-3,"PgUp",
	 HelpItem.hotkeycolor);
   }
if (pagenum!=HelpItem.qpages-1)
   {
   Rbarc(HelpItem.right-7*LINE_WIDTH,HelpItem.bottom-LINE_HEIGHT-6,
	 HelpItem.right-1*LINE_WIDTH,HelpItem.bottom,SOLID_FILL,
	 HelpItem.groundcolor);
   Rtextc(HelpItem.right-6*LINE_WIDTH,HelpItem.bottom-LINE_HEIGHT-3,"PgDn",
	 HelpItem.hotkeycolor);
   }
Rbarc(HelpItem.left+2*LINE_WIDTH,HelpItem.bottom-LINE_HEIGHT-6,
      HelpItem.left+15*LINE_WIDTH,HelpItem.bottom,SOLID_FILL,
      HelpItem.groundcolor);
Rtextc(HelpItem.left+3*LINE_WIDTH,HelpItem.bottom-LINE_HEIGHT-3,"Esc",
      HelpItem.hotkeycolor);
Rtextc(HelpItem.left+8*LINE_WIDTH,HelpItem.bottom-LINE_HEIGHT-3,"Alt-F1",
      HelpItem.hotkeycolor);
for (i=0;i<HelpItem.qentries;i++) HShowEntry(i,'N');
}


void HShowHelpString(int x, int y, char *str)
{
char stext[MAX_HELPSTRINGLENGTH+1],*s;
int i,j;

if (str==NULL) return;
for (i=0;i<strlen(str);i++)
   {
   if (i==0 && str[i]==ITEM_MARKER)
      {
      for (j=0;j<4;j++)  stext[j]=' ';
      i=3;
      }
   else if (str[i]==ENTRY_BEGIN_MARKER)
      {
      s=strchr(str+i,ENTRY_END_MARKER);
      if (s==NULL) gerrmes(H_NOENTRYEND,"HShowHelpString",0,SYSTEM_ERROR);
      HAddEntry(i+1,s-str-i,x,y,str+i+1);
      for (j=0;j<s-str-i+1;j++)	 stext[i+j]=' ';
      i+=s-str-i;
      }
   else stext[i]=str[i];
   }
stext[strlen(str)]=0;
Rtextc(x,y,stext,HelpItem.textcolor);
}


void HAddEntry(int pos, int l, int x, int y, char *str)
{
int i;

if (HelpItem.qentries==MAX_HELPENTRIES)
    gerrmes(H_MANYENTRIES,"HHAddEntry",0,SYSTEM_ERROR);
HelpItem.HelpEntry[HelpItem.qentries].x=x+(pos+2)*LINE_WIDTH;
HelpItem.HelpEntry[HelpItem.qentries].y=y;
HelpItem.HelpEntry[HelpItem.qentries].l=l-1;

HelpItem.HelpEntry[HelpItem.qentries].left =
	 HelpItem.HelpEntry[HelpItem.qentries].x-1*LINE_WIDTH;
HelpItem.HelpEntry[HelpItem.qentries].top = y-4;
HelpItem.HelpEntry[HelpItem.qentries].right =
	 HelpItem.HelpEntry[HelpItem.qentries].x +
	 (HelpItem.HelpEntry[HelpItem.qentries].l-2)*LINE_WIDTH;
HelpItem.HelpEntry[HelpItem.qentries].bottom = y+LINE_HEIGHT+2;

HelpItem.HelpEntry[HelpItem.qentries].itemnum=
    (str[0]-48)*100 + (str[1]-48)*10 + str[2]-48;
if (HelpItem.HelpEntry[HelpItem.qentries].itemnum<0 ||
    HelpItem.HelpEntry[HelpItem.qentries].itemnum>=MAX_HELPITEMS )
      gerrmes(H_BADITEMNUM,"HAddEntry",0,SYSTEM_ERROR);
for (i=3;i<l-1;i++) HelpItem.HelpEntry[HelpItem.qentries].str[i-3]=str[i];
HelpItem.HelpEntry[HelpItem.qentries].str[i-3]=0;
HelpItem.qentries++;
}

void HShowEntry(int n, char rezhim)    /* H/N high/normal */
{
int col;

if (n<0 || n>HelpItem.qentries-1) return;
col=(rezhim=='H') ? HelpItem.highentrygroundcolor : HelpItem.groundcolor ;
Rbarc(HelpItem.HelpEntry[n].left,HelpItem.HelpEntry[n].top,
       HelpItem.HelpEntry[n].right,HelpItem.HelpEntry[n].bottom,
       SOLID_FILL,col);
col=(rezhim=='H') ? HelpItem.highentrycolor : HelpItem.entrycolor ;
Rtextc(HelpItem.HelpEntry[n].x,HelpItem.HelpEntry[n].y,
	HelpItem.HelpEntry[n].str,col);
}
/*
закрыть HELP
*/


void HAddMem(int n)
{
int i,j;

for (i=0;i<MAX_MEMITEMS;i++)
   if (HelpItem.mem[i]==n)
      {
      for (j=i;j>0;j--) HelpItem.mem[j]=HelpItem.mem[j-1];
      HelpItem.mem[0]=n;
      return;
      }
for (i=MAX_MEMITEMS-1;i>0;i--) HelpItem.mem[i]=HelpItem.mem[i-1];
HelpItem.mem[0]=n;
return;
}