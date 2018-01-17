#include <graphics.h>
#include <string.h>
#include <dos.h>
#include "graph.h"
#include "list.hpp"
#include "keys.h"
#include "util.h"
#include "help.h"


#include <stdio.h> // TEMP

extern int LINE_HEIGHT,LINE_WIDTH;
/*
extern char ALL_SYMBOLS[];

void main()
{
//VIEW_LIST b;
ENTER_LIST b;
char *test[15]={"AAA СТРОКА # 1","AAC стpока #21","ABCC еще одна стpока","BADD А вот еще одна!","BAFА я последняя",
  "BE Угу-гу","BERF чепуха","BERFT откpовенный бpед","BERT тестовая стpока","BURT ни дня без стpочки",
  "BX вот так","BXA System error","BXC Internal stack overflow","BXCT Press any key to reboot...","BXDEN System halted"};
char st[16]="LLLGLGLDDDAAALL";
Ropen(GM_DETECT);
Rmouse_set();
RbarcM(0,0,1024,768,SOLID_FILL,DARKGRAY);

b.SetLTCorner(200,200);
b.SetRBCorner(950,350);
b.SetBarWidth(19);
//b.SetColors(BLUE,WHITE);  // для VIEW_LIST
b.SetColors(BLUE,WHITE,CYAN,BLACK);   // для ENTER_LIST
b.SetBorder(L_DOUBLE_BORD,WHITE);
b.SetDirection(L_HOR);
b.SetMargins(25,25,10);
b.SetData(test,15,st);
/* для ENTER_LIST
b.SetStatusType('L',LIGHTRED,MAGENTA);
b.SetStatusType('G',WHITE,BLACK);
b.SetStatusType('D',LIGHTCYAN,RED);
b.SetStatusType('A',YELLOW,LIGHTBLUE);


/* для VIEW_LIST
b.SetStatusType('L',LIGHTRED);
b.SetStatusType('G',WHITE);
b.SetStatusType('D',LIGHTCYAN);
b.SetStatusType('A',YELLOW);

b.SetColumns(L_HOR_INTERVAL,20);

b.SetBarButtonHeight(25);
b.SetBarColors(BLUE,LIGHTCYAN,WHITE,CYAN,DARKGRAY);
   //       bar   dot      arrow  button     border

b.SetABCOrder(NOT_CASE_SENSITIVE,ALL_SYMBOLS);
b.SetABCSearchBuffer(VISIBLE_SEARCH_BUFFER,15);

b.Monitor();

b.CloseSearchBufferWindow();
getsym();
Rclose();
}
*/
/**************************** ENTER_LIST *********************************/
void ENTER_LIST::SetStatusType(char v, int c, int hc)
{
stp[qstp].high_color=hc;
LIST::SetStatusType(v,c);
}


int ENTER_LIST::IfMyKey(int key, int mx, int my)
{
if (key==ENTER) return MINE;
if (qcolumns!=1 && (key==LEFT || key==RIGHT)) return MINE;
if (key==-1 && IfMyMouse(mx,my)!=NOT_MINE) return MINE;
return LIST::IfMyKey(key,mx,my);
}


int ENTER_LIST::Monitor()
{
int r,k,mx,my;

DrawList();
DrawScreen();

while(1)
   {
   k=Revent(&mx,&my);
   r=IfMyKey(k,mx,my);
   if (r==GM_NOK) continue;
   if (k==ESC || k==-2) return GM_NOK;
   r=Manage(k,mx,my);
   if (r!=GM_DONE) return r;
   }
}


