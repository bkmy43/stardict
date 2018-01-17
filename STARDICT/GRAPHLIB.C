#include "graph.h"
#include "ftype.h"
#include "keys.h"
#include <graphics.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>

#include <io.h>
#include <fcntl.h>
#include <dir.h>
#include <bios.h>

//#define CIRCLE  // для набоpа функций pисования кpугов и т.п.


union REGS inregs;
int GRX, GRY;  // количество пикселей по вертикали и по горизонтали для
	       // данного адептера
int COL; // COL_MONO для монохромного адаптера; COL_MULTI для остальных
double KOEF; // коэфициент искривления для EGA  /* NEW */
int GRINIT=GROFF;  // инициализирована ли графика
int GRMOUSE=GROFF; // включена ли мышь
int MesSwitch=0;   // переменная, использумая в функциях Rimagesize и
           // Rmessage, определяющая способ выдачи сообщений об ошибке
int LINE_HEIGHT,   // высота и ширина стандартного сивола,
    LINE_WIDTH;    // выраженная в условных единицах
int RVISUAL_PAGE=0; // текущая visual page
int RACTIVE_PAGE=0; // текущая active page


int FUON=1,FUerr;
struct FUFONT *FUCurrentFont=0;
struct FUREF *FUCurrentRef=0;
// struct FUHANDLER FUhandler={0,0,0,0,0};
struct GWAIT GWait={0};
struct Rfunckey *Rfkeys[MAX_FKEYS];
char BGIPath[MAXPATH]="";

extern int MouseIs; /****$$$$****/


/********************** OPEN - CLOSE functions **************************/
void Ropen(int driver)
{
int far *gd,*gm,res=-1;
int gr_dr,gr_mode,i;

gd=&gr_dr;
gm=&gr_mode;

if (driver==GM_DETECT)  detectgraph(gd,gm);
else *gd=driver;

switch(*gd)
   {
   case 1:
	  GRX=640;
	  GRY=200;
	  COL=COL_MULTI;
	  KOEF=1.0;     /* NEW */
	  LINE_HEIGHT=30;
	  LINE_WIDTH=13;
	  gr_mode=CGAHI;
	  break;
   case 3:
	  GRX=640;
	  GRY=350;
	  COL=COL_MULTI;
	  KOEF=0.94;   /* NEW */
	  LINE_HEIGHT=17;
	  LINE_WIDTH=13;
	  gr_mode=EGAHI;
	  break;
   case 7:
	  GRX=720;
	  GRY=350;
	  COL=COL_MONO;
	  KOEF=-1;   /* NEW */
	  LINE_HEIGHT=17;
	  LINE_WIDTH=13;
	  gr_mode=HERCMONOHI;
	  break;
   case 8:
	  GRX=640;
	  GRY=400;
	  COL=COL_MULTI;
	  KOEF=-1; // !!! не установлено   /* NEW */
	  LINE_HEIGHT=15;
	  LINE_WIDTH=13;
	  gr_mode=ATT400HI;
	  break;
   case 9:
	  GRX=640;
	  GRY=480;
	  COL=COL_MULTI;
	  KOEF=1.0; /* NEW */
	  LINE_HEIGHT=13;
	  LINE_WIDTH=13;
	  gr_mode=VGAHI;
	  break;
   default:
	  printf("\nCan't support this graphic griver !");
	  goto mend;
   }

initgraph ( (int far *) &gr_dr, (int far *) &gr_mode,BGIPath);
res=graphresult();
mend:
if (res!=0)
  {
  printf("\nERROR ! Can't initialize graphics (code = %d).",res);
  GRINIT=GROFF;
  }
else GRINIT=GRON;

for (i=0;i<MAX_FKEYS;i++) Rfkeys[i]=0;
RVISUAL_PAGE=0;
RACTIVE_PAGE=0;
}


void Rclose()
{
int i;
closegraph();
for (i=0;i<MAX_FKEYS;i++) if (Rfkeys!=0) farfree(Rfkeys[i]);
}


void Rexit(int cod)
{
if (GRINIT==GRON) Rclose();
exit(cod);
}


/************************** MAIN GRAPHIC functions ************************/
void Rclr(int col)
{
Rbcol(col);
cleardevice();
}

void Rtext(int x, int y, char *s)
{
int rx,ry;
rx=x; ry=y;
Rconvert(&rx,&ry);
if (FUON!=1 || FUCurrentFont==0 || FUCurrentRef==0) outtextxy(rx,ry,s);
else FUtext(rx,ry,s);
}

void Rtextc(int x, int y, char *s, int col)
{
Rcol(col);
Rtext(x,y,s);
}

int Rtext_with_high_let(int x, int y, char *s, int hletnum, int hletcol)
{
int i;
char *ss;

ss=(char *)farmalloc(strlen(s));
if (ss==NULL)
   {
   gerrmes(NOMEM,"R_text_with_high_let",0,COMMON_ERROR);
   return(GM_NOK);
   }
for (i=0;i<strlen(s)+1;i++)  ss[i]=(s[i]==0 || hletnum==i) ? s[i] : ' ' ;
Rtext(x,y,s);
Rtextc(x,y,ss,hletcol);
farfree(ss);
return(GM_OK);
}


void Rtext_with_break(int x, int y, int interval, char *s)
{
int i=0;

if (interval==0) Rtext(x,y,s);
else
   while(s[i]!=0)
      {
      Rsym(x+i*(interval+1)*LINE_WIDTH,y,s[i]);
      i++;
      }
}

void Rsym(int x, int y, char c)
{
char s[2]={" "};
s[0]=c;
Rtext(x,y,s);
}

void Rsymc(int x, int y, char c, int col)
{
Rcol(col);
Rsym(x,y,c);
}

void Rdigit(int x, int y, int digit)
{
char s[10];
itoa(digit,s,10);
Rtext(x,y,s);
}

void Rdigitc(int x, int y, int digit, int col)
{
char s[10];
itoa(digit,s,10);
Rtextc(x,y,s,col);
}

void Rdigit_right(int x, int y, int digit, int qsymbols)
{
char s[10],ss[20]={"                   "};
itoa(digit,s,10);
strcpy(ss+qsymbols-strlen(s),s);
Rtext(x,y,ss);
}

