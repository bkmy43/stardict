// #ifdef LOADDATA_EXIST
// #define SELECTFILE_SAVE


#include "graph.h"
#include "keys.h"
#include <stdlib.h>
//#include "help.h"  // если подключен, то функционирует
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <graphics.h>
#include <string.h>
#include <dir.h>
#include <direct.h>
#include <dos.h>
#include "listdir.h"


#define LP_FILE_CHOISE   0
#define LP_DRIVE_CHOISE  1
#define LP_CFG_CHOISE    2

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))


struct LIST_ENTER_ITEM ListEnterItem;

extern int LINE_WIDTH,LINE_HEIGHT;
extern int LANG;
//int LANG=GW_RUS;



int ShowList(char *str[], int qstr, int left, int top, int right, int bottom,
	      int bordtype, int interval, int otstupx,
	      int groundcolor, int textcolor, int highcolor, int bordcolor,
	      int helpitem, int ListParam)
{
int i,c,r,curstr=0,qstr_on_page,first=0,mx,my;

qstr_on_page=(bottom-top)/interval-2+1;

Rmouse_hide();
Rbarc(left,top,right,bottom,1,groundcolor);
if (bordtype==SINGLE_BORD)  Rramc(left+4,top+4,right-4,bottom-4,bordcolor);
if (bordtype==DOUBLE_BORD)  Rramc(left+8,top+8,right-8,bottom-8,bordcolor);

for (i=0;i<qstr;i++)
    PaintString(str,i,qstr_on_page,0,top,otstupx+left,interval,
		groundcolor,textcolor,highcolor,0);

ret:
Rmouse_show();
c=Revent(&mx,&my);
Rmouse_hide();
switch(c)
   {
   case UP:
	if (curstr==0) break;
	if (curstr!=first)
	   {
	    curstr--;
	    PaintString(str,curstr+1,qstr_on_page,first,top,otstupx+left,interval,
		groundcolor,textcolor,highcolor,curstr);
	    PaintString(str,curstr,qstr_on_page,first,top,otstupx+left,interval,
		groundcolor,textcolor,highcolor,curstr);
	   }
	else
	   {
	   Rbarc(left+10,top+10,right-10,bottom-10,1,groundcolor);
	   first--;
	   curstr--;
	   for (i=0;i<qstr;i++)
	       PaintString(str,i,qstr_on_page,first,top,otstupx+left,interval,
		  groundcolor,textcolor,highcolor,curstr);
	   }
	break;
   case DOWN:
	if (curstr==qstr-1) break;
	if (curstr!=first+qstr_on_page-1)
	   {
	    curstr++;
	    PaintString(str,curstr-1,qstr_on_page,first,top,otstupx+left,interval,
		groundcolor,textcolor,highcolor,curstr);
	    PaintString(str,curstr,qstr_on_page,first,top,otstupx+left,interval,
		groundcolor,textcolor,highcolor,curstr);
	   }
	else
	   {
	   Rbarc(left+10,top+10,right-10,bottom-10,1,groundcolor);
	   first++;
	   curstr++;
	   for (i=0;i<qstr;i++)
	       PaintString(str,i,qstr_on_page,first,top,otstupx+left,interval,
		  groundcolor,textcolor,highcolor,curstr);
	   }
	break;
   case PGUP:
	if (first==0) break;
	first=max(first-qstr_on_page,0);
	curstr=first;
	Rbarc(left+10,top+10,right-10,bottom-10,1,groundcolor);
	   for (i=0;i<qstr;i++)
	       PaintString(str,i,qstr_on_page,first,top,otstupx+left,interval,
		  groundcolor,textcolor,highcolor,curstr);
	break;
   case PGDN:
	if (first>=qstr-qstr_on_page) break;
	first=min(qstr-qstr_on_page,first+qstr_on_page);
	curstr=min(first+qstr_on_page-1,qstr-1);
	Rbarc(left+10,top+10,right-10,bottom-10,1,groundcolor);
	   for (i=0;i<qstr;i++)
	       PaintString(str,i,qstr_on_page,first,top,otstupx+left,interval,
		  groundcolor,textcolor,highcolor,curstr);
	break;
   case HOME:
	first=0;
	curstr=0;
	Rbarc(left+10,top+10,right-10,bottom-10,1,groundcolor);
	   for (i=0;i<qstr;i++)
	       PaintString(str,i,qstr_on_page,first,top,otstupx+left,interval,
		  groundcolor,textcolor,highcolor,curstr);
	break;
   case END:
	first=max(qstr-qstr_on_page,0);
	curstr=qstr-1;
	Rbarc(left+10,top+10,right-10,bottom-10,1,groundcolor);
	   for (i=0;i<qstr;i++)
	       PaintString(str,i,qstr_on_page,first,top,otstupx+left,interval,
		  groundcolor,textcolor,highcolor,curstr);
	break;
   case ESC:  return(-1);
   case TAB:  if (ListParam==LP_FILE_CHOISE) return(-2); else break;
   case ENTER:
	menter:
	ListEnterItem.curstr=curstr;
	return(curstr);
   case -1:
	InitListEnterItem(left,top,right,bottom,interval,otstupx,qstr,first);
	r=SetListEnterItem(mx,my);
	if (r==0) break;
	if (curstr==ListEnterItem.curstr) goto menter;
	PaintString(str,curstr,qstr_on_page,first,top,otstupx+left,interval,
	    groundcolor,textcolor,highcolor,ListEnterItem.curstr);
	curstr=ListEnterItem.curstr;
	PaintString(str,curstr,qstr_on_page,first,top,otstupx+left,interval,
	    groundcolor,textcolor,highcolor,ListEnterItem.curstr);
	break;
   case F1:
   #ifdef HELP_ON
	if (helpitem!=-1) HShowHelp(helpitem);
   #endif
	break;
   default:  break;
   }
goto ret;
}