int ENTER_LIST::Manage(int key, int mx, int my)
// возвpат GM_NOK - ошибка; GM_DONE - клавиша отpаботала;
//                  или номеp выбpанной стpоки
{
int r;

if (IfMyKey(key,mx,my)==NOT_MINE)
   {
   ClearSearchBuffer();
   return GM_NOK;
   }

switch(key)
   {
   case PGUP:
	up:
	if (curline==0 && LineOfCurstring()==0) { beep(); return GM_DONE; }
	if (curline==0)
	   {
	   DrawStringFull(curstring,'N');
	   if (GetDirection()==L_VERT) curstring-=LineOfCurstring();
	      else  curstring-=LineOfCurstring()*qcolumns;
	   DrawStringFull(curstring,'V');
	   }
	else if (curline<qlines_per_page)
	   {
	   if (GetDirection()==L_VERT) curstring-=curline;
	      else  curstring-=curline*qcolumns;
	   curline=0;
	   DrawScreen();
	   }
	else
	   {
	   curline-=qlines_per_page ;
	   if (GetDirection()==L_VERT) curstring-=qlines_per_page;
		else curstring-=qlines_per_page*qcolumns;
	   DrawScreen();
	   }
	ClearSearchBuffer();
	return GM_DONE;

   case PGDN:
	down:
	if (curline==qlines-1) { beep(); return GM_DONE; }
	if (LineOfCurstring()==qlines-1) { beep(); return GM_DONE; }

	if (qlines-curline<=qlines_per_page)
	   {
	   if (GetDirection()==L_VERT)
	      {
	      curstring+=qlines-curline-1;
	      if (curstring>=qstrings) curstring=qstrings-1;
	      else curline=qlines-1;
	      }
	     else
	      {
	      curstring+=(qlines-curline-1)*qcolumns;
	      if (curstring>=qstrings) curstring=qstrings-1;
	      curline=qlines-1;
	      }
	   DrawScreen();
	   }
	else
	   {
	   curline+=qlines_per_page;
	   if (GetDirection()==L_VERT)	 curstring+=qlines_per_page;
	   else
	      {
	      curstring+=qlines_per_page*qcolumns;
	      if (curstring>=qstrings) curstring=qstrings-1;
	      }
	   DrawScreen();
	   }
	ClearSearchBuffer();
	return GM_DONE;

   case HOME:
	if (curstring==0) { beep(); return GM_DONE; }
	curline=0;
	curstring=0;
	DrawScreen();
	ClearSearchBuffer();
	return GM_DONE;

   case END:
	if (curline>=qlines-1) { beep(); return GM_DONE; }
	curline=(GetDirection()==L_HOR) ? qlines-1 : (qstrings-1)%qlines ;
	curstring=qstrings-1;
	DrawScreen();
	ClearSearchBuffer();
	return GM_DONE;

   case UP:
	if (LineOfCurstring()==0) { beep(); return GM_DONE; }
	if (LineOfCurstring()==curline)
	   {
	   curline--;
	   if (GetDirection()==L_VERT) curstring--;
	     else curstring-=qcolumns;
	   DrawScreen();
	   }
	else
	   {
	   DrawStringFull(curstring,'N');
	   if (GetDirection()==L_VERT) curstring--;
	     else curstring-=qcolumns;
	   DrawStringFull(curstring,'V');
	   }
	ClearSearchBuffer();
	return GM_DONE;

   case DOWN:
	if (LineOfCurstring()>=qlines-1) { beep(); return GM_DONE; }
	if (LineOfCurstring()==curline+qlines_per_page-1)
	   {
	   curline++;
	   if (GetDirection()==L_VERT) curstring++;
	     else curstring+=qcolumns;
	   if (curstring>=qstrings) curstring=qstrings-1;
	   DrawScreen();
	   }
	else
	   {
	   DrawStringFull(curstring,'N');
	   if (GetDirection()==L_VERT) curstring++;
	     else curstring+=qcolumns;
	   if (curstring>=qstrings) curstring=qstrings-1;
	   DrawStringFull(curstring,'V');
	   }
	ClearSearchBuffer();
	return GM_DONE;

   case ENTER:
	ClearSearchBuffer();
	return curstring;


   case LEFT:
	if (qcolumns==1) { beep(); return GM_DONE; }
	DrawStringFull(curstring,'N');
	if (GetDirection()==L_HOR)
	   {
	   if (curstring%qcolumns==0)
	      {
	      curstring+=qcolumns-1;
	      if (curstring>=qstrings) curstring=qstrings-1;
	      }
	   else curstring--;
	   }
	else
	   {
	   if (curstring<qlines)
	      {
	      curstring+=qlines*(qcolumns-1);
	      if (curstring>=qstrings) curstring-=qlines;
	      }
	   else curstring-=qlines;
	   }
	DrawStringFull(curstring,'V');
	ClearSearchBuffer();
	return GM_DONE;


   case RIGHT:
	if (qcolumns==1) { beep(); return GM_DONE; }
	DrawStringFull(curstring,'N');
	if (GetDirection()==L_HOR)
	   {
	   if (curstring%qcolumns==qcolumns-1) curstring-=(qcolumns-1);
	   else
	      curstring=(curstring!=qstrings-1)  ? curstring+1
		       : curstring-(qstrings-1)%qcolumns ;
	   }
	else
	   {
	   if (curstring+qlines<qstrings) curstring+=qlines;
	   else if (curstring-qlines*(qcolumns-1)>=0)
		   curstring-=qlines*(qcolumns-1);
	   }
	DrawStringFull(curstring,'V');
	ClearSearchBuffer();
	return GM_DONE;

   case -1:
	if (sl.IfMyMouse(mx,my)==MINE)
	   {
	   r=sl.DefinePercent(my);
	   if (r==L_UP) goto up;
	   else if (r==L_DOWN) goto down;
	   else
	      {
	      curline=(int)(r/100.0*qlines);
	      if (curline>qlines-1) curline=qlines-1;
	      curstring=(GetDirection()==L_HOR) ? curline*qcolumns : curline ;
	      }
	   DrawScreen();
	   }
	else if ( (r=IfMyMouse(mx,my))!=NOT_MINE)
	   {
	   DrawStringFull(curstring,'N');
	   if (curstring==r) return curstring;
	   curstring=r;
	   DrawStringFull(curstring,'V');
	   }
	ClearSearchBuffer();
	return GM_DONE;

   case BKSPACE:
	if (IfABCOrderExists()==NO) return GM_NOK;
	r=FindStringAccordingToSearchBuffer(curline,key);
	if (r==-1) { beep(); return GM_NOK; }
	if (IfStringIsOnPage(r)==YES)
	   {
	   DrawStringFull(curstring,'N');
	   curstring=r;
	   DrawStringFull(curstring,'V');
	   }
	else
	   {
	   curstring=r;
	   curline=LineOfCurstring();
	   DrawScreen();
	   }
	RepaintSearchBuffer();
	return GM_DONE;

   case F1: if (help_item_num!=-1) HShowHelp(help_item_num);
	return GM_DONE;

   default:
	if (IfABCOrderExists()==NO) return GM_NOK;
	if (key<32 || key>=256) return GM_NOK;
	int num;
	num=GetNumInABC(key);
	if (num==-1) { beep(); return GM_DONE; }
	r=FindStringAccordingToSearchBuffer(curline,key);
	if (r==-1) { beep(); return GM_DONE; }
	if (IfStringIsOnPage(r)==YES)
	   {
	   DrawStringFull(curstring,'N');
	   curstring=r;
	   DrawStringFull(curstring,'V');
	   }
	else
	   {
	   curstring=r;
	   curline=LineOfCurstring();
	   DrawScreen();
	   }
	RepaintSearchBuffer();
	return GM_DONE;
   }
}