void Rcol(int col)
{
if (COL==COL_MULTI) setcolor(col); else setcolor(1);
}

void Rbcol(int col)
{
if (COL==COL_MULTI) setbkcolor(col); else setbkcolor(0);
}

void Rline(int x1, int y1, int x2, int y2)
{
int rx1,ry1,rx2,ry2;
rx1=x1; ry1=y1;
rx2=x2; ry2=y2;
Rconvert(&rx1,&ry1);
Rconvert(&rx2,&ry2);
line(rx1,ry1,rx2,ry2);
}

void Rlinec(int x1, int y1, int x2, int y2, int col)
{
Rcol(col);
Rline(x1,y1,x2,y2);
}

void RThickLinec(int x1, int y1, int x2, int y2, int col,
		 char style, char thickness)
{
struct linesettingstype l;
getlinesettings(&l);
setlinestyle(style,1,thickness);
Rcol(col);
Rline(x1,y1,x2,y2);
setlinestyle(l.linestyle,1,l.thickness);
}



void Rbarc(int l, int t, int r, int b, int fill, int col)
{
setfillstyle(fill,col);
Rbar(l,t,r,b);
}


void Rbar(int l, int t, int r, int b)
{
int rl,rt,rr,rb;
rl=l; rt=t;
rr=r; rb=b;
Rconvert(&rl,&rt);
Rconvert(&rr,&rb);
bar(rl,rt,rr,rb);
}


void Rram(int l, int t, int r, int b)
{
Rline(l,t,r,t);
Rline(r,t,r,b);
Rline(r,b,l,b);
Rline(l,b,l,t);
}

void Rramc(int l, int t, int r, int b, int col)
{
Rcol(col);
Rram(l,t,r,b);
}

void Rcircle(int x, int y, int r)
{
int rx,ry,rr,rd=0;
rx=x; ry=y; rr=r;
Rconvert(&rx,&ry);
Rconvert(&rr,&rd);
//Rconvert(&rd,&rr);
circle(rx,ry,rr);
}

void Rcirclec(int x, int y, int r, int col)
{
Rcol(col);
Rcircle(x,y,r);
}

#ifdef CIRCLE
void Rfillcircle(int x, int y, int r, int col)    /* NEW */
{
int rx,ry,rr;
int nx,ny;
rx=x; ry=y; rr=r;

Rpoint_on_circle_coordinates(&nx,&ny,rx,ry,rr,270.0);
Rconvert(&rx,&ry);
Rconvert(&rr,&ny);

setfillstyle(EMPTY_FILL,col);
fillellipse(rx,ry,rr,ry-ny);
}

void Rfloodfill(int x, int y, int fill, int col, int bordercol) // NEW
{
int rx,ry;

rx=x; ry=y;
Rconvert(&rx,&ry);
setfillstyle(fill,col);
floodfill(rx,ry,bordercol);
}

void Rpoint_on_circle_coordinates(int *x, int *y, int xx, int yy,
				  int radius, double angle)   /* NEW */
{
*x=(int) (radius*fcos(angle) + xx);
*y=(int) (radius*fsin(angle)/KOEF + yy);
}

void Rradius(int xx, int yy, double angle, int from, int to) /* NEW */
{
int x1,y1,x2,y2;
Rpoint_on_circle_coordinates(&x1,&y1,xx,yy,from,angle);
Rpoint_on_circle_coordinates(&x2,&y2,xx,yy,to,angle);
Rline(x1,y1,x2,y2);
}


void RMovingCircle(int x, int y, int srcRad, int destRad, int fcol, int col)
{
int i,add=1;
if (srcRad>destRad) add=-1;
i=srcRad;
while(i!=destRad)
   {
   Rcol(col);
   Rcircle(x,y,i+add);
   Rcol(fcol);
   Rcircle(x,y,i);
   i+=add;
   }
}
#endif

void Rputpixel(int x, int y, int col)
{
int rx,ry,c;
rx=x; ry=y;
Rconvert(&rx,&ry);
if (COL==COL_MULTI) c=col; else c=1;
putpixel(rx,ry,c);
}

unsigned far Rgetpixel(int x, int y)
{
int rx,ry;
rx=x; ry=y;
Rconvert(&rx,&ry);
return( getpixel(rx,ry) );
}


int RSwitchVisualPage()
{
RVISUAL_PAGE=(RVISUAL_PAGE==0) ? 1 : 0 ;
setvisualpage(RVISUAL_PAGE);
Rmouse_page(RVISUAL_PAGE);
return RVISUAL_PAGE;
}

int RSwitchActivePage()
{
RACTIVE_PAGE=(RACTIVE_PAGE==0) ? 1 : 0 ;
setactivepage(RACTIVE_PAGE);
return RACTIVE_PAGE;
}


/************************** CONVERT functions ***************************/
void Rconvert(int *rx, int *ry)
{
*rx=(int)((long)*rx*(long)GRX/1024l);
*ry=(int)((long)*ry*(long)GRY/768l);
if (*rx<0) *rx=0;
if (*ry<0) *ry=0;
if (*rx>=GRX) *rx=GRX-1;
if (*ry>=GRY) *ry=GRY-1;
}

void Rbackconvert(int *rx, int *ry)
{
*rx=(int)((long)*rx*1024l/(long)GRX);
*ry=(int)((long)*ry*768l/(long)GRY);
if (*rx<0) *rx=0;
if (*ry<0) *ry=0;
if (*rx>=1024) *rx=1024;
if (*ry>=768)  *ry=768;
}


/************************ LINE functions ****************************/
void RPutLineClear(int x, int y, char *s, int SymWidth, int col)
{
int f,l;

l=strlen(s);
f=getbkcolor();
Rbarc(x,y,x+l*SymWidth,y+8,SOLID_FILL,f);
RPutLine(x,y,s,SymWidth,col);
}

void RPutLine(int x, int y, char *s, int SymWidth, int col)
{
int i,l;
l=strlen(s);
Rcol(col);
for (i=0;i<l;i++)  Rsym(x+i*SymWidth,y-4,s[i]);
}

void RPutLineWithHighSym(int x, int y, char *s, int SymWidth, int col,
			 int SymNum, int highcol)
{
RPutLine(x,y,s,SymWidth,col);
if (SymNum<strlen(s))
   {
   Rcol(highcol);
   Rsym(x+SymNum*SymWidth,y-4,s[SymNum]);
   }
}