void PaintString(char *str[], int n, int qstr_on_page, int first,
		 int top, int left, int interval,
		 int groundcolor, int textcolor, int highcolor,
		 int cur)  /* если cur==-1, то фон */
{
RPaintString(str[n],n,qstr_on_page,first,top,left,interval,groundcolor,
	     textcolor,highcolor,cur);
}


void InitListEnterItem(int left, int top, int  right, int bottom,
     int interval, int otstupx, int qstr, int first)
{
ListEnterItem.left=left;
ListEnterItem.top=top;
ListEnterItem.right=right;
ListEnterItem.bottom=bottom;
ListEnterItem.interval=interval;
ListEnterItem.otstupx=otstupx;
ListEnterItem.qstr=qstr;
ListEnterItem.first=first;
}


int SetListEnterItem(int mx, int my)
{
int y;

if (mx<ListEnterItem.otstupx+ListEnterItem.left ||
    mx>ListEnterItem.right ||
    my<ListEnterItem.top+ListEnterItem.interval/2 ||
    my>ListEnterItem.bottom-ListEnterItem.interval/2)
   return 0;
y=my-ListEnterItem.top-ListEnterItem.interval;
ListEnterItem.curstr=y/ListEnterItem.interval+ListEnterItem.first;
if (ListEnterItem.curstr>ListEnterItem.qstr-1) return 0;
else return 1;
}