void ENTER_LIST::GetStringField(int n, int *left, int *top, int *right, int *bottom)
{
int x,y;

DefineStringCoordinates(n,&x,&y);

*left=x-4;
*right=x+DefineMaxStringLength()*LINE_WIDTH+4;
*top=y-GetVertInterval()/2;
*bottom=y+LINE_HEIGHT+GetVertInterval()/2;
}


void ENTER_LIST::DrawStringFull
(int n, int rezhim)
// rezhim == 'N' или 'V' норвальный/выделенный
{
int left,top,right,bottom;
int gcol,tcol;

if (rezhim=='N') gcol=ground_color; else gcol=high_ground_color;
tcol=DefineStringColor(n,rezhim);

GetStringField(n,&left,&top,&right,&bottom);
RbarcM(left,top,right,bottom,1,gcol);
DrawString(n,tcol);
}


int ENTER_LIST::LineOfString(int n)
{
if (GetDirection()==L_HOR)    return  n/qcolumns;
	       else           return  n%qlines;
}


int ENTER_LIST::IfStringIsOnPage(int n)
{
if (LineOfString(n)>=curline && LineOfString(n)<curline+qlines_per_page)
   return YES;
else return NO;}


int ENTER_LIST::IfMyMouse(int x, int y)
{
int l,t,r,b;

if (GetDirection()==L_HOR)
   for (int i=0;i<qstrings_per_page;i++)
      {
      GetStringField(i+curline*qcolumns,&l,&t,&r,&b);
      if (x>=l && x<=r && y>=t && y<=b)  return  i+curline*qcolumns;
      }
else
   for (int i=0;i<qlines_per_page;i++)
      for (int j=0;j<qcolumns;j++)
	 {
	 if (i+curline>qlines-1) break;
	 GetStringField(i+curline+j*qlines,&l,&t,&r,&b);
	 if (x>=l && x<=r && y>=t && y<=b)  return  i+curline+j*qlines;
	 }

return NOT_MINE;
}


