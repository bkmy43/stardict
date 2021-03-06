#include "graph.h"
#include "box.hpp"
#include "keys.h"
#include <string.h>
#include <graphics.h>


int  ftoupper(int c); // !!! ���� �㭪樨 ��室���� � GMENU.C

extern int LINE_WIDTH,LINE_HEIGHT;
extern int LANG;


int Box(int type, int pal,
	      char *s0, char *s1, char *s2, char *s3, char *s4,
	      char *s5, char *s6, char *s7, char *s8, char *s9)
{
BOX b;
int r;

b.SetHorizontal(CENTER_JUSTIFIED,1024/2,25,CENTER_JUSTIFIED,20,EDGE_JUSTIFIED);
    // ⨯ ������� ��p����⠫쭮� ���p������ BOX: �� ����� �p���� ��� �� 業�p�
    // ��p����⠫쭠� ���p����� BOX
    // p����ﭨ� �� ����� �p����� BOX �� ��砫� ��p��
    // ⨯ ������� ��p����⠫��� ���p����� KEYs: �� ����� �p���� ��� 業�p�p������
    // ��������� ���p��� ����� KEYs (���뢠���� ⮫쪮, �᫨ ��p��� ��p�⪨�)
    // ⨯ ������� ��p����⠫��� ���p����� ��p��: �� ����� �p���� ��� 業�p�p������
b.SetVertical(CENTER_JUSTIFIED,768/2,20,10,55);
    // ⨯ ������� ��p⨪��쭮� ���p������ BOX: �� ��p孥� �p���� ��� �� 業�p�
    // ��p⨪��쭠� ���p����� BOX
    // p����ﭨ� �� ��p孥� �p����� BOX �� ��p��� ��p��� (��� ��
		//  - �� ��᫥���� ��p��� �� ��p設� KEY)
   // p����ﭨ� �� ������ �p����� ��p��� �� ��p孥� �p����� ᫥���饩 ��p���
   // p����ﭨ� �� ��p孥� �p����� KEY �� ������ �p����� BOX
b.SetKeyMargins(10,7);
   // p����ﭨ� �� ������/�p����� �p�� KEY �� ��砫�/���� ��� ��p���
   // p����ﭨ� �� ��p孥��/������� �p�� KEY �� ��� ��p���

   // ⥪�� ��p���
if (s0!=0) b.SetMessageString(s0);
if (s1!=0) b.SetMessageString(s1);
if (s2!=0) b.SetMessageString(s2);
if (s3!=0) b.SetMessageString(s3);
if (s4!=0) b.SetMessageString(s4);
if (s5!=0) b.SetMessageString(s5);
if (s6!=0) b.SetMessageString(s6);
if (s7!=0) b.SetMessageString(s7);
if (s8!=0) b.SetMessageString(s8);
if (s9!=0) b.SetMessageString(s9);

b.SetShadow(15,10,DARKGRAY);
  // ��p�������� p����p ⥭�
  // ��p⨪���� p����p ⥭�
  // 梥� ⥭�
if (type==B_ONE_BUTTON)
   {
   if (pal==B_ERROR)  b.SetBoxKey(" NOT OK ",1,0);
   else b.SetBoxKey(" OK ",1,0);
    // ��p��� KEY
    // H���p �㪢� � ��p���, ���p�� �㤥� ��p�祩 �����襩
    // ����p��⨢��� ������, �� ���p�� �㤥� p����p����� KEY]
   }
else
   {
   if (LANG==GW_RUS)
      {
      b.SetBoxKey(" �� ",1,0);
      b.SetBoxKey(" ��� ",1,ESC);
      }
   else
      {
      b.SetBoxKey(" YES ",1,0);
      b.SetBoxKey(" NO ",1,ESC);
      }
   }
if (pal==B_ERROR)
   {
   b.SetColors(RED,LIGHTGRAY,WHITE);
   // 梥� BOX�
   // 梥� ��p��p� BOX�
   // 梥� ��p��
   b.SetBoxKeyColors(LIGHTGRAY,WHITE,RED,DARKGRAY,YELLOW,BLACK);
   // 梥� KEYs (�� ⥪�饣�)
   // 梥� ⥪�� �� KEYs (�� ⥪�饬)
   // 梥� ��p�祩 �㪢�
   // 梥� ⥭� KEYs
   // 梥� ⥪�饣� KEYs
   // 梥� ⥪�� �� ⥪�饬 KEYs
   }
 else
   {
   b.SetColors(CYAN,DARKGRAY,BLUE);
   b.SetBoxKeyColors(LIGHTGRAY,WHITE,RED,DARKGRAY,YELLOW,BLACK);
   }
r=b.Draw(0,B_SAVE);  // ��p�ᮢ��� � �������� BOX
  // ����p �᪮��� ⥪�饣� KEY
  // ������� �� ��p����� ��p�� ��� BOX (B_SAVE / B_NOT_SAVE)
  // ! ����p�� - ����p KEY (0-...)
return r;
}