void RPutLineWithCursor(int x, int y, char *s, int SymWidth, int col,
			 int SymNum, int highcol, int bcol)
{
int i;

for (i=0;i<strlen(s);i++)  Rsymc(x+i*SymWidth,y,'_',bcol);
RPutLine(x,y,s,SymWidth,col);
if (SymNum<strlen(s))
   {
   Rcol(highcol);
   Rsym(x+SymNum*SymWidth,y,'_');
   }
}



void RPaintString(char *str, int n, int qstr_on_page, int first,
		  int top, int left, int interval,
		  int groundcolor, int textcolor, int highcolor,
		  int cur)  /* если cur==-1, то фон */
{
int y,col;

if (n<first || n>=first+qstr_on_page) return;
y=top+interval*(n-first+1);
if (cur==-1) col=groundcolor;
   else col=(n==cur) ? highcolor : textcolor ;
//Rmouse_hide();
Rtextc(left,y,str,col);
//Rmouse_show();

}

/**************************** IMAGE functions *************************/
void Rgetimage(int l, int t, int r, int b, char *buf)
{
int rl,rt,rr,rb,m=0;

rl=l; rt=t; rr=r; rb=b;
Rconvert(&rl,&rt);
Rconvert(&rr,&rb);
if (GRMOUSE==GRON) { m=1; Rmouse_hide(); }
getimage(rl,rt,rr,rb,buf);
if (m==1) Rmouse_show();
}

void Rputimage(int l, int t, char *buf, int op)
{
int rl,rt,m=0;

rl=l; rt=t;
Rconvert(&rl,&rt);
if (GRMOUSE==GRON) { m=1; Rmouse_hide(); }
putimage(rl,rt,buf,op);
if (m==1) Rmouse_show();
}

unsigned far Rimagesize(int l, int t, int r, int b)
{
int rl,rt,rr,rb;
unsigned size;

rl=l; rt=t; rr=r; rb=b;
Rconvert(&rl,&rt);
Rconvert(&rr,&rb);
if ((long)(rb-rt+1)*(long)(rr-rl+1)>65536l)
   {
   if (MesSwitch==1) printf("\nImage size overflow !");
   else if (MesSwitch==0) gerrmes(BIGIMAGE,"Rimagesize",0,SYSTEM_ERROR);
   else return 0;
   }
size=imagesize(rl,rt,rr,rb);
return(size);
}


/********************* MESSAGE functions **************************/
int gerrmes(int mesnum, char *proc, int errnum, int errtype)
{
char str[80],*s;
int col,fcol,rcol;

switch(errtype)
   {
   case SYSTEM_ERROR:
		   col=WHITE;
		   fcol=RED;
		   rcol=LIGHTGRAY;
		   break;
   case COMMON_ERROR:
		   col=WHITE;
		   fcol=MAGENTA;
		   rcol=LIGHTCYAN;
		   break;
   case MESSAGE: default:
		   col=YELLOW;
		   fcol=BLUE;
		   rcol=LIGHTGRAY;
		   break;
   }



switch(mesnum)
   {
   case GW_DOUBLEINIT: s=" GWait is already initialized";   break;
   case GW_NOINIT:     s=" GWait is not initialized";       break;

   case NOMEM:       s=" Not enough memory";         break;
   case BIGIMAGE:    s=" Too large image to save";   break;

   case NOSAVEMEM:   s=" Not enough memory to initialize save buffer";  break;
   case LOWBUFFER:   s=" Buffer is too small";       break;
   case OPEN_ERROR:  s=" Can't open file";           break;
   case READ_ERROR:  s=" I/O error (reading)";       break;
   case WRITE_ERROR: s=" I/O error (writing)";       break;
   case CREAT_ERROR: s=" Can't create file";         break;
   case BAD_TYPE:    s=" This item type is available only for vertical menu"; break;
   case KEY_BUFFER:  s=" Key buffer overflow";       break;
   case NOLASTSTRING: s=" Function Add-LAST-StringToItem is needed"; break;
   case MANYMENUS:   s=" Too many menues. Change #define MAX_MANUS"; break;
   case MANYITEMS:   s=" Too many items. Change #define MAX_ITEMS";  break;
   case NOMEMENOUGH: s=" Not enough memory";         break;
   case BADSTRLEN:   s=" String length is not OK";   break;
   case CANTOUTTEXT: s=" Can't draw string";         break;
   case CANTSAVE:    s=" Can't save menu";           break;
   case CANTRESTORE: s=" Can't restore menu";        break;
   case CANTDRAW:    s=" Can't draw menu #";         break;
   case CANTFIND:    s=" Can't find saved file";     break;
   case ALREADYACTIVE: s=" This menu is already active";   break;
   case BADITEMNUM:  s=" Bad item number";           break;
   case BADNAMELEN:  s=" Bad length of the filename";   break;
   case BADITEMTYPE: s=" Incorrect item type";       break;

   case BADMENUNUM:  s=" Incorrect menu number";     break;
   case MANYAMENUS:  s=" Too many active menues. #define MAX_LOG_ITEMS"; break;

   case FU_BADNUM:  s=" Bad number of symbol";      break;
   case FU_NOCLEAR: s=" Clear font before reloading";   break;
   case FU_PRINTER: s=" Printer is not ready";      break;
   case FU_SETFONT: s=" Current font is not set";   break;
   case FU_SETREF:  s=" Current reference is not set";  break;

   case GC_NOGRAPH: s=" Graphics is not initialised "; break;
   case GC_BADKORD: s=" Bad parameters ";           break;
   case GC_NOLINEINIT: s=" Line marker is not initialized "; break;
   case GC_NOLINEMARK: s=" No line marked ";        break;

   case FT_BADFORMAT:  s=" Bad format !";                   break;
   case FT_BADPARAM:   s=" Bad parameters !";               break;

   case MN_OVERFLOW:   s=" Too many items/strings/keys";    break;
   case MN_BADTYPE:    s=" Undefined item type";            break;
   case MN_LONGSTRING: s=" String is too long";             break;

   case H_BADFILE:     s="Bad help file";                   break;
   case H_NOMEM:       s="Not enough memory to load help";  break;
   case H_LONG:        s="Help item is too long";           break;
   case H_NOENTRYEND:  s="Entry end marker not found";      break;
   case H_MANYENTRIES: s="Too many entries on help screen"; break;
   case H_BADITEMNUM:  s="Bad help item number";            break;

   case FKEY_ERROR:   s=" Function keys buffer overflow ";  break;
   case FKEY_NOKEY:   s=" Function key not found ";         break;
   case FKEY_PRESENT: s=" This key is already exists ";     break;
   case FKEY_COLOR:   s=" Set a shadow color, please ";     break;
   case FKEY_SWITCH:  s=" This mode is already set ";       break;

   default:;
   }

if (proc!=0) sprintf(str,"%s (%s) #%i",s,proc,errnum);
else sprintf(str,"%s  #%i",s,errnum);

Rmessage(str,col,fcol,rcol);
if (errtype==SYSTEM_ERROR) Rexit(1);

return 0;
}