int ENTER_LIST::DefineStringColor(int n, char rezhim)
// rezhim == 'N' или 'V' норвальный/выделенный
{
if (status==0)
  if (rezhim=='N') return text_color; else return high_text_color;
for (int i=0;i<qstp;i++)
   {
   if (stp[i].value==status[n])
   if (rezhim=='N') return stp[i].color; else return stp[i].high_color;
   }
if (rezhim=='N') return text_color; else return high_text_color;
}

/**************************** VIEW_LIST *********************************/


int VIEW_LIST::Monitor()
{
int r,k,mx,my;

DrawList();
DrawScreen();

while(1)
   {
   k=Revent(&mx,&my);
   r=IfMyKey(k,mx,my);
   if (r==GM_NOK) continue;
   if (k==ESC || k==-2) return GM_NOK;
   Manage(k,mx,my);
   }
}



int VIEW_LIST::Manage(int key, int mx, int my)
{
int r;

if (IfMyKey(key,mx,my)==NOT_MINE)
   {
   ClearSearchBuffer();
   return GM_NOK;
   }

switch(key)
   {
   case PGUP:
	if (curline==0) { beep(); return GM_OK; }
	curline=(curline<qlines_per_page) ? 0 : curline-qlines_per_page ;
	DrawScreen();
	ClearSearchBuffer();
	return GM_OK;

   case PGDN:
	if (curline>=qlines-1) { beep(); return GM_OK; }
	curline+=qlines_per_page ;
	if (curline>qlines-1)  curline=qlines-1;
	DrawScreen();
	ClearSearchBuffer();
	return GM_OK;

   case HOME:
	if (curline==0) { beep(); return GM_OK; }
	curline=0;
	DrawScreen();
	ClearSearchBuffer();
	return GM_OK;

   case END:
	if (curline>=qlines-1) { beep(); return GM_OK; }
	curline=qlines-1;
	DrawScreen();
	ClearSearchBuffer();
	return GM_OK;

   case UP:
	if (curline==0) { beep(); return GM_OK; }
	curline--;
	DrawScreen();
	ClearSearchBuffer();
	return GM_OK;

   case DOWN:
	if (curline>=qlines-1) { beep(); return GM_OK; }
	curline++;
	DrawScreen();
	ClearSearchBuffer();
	return GM_OK;

   case F1: if (help_item_num!=-1) HShowHelp(help_item_num);
	return GM_OK;

   case -1:
	ClearSearchBuffer();
	r=sl.DefinePercent(my);
	if (r==L_UP)
	   {
	   if (curline==0) { beep(); return GM_OK; }
	   curline--;
	   }
	else if (r==L_DOWN)
	   {
	   if (curline>=qlines-1) { beep(); return GM_OK; }
	   curline++;
	   }
	else
	   {
	   curline=(int)(r/100.0*qlines);
	   if (curline>qlines-1) curline=qlines-1;
	   }
	DrawScreen();
	return GM_OK;

   case BKSPACE:
	if (IfABCOrderExists()==NO) return GM_NOK;
	r=FindStringAccordingToSearchBuffer(curline,key);
	if (r==-1) { beep(); return GM_OK; }
	curline=r;
	DrawScreen();
	RepaintSearchBuffer();
	return GM_OK;

   default:
	if (IfABCOrderExists()==NO) return GM_NOK;
	if (key<32 || key>=256) return GM_NOK;
	int num;
	num=GetNumInABC(key);
	if (num==-1) { beep(); return GM_OK; }
	r=FindStringAccordingToSearchBuffer(curline,key);
	if (r==-1) { beep(); return GM_OK; }
	curline=r;
	DrawScreen();
	RepaintSearchBuffer();
	return GM_OK;
   }
}