/***************************** BOX_KEY ******************************/
BOX_KEY::BOX_KEY()
{
left=-1;
top=-1;
right=-1;
bottom=-1;
str=0;
highletnum=-1;
additional_key=NO_KEY;
}

void BOX_KEY::SetLTBCorner(int l, int t, int b)
{
left=l;
top=t;
bottom=b;
}


void BOX_KEY::SetKey(char *s, int h, int akey)
{
str=s;
highletnum=h;
additional_key=akey;
}


int BOX_KEY::GetLength()
{
return strlen(str)*LINE_WIDTH;
}


int  BOX_KEY::IfMyKey(int r, int mx, int my)
{
if ( r==-2 && additional_key==ESC ) return MINE;
if ( r==-1 && mx>=left && mx<=right && my>=top && my<=bottom ) return MINE;
if ( ftoupper(r)==ftoupper(additional_key)
  || ftoupper(r)==ftoupper(str[highletnum]) ) return MINE;
return NOT_MINE;
}


void BOX_KEY::Draw(int gcol, int textcol, int highletcol, int shcol,
		   int hor_otstup)
{
right=left+GetLength()+hor_otstup*2;
RbarcM(left+5,top+5,right+5,bottom+5,1,shcol);
RbarcM(left,top,right,bottom,1,gcol);
Rcol(textcol);
Rtext_with_high_let(left+hor_otstup,(top+bottom-LINE_HEIGHT)/2,str,highletnum,highletcol);
}


/******************************* BOX **********************************/
BOX::BOX()
{

for (int i=0;i<B_MAX_MES_STRINGS;i++) mes[i]=0;
qmes=0;
qkeys=0;

left=-1;
top=-1;
right=-1;
bottom=-1;

hor_justifying_type=-1;
x=-1;
hor_otstup=-1;
key_justifying_type=-1;
key_min_interval=-1;
mes_justifying_type=-1;

vert_justifying_type=-1;
y=-1;
top_mes_otstup=-1;
mes_interval=-1;
key_bottom_otstup=-1;

hor_key_otstup=-1;
vert_key_otstup=-1;

curkey=-1;
allkeyslength=0;

groundcol=-1;
bordcol=-1;
mescol=-1;

shadow_hor_otstup=0;
shadow_vert_otstup=0;
shadowcol=-1;

key_groundcol=-1;
key_textcol=-1;
key_highletcol=-1;
key_shadowcol=-1;
key_highgroundcol=-1;
key_hightextcol=-1;
}


void BOX::InitBox()
{
for (int i=0;i<B_MAX_MES_STRINGS;i++) mes[i]=0;
qmes=0;
}

void BOX::SetHorizontal(int horjust, int xx, int otstup, int keyjust,
			int keyminint, int mesjust)
{
hor_justifying_type=horjust;
x=xx;
hor_otstup=otstup;
key_justifying_type=keyjust;
key_min_interval=keyminint;
mes_justifying_type=mesjust;
}