int Rmessage(char *str, int col, int fcol, int bordcol)
{
int len,l,t,r,b,retsym,mx,my;
unsigned size;
char *buf;
int vp=0,ap=0;

if (GRINIT==GROFF) { printf("\n%s",str); getch(); return -1; }

if (RVISUAL_PAGE==1) { vp=1; RSwitchVisualPage(); }
if (RACTIVE_PAGE==1) { ap=1; RSwitchActivePage(); }

len=strlen(str);
l=512-len/2*LINE_WIDTH-25;
t=300-30;
r=512+len/2*LINE_WIDTH+25;
b=300+30;
if (r>=1024 || b>=768 || l<0 || t<0)
   {
   printf("\nCan't create message. The message string is too long.");
   printf("\nMESSAGE: %s",str);
   getch();
   goto ret;
   }
MesSwitch=1;
size=Rimagesize(l,t,r,b);
buf=(char *)farmalloc(size);
if (buf==NULL)
   {
   printf("\nCan't create message. Not enough memory.");
   printf("\nMESSAGE: %s",str);
   getch();
   goto ret;
   }

Rmouse_hide();
Rgetimage(l,t,r,b,buf);
Rbarc(l,t,r,b,SOLID_FILL,fcol);
Rramc(l+4,t+4,r-4,b-4,bordcol);
Rtextc(l+15,t+20,str,col);
Rmouse_show();
retsym=Revent(&mx,&my);
Rmouse_hide();
Rputimage(l,t,buf,0);
farfree(buf);

ret:
MesSwitch=0;
if (vp==1) RSwitchVisualPage();
if (ap==1) RSwitchActivePage();
Rmouse_show();
return(retsym);
}



void termes(char *str, char *func)
{
char s[80];
sprintf(s,"%s (in %s)",str,func);
Rmessage(s,BLACK,CYAN,BLUE);
Rexit(1);
}


/*------------------------------------------------------------------------*/
/************************** RMOUSE & MONITOR ******************************/
int getsym_filtered(int filter)
{
int c;

ret:
c=getsym();
if (c<32 || c>1000) return c;
switch(filter)
   {
   case DIGIT_INT:  if (c<'0' || c>'9') goto ret;               else  return(c);
   case DIGIT_REAL: if ( (c<'0' || c>'9') && c!='.') goto ret;  else  return(c);
   case TEXT:       if (c>241) goto ret;                      else  return(c);
   case FILENAME_PART:   case FILENAME_FULL:
	    if (c>='A' && c<='Z'  ||  c>='0' && c<='9') return(c);
	    if (c=='~' || c=='!' || c=='@' || c=='#' || c=='$' ||
		c=='%' || c=='^' || c=='&' || c=='_' || c=='-')  return(c);
	    if (c>='a' && c<='z')            return(toupper(c));
	    if (filter==FILENAME_FULL && c=='.')       return(c);
	    goto ret;
   case DATE: if (c>='0' && c<='9' || c=='-')  return(c);
	      if (c==' ') return('0');
	      if (c=='.' || c==':' || c=='/' || c==',') return ('.');
	      goto ret;
   }
goto ret;
}

int getsym()
{
int c;
c=getch();
if (c==0)
  {
  c=getch();
  return(c+1000);
  }
return (c);
}


void RbarcM(int l, int t, int r, int b, int fill, int col)
{
Rmouse_hide();
Rbarc(l,t,r,b,fill,col);
Rmouse_show();
}

void RsymcM(int x, int y, char c, int col)
{
Rmouse_hide();
Rsymc(x,y,c,col);
Rmouse_show();
}

void RtextcM(int x, int y, char *s, int col)
{
Rmouse_hide();
Rtextc(x,y,s,col);
Rmouse_show();
}

void RramcM(int l, int t, int r, int b, int col)
{
Rmouse_hide();
Rramc(l,t,r,b,col);
Rmouse_show();
}


int Rmouse_set()
{
int a,c,d;

if (MouseIs==0) { GRMOUSE=GROFF; return GROFF; }  /*******$$$$*****/

a=inregs.x.ax;
c=inregs.x.cx;
d=inregs.x.dx;

inregs.x.ax=0x21;
int86(0x33,&inregs,&inregs);
if (inregs.x.ax==0xFFFF)
   {
   inregs.x.ax=0;
   int86(0x33,&inregs,&inregs);
   if (inregs.x.ax!=0xFFFF)
      {
      GRMOUSE=GROFF;
      return GROFF;
      }
   }
inregs.x.ax=1;
int86(0x33,&inregs,&inregs);
inregs.x.ax=0x07;
inregs.x.cx=0;
inregs.x.dx=GRX;
int86(0x33,&inregs,&inregs);
inregs.x.ax=0x08;
inregs.x.dx=GRY;
int86(0x33,&inregs,&inregs);

inregs.x.ax=a;
inregs.x.cx=c;
inregs.x.dx=d;
GRMOUSE=GRON;

return(GRON);
}


int Rmouse_status(int *x, int *y)
{
int a,b,ret=0;

if (GRMOUSE==GROFF) return -1;
a=inregs.x.ax;
b=inregs.x.bx;
inregs.x.ax=0x05;
inregs.x.bx=0;
int86(0x33,&inregs,&inregs);
if (inregs.x.bx>0)
   {
   *x=inregs.x.cx;
   *y=inregs.x.dx;
   ret=1;
   }
else
   {
   inregs.x.ax=0x05;
   inregs.x.bx=1;
   int86(0x33,&inregs,&inregs);
   if (inregs.x.bx>0)
      {
      *x=inregs.x.cx;
      *y=inregs.x.dx;
      ret=2;
      }
   }
inregs.x.ax=a;
inregs.x.bx=b;
return(ret);
}