int ListDir(char *filemask, char path[MAXPATH], char *rezfile,
	    int x, int y)
    // path[0] = 0, если нужна текущая дирректория
    // return: 0 - OK; 1 - не хватало памяти
{
int r,left,top,wid=400,hei=300,ret=0;
char *buf;
unsigned long m;

left=min(x,1023-wid-20);
top=min(y,768-hei-20);
m=Rimagesize(left,top,left+wid+20,top+hei+20);
buf=(char *)farmalloc(m);
if (buf==NULL) { Rmessage(ldirmes(9),YELLOW,BLUE,LIGHTGRAY); ret=1; }
else Rgetimage(left,top,left+wid+20,top+hei+20,buf);

ClearListDirScreen(left,top,wid,hei);
if (path[0]==0) getcwd(path,MAXPATH);
RInitFkey(left+5+LINE_WIDTH*23,top+70,10," UP   ",UP,LIGHTRED,DARKGRAY,NOCOLOR);
RInitFkey(left+5+LINE_WIDTH*23,top+100,10," PGUP ",PGUP,LIGHTRED,DARKGRAY,NOCOLOR);
RInitFkey(left+5+LINE_WIDTH*23,top+130,10," DOWN ",DOWN,LIGHTRED,DARKGRAY,NOCOLOR);
RInitFkey(left+5+LINE_WIDTH*23,top+160,10," PGDN ",PGDN,LIGHTRED,DARKGRAY,NOCOLOR);
RInitFkey(left+5+LINE_WIDTH*23,top+190,10," ENTER",ENTER,LIGHTRED,DARKGRAY,NOCOLOR);
RInitFkey(left+5+LINE_WIDTH*23,top+220,10," TAB  ",TAB,LIGHTRED,DARKGRAY,NOCOLOR);
RInitFkey(left+5+LINE_WIDTH*23,top+250,10," ESC  ",ESC,LIGHTRED,DARKGRAY,NOCOLOR);
Rmouse_hide();
Rtextc(left+LINE_WIDTH*16,top+177,ldirmes(0),DARKGRAY);
Rtextc(left+LINE_WIDTH*16,top+193,ldirmes(1),DARKGRAY);
Rtextc(left+LINE_WIDTH*16,top+210,ldirmes(2),DARKGRAY);
Rtextc(left+LINE_WIDTH*16,top+226,ldirmes(3),DARKGRAY);
Rtextc(left+LINE_WIDTH*16,top+ 85,ldirmes(4),DARKGRAY);
Rtextc(left+LINE_WIDTH*16,top+145,ldirmes(5),DARKGRAY);
Rtextc(left+LINE_WIDTH*16,top+250,ldirmes(6),DARKGRAY);

ret:
r=SelectFile(filemask,path,rezfile,left+20,top+60,8);
if (r==-2 || r==-3)
  {
  r=SelectDrive(left+20,top+60,8);
  if (r!=ESC)
     {
     _getdcwd(r-'A'+1,path,MAXPATH);
     goto ret;
     }
  }

RRemoveFkey(UP);
RRemoveFkey(DOWN);
RRemoveFkey(PGUP);
RRemoveFkey(PGDN);
RRemoveFkey(ENTER);
RRemoveFkey(TAB);
RRemoveFkey(ESC);

if (ret==0)
   {
   Rputimage(left,top,buf,0);
   farfree(buf);
   }
else
   RbarcM(left,top,left+wid+20,top+hei+20,SOLID_FILL,getbkcolor());
return(ret);
}


char *ReducePath(char *path, int maxlen)
{
int i,j;

if (strlen(path)<maxlen)  return(path);

for (i=strlen(path);path[i]!='\\';i--);
for (j=i-1;j+4+strlen(path)-i>maxlen || path[j]!='\\';j--);
path[j+1]='.';
path[j+2]='.';
path[j+3]='.';
strcpy(path+j+4,path+i);
return(path);
}


void ShowPath(char *path, int maxlen, int left, int top)
{
char showpath[MAXPATH];
strcpy(showpath,path);
ReducePath(showpath,maxlen);
Rbarc(left,top-LINE_HEIGHT/2,left+maxlen*LINE_WIDTH,
      top+(int)(LINE_HEIGHT*1.5),SOLID_FILL,CYAN);
Rtextc(left,top,showpath,MAGENTA);
}


int SelectFile(char *filemask, char path[MAXPATH], char *rezfile,
	       int left, int top, int maxstr)