void BOX::SetVertical(int vertjust, int yy, int topmesotstup, int mesint,
		      int keybototstup)
{
vert_justifying_type=vertjust;
y=yy;
top_mes_otstup=topmesotstup;
mes_interval=mesint;
key_bottom_otstup=keybototstup;
}


void BOX::SetKeyMargins(int hor, int vert)
{
hor_key_otstup=hor;
vert_key_otstup=vert;
}


void BOX::SetMessageString(char *s)
{
if (qmes>=B_MAX_MES_STRINGS)
   termes("Too many message strings. Increase constant value.","BOX::SetMessageString");

mes[qmes]=s;
qmes++;
}


void BOX::SetColors(int gcol, int bcol, int mcol)
{
groundcol=gcol;
bordcol=bcol;
mescol=mcol;
}

void BOX::SetShadow(int shhor, int shvert, int shcol)
{
shadow_hor_otstup=shhor;
shadow_vert_otstup=shvert;
shadowcol=shcol;
}


void BOX::SetBoxKey(char *s, int hletnum, int akey)
{
if (qkeys>=B_MAX_BOX_KEYS)
	    termes("Too many keys. Increase constant value.","BOX::SetBoxKey");
if (akey==ENTER)
   termes("Using ENTER is an additional_key is prohibited.","BOX::SetBoxKey");

bk[qkeys].SetKey(s,hletnum,akey);
qkeys++;
allkeyslength+=strlen(s);
}


void BOX::SetBoxKeyColors(int gcol, int tcol, int hletcol, int shcol,
			  int hgcol, int htcol)
{
key_groundcol=gcol;
key_textcol=tcol;
key_highletcol=hletcol;
key_shadowcol=shcol;
key_highgroundcol=hgcol;
key_hightextcol=htcol;
}



int BOX::Draw(int ckey, int ifsave)
{
if ( qkeys<1 ) termes("Use SetBoxKey()","BOX::Draw");
if ( key_groundcol==-1 || key_textcol==-1 || key_highletcol==-1
  || key_shadowcol==-1 || key_highgroundcol==-1 || key_hightextcol==-1 )
	     termes("Use SetBoxKeyColors()","BOX::Draw");
if ( groundcol==-1 || bordcol==-1 || mescol==-1 )
	     termes("Use SetColors()","BOX::Draw");
if ( hor_justifying_type==-1 || x==-1 || hor_otstup==-1
  || key_justifying_type==-1 || key_min_interval==-1
  || mes_justifying_type==-1 )    termes("Use SetHorizontal()","BOX::Draw");
if ( vert_justifying_type==-1 || y==-1 || top_mes_otstup==-1
  || mes_interval==-1 || key_bottom_otstup==-1 )
	     termes("Use SetVertical()","BOX::Draw");
if ( hor_key_otstup==-1 || vert_key_otstup==-1 )
	     termes("Use SetKeyMargins()","BOX::Draw");

curkey=ckey;

CountBoxParams();

int is=ifsave;
char *buf;

if (is==B_SAVE)
   {
   unsigned int im;
   im=Rimagesize(left,top,right+shadow_hor_otstup,bottom+shadow_vert_otstup);
   if (im==0xFFFF) is=B_NOT_SAVE;
   else if (! (buf=new char [im]) )  is=B_NOT_SAVE;
   else Rgetimage(left,top,right+shadow_hor_otstup,bottom+shadow_vert_otstup,buf);
   }

if (shadow_hor_otstup!=0 || shadow_vert_otstup!=0)
   RbarcM(left+shadow_hor_otstup,top+shadow_vert_otstup,
	 right+shadow_hor_otstup,bottom+shadow_vert_otstup,1,shadowcol);

if (is==B_SAVE)  Rputimage(left,top,buf,1);

RbarcM(left,top,right,bottom,1,groundcol);
RramcM(left+5,top+5,right-5,bottom-5,bordcol);

for (int i=0;i<qmes;i++) DrawMessage(i);



int l;
for (i=0;i<qkeys;i++)
   {
   l=DefineKeyLeft(i);
   bk[i].SetLTBCorner(l,bottom-key_bottom_otstup,
	       bottom-key_bottom_otstup+vert_key_otstup*2+LINE_HEIGHT);
   DrawKey(i);
   }


int r;
r=Monitor();

if (is==B_SAVE)
   {
   Rputimage(left,top,buf,0);
   delete buf;
   }
return r;
}