void Rmouse_hide(void)
{
int a;
if (GRMOUSE!=GRON) return;
/* if (GRMOUSE==GROFF) return; */
a=inregs.x.ax;
inregs.x.ax=0x02;
int86(0x33,&inregs,&inregs);
inregs.x.ax=a;
GRMOUSE=GRHIDDEN;
}


void Rmouse_show(void)
{
int a;
if (GRMOUSE!=GRHIDDEN) return;
/*  if (GRMOUSE==GROFF) return; */
a=inregs.x.ax;
inregs.x.ax=0x01;
int86(0x33,&inregs,&inregs);
inregs.x.ax=a;
GRMOUSE=GRON;
}

void Rmouse_page(int page)
{
int a,b;
if (GRMOUSE!=GRON) return;
a=inregs.x.ax;
b=inregs.x.bx;
inregs.x.ax=0x1d;
inregs.x.bx=page;
int86(0x33,&inregs,&inregs);
inregs.x.ax=a;
inregs.x.bx=b;
}


int Revent(int *x, int *y)
    /* возвращает клавишу
       или -1/-2 , если нажали мышь (правую/левую) - тогда в x и y - координаты
       выбор мышью функциональных клавиш обрабатывается, как нажатие клавиши
    */
{
int i,c,mstat;

if (GRMOUSE!=GROFF && MouseIs==0) { Rmouse_hide(); GRMOUSE=GROFF; }  /****$$$$****/
if (GRMOUSE==GROFF && MouseIs==1) Rmouse_set();   /****$$$$****/

ret:
while(1==1)
   {
   if (bioskey(1)!=0) break;
   if (bioskey(2)&0x0004) break;
   mstat=Rmouse_status(x,y);
   if (mstat!=1 && mstat!=2) goto ret;
   Rbackconvert(x,y);
   for (i=0;i<MAX_FKEYS;i++)
      if (mstat==1 && Rfkeys[i]!=0 &&
	*x>=Rfkeys[i]->l-Rfkeys[i]->ot && *x<=Rfkeys[i]->r+Rfkeys[i]->ot &&
	*y>=Rfkeys[i]->t-Rfkeys[i]->ot && *y<=Rfkeys[i]->b+Rfkeys[i]->ot )
	   {
	   if (Rfkeys[i]->active==FKEY_ON)    return(Rfkeys[i]->fkey);
	      else Rsound(600,8);
	   }
   return -mstat;
   }
c=getsym();
return(c);
}


/*************************** FKEYS ************************************/
void RInitFkey(int x, int y, int otstup, char *fkeyname, int fkey,
	       int col, int fcol, int shadowcol)
{
int l,t,r,b;

l=x;
r=x+strlen(fkeyname)*LINE_WIDTH;
t=y-6;
b=y+LINE_HEIGHT+1;

RSetFkey(l,t,r,b,otstup,fkey,fkeyname,col,fcol,shadowcol);
Rmouse_hide();
if (shadowcol!=NOCOLOR)  Rbarc(l+6,t+4,r+6,b+4,SOLID_FILL,shadowcol);
Rbarc(l,t,r,b,SOLID_FILL,fcol);
Rtextc(x,y,fkeyname,col);
Rmouse_show();
}


void RSwitchFkey(int fkey, int rezhim)
{
int i,color;

for(i=0;i<MAX_FKEYS;i++)
 if (Rfkeys[i]->fkey==fkey)
    {
    if (Rfkeys[i]->shcol==NOCOLOR) gerrmes(FKEY_COLOR,"RSwitchFkey",0,SYSTEM_ERROR);
    // if (Rfkeys[i]->active==rezhim) gerrmes(FKEY_SWITCH,"RSwitchFkey",0,SYSTEM_ERROR);
    Rfkeys[i]->active=rezhim;
    Rmouse_hide();
    color=(rezhim==FKEY_ON) ? Rfkeys[i]->fcol : Rfkeys[i]->shcol ;
    Rbarc(Rfkeys[i]->l,Rfkeys[i]->t,Rfkeys[i]->r,Rfkeys[i]->b,SOLID_FILL,color);
    Rtextc(Rfkeys[i]->l,Rfkeys[i]->t+6,Rfkeys[i]->fkeyname,Rfkeys[i]->col);
    Rmouse_show();
    return;
    }
gerrmes(FKEY_ERROR,"RSwitchFkey",0,SYSTEM_ERROR);
}


void RSwitchAllFkeys(int rezhim)
{
int i,color;

for (i=0;i<MAX_FKEYS;i++)
   if (Rfkeys[i]!=0)
      {
      if (Rfkeys[i]->shcol==NOCOLOR) gerrmes(FKEY_COLOR,"RSwitchFkey",0,SYSTEM_ERROR);
      Rfkeys[i]->active=rezhim;
      Rmouse_hide();
      color=(rezhim==FKEY_ON) ? Rfkeys[i]->fcol : Rfkeys[i]->shcol ;
      Rbarc(Rfkeys[i]->l,Rfkeys[i]->t,Rfkeys[i]->r,Rfkeys[i]->b,SOLID_FILL,color);
      Rtextc(Rfkeys[i]->l,Rfkeys[i]->t+6,Rfkeys[i]->fkeyname,Rfkeys[i]->col);
      Rmouse_show();
      }
}

void RRemoveFkey(int fkey)
{
int i;
for(i=0;i<MAX_FKEYS;i++)
   if (Rfkeys[i]!=0 && Rfkeys[i]->fkey==fkey)
      {
      farfree(Rfkeys[i]->fkeyname);
      Rfkeys[i]->fkeyname=0;
      farfree(Rfkeys[i]);
      Rfkeys[i]=0;
      return;
      }
gerrmes(FKEY_NOKEY,"RRemoveFkey",0,SYSTEM_ERROR);
}