/**************************** SLIDEBAR *********************************/
SLIDEBAR::SLIDEBAR()
{
left=-1;
top=-1;
right=-1;
bottom=-1;
width=-1;
height=-1;
button_height=-1;
dotted_bar_height=-1;
current_percent=0;

bar_color=-1;
dot_color=-1;
arrow_color=-1;
button_color=-1;
border_color=-1;

}


void SLIDEBAR::DrawSlideBar()
{
char uparrow[2]={24,0};
char downarrow[2]={25,0};

if (left==-1) termes("Set left margin !","SLIDEBAR::DrawSlideBar");
if (top==-1) termes("Set top margin  !","SLIDEBAR::DrawSlideBar");
if (right==-1) termes("Set right margin  !","SLIDEBAR::DrawSlideBar");
if (bottom==-1) termes("Set bottom margin !","SLIDEBAR::DrawSlideBar");
if (width==-1) termes("Set width !","SLIDEBAR::DrawSlideBar");
if (height==-1) termes("Set height !","SLIDEBAR::DrawSlideBar");
if (button_height==-1) termes("Set button height !","SLIDEBAR::DrawSlideBar");

if (bar_color==-1 || dot_color==-1 || arrow_color==-1
    ||  button_color==-1 || border_color==-1)
       termes("Set colors !","SLIDEBAR::DrawSlideBar");

RbarcM(left,top,right,top+button_height,1,button_color);
RtextcM(left+width/2-LINE_WIDTH/2+2,top+(button_height-LINE_HEIGHT)/2,uparrow,arrow_color);
RramcM(left,top,right,top+button_height,border_color);
RbarcM(left,bottom-button_height,right,bottom,SOLID_FILL,button_color);
RtextcM(left+width/2-LINE_WIDTH/2+2,bottom-(button_height+LINE_HEIGHT)/2,downarrow,arrow_color);
RramcM(left,bottom-button_height,right,bottom,border_color);

//Rbcol(bar_color);  !!!!
RbarcM(left,top+button_height,right,bottom-button_height,CLOSE_DOT_FILL,dot_color);

DrawButton(current_percent);
}


void SLIDEBAR::DrawButton(int percent)
{
int h,perc;

if (left==-1 || top==-1 || right==-1 || bottom==-1) return;

if (percent<0) perc=0;
else if (percent>100) perc=100;
else perc=percent;

h=(int)((height-button_height*3)/100.0*perc);
h+=top+button_height;

//Rbcol(bar_color);   !!!!
RbarcM(left,top+button_height,right,bottom-button_height,CLOSE_DOT_FILL,dot_color);

RbarcM(left,h,right,h+button_height,SOLID_FILL,button_color);

current_percent=percent;
}


int SLIDEBAR::IfMyMouse(int x, int y)
// возвpат - MINE или NOT_MINE
{
if (x<left || x>right || y<top || y>bottom) return NOT_MINE;
else return MINE;
}


int SLIDEBAR::DefinePercent(int y)
// возвpащает количество пpоцентов текста, соответствующих должному положению
// экpана, либо L_UP или L_DOWN, если нажаты кнопки SlideBar'a
{
int p,len;

if (y<top+button_height) return L_UP;
else if (y>bottom-button_height) return L_DOWN;
else if (y<=bottom-button_height && y>bottom-button_height*2) p=100;
else
   {
   len=height-button_height*3;
   p=(int)(100.0*(y-top-button_height)/len);
   }
return p;
}

/****************************** LIST ************************************/
LIST::LIST(int size=L_MAX_STRINGS) // +++
//LIST::LIST() // +++
{
left=-1;
top=-1;
right=-1;
bottom=-1;

direction=-1;
hor_otstup=-1;
vert_otstup=-1;
hor_interval=-1;
vert_interval=-1;

//for (int i=0;i<L_MAX_STRINGS;i++) strings[i]=0; // +++
//for (i=0;i<L_MAX_STRINGS;i++) status[i]=0; //+++
if (! (strings=new char * [size]) ) { printf("\nNOMEM"); return; }
if (! (status=new char [size]) ) { printf("\nNOMEM"); return; }

qstrings=-1;
qlines_per_page=-1;
qcolumns=-1;
qlines=-1;
qstrings_per_page=-1;

border_type=-1;
border_color=-1;

ground_color=-1;
text_color=-1;

curline=-1;
//curpage=-1;
max_string_length=-1;

for (int i=0;i<L_MAX_INS_FLAGS;i++)  insert_flags[i]=-1;
slide_bar=-1;
for (i=0;i<L_MAX_STATUS_TYPES;i++)
     { stp[i].value=0;  stp[i].color=-1;  stp[i].high_color=-1; }
qstp=0;

empty_list_allowed=NO;


abc_order.type=CASE_SENSITIVE;
abc_order.abc=0;
abc_order.index=0;
abc_order.search_buffer_type=INVISIBLE_SEARCH_BUFFER;
abc_order.search_buffer=0;
abc_order.search_buffer_length=-1;
abc_order.search_buffer_buf=0;

help_item_num=-1;
}