// return:  -1 - ESC
//          -2 - TAB
//          -3 - no files or derectories
{
int i,q,r,qdir,right,bottom;
char *str,*strings[D_MAXFILES];
#ifdef SELECTFILE_SAVE
 char *buf;
#endif

str=(char *)farmalloc(13*D_MAXFILES);
if (str==NULL) gerrmes(NOMEM,"SelectFile",1,SYSTEM_ERROR);

for (i=0;i<D_MAXFILES;i++) strings[i]=str+i*13;

ret:
ShowPath(path,28,left,top-LINE_HEIGHT*2);
q=Dget(str,path,filemask,&qdir);
if (q==-1)
   {
   Rmessage(ldirmes(7),WHITE,RED,LIGHTGRAY);
   q=D_MAXFILES;
   }
else if (q==0)
   {
   Rmessage(ldirmes(8),WHITE,RED,LIGHTGRAY);
   r=-3;
   goto mend;
   }

right=left+LINE_WIDTH*14;
bottom=top+(int)(LINE_HEIGHT*1.5) * (min(maxstr,q)+1);

#ifdef SELECTFILE_SAVE
buf=(char *)farmalloc(Rimagesize(left,top,right,bottom));
if (buf==NULL) gerrmes(NOMEM,"SelectFile",1,SYSTEM_ERROR);
Rgetimage(left,top,right,bottom,buf);
#endif

r=ShowList(strings,q,left,top,right,bottom,SINGLE_BORD,
	   (int)(LINE_HEIGHT*1.5),(int)(LINE_WIDTH*1.5),
	   BLUE,YELLOW,LIGHTRED,LIGHTGRAY,44,LP_FILE_CHOISE);

Rbarc(left,top,right,bottom,SOLID_FILL,CYAN);

#ifdef SELECTFILE_SAVE
Rputimage(left,top,buf,0);
farfree(buf);
#endif

if (r==-1 || r==-2) goto mend;
if (r<qdir)
   {
   if (strcmp(strings[r],"..")==0)
      {
      for (i=strlen(path);path[i]!='\\';i--);
      path[i]=0;
      goto ret;
      }
   if (path[strlen(path)-1]!='\\') strcat(path,"\\");
   strcat(path,strings[r]);
   goto ret;
   }
else
   {
   strcpy(rezfile,path);
   if (path[strlen(path)-1]!='\\') strcat(rezfile,"\\");
   strcat(rezfile,strings[r]);
   }

mend:
farfree(str);
return r;
}


int SelectDrive(int left, int top, int maxstr)
    // возврат: буквенное обозначение драйва
{
int i,j=0,k,r,curdrive,maxdrives,right,bottom,drive;
char *drname[40];

curdrive=getdisk();
maxdrives=setdisk(curdrive);

for (i=0;i<maxdrives;i++)
   if (_chdrive(i+1)!=-1)
      {
      drname[j]=(char *)farmalloc(12);
      if (drname[j]==NULL) gerrmes(NOMEM,"SelectDrive",1,SYSTEM_ERROR);
      for (k=0;k<11;k++) *(drname[j]+k)=' ';
      *(drname[j]+5)=i+'A';
      *(drname[j]+6)=':';
      *(drname[j]+11)=0;
      j++;
      }
setdisk(curdrive);

right=left+LINE_WIDTH*14;
bottom=top+(int)(LINE_HEIGHT*1.5) * (min(maxstr,j)+1);


r=ShowList(drname,j,left,top,right,bottom,SINGLE_BORD,
	   (int)(LINE_HEIGHT*1.5),(int)(LINE_WIDTH*1.5),
	   BLUE,YELLOW,LIGHTRED,LIGHTGRAY,43,LP_DRIVE_CHOISE);

RbarcM(left,top,right,bottom,SOLID_FILL,CYAN);

if (r!=-1) drive=*(drname[r]+5); else drive=-1;

for (i=0;i<j;i++) farfree(drname[i]);
return(drive);
}

int Dget(char *str, char *path, char *filemask, int *qdir)
{
int q;
q=Dgetfiles(str,path,"*.*",'D');
if (q==-1) return -1;
*qdir=q;
q=Dgetfiles(str+q*13,path,filemask,'F');
if (q==-1) return -1; else return q+(*qdir);
}

int Dgetfiles(char *str, char *path, char *mask, char type)
				/* F - файлы;  D - директории */
{
int f,q=0;
char p[MAXPATH];
struct ffblk ff;

if (path[0]!=0)
   {
   strcpy(p,path);
   if (path[strlen(path)-1]!='\\') strcat(p,"\\");
   }
else p[0]=0;
strcat(p,mask);
if (type=='F') f=findfirst(p,&ff,FA_ARCH);
else f=findfirst(p,&ff,FA_DIREC);
if (f!=0) return q;
if (ff.ff_attrib==16 && type=='F' ||   ff.ff_attrib==32 && type=='D' ||
    strcmp(ff.ff_name,".")==0     );
   else strcpy(str+13*q++,ff.ff_name);

while(1)
   {
   f=findnext(&ff);
   if (f!=0) return q;
   if (ff.ff_attrib==16 && type=='F') continue;
   if (ff.ff_attrib==32 && type=='D') continue;
   if (strcmp(ff.ff_name,".")!=0) strcpy(str+13*q++,ff.ff_name);
   if (q>=D_MAXFILES) return(-1);
   }
}