void RSetFkey(int l, int t, int r, int b, int ot, int fkey,
	      char *fkeyname, int col, int fcol, int shcol)
{
int i;
for(i=0;i<MAX_FKEYS;i++)
   if (Rfkeys[i]==0)
      {
      Rfkeys[i]=(struct Rfunckey *)farmalloc(sizeof(struct Rfunckey));
      if (Rfkeys[i]==NULL) gerrmes(NOMEM,"RSetFkey",1,SYSTEM_ERROR);
      Rfkeys[i]->fkeyname=(char *)farmalloc(strlen(fkeyname)+1);
      if (Rfkeys[i]->fkeyname==NULL) gerrmes(NOMEM,"RSetFkey",2,SYSTEM_ERROR);
      Rfkeys[i]->l=l;
      Rfkeys[i]->t=t;
      Rfkeys[i]->r=r;
      Rfkeys[i]->b=b;
      Rfkeys[i]->ot=ot;
      Rfkeys[i]->fkey=fkey;
      strcpy(Rfkeys[i]->fkeyname,fkeyname);
      Rfkeys[i]->col=col;
      Rfkeys[i]->fcol=fcol;
      Rfkeys[i]->shcol=shcol;
      Rfkeys[i]->active=FKEY_ON;
      return;
      }
    else if (Rfkeys[i]->fkey==fkey)
	 gerrmes(FKEY_PRESENT,"RSetFkey",0,SYSTEM_ERROR);
gerrmes(FKEY_ERROR,"RSetFkey",0,SYSTEM_ERROR);
}



/****************************** RENTER ***********************************/
char *REnterString(char *str, int x, int y, int length, int SymWidth,
		  int col, int bcol, int curscol, int rezhim, int IfSave)
{
char *buf,*save;
int i,c,cursym=0,ins=1,r;

save=(char *)farmalloc(strlen(str)+1);
if (save==NULL) { gerrmes(NOMEM,"REnterString",1,COMMON_ERROR); return 0; }
memcpy(save,str,strlen(str)+1);

if (IfSave==YES)
   {
   buf=(char *)farmalloc
		(Rimagesize(x,y,x+length*SymWidth+10,y+LINE_HEIGHT+10));
   if (buf==NULL)
      { gerrmes(NOMEM,"REnterString",2,COMMON_ERROR); farfree(save); return 0; }
   Rgetimage(x,y,x+length*SymWidth+10,y+LINE_HEIGHT+10,buf);
   }

Rbarc(x,y,x+length*SymWidth+10,y+LINE_HEIGHT+10,SOLID_FILL,bcol);
RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
ret:
c=getsym_filtered(rezhim);
switch(c)
   {
   case LEFT:
	    cursym=(cursym==0) ? strlen(str)-1 : cursym-1;
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	    goto ret;
   case RIGHT:
	    cursym=(cursym==strlen(str)-1) ? 0 : cursym+1;
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	    goto ret;
   case INS:  ins=(ins==0) ? 1 : 0 ;   goto ret;
   case HOME:
	    cursym=0;
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	    goto ret;
   case END:
	    cursym=strlen(str)-1;
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	    goto ret;
   case ENTER:
	    if (rezhim==DATE)
	    {
	    Rbarc(x,y,x+length*SymWidth+10,y+LINE_HEIGHT+10,SOLID_FILL,bcol);
	    r=IfDate(str);
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	    if (r!=GM_OK) goto ret;
	    }
	    break;
   case ESC:
	    memcpy(str,save,strlen(str)+1);
	    break;
   case DEL:
	    if (strlen(str)==0) goto ret;
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,bcol,cursym,bcol,bcol);
	    for (i=cursym;i<strlen(str);i++) str[i]=str[i+1];
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	    goto ret;
   case BKSPACE:
	    if (cursym==0 || strlen(str)<=1) goto ret;
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,bcol,cursym,bcol,bcol);
	    for (i=cursym-1;i<strlen(str);i++) str[i]=str[i+1];
	    cursym--;
	    RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	    goto ret;
   default:
	  if (c>241)  goto ret;
	  if (c==46 && rezhim==DIGIT_REAL && strchr(str,'.')!=NULL )
								 goto ret;
	  RPutLineWithCursor(x+8,y+10,str,SymWidth,bcol,cursym,bcol,bcol);
	  if (strlen(str)==0)
	     {
	     str[0]=c;
	     str[1]=0;
	     }
	  else if (cursym==length-1)   str[cursym]=c;
	  else if (cursym==strlen(str)-1)
		   {
		   str[cursym+1]=c;
		   str[cursym+2]=0;
		   }
	  else if (ins==0) str[cursym]=c;
	  else
		{

		if (strlen(str)==length)
		   for (i=strlen(str)-1;i>cursym;i--)  str[i]=str[i-1];
		else
		   for (i=strlen(str)+1;i>cursym;i--)  str[i]=str[i-1];
		str[cursym]=c;
		}
	  if (cursym!=strlen(str)-1) cursym++;
	  RPutLineWithCursor(x+8,y+10,str,SymWidth,col,cursym,curscol,bcol);
	  goto ret;
   }


if (IfSave==YES)
   {
   Rputimage(x,y,buf,0);
   farfree(buf);
   }

farfree(save);
return(str);
}


char *REnterStringBox(char *str, int left, int top, int length, int SymWidth,
		  int col, int bcol, int curscol, int rezhim,
		  int boxcol, int bordcol, int mescol,
		  char *mes1, char *mes2)
{
int right,bottom;
int mleft,mtop;
char *buf;
int l=0;

if (mes1!=0) l=strlen(mes1);
if (mes2!=0 && l<strlen(mes2)) l=strlen(mes2);
l=l*LINE_WIDTH+30;

right=(length*SymWidth+60>l) ? left+length*SymWidth+60 : left+l ;

bottom=top+LINE_HEIGHT+50;
if (mes1!=0) bottom+=LINE_HEIGHT*2;
if (mes2!=0) bottom+=LINE_HEIGHT*2;

buf=(char *)farmalloc(Rimagesize(left,top,right,bottom));
if (buf==NULL)  { gerrmes(NOMEM,"REnterStringBox",2,COMMON_ERROR); return 0; }
Rgetimage(left,top,right,bottom,buf);

RbarcM(left,top,right,bottom,1,boxcol);
RramcM(left+4,top+4,right-4,bottom-4,bordcol);

if (mes1!=0)
   {
   mleft=left+(right-left-strlen(mes1)*LINE_WIDTH)/2;
   mtop=top+25;
   RtextcM(mleft,mtop,mes1,mescol);
   }

if (mes2!=0)
   {
   mleft=left+(right-left-strlen(mes2)*LINE_WIDTH)/2;
   mtop=(mes1==0) ? top+35 : top+20+LINE_HEIGHT*2;
   RtextcM(mleft,mtop,mes2,mescol);
   }

REnterString(str,left+25,bottom-30-LINE_HEIGHT,length,SymWidth,
	     col,bcol,curscol,rezhim,NO);

Rputimage(left,top,buf,0);
farfree(buf);
return str;
}