LIST::~LIST()
{
if (strings!=0) delete strings;
strings=0;

if (status!=0) delete status;
status=0;

if (abc_order.index!=0)  delete abc_order.index;
abc_order.index=0;

if (abc_order.search_buffer!=0)  delete abc_order.search_buffer;
abc_order.search_buffer=0;

CloseSearchBufferWindow();
}


void LIST::SetData(char *str[], int qstr, char *st)
{
if (qstr>L_MAX_STRINGS) termes("Too many strings","LIST:SetData");
qstrings=qstr;
for (int i=0;i<qstr;i++) strings[i]=str[i];
if (st!=0) for (int i=0;i<qstr;i++) status[i]=st[i];
max_string_length=-1;
max_string_length=DefineMaxStringLength();

if (IfABCOrderExists()==YES) CreateABCIndex();
}


void LIST::SetABCOrder(int t, char *abc)
{
if (t!=CASE_SENSITIVE && t!=NOT_CASE_SENSITIVE)
   termes("Bad parameter","LIST::SetABCOrder");
abc_order.type=t;
abc_order.abc=abc;

if (qstrings!=-1) CreateABCIndex();
}


void LIST::SetABCSearchBuffer(int t, int len)
{
abc_order.search_buffer_type=t;
abc_order.search_buffer_length=len;
if ( ! (abc_order.search_buffer=new char [len+1] ) )
	 gerrmes(NOMEM,"LIST::SetABCSearchBuffer",0,SYSTEM_ERROR);
for (int i=0;i<len+1;i++) abc_order.search_buffer[i]=0;
}


void LIST::SetColumns(int rezhim, int qcol)
{
if (rezhim==L_Q_COLS)   qcolumns=qcol;
else
   {
   if (qstrings==-1) termes("Set Strings !","SetColumns");
   qcolumns=(right-left-hor_otstup*2+qcol) /
	       (max_string_length*LINE_WIDTH+qcol);
   if (qcolumns<=0)
      termes("The screen width is too small for the strings","LIST::SetColumns");
   }
hor_interval=(right-left-hor_otstup*2)/qcolumns;
}


void LIST::SetMargins(int hor, int vert, int inter)
{
hor_otstup=hor;
vert_otstup=vert;
vert_interval=inter;
qlines_per_page=(bottom-top-vert_otstup*2+inter)/(LINE_HEIGHT+inter);
}


void LIST::SetStatusType(char v, int c)
{
stp[qstp].value=v;
stp[qstp].color=c;
qstp++;
}




int LIST::DefineStringColor(int n)
{
if (status==0) return text_color;
for (int i=0;i<qstp;i++)  if (stp[i].value==status[n])  return stp[i].color;
return text_color;
}




int LIST::IfMyKey(int key, int mx, int my)
// возвpат - MINE или NOT_MINE
{
if (key==ESC || key==-2) return MINE;
if (slide_bar==1 && sl.IfMyMouse(mx,my)==MINE ) return MINE;
if (key==UP || key==DOWN || key==PGUP || key==PGDN || key==HOME || key==END)
	 return MINE;
if (key==F1) return MINE;

if (IfABCOrderExists()==YES)
   {
   if (key>=32 && key<256) if (GetNumInABC(key)!=-1) return MINE;
   if (key==BKSPACE)  return MINE;
   }

return NOT_MINE;
}


int LIST::GetNumInABC(int c)
// номеp в abc_order.abc  или -1
{
char *s;
char cc;

if (IfABCOrderExists()==NO) return -1;

cc=(abc_order.type==CASE_SENSITIVE) ? c : ftoupper(c) ;
s=strchr(abc_order.abc,cc);
if (s==0) return -1;
return (int)(s-abc_order.abc);
}