void ClearListDirScreen(int x, int y, int wid, int hei)
{
Rmouse_hide();
Rbarc(x+20,y+20,x+wid+20,y+hei+20,SOLID_FILL,BLACK);
Rbarc(x,y,x+wid,y+hei,SOLID_FILL,CYAN);
Rramc(x+5,y+6,x+wid-5,y+hei-6,BLUE);
Rramc(x+10,y+12,x+wid-10,y+hei-12,BLUE);
Rmouse_show();
}


char *ldirmes(int n)
{
if (LANG==GW_ENG)
 switch(n)
    {
    case 0: return "Choose";
    case 1: return "  file";
    case 2: return "Change";
    case 3: return " drive";
    case 4: return "   Up";
    case 5: return " Down";
    case 6: return " Exit";
    case 7: return " Too many files in the directory";
    case 8: return " No files or directories on the drive";
    case 9: return " Not enough memory !";
    default: return "";
    }
else
 switch(n)
    {
    case 0: return "Выбрать";
    case 1: return "   файл";
    case 2: return "Сменить";
    case 3: return "   диск";
    case 4: return "  Вверх";
    case 5: return "   Вниз";
    case 6: return "  Отказ";
    case 7: return "  Слишком много файлов в директории";
    case 8: return "  Файлов и директорий не найдено";
    case 9: return "  Маловато памяти !";
    default: return "";
    }
}


#ifdef LOADDATA_EXIST
/********************** CFG ******************************/
#include "data.h"

int SelectCfgFile(char *cfgname)
{
int i,q,r,qdir,left,top,right,bottom;
char path[MAXPATH],*str,*strings[D_MAXFILES],*buf;

str=(char *)farmalloc(13*D_MAXFILES);
if (str==NULL) gerrmes(NOMEM,"SelectCfgFile",1,SYSTEM_ERROR);

for (i=0;i<D_MAXFILES;i++) strings[i]=str+i*13;

getcwd(path,MAXPATH);

ret:
q=Dget(str,path,"*.cfg",&qdir);
if (q==-1)
   {
   Rmessage(listmes(0),WHITE,RED,LIGHTGRAY);
   r=-1;
   goto mend;
   }
else if (q==0)
   {
   Rmessage(listmes(1),WHITE,RED,LIGHTGRAY);
   r=-1;
   goto mend;
   }

left=400;
top=200;
right=400+LINE_WIDTH*15;
bottom=200+(int)(LINE_HEIGHT*1.5) * (min(15,q)+1);

buf=(char *)farmalloc(Rimagesize(left,top,right,bottom));
if (buf==NULL) gerrmes(NOMEM,"SelectCfgFile",1,SYSTEM_ERROR);
Rgetimage(left,top,right,bottom,buf);

r=ShowList(strings,q,left,top,right,bottom,SINGLE_BORD,
	   (int)(LINE_HEIGHT*1.5),(int)(LINE_WIDTH*1.5),
	   BLUE,YELLOW,LIGHTRED,LIGHTGRAY,41,LP_CFG_CHOISE);
Rputimage(left,top,buf,0);
farfree(buf);

if (r==-1) { r=-1; goto mend; }
if (r<qdir)
   {
   if (strcmp(strings[r],"..")==0)
      {
      for (i=strlen(path);path[i]!='\\';i--);
      path[i]=0;
      goto ret;
      }
   strcat(path,"\\");
   strcat(path,strings[r]);
   goto ret;
   }
else
   {
   strcpy(cfgname,path);
   strcat(cfgname,"\\");
   strcat(cfgname,strings[r]);
   }

mend:
farfree(str);
return r;
}