/*++++++++++++++++++++++++ PRIVATE ++++++++++++++++++++++++++++++++++*/
void BOX::DrawKey(int n)
{
if (n<0 || n>=qkeys) termes("Bad key nubmer","BOX::DrawKey");

if (n==curkey)
   bk[n].Draw(key_highgroundcol,key_hightextcol,key_highletcol,key_shadowcol,hor_key_otstup);
else  bk[n].Draw(key_groundcol,key_textcol,key_highletcol,key_shadowcol,hor_key_otstup);
}


int BOX::DefineMaxMessageLength()
{
int l=0;

for (int i=0;i<qmes;i++) if ( strlen(mes[i]) > l ) l=strlen(mes[i]);
return l;
}

void BOX::CountBoxParams()
{
int mes_width,key_width,width;
mes_width=DefineMaxMessageLength()*LINE_WIDTH+hor_otstup*2;
key_width=allkeyslength*LINE_WIDTH+hor_key_otstup*2+hor_otstup*2;
width=(mes_width>key_width) ?  mes_width : key_width ;
if (hor_justifying_type==EDGE_JUSTIFIED)
   {
   left=x;
   right=left+width;
   }
else
   {
   left=x-width/2;
   right=x+width/2;
   }


int height;
height=qmes*(LINE_HEIGHT+mes_interval)+top_mes_otstup*2+key_bottom_otstup;
if (hor_justifying_type==EDGE_JUSTIFIED)
   {
   top=y;
   bottom=top+height;
   }
else
   {
   top=y-height/2;
   bottom=y+height/2;
   }

}


void BOX::DrawMessage(int n)
{
int x,y;

if (mes_justifying_type==EDGE_JUSTIFIED) x=left+hor_otstup;
  else x=(right+left-strlen(mes[n])*LINE_WIDTH)/2;

y=top+top_mes_otstup+n*(LINE_HEIGHT+mes_interval);

RtextcM(x,y,mes[n],mescol);
}


int BOX::DefineKeyLeft(int n)
{
int x,r;

x=(right-left-hor_otstup*2)/qkeys;  // �p���p���⢮ ��� ������� key
if (key_justifying_type==EDGE_JUSTIFIED)  r=left+hor_otstup+x*n;
  else r=left+hor_otstup+x*n+x/2-bk[n].GetLength()/2-hor_key_otstup;
return r;
}


int BOX::Monitor()
{
int r,mx,my;

while(1)
   {
   r=Revent(&mx,&my);
   switch(r)
      {
      case LEFT:
	   if (qkeys==0) break;
	   if (curkey==0)
	      {
	      curkey=qkeys-1;
	      DrawKey(qkeys-1);
	      DrawKey(0);
	      }
	   else
	      {
	      curkey--;
	      DrawKey(curkey+1);
	      DrawKey(curkey);
	      }
	   break;

      case RIGHT:
	   if (qkeys==0) break;
	   if (curkey==qkeys-1)
	      {
	      curkey=0;
	      DrawKey(qkeys-1);
	      DrawKey(0);
	      }
	   else
	      {
	      curkey++;
	      DrawKey(curkey-1);
	      DrawKey(curkey);
	      }
	   break;

      case ENTER:  return curkey;

      default:
	   for (int i=0;i<qkeys;i++)
	      if (bk[i].IfMyKey(r,mx,my)==MINE) return i;
	   break;
      }
   }
}