int LIST::FindStringAccordingToSearchBuffer(int search_from, int newsym)
// возвpащает номеp стpоки, соответствующей буфеpу (добавляя newsym в буфеp)
// или -1, если таковой нет или пpи ошибке
{
int l,num,r;

l=strlen(abc_order.search_buffer);

if (newsym==BKSPACE)
   {
   if (l==0) return -1;
   abc_order.search_buffer[l-1]=0;
   if (l==1) return 0;
   num=GetNumInABC(abc_order.search_buffer[0]);
   r=abc_order.index[num];
   for (int i=r;i<qstrings;i++)
      if (CompareStrings(GetString(i),abc_order.search_buffer,l-1,
						      abc_order.type) =='=')
						       return i;
   termes("Error !","LIST::FindStringAccordingToSearchBuffer");
   }


if (l==abc_order.search_buffer_length) return -1;
num=GetNumInABC(newsym);
if (num==-1) return -1;
if (l==0)
   {
   r=abc_order.index[num];
   if (r!=-1) abc_order.search_buffer[0]=(abc_order.type==CASE_SENSITIVE) ?
		    newsym : ftoupper(newsym);
   return r;
   }
else
   {
   abc_order.search_buffer[l]=(abc_order.type==CASE_SENSITIVE) ?
			    newsym : ftoupper(newsym);
   for (int i=search_from;i<qstrings;i++)
      {
      if (CompareStrings(GetString(i),abc_order.search_buffer,
			      l+1,abc_order.type) =='=')  return i;
      if (CompareStrings(GetString(i),abc_order.search_buffer,
                              l+1,abc_order.type) =='>') break;
      }
   abc_order.search_buffer[l]=0;
   return -1;
   }
}


void LIST::ClearSearchBuffer()
{
if (IfABCOrderExists()==NO) return;
for (int i=0;i<abc_order.search_buffer_length;i++)
     abc_order.search_buffer[i]=0;
RepaintSearchBuffer();
}


void LIST::OpenSearchBufferWindow()
{
int l,t,r,b;
unsigned int p;

DefineSearchBufferCoordinates(&l,&t,&r,&b);
p=Rimagesize(l,t,r,b);
if ( !(abc_order.search_buffer_buf=new char [p]) )
	     gerrmes(NOMEM,"LIST::OpenSearchBufferWindow",0,SYSTEM_ERROR);
Rgetimage(l,t,r,b,abc_order.search_buffer_buf);
RepaintSearchBuffer();
}


void LIST::CloseSearchBufferWindow()
{
if (abc_order.search_buffer_type==INVISIBLE_SEARCH_BUFFER) return;
if (abc_order.search_buffer_buf!=0)
   {
   int l,t,r,b;
   DefineSearchBufferCoordinates(&l,&t,&r,&b);
   Rputimage(l,t,abc_order.search_buffer_buf,0);
   delete abc_order.search_buffer_buf;
   abc_order.search_buffer_buf=0;
   }
}


void LIST::RepaintSearchBuffer()
{
int l,t,r,b;

if (IfABCOrderExists()==NO) return;
if (abc_order.search_buffer_type==INVISIBLE_SEARCH_BUFFER) return;
DefineSearchBufferCoordinates(&l,&t,&r,&b);
RbarcM(l,t,r,b,SOLID_FILL,ground_color);
RramcM(l+5,t+5,r-5,b-5,border_color);
RtextcM(l+LINE_WIDTH,t+LINE_HEIGHT,abc_order.search_buffer,text_color);
}



/*++++++++++++++++++++++++++++ PRIVATE ++++++++++++++++++++++++++++++++*/