int IfDate(char *str)
{
char *c,s[15];
int i,d,m,y;


c=strchr(str,'-');
if (c!=NULL) { c=strchr(c+1,'-'); if (c!=NULL) return GM_NOK; }

c=strchr(str,'.');
if (c==NULL) return GM_NOK;
i=(int)(c-str);
if (i==1)  // вставить 0
   {
   memmove(str+1,str,strlen(str)+1);
   str[0]='0';
   }
else if (i!=2 ) return GM_NOK;

c=strchr(str+3,'.');
if (c==NULL) return GM_NOK;
i=(int)(c-str-3);
if (i==1)  // вставить 0
   {
   memmove(str+4,str+3,strlen(str)+1-3);
   str[3]='0';
   }
else if (i!=2 ) return GM_NOK;

c=strchr(str+6,'.');
if (c!=NULL) return GM_NOK;

memcpy(s,str,2);
s[2]=0;
d=atoi(s);

memcpy(s,str+3,2);
s[2]=0;
m=atoi(s);

memccpy(s,str+6,'.',4);
s[4]=0;
y=atoi(s);

if (d<1 || m<1 || m>12 || y==0) return GM_NOK;
if (d>days_in_month(m,y)) return GM_NOK;
return GM_OK;
}


int days_in_month (int m, int y)
{
if (m==1 || m==3 || m==5 || m==7 || m==8 || m==10 || m==12 ) return(31);
else if (m==2 && y%4==0 && ( y%100!=0 || y/100%4==0 || y<1582) ) return(29);
else if (m==2) return(28);
else return(30);
}


int REnterFilename(char *name, int x, int y, int rezhim,
		     int col, int bcol, int curscol)
/* Вводит имя файла с расширением (rezhim==FILENAME_FULL) или
   без (FILENAME_PART)
   возврат  0 - OK
	   -1 - ошибка */
{
int i,n=0,length;

length=(rezhim==FILENAME_PART) ? 8 : 12 ;
REnterString(name,x,y,length,12,col,bcol,curscol,rezhim,YES);
if (rezhim==FILENAME_FULL)
   {
   for (i=0;i<strlen(name);i++) if (name[i]=='.') n++;
   if (n>1) { name[0]=0; return -1; }
   }
if (name[0]==0) return -1;
return 0;
}


/***************************** FONTER ********************************/

void FUOn()
{
FUON=1;
}

void FUOff()
{
FUON=0;
}

/* FONT */
void FUSetCurrentFont(struct FUFONT *f)
{
FUCurrentFont=f;
}

/* REF */
void FUSetCurrentRef(struct FUREF *f)
{
FUCurrentRef=f;
}


/* REF */
void FUSetRef(struct FUREF *f, int qsym, char *num, char *sym)
{
f->qsym=qsym;
f->num=num;
f->sym=sym;
}

/* REF */
int FUIfinRef(int sym)
/* если ASCII символ попадает в FUCurrentRef, возвращается его номер в шрифте,
   если нет, то -1 */
{
int i;
for (i=0; i<FUCurrentRef->qsym; i++)
    if ( FUCurrentRef->sym[i]==sym ) return FUCurrentRef->num[i];
return -1;
}

/* FONT */
void FUPutSym(int x, int y, int num, int fonttype)
			   /* FU_SMALLFONT 8; FU_LARGEFONT 32 */
{
int rx,ry;

if (FUON==0) return;
else if (FUCurrentFont==0) { gerrmes(FU_SETFONT,"FUPutSym",0,COMMON_ERROR); return; }
else if (num>=FUCurrentFont->qsym) { gerrmes(FU_BADNUM,"FUPutSym",0,COMMON_ERROR); return; }
rx=x; ry=y;
Rconvert(&rx,&ry);
RPutSym(rx,ry,FUCurrentFont->buf,num,fonttype);
}


/* FONT */
void FUPutSymc(int x, int y, int num, int fonttype, int col)
			   /* FU_SMALLFONT 8; FU_LARGEFONT 32 */
{
if (FUON==0) return;
Rcol(col);
FUPutSym(x,y,num,fonttype);
}


/* FONT REF */
void FUtext(int x, int y, char *s)
{
char c[2]={" "};
int i,j;

if (FUON==0) return;
if (FUCurrentRef==0) { gerrmes(FU_SETREF,"FUtext",0,COMMON_ERROR); return; }
else if (FUCurrentFont==0) { gerrmes(FU_SETFONT,"FUtext",0,COMMON_ERROR); return; }
for (i=0;i<strlen(s);i++)
   {
   for (j=0;j<FUCurrentRef->qsym;j++) if (FUCurrentRef->sym[j]==s[i]) break;
   if (j==FUCurrentRef->qsym)
      {
      c[0]=s[i];
      outtextxy(x+i*8,y,c);
      }
   else
      RPutSym(x+i*8,y,FUCurrentFont->buf,FUCurrentRef->num[j],FU_SMALLFONT);
   }
}


/* REF */
char *FUConvertC(char *s)
{
char *c,cc;
int i,j=0,l,n;

if (FUON==0) return 0;
if (FUCurrentRef==0) { gerrmes(FU_SETREF,"FUConvertC",0,COMMON_ERROR); return 0; }
if ( (l=strlen(s))<=0) return(0);
c=(char *)farmalloc(l);
if (c==NULL) gerrmes(NOMEM,"FUConvertC",0,SYSTEM_ERROR);
for (i=0;i<l;i++)
   {
   if (s[i]==FU_SYSTEMSYMBOL)
      {
      n=(s[i+1]-48)*10+s[i+2]-48; // номер по порядку в FUREF
      // !!! message для неправильного n
      cc=FUCurrentRef->sym[n];
      c[j++]=cc;
      i+=3;
      }
   c[j++]=s[i];
   }
c[j]=0;
strcpy(s,c);
farfree(c);
return(s);
}