int ShowCfgList(char *cfgname, int left, int top, int interval,
		int otstupx, int groundcolor, int textcolor, int bordcolor,
		int highgroundcolor, int hightextcolor)
{
int i,c,f,right,bottom,cur=0,mx,my;
char *buf,*str[7];

f=_open(cfgname,O_RDONLY | O_BINARY);
if (f==-1) gerrmes(OPEN_ERROR,"ShowCfgList",0,SYSTEM_ERROR);

right=left+otstupx*2+(FNAM+FTYP+1)*LINE_WIDTH;
bottom=top+interval*8;

buf=(char *)farmalloc(Rimagesize(left,top,right,bottom));
if (buf==NULL) gerrmes(NOMEM,"ShowCfgList",0,SYSTEM_ERROR);
for (i=0;i<7;i++)
   {
   str[i]=(char *)farmalloc(FNAM+FTYP+2);
   if (str[i]==NULL) gerrmes(NOMEM,"ShowCfgList",i+1,SYSTEM_ERROR);
   lseek(f,800+i*600,SEEK_SET);
   read(f,str[i],FNAM);
   *(str[i]+FNAM)=' ';
   read(f,str[i]+FNAM+1,FTYP);
   *(str[i]+FNAM+FTYP+1)=0;
   }

Rmouse_hide();
Rgetimage(left,top,right,bottom,buf);
Rbarc(left,top,right,bottom,SOLID_FILL,groundcolor);
Rramc(left+6,top+6,right-6,bottom-6,bordcolor);

for (i=1;i<7;i++) ShowCfgString(str[i],i,left,top,interval,otstupx,
				groundcolor,textcolor);
ShowCfgString(str[0],0,left,top,interval,otstupx,highgroundcolor,hightextcolor);

ret:
Rmouse_show();
c=Revent(&mx,&my);
Rmouse_hide();
switch(c)
   {
   case UP:
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      groundcolor,textcolor);
	cur=(cur==0) ? 6 : cur-1 ;
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      highgroundcolor,hightextcolor);
	break;
   case DOWN:
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      groundcolor,textcolor);
	cur=(cur==6) ? 0 : cur+1 ;
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      highgroundcolor,hightextcolor);
	break;
   case HOME:
	if (cur==0) break;
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      groundcolor,textcolor);
	cur=0;
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      highgroundcolor,hightextcolor);
	break;
   case END:
	if (cur==6) break;
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      groundcolor,textcolor);
	cur=6;
	ShowCfgString(str[cur],cur,left,top,interval,otstupx,
		      highgroundcolor,hightextcolor);
	break;
   case ENTER:
	     #ifndef DEMO
	       goto mend;
	     #else
		Rmessage(listmes(2),WHITE,RED,LIGHTGRAY);
		break;
	     #endif
   case ESC:   cur=-1; goto mend;
   case F1:
#ifdef HELP_ON
	 HShowHelp(42);
#endif
	 break;
   case -1:
	if (mx<left+otstupx || mx>right-otstupx) break;
	for (i=0;i<7;i++)
	  if (my>top+interval/2+interval*i-6
	   && my<top+interval/2+interval*i+LINE_HEIGHT+4)
	     {
	     if (cur==i)
	     #ifndef DEMO
		goto mend;
	     #else
		Rmessage(stmes(28),WHITE,RED,LIGHTGRAY);
	     #endif
	     ShowCfgString(str[cur],cur,left,top,interval,otstupx,
			   groundcolor,textcolor);
	     cur=i;
	     ShowCfgString(str[cur],cur,left,top,interval,otstupx,
			   highgroundcolor,hightextcolor);
	     break;
	     }
	break;
   }
goto ret;

mend:
Rputimage(left,top,buf,0);
farfree(buf);
for (i=0;i<7;i++) farfree(str[i]);
return(cur);
}


void ShowCfgString(char *str, int cur, int left, int top, int interval,
		   int otstupx, int groundcolor, int textcolor)
{
Rbarc(left+otstupx,top+interval/2+interval*cur-4,
      left+otstupx+(FNAM+FTYP+1)*LINE_WIDTH,
      top+interval/2+interval*cur+LINE_HEIGHT+4,SOLID_FILL,groundcolor);
Rtextc(left+otstupx,top+interval/2+interval*cur,str,textcolor);
}


char *listmes(int n)
{
if (LANG==GW_RUS)
switch(n)
   {
   case  0: return "Слишком много файлов и директорий";
   case  1: return " Файлов и директорий на диске не обнаружено. ";
   case  2: return " ЭТО ДЕМОНСТРАЦИОННАЯ ВЕРСИЯ. ФУНКЦИЯ НЕДОСТУПНА. ";
   default: return "";
   }
else
switch(n)
   {
   case  0: return "Too many files & directories";
   case  1: return " No files or directories were found on drive. ";
   case  2: return " THIS IS A DEMO VERSION. THE FUNCTION IS NOT AVAILABLE. ";
   default: return "";
   }
}
#endif