void LIST::DrawList()
{
if (left==-1 || top==-1) termes("Use SetLTCorner","LIST::DrawList");
if (right==-1 || bottom==-1) termes("Use SetRBCorner","LIST::DrawList");
if (direction==-1) termes("Use SetDirection","LIST::DrawList");
if (hor_otstup==-1 || vert_otstup==-1 || vert_interval==-1) termes("Use SetMargins","LIST::DrawList");
if (border_type==-1 || border_color==-1 ) termes("Use SetBorder","LIST::DrawList");
if (text_color==-1 || ground_color==-1 ) termes("Use SetColors","LIST::DrawList");
if (empty_list_allowed==NO && (qstrings==-1 || strings[0]==0)  )
				 termes("Use SetData","LIST::DrawList");
if (empty_list_allowed==NO && (qcolumns==-1 || hor_interval==-1) )
				 termes("Use  SetColumns","LIST::DrawList");



RbarcM(left,top,right,bottom,SOLID_FILL,ground_color);

if (border_type!=L_NO_BORD)
   {
   RramcM(left+5,top+5,right-5,bottom-5,border_color);
   if (border_type==L_DOUBLE_BORD) RramcM(left+10,top+10,right-10,bottom-10,border_color);
   }


if (qstrings<=0 || strings[0]==0) return;

qlines=(qstrings%qcolumns==0) ?  qstrings/qcolumns : qstrings/qcolumns+1 ;
qstrings_per_page= qcolumns * qlines_per_page ;
curline=0;

if (qstrings>qstrings_per_page)
   {
   slide_bar=1;
   SetBarTop(top);
   SetBarBottom(bottom);
   SetBarRight(right);
   DrawSlideBar();
   }
else slide_bar=0;


if (IfABCOrderExists()==NO) return;
if (abc_order.search_buffer_type==INVISIBLE_SEARCH_BUFFER) return;
OpenSearchBufferWindow();
}


void LIST::DrawScreen()
{
double r;

RbarcM(left+hor_otstup-4,top+vert_otstup-vert_interval/2,
       right-hor_otstup+4,bottom-vert_otstup+vert_interval/2,
       SOLID_FILL,ground_color);

if (direction==L_HOR)
   for (int i=0;i<qstrings_per_page;i++)
	 DrawString(i+curline*qcolumns,DefineStringColor(i+curline*qcolumns));
else
   for (int i=0;i<qlines_per_page;i++)
      for (int j=0;j<qcolumns;j++)
	 {
	 if (i+curline>qlines-1) break;
	 DrawString(i+curline+j*qlines,DefineStringColor(i+curline+j*qlines));
	 }

r=curline*100.0/qlines;
if (r-(int)r >= 0.5) r+=1.0;
sl.DrawButton((int)r);
}


void LIST::DrawString(int n, int col)
{
int x,y;

DefineStringCoordinates(n,&x,&y);
if (x!=-1) RtextcM(x,y,strings[n],col);
}


int LIST::DefineMaxStringLength()
{
int l=0;

if (max_string_length!=-1) return max_string_length;

for (int i=0;i<qstrings;i++) if (strlen(strings[i])>l) l=strlen(strings[i]);
return l;
}


void LIST::DefineStringCoordinates(int n, int *x, int *y)
{
int col,lin;

if (n>=qstrings)
   {
   *x=-1;
   *y=-1;
   return;
   }

if (direction==L_HOR)
   {
   col=n%qcolumns;
   lin=n/qcolumns-curline;
   }
else
   {
   col=n/qlines;
   lin=n%qlines-curline;
   }


*x=left+hor_otstup+col*hor_interval;
*y=top+vert_otstup+lin*(LINE_HEIGHT+vert_interval);
}


void LIST::CreateABCIndex()
{
int l,num;
char curlet=0;

l=strlen(abc_order.abc);

if (! (abc_order.index=new int [l]) )
	gerrmes(NOMEM,"LIST::CreateABCIndex",0,SYSTEM_ERROR);

for (int i=0;i<l;i++) abc_order.index[i]=-1;

for (i=0;i<qstrings;i++)
   {
   if (*GetString(i)!=curlet)
      {
      curlet=*GetString(i);
      if ( (num=GetNumInABC(curlet))==-1)  continue;
      abc_order.index[num]=i;
      }
   }
}


void LIST::DefineSearchBufferCoordinates(int *l, int *t, int *r, int *b)
{
*l=(right+left-LINE_WIDTH*(abc_order.search_buffer_length+2))/2;
*t=bottom+LINE_HEIGHT*2;
*r=*l+LINE_WIDTH*(abc_order.search_buffer_length+2);
*b=*t+LINE_HEIGHT*3;
}


void LIST::beep()
{
sound(300); delay(70); nosound();
}