/* FONT */
int FUPrintSym(int num, int printer)
{
int r;

if (FUON==0) return -2;
if (FUCurrentFont==0) { gerrmes(FU_SETFONT,"FUPrintSym",0,COMMON_ERROR); return -1; }
r=RPrintSym(FUCurrentFont->buf,num,printer);
//if (r==-1) gerrmes(FU_PRINTER,"FUPrintSym",0,COMMON_ERROR);
return r;
}



char *FULoadFont(struct FUFONT *font, int fonttype)
			   /* FU_SMALLFONT 8; FU_LARGEFONT 32 */
{
int f,r,l;
struct ffblk ff;

if (font->buf!=0) { gerrmes(FU_NOCLEAR,"FULoadFont",0,COMMON_ERROR); return 0; }

r=findfirst(font->filename,&ff,FA_ARCH);
if (r!=0) { FUerr=FU_NOFILE; return 0; }

f=_open(font->filename,O_BINARY | O_RDWR);
if (f==-1) { FUerr=FU_CANTOPEN; return 0; }

l=(int)filelength(f);
if (l<=0 || l%fonttype != 0) { FUerr=FU_BADFILE; return 0; }

font->buf=(char *)farmalloc(l);
if (font->buf==NULL) { FUerr=FU_NOMEM; return 0; }

r=read(f,font->buf,l);
if (r==-1) { FUerr=FU_CANTREAD; return 0; }

font->qsym=l/fonttype;

close(f);
FUerr=FU_OK;
return(font->buf);
}


void FUReleaseFont(struct FUFONT *f)
{
f->qsym=0;
f->filename[0]=0;
farfree(f->buf);
f->buf=0;
}


void RPutSym(int x, int y, char *buf, int num, int fonttype)
			   /* FU_SMALLFONT 8; FU_LARGEFONT 32 */
{
int i,j,col;

col=getcolor();

for (i=0;i<fonttype;i++)
   for (j=0;j<8;j++)
	  if ( ( *(buf+num*fonttype+i) & (1<<j) ) != 0 ) putpixel(x+i%16,y+j+8*(i/16),col);
}



int RPrintSym(char *buf, int num, int printer)
				  /* FU_EPSON 0; FU_IBM 1 */
{
int i,j,r;
char c;

if (printer==FU_EPSON)
   { FUB(27); FUB(42);FUB(6);FUB(8%256);FUB(8/256); }
 /* Epson Select Graphics Mode - 90 dots/inch horizontal for 640 columns */
else { FUB(27);FUB(76);FUB(8%256);FUB(8/256); }
 /* IBM low-speed double-density graphics mode */

for (i=0;i<8;i++)
   {
   c=0;
   for (j=0;j<8;j++)
      {
      if ( ( *(buf+num*8+i) & (1<<j) ) !=0 )    c+=(1<<(7-j));
      }
   FUB(c);
   }
return(0);
}

/****************************** GWAIT *******************************/
void GWaitInit(int l, int t, int lang, int srcdigit,
	       int col, int fcol, int rcol, int pcol, int tcol)
{
char *s[3]={" WAIT       %","ЖДИТЕ       %","WARTEN      %"};
int r,b;

r=l+13*LINE_WIDTH+80;
b=t+LINE_HEIGHT+80;
GWait.l=l;
GWait.t=t;
GWait.r=r;
GWait.b=b;
GWait.curdigit=srcdigit;
GWait.fcol=fcol;
GWait.pcol=pcol;
GWait.tcol=tcol;
GWait.time=time(NULL);
GWait.elapstime[0]=0;
GWait.estimtime[0]=0;

if (GWait.buf!=NULL) gerrmes(GW_DOUBLEINIT,"GWaitInit",0,SYSTEM_ERROR);
GWait.buf=(char *)farmalloc(Rimagesize(l,t,r,b));
if (GWait.buf==NULL) gerrmes(NOMEM,"GWaitInit",0,SYSTEM_ERROR);

Rmouse_hide();
Rgetimage(l,t,r,b,GWait.buf);
Rbarc(l,t,r,b,SOLID_FILL,fcol);
Rramc(l+8,t+6,r-8,b-6,rcol);
Rtextc(l+40,t+20,s[lang],col);
Rcol(pcol);
Rdigit_right(l+40+7*LINE_WIDTH,t+20,GWait.curdigit,3);
Rsymc(GWait.l+40+6*LINE_WIDTH,GWait.t+50,62,col);

}


void GWaitChange(int digit)
{
time_t t;
long estim,elaps;

if (GWait.buf==NULL) gerrmes(GW_NOINIT,"GWaitChange",0,SYSTEM_ERROR);
Rmouse_hide();
Rcol(GWait.fcol);
Rdigit_right(GWait.l+40+7*LINE_WIDTH,GWait.t+20,GWait.curdigit,3);
GWait.curdigit=digit;
Rcol(GWait.pcol);
Rdigit_right(GWait.l+40+7*LINE_WIDTH,GWait.t+20,GWait.curdigit,3);

t=time(NULL);
Rtextc(GWait.l+40,GWait.t+50,GWait.elapstime,GWait.fcol);
Rtext(GWait.r-30-5*LINE_WIDTH,GWait.t+50,GWait.estimtime);
elaps=(long)(t-GWait.time);
Seconds2String(elaps,GWait.elapstime,"MM:SS");
Rtextc(GWait.l+40,GWait.t+50,GWait.elapstime,GWait.tcol);
if (digit!=0)
   {
   estim=elaps*100/digit;
   Seconds2String(estim,GWait.estimtime,"MM:SS");
   Rtext(GWait.r-30-5*LINE_WIDTH,GWait.t+50,GWait.estimtime);
   }
}


void GWaitClose()
{
if (GWait.buf==NULL) gerrmes(GW_NOINIT,"GWaitChange",0,SYSTEM_ERROR);
Rputimage(GWait.l,GWait.t,GWait.buf,0);
farfree(GWait.buf);
GWait.buf=0;
Rmouse_show();
}


void Rsound(int freq, int millisec)
{ sound(freq); delay(millisec); nosound(); }
