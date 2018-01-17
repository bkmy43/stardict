#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdio.h>
#include <alloc.h>
#include <io.h>
#include <fcntl.h>
//#include <dos.h>
//#include <direct.h>

//#include "graph.h"
#include "gmenu.h"
#include "util.h"
#include "ftype.h"
#include "gmenu.hpp"
#include "menucol.h"
#include "keys.h"
#include "box.hpp"
#include "pcx.h"
#include "help.h"

#include "grview.h"



void menu(void);
int LeaveMainMenu();
void FinishAll(void);
int  finish(void);

void CheckFiles(void);
void ClearScreen(void);
int  AboutProgram(void);
int  AboutSystem(void);
int  LoadConfiguration(void);
int  SaveConfiguration(void);
//void SaveAndSet(void);
int  IfConfigurationSaved(void);
//int  SetTextEditorPath();
void AddColorStrings(int menunum);
int PreparePrinter();

#define BEGIN_DATE        (1999*12 + 12)
#define MAX_DELTA_DATE    6
#define MIN_VERSION       0
#define MAX_VERSION       10

void  WarningIfOldDate(void);
int   GetVersion();
void  StopIfWrongVersion(void);
int SetDefaultColors();

char *stmes(int n);
int dummy(void);
void zast(void);
char *GetPath(const char *filename);
void Start();

extern GMMENU GMmenu;
extern char BGIPath[MAXPATH];


const int NO_MOUSE=0;
const int IS_MOUSE=1;
int MouseIs,SavedMouseIs;
int PROTECT1,PROTECT2;
//int Direct,SavedDirect;
//int SavedOptimisation,Optimisation;
//int SavedCurLang;
//int SavedAlterCurLang;
//int SavedCurTheme;
//int GColor,LColor,DColor,SColor;
//int SavedGColor,SavedLColor,SavedDColor,SavedSColor;

GMEVENT ev;
int mh0,mh1,mh4,mh5,mh6,mh7;
int LANG=GW_RUS;
char VER[]="1.0",COP[]="60";
char * pfname="stardict";

#define PROTECTED
// #define _PRINT_ALLOWED --> � thart.h
char MainPath[MAXPATH], Path[MAXPATH];

const char *ExeFile="STARDICT.EXE";
const char *CfgFile="STARDICT.CFG";
const char *IndexFile = "index.dat";
const char *DataFile  = "data.dat";
const char *ThemesFile= "themes.dat";
char *ZastFile="stardict.zst";
const char *HelpFile="stardict.hlp";

const ESC_IN_MAIN_MENU = -1;
const QUIT_IN_MAIN_MENU = -2;

int MainMenuLeaveMode = QUIT_IN_MAIN_MENU;
//******************************************************************
//����� ��� ��ᬮ�� ��⥩ � ⥬
//堭����� 䠩���
int themes, index, data;

int OpenDataFiles( void );
int InitViewer( void );
int UserChooseArt( void );
int UserChooseTheme( void );
int UserChooseFromHistory( void );
int UserPrintArt( void );
int UserPrintArtToFile( void );

void ActivateViewer( );
//int test();

//�����⥫� �� 䠩���� ������ � ����묨
BufRead* pIndex;
BufRead* pData;
BufRead* pThemes;
//��ᬮ��騪

struct ViewerColors VColors={defClrText, defClrBack,defClrRef,
			     defClrActRef, defClrBackActRef, defClrBord};
#ifdef _HISTORY
extern TViewerHistory Hist;
#endif

TGraphView* pViewer=NULL;
//************************************************************************
/*int test()
{
  return 1;
} */
//#define MEM_DEBUG

void main(int argc, char *argv[])
{
#ifdef MEM_DEBUG
unsigned long mem;
mem=farcoreleft();
printf("\nBEFORE WE BEGIN  %lu",mem);
getch();
#endif

#ifdef PROTECTED
Start();
#endif

char *s;

strcpy(MainPath,argv[0]);
s=strstr(MainPath,ExeFile);
if (s==NULL) return; else *s=0;
strcpy(BGIPath,MainPath);
CheckFiles();
OpenDataFiles();
Ropen(GM_EGA);
if (argc!=2 || strcmp(argv[1],"-z")!=0 && strcmp(argv[1],"-Z")!=0 ) zast();

Rmouse_set();

LoadConfiguration();

HInitHelp(GetPath(HelpFile),260,200,30,35,10,BLACK,CYAN,RED,WHITE,BLUE,BLUE,RED);

ClearScreen();
//StopIfWrongVersion();
//WarningIfOldDate();
menu();
InitViewer();
ActivateMenu(mh0);
}


void menu()
{
SetSaveBufferSize(15,30);

InitMenu(&mh0,0,0,'H',LANG,SINGLE_BORD,0,0,0,0,138,1000,
	 GROUNDCOLOR,GROUNDBORDCOLOR,BORDCOLOR,BARCOLOR,HIGHBARCOLOR,
	 HIDEBARCOLOR,TEXTCOLOR,HIGHTEXTCOLOR,HIDETEXTCOLOR,
	 HIDEHIGHTEXTCOLOR,HIGHLETCOLOR);

SetExitKey(mh0,0);

InitMenu(&mh1,200,50,'V',LANG,SINGLE_BORD,0,0,0,0,12,1000,
	 GROUNDCOLOR,GROUNDBORDCOLOR,BORDCOLOR,BARCOLOR,HIGHBARCOLOR,
	 HIDEBARCOLOR,TEXTCOLOR,HIGHTEXTCOLOR,HIDETEXTCOLOR,
	 HIDEHIGHTEXTCOLOR,HIGHLETCOLOR);


InitMenu(&mh4,450,50,'V',LANG,SINGLE_BORD,0,0,0,0,12,1000,
	 GROUNDCOLOR,GROUNDBORDCOLOR,BORDCOLOR,BARCOLOR,HIGHBARCOLOR,
	 HIDEBARCOLOR,TEXTCOLOR,HIGHTEXTCOLOR,HIDETEXTCOLOR,
	 HIDEHIGHTEXTCOLOR,HIGHLETCOLOR);

InitMenu(&mh5,20,50,'V',LANG,SINGLE_BORD,0,0,0,0,12,1000,
	 GROUNDCOLOR,GROUNDBORDCOLOR,BORDCOLOR,BARCOLOR,HIGHBARCOLOR,
	 HIDEBARCOLOR,TEXTCOLOR,HIGHTEXTCOLOR,HIDETEXTCOLOR,
	 HIDEHIGHTEXTCOLOR,HIGHLETCOLOR);

InitMenu(&mh6,500,130,'V',LANG,SINGLE_BORD,0,0,0,0,12,1000,
	 GROUNDCOLOR,GROUNDBORDCOLOR,BORDCOLOR,BARCOLOR,HIGHBARCOLOR,
	 HIDEBARCOLOR,TEXTCOLOR,HIGHTEXTCOLOR,HIDETEXTCOLOR,
	 HIDEHIGHTEXTCOLOR,HIGHLETCOLOR);

#ifdef _PRINT_ALLOWED
InitMenu(&mh7,250,180,'V',LANG,SINGLE_BORD,0,0,0,0,12,1000,
	 GROUNDCOLOR,GROUNDBORDCOLOR,BORDCOLOR,BARCOLOR,HIGHBARCOLOR,
	 HIDEBARCOLOR,TEXTCOLOR,HIGHTEXTCOLOR,HIDETEXTCOLOR,
	 HIDEHIGHTEXTCOLOR,HIGHLETCOLOR);
#endif


SetStatusColor(mh0,LIGHTCYAN,DARKGRAY);
InitMenuItem(mh0," "," + ","",0,0,0,mh5,0,2);
AddStatus(mh0,0," "," ���p���� � �p��p���� � � ���ﭨ� ��⥬�","");
InitMenuItem(mh0," "," ������ ","",0,'�',0,mh1,0,22);
AddStatus(mh0,1," "," ������ ��⥩ � ⥬���᪨� ࠧ���� ","");
InitMenuItem(mh0," "," ������������ ","",0,'�',0,mh4,0,8);
AddStatus(mh0,2," "," ��⠭���� ��p����p�� ��� p����� �ணࠬ�� ","");
//InitFuncItem(mh0," "," ���� ","",0,'�',0,UserChooseArt,0,1,999);
//AddStatus(mh0,3," "," ���� ��� �����㦥��� �訡�� � �ணࠬ�� ","");
InitFuncItem(mh0," "," ����� ","",0,'�',0,LeaveMainMenu,0,1,10);
AddStatus(mh0,3," "," ��室 �� �p��p���� ","");

SetStatusColor(mh1,LIGHTCYAN,DARKGRAY);            //!!!!!!!!!!!!!!!!
InitFuncItem(mh1," "," ������ ������ ","",0,'�',0,UserChooseArt,0,2,5);
AddStatus(mh1,0," "," ���᮪ ��⥩ �� ��䠢��� ","");
InitFuncItem(mh1," "," ������ ��� ","",0,'�',0,UserChooseTheme,0,2,6);
AddStatus(mh1,1," "," ���᮪ ⥬ �� ��䠢��� ","");
InitFuncItem(mh1," "," ������� ","",0,'�',0,UserChooseFromHistory,0,2,7);
AddStatus(mh1,2," "," �롮� ����� �� ࠭�� ��ᬮ�७��� ��⥩ ","");
HideItem(mh1, 2); // ����� ���� ����
#ifdef _PRINT_ALLOWED
InitMenuItem(mh1," PRINT... "," ������.. ","",'P','�',0,mh7,0,0);
AddStatus(mh1,3," "," ����� ⥪�� ���� ","");
HideItem(mh1, 3); // ����� �� �� ��࠭�
#endif

SetStatusColor(mh4,LIGHTCYAN,DARKGRAY);
InitSwitchItem(mh4," MOUSE     "," ����          ","",'M','�',0,&MouseIs,0,9);
AddStringToItem(mh4,"       OFF ","     ��� "," ");
AddLastStringToItem(mh4,"        ON ","     ���� "," ");
AddStatus(mh4,0," "," ����祭��/�몫�祭�� ��������� ���� ","");
InitMenuItem(mh4," COLORS... "," �����.. ","",'C','�',0,mh6,0,11);
AddStatus(mh4,1," "," ����ன�� 梥⮢ ��� ��ᬮ�� ��⥩ ","");


SetStatusColor(mh5,LIGHTCYAN,DARKGRAY);
InitFuncItem(mh5," ABOUT PROGRAM "," � ��������� ","",'P','�',0,AboutProgram,0,2,3);
AddStatus(mh5,0," "," ������� �᭮���� ���ଠ�� � �ணࠬ�� ","");
InitFuncItem(mh5," SYSTEM INFO "," ��������� ���������� ","",'S','�',0,AboutSystem,0,2,4);
AddStatus(mh5,1," "," ������� �᭮���� ���ଠ�� � ���ﭨ� ��⥬� ","");

SetStatusColor(mh6,LIGHTCYAN,DARKGRAY);
InitSwitchItem(mh6,""," ��� ������           ","",0,'�',0,&(VColors.ClrBack),0,12);
AddColorStrings(mh6);
AddStatus(mh6,0," "," ��⠭���� 䮭� ���� ","");
InitSwitchItem(mh6,""," ����� ������         ","",0,'�',0,&(VColors.ClrText),0,13);
AddColorStrings(mh6);
AddStatus(mh6,1," "," ��⠭���� 梥� ⥪�� ���� ","");
InitSwitchItem(mh6,""," ����� ������         ","",0,'�',0,&(VColors.ClrRef),0,14);
AddColorStrings(mh6);
AddStatus(mh6,2," "," ��⠭���� 梥� ⥪�� ��뫪� ","");
InitSwitchItem(mh6,""," ��� ������� ������   ","",0,'�',0,&(VColors.ClrBackActRef),0,15);
AddColorStrings(mh6);
AddStatus(mh6,3," "," ��⠭���� 䮭� ⥪�饩 ��뫪� ","");
InitSwitchItem(mh6,""," ����� ������� ������ ","",0,'�',0,&(VColors.ClrActRef),0,16);
AddColorStrings(mh6);
AddStatus(mh6,4," "," ��⠭���� 梥� ⥪�� ⥪�饩 ��뫪� ","");
InitSwitchItem(mh6,""," �����                ","",0,'�',0,&(VColors.ClrBord),0,23);
AddColorStrings(mh6);
AddStatus(mh6,5," "," ��⠭���� 梥� ࠬ�� ","");
InitFuncItem(mh6," "," �������� ����� ","",0,'�',0,SetDefaultColors,0,2,24);
AddStatus(mh6,6," "," ����⠭����� ��室�� 梥� ","");

#ifdef _PRINT_ALLOWED
SetStatusColor(mh7,LIGHTCYAN,DARKGRAY);
InitFuncItem(mh7," PRINT "," ������ ","",'P','�',0,UserPrintArt,0,2,0);
AddStatus(mh7,0," "," �뢮� ⥪�� ���� �� �ਭ�� ","");
InitFuncItem(mh7," PRINT TO FILE "," ������ � ���� ","",'F','�',0,UserPrintArtToFile,0,2,0);
AddStatus(mh7,1," "," �뢮� ⥪�� ���� � ASCII 䠩� ","");

//InitFilenameItem(mh7, "FILE NAME","��� �����","",'N','�',0, "*.txt", pfname, GM_NOEXTENSION, 0, 0);
InitStringItem(mh7, " FILE NAME: "," ��� �����: ","",'N','�',0,pfname,8,TEXT,0,0);
AddStatus(mh7,2," "," �������� 䠩�� ","");
#endif

}


int LeaveMainMenu()
{
    if( MainMenuLeaveMode == QUIT_IN_MAIN_MENU )
       {   finish();  return 0; }
    if( MainMenuLeaveMode == ESC_IN_MAIN_MENU )
       {
	   MainMenuLeaveMode = QUIT_IN_MAIN_MENU;
	   ActivateViewer();
	   return 1;
       }
    return 1;
}

void FinishAll()
{
ClearScreen();
delete pViewer;

DestroyAllMenus();
HCloseHelp();

Rclose();
delete pIndex;
delete pData;
delete pThemes;
close( themes );
close( index );
close( data );
#ifdef MEM_DEBUG
unsigned long mem=farcoreleft();
printf("\nAFTER ALL %lu",mem);
#endif
}


int finish(void)
{
int r;

r=Box(B_TWO_BUTTON,B_MESSAGE,"�� �����H�, ��� ������",
			   "����H��� ���������?");
if (r==1)
   {
//   GMmenu.Operate(0);
   return 0;
   }
/*
if (IfConfigurationSaved()==NO)
   {
   r=Box(B_TWO_BUTTON,B_MESSAGE,"��H��������� H� ���� �����H�H�.",
				"������� �� �� �����H��� ��?");
   if (r==0) SaveConfiguration();
   }
*/
SaveConfiguration();
FinishAll();
textmode(LASTMODE);
exit(1);
return(0);
}

int stop(void)
{
textmode(LASTMODE);
exit(0);
return(0);
}
const int QFILES=9;

void CheckFiles(void)
{
struct ffblk ff;
int i,r,y=1,err=0;
char *s[30];
char path[MAXPATH];
unsigned long mem;

clrscr();
textattr(142);
gotoxy(1,1);
cprintf("Loading...");
textattr(LIGHTRED);

s[0]="STARDICT.EXE";
s[1]="STARDICT.CFG";
s[2]="STARDICT.ZST";
s[3]="STARDICT.HLP";
s[4]="EGAVGA.BGI";
s[5]="TRIP.CHR";
s[6]="DATA.DAT";
s[7]="THEMES.DAT";
s[8]="INDEX.DAT";

for (i=0;i<QFILES;i++)
   {
   strcpy(path,MainPath);
   strcat(path,s[i]);
   r=findfirst(path,&ff,FA_ARCH);
   if (r!=0)
      {
      err=1;
      if (y<25) gotoxy(1,y); else gotoxy(1,25);
      cprintf("%s  %13s.","��������� �������� ����� 䠩�",s[i]);
      y++;
      getch();
      }
   }
if (err==1)
   {
   if (y<25) gotoxy(1,y); else gotoxy(1,25);
   cprintf("%s","�p��p���� ����p襭�.");
   exit(2);
   }

if (diskspace(MainPath[0]-64)<10000 /* ??? && diskspace(MainPath[0]-64)!=-1 */
    || diskspace(_getdrive())<10000 )
   {
   if (y<25) gotoxy(1,y); else gotoxy(1,25);
   cprintf("%s"," ���� ���� �� ��᪥. �������� ᡮ�. �த������ (Y/N) ? ");
   y++;
   if (ftoupper(getche())!='Y') goto mend;
   }


mem=farcoreleft();

if (mem<270000l)
   {
   if (y<25) gotoxy(1,y); else gotoxy(1,25);
   cprintf("%s"," ���� ����⨢��� �����. �������� ᡮ�. �த������ (Y/N) ? ");
   y++;
   if (ftoupper(getche())!='Y') goto mend;
   }

textcolor(WHITE);
return;

mend:
textcolor(WHITE);
exit(1);
}

//*******************************************************************
int OpenDataFiles( void )
 {
   _fmode = O_BINARY;

   themes = open( GetPath(ThemesFile), O_RDWR | O_BINARY );
   index = open( GetPath(IndexFile), O_RDWR | O_BINARY );
   data = open( GetPath(DataFile), O_RDWR | O_BINARY );

   if( index == -1  )
     {   Rmessage( "�訡�� �� ����⨨ 䠩�� index.dat", LIGHTRED,CYAN,BLACK );
	 stop(); }
   if(  themes==-1  )
     {   Rmessage( "�訡�� �� ����⨨ 䠩�� themes.dat", LIGHTRED,CYAN,BLACK );
	 stop();; }
   if(  data==-1  )
     {   Rmessage( "�訡�� �� ����⨨ 䠩�� data.dat", LIGHTRED,CYAN,BLACK );
	 stop();; }

   pIndex = new BufRead;
   pData = new BufRead;
   pThemes = new BufRead;

   pIndex->SetFileHandler( index );
   pData->SetFileHandler( data );
   pThemes->SetFileHandler( themes );

   return 1;
}

int InitViewer( void )
{
  pViewer = NULL;
  pViewer = new TGraphView( pIndex, pData, pThemes, EGA, 14, 2, 2, 1, 3, OFF );
  if( pViewer == NULL )
      gerrmes(NOMEM,"InitViewer",0,SYSTEM_ERROR);

  return OK;
}

void ActivateViewer( )
{
    int thnum;
    int artnum;
    ClearScreen();
    Rmouse_hide();
    pViewer->SetMouseMode( MouseIs );
    if( pViewer->artnum == 0  )  return;  // �᫨ ��� ���� �� viewer'e

    while( (thnum=pViewer->view() ) != 0 )
     {
       #ifdef _HISTORY
       Hist.Clear();
       #endif
       if( (artnum = ShowTheme( pIndex, pThemes, thnum )) != 0 )
	   pViewer->ChangeArt( artnum );
     }
    #ifdef _HISTORY
    if( Hist.hnum != 0 )  RebirthItem(mh1, 2);  //����� �� ����
    #endif

    #ifdef _PRINT_ALLOWED
    RebirthItem(mh1, 3); // ����� ࠧ�襭�
    #endif
}

int UserChooseArt( void )
{
   int artnum;
   pViewer->SetMouseMode( MouseIs );
   if( (artnum = pViewer->ChooseArt()) == 0 )    // �⪠� �� ��ᬮ��
    {
      if( pViewer->artnum == 0 ) // �᫨ ��� ����
	  ClearScreen();
      else
	  pViewer->show();
      return NOK;
    }
   pViewer->ChangeArt( artnum );
   ActivateViewer();
   return OK;
}

int UserChooseTheme( void )
{
   int thnum, artnum;
   pViewer->SetMouseMode( MouseIs );
   if( (thnum = ChooseTheme( pThemes)) == 0 )  // �⪠� �� ��ᬮ��
    {
      if( pViewer->artnum == 0 ) // �᫨ ��� ����
	  ClearScreen();
      else
	  pViewer->show();
      return NOK;
    }
   if( (artnum=ShowTheme( pIndex, pThemes, thnum)) == 0 )
    {
      if( pViewer->artnum == 0 ) // �᫨ ��� ����
	  ClearScreen();
      else
	  pViewer->show();
      return NOK;
    }
   pViewer->ChangeArt( artnum );
   ActivateViewer();
   return OK;
}

int UserChooseFromHistory( void )
{
   int artnum;
   pViewer->SetMouseMode( MouseIs );
   if( (artnum = pViewer->ChooseArtFromHistory()) == 0 )    // �⪠� �� ��ᬮ��
    {
      if( pViewer->artnum == 0 ) // �᫨ ��� ����
	  ClearScreen();
      else
	  pViewer->show();
      return NOK;
    }
   pViewer->ChangeArt( artnum );
   ActivateViewer();
   return OK;
}

#ifdef _PRINT_ALLOWED
int UserPrintArt( void )
{
if (PreparePrinter()==-1) return NOK;
   if( pViewer->PrintArt() != OK )
     Rmessage( "�訡�� �� �뢮�� ���� �� �ਭ�� !", LIGHTRED,CYAN,BLACK );

   ActivateViewer();
   return OK;
}

int UserPrintArtToFile( void )
{
   if( pViewer->PrintArtToFile(pfname) != OK )
     Rmessage( "�訡�� �� ���� ���� � 䠩� !", LIGHTRED,CYAN,BLACK );
   ActivateViewer();
   return OK;
}

#endif

//**********************************************************************

void ClearScreen(void)
{
RbarcM(0,0,1023,767,WIDE_DOT_FILL,LIGHTBLUE);
RbarcM(30,20,994,749,CLOSE_DOT_FILL,BLUE);
}


int AboutProgram(void)
{
char str[80],*s[15];
int i,mx,my;

ClearScreen();
RbarcM(100,150,920,620,SOLID_FILL,BLUE);
RramcM(106,156,914,614,LIGHTGRAY);
RramcM(112,162,908,608,LIGHTGRAY);
RInitFkey(850,640,10," ESC ",ESC,RED,LIGHTGRAY,DARKGRAY);

sprintf(str,"  (C) 1996 F.Rozhanski     %s  %s    %s  %s",
       stmes(13),VER,stmes(14),COP);

if (LANG==GW_ENG)
{
s[0]="";
s[1]="";
s[2]="";
s[3]="";
s[4]="";
s[5]="";
s[6]="";
s[7]="";
s[8]="";
s[9]="";
s[10]="";
s[11]="";
s[12]="";
}
else
{
s[0]="  STARDICT - �� 㭨���쭠� �ணࠬ��,  �।�⠢����� ᮡ��";
s[1]="���஫����᪨� ᫮����-�ࠢ�筨�.  STARDICT ॠ��������  �";
s[2]="���� �����⥪�⮢�� ��⥬�, ����饩 �� ᫮����� ��⥩ �";
s[3]="��⥬�� ��४����� ��뫮�. � ������ ���� ᮤ�ন��� ��-";
s[4]="�ଠ��, ��ࠦ���� ᮢ६���� ���஫����᪨� �।�⠢��-";
s[5]="��� �� ������ ᮮ⢥�����饣� ������.  �� ⥪��� STARDICT";
s[6]="�ਣ������  � �� ����� ���� ��ᯮ����� ������묨 ࠭�� ���-";
s[7]="஫����᪨�� ᫮���ﬨ � ��横������ﬨ.                   ";
s[8]="     �ணࠬ�� ����⠭�  ��  �ப�� ��� ��,  ��쥧��";
s[9]="����������� ���஫�����.                                 ";
s[10]="";
s[11]=" ��������� ���������: ";
s[12]="  �.��⥬쥢� - ���⠢��";
s[13]="  �.��������᪨� - �ணࠬ��";
s[14]="  �.�����᪨� - ⥪���";
}

for (i=0;i<15;i++) RtextcM(120,200+i*27,s[i],LIGHTCYAN);
RtextcM(130,170,str,LIGHTRED);

Revent(&mx,&my);

RRemoveFkey(ESC);
ClearScreen();
//*********************************************************
ActivateViewer();
//*********************************************************
return 0;
}



int AboutSystem(void)
{
int mx,my,col;
signed long m;
char *s,str[15],ms[15];
time_t t;
struct date dd;


ClearScreen();
Rmouse_hide();
Rbarc(150,150,870,570,SOLID_FILL,BLUE);
Rramc(156,156,864,564,LIGHTGRAY);
Rramc(162,162,858,558,LIGHTGRAY);
RInitFkey(800,590,10," ESC ",ESC,RED,LIGHTGRAY,DARKGRAY);

/* ������ */
m=farcoreleft();
Rtextc(180,200,stmes(15),LIGHTCYAN);
Rdigitc(565,200,(int)(m/1000),YELLOW);
col=LIGHTRED;
if (m<70000l) s=stmes(16);
else if (m<140000l) s=stmes(17);
else { s=stmes(18); col=WHITE; }
Rtextc(680,200,s,col);

/* ���� �� ��᪥ */
m=diskspace(MainPath[0]-64);
Rtextc(180,240,stmes(19),LIGHTCYAN);
if (m!=-1) sprintf(ms,"%9ld",m/1000); else sprintf(ms,"       ?");
Rtextc(490,240,ms,YELLOW);
col=LIGHTRED;
if (m<20000l) s=stmes(16);
else if (m<70000l) s=stmes(17);
else { s=stmes(18); col=WHITE; }
Rtextc(680,240,s,col);

/* ��� */
Rtextc(180,360,"���",LIGHTCYAN);
getdate(&dd);
sprintf(str,"%02d.%02d.%4d",dd.da_day,dd.da_mon,dd.da_year);
Rtextc(560,360,str,YELLOW);

/* �p��� */
Rtextc(180,400,"�p���",LIGHTCYAN);
t=time(NULL)-5l*3600l;
Seconds2String((long)t,str,"HH:MM:SS");
Rtextc(560,400,str,YELLOW);

Rmouse_show();
Revent(&mx,&my);

RRemoveFkey(ESC);
ClearScreen();
//*********************************************************
ActivateViewer();
//*********************************************************
return 0;
}


int LoadConfiguration()
{
int f,r;
char filename[MAXPATH];
ffblk ff;

strcpy(filename,CfgFile);
if (findfirst(filename,&ff,FA_ARCH)!=0)
   {
   strcpy(filename,MainPath);
   strcat(filename,CfgFile);
   if (findfirst(filename,&ff,FA_ARCH)!=0)
      {
      Box(B_ONE_BUTTON,B_MESSAGE,"H� ���� H���� ���� � ��H����������.",
				 "��������� ����H����H� �� ������H��");
      MouseIs=NO_MOUSE;

      return GM_OK;
      }
   }

f=_open(filename,O_RDONLY | O_BINARY);
if (f==-1) gerrmes(OPEN_ERROR,"LoadConfiguration",0,SYSTEM_ERROR);

r=read(f,&SavedMouseIs,2);
if (r!=2)
   {
   SavedMouseIs=NO_MOUSE;
   gerrmes(READ_ERROR,"LoadConfiguration",1,COMMON_ERROR);
   }
MouseIs=SavedMouseIs;

r=read(f,&PROTECT1,2);
if (r!=2)   gerrmes(READ_ERROR,"LoadConfiguration",2,COMMON_ERROR);

r=read(f,&PROTECT2,2);
if (r!=2)   gerrmes(READ_ERROR,"LoadConfiguration",3,COMMON_ERROR);

r=read(f,&VColors,sizeof(VColors));
if (r!=sizeof(VColors))
	    gerrmes(READ_ERROR,"LoadConfiguration",4,COMMON_ERROR);


return GM_OK;
}


int SaveConfiguration()
{
int f,r;
char filename[MAXPATH]="";


strcpy(filename,MainPath);
strcat(filename,CfgFile);

f=_creat(filename,FA_ARCH | O_BINARY);
if (f==-1)
   {
   gerrmes(CREAT_ERROR,"SaveConfiguration",0,COMMON_ERROR);
   return GM_NOK;
   }

r=write(f,&MouseIs,2);
if (r!=2)   gerrmes(WRITE_ERROR,"SaveConfiguration",1,COMMON_ERROR);

r=write(f,&PROTECT1,2);
if (r!=2)   gerrmes(WRITE_ERROR,"SaveConfiguration",2,COMMON_ERROR);

r=write(f,&PROTECT2,2);
if (r!=2)   gerrmes(WRITE_ERROR,"SaveConfiguration",3,COMMON_ERROR);

r=write(f,&VColors,sizeof(VColors));
if (r!=sizeof(VColors))
	    gerrmes(WRITE_ERROR,"SaveConfiguration",4,COMMON_ERROR);

return GM_OK;
}

/*
int IfConfigurationSaved()
{
if ( MouseIs==SavedMouseIs ) return YES;
return NO;
}
*/


void AddColorStrings(int menunum)
{
char *colorse[16]={"       BLACK","        BLUE",
		   "       GREEN","        CYAN",
		   "         RED","     MAGENTA",
		   "       BROWN","   LIGHTGRAY",
		   "    DARKGRAY","   LIGHTBLUE",
		   "  LIGHTGREEN","   LIGHTCYAN",
		   "    LIGHTRED","LIGHTMAGENTA",
		   "      YELLOW","       WHITE"},

     *colorsr[16]={"     ������","      �����",
		   "    �������","       ����",
		   "    �������","   ��������",
		   " ����������","   ��.�����",
		   "�����-�����","    �������",
		   " ��.�������","    ��.����",
		   " ��.�������","��.��������",
		   "     ������","      �����"};

AddStringToItem(menunum,colorse[0],colorsr[0]," ");
AddStringToItem(menunum,colorse[1],colorsr[1]," ");
AddStringToItem(menunum,colorse[2],colorsr[2]," ");
AddStringToItem(menunum,colorse[3],colorsr[3]," ");
AddStringToItem(menunum,colorse[4],colorsr[4]," ");
AddStringToItem(menunum,colorse[5],colorsr[5]," ");
AddStringToItem(menunum,colorse[6],colorsr[6]," ");
AddStringToItem(menunum,colorse[7],colorsr[7]," ");
AddStringToItem(menunum,colorse[8],colorsr[8]," ");
AddStringToItem(menunum,colorse[9],colorsr[9]," ");
AddStringToItem(menunum,colorse[10],colorsr[10]," ");
AddStringToItem(menunum,colorse[11],colorsr[11]," ");
AddStringToItem(menunum,colorse[12],colorsr[12]," ");
AddStringToItem(menunum,colorse[13],colorsr[13]," ");
AddStringToItem(menunum,colorse[14],colorsr[14]," ");
AddLastStringToItem(menunum,colorse[15],colorsr[15]," ");
}


char *stmes(int n)
{
if (LANG==GW_RUS)
   {
   switch(n)
      {
      case 13: return "�����";
      case 14: return "����� N";
      case 15: return "��������� ������                   ��";
      case 16: return "(���� !!!)";
      case 17: return "(�� ����� !)";
      case 18: return "(����������)";
      case 19: return "��������� ����� �� �����           K�";

      /*** MAINMENU *****/
//      case 29: return " �� ���� ���� 䠩� ";
//      case 30: return " ��������� ���������. ";
//      case 31: return " ������ <ENTER>... ";
//      case 32: return " ���� ���� �� ��᪥. �������� ᡮ�. �த������ (Y/N) ? ";
//      case 33: return " ���� ����⨢��� �����. �������� ᡮ�. �த������ (Y/N) ? ";



      case 50: return "���誮� ����让 䠩�.";
      default: return "";
      }
   }
else
   {
   switch(n)
      {
      /*** MAINMENU *****/
      case 14: return "Copy #";
      case 15: return "AVAILABLE MEMORY                   Kb";
      case 16: return "(NOT ENOUGH)";
      case 17: return "(NOT MUCH !)";
      case 18: return "(ENOUGH)";
      case 19: return "FREE SPACE ON DRIVE                Kb";

      /*** MAINMENU *****/
//      case 29: return " Can't find file ";
//      case 30: return " PROGRAM TERMINATED. ";
//      case 31: return " Press any key... ";
//      case 32: return " Not enough free space on drive. Try anyway (Y/N) ? ";
//      case 33: return " Not enough memory. Try anyway (Y/N) ? ";

      case 50: return "File is too big.";
      default: return "";
      }
   }
}


int dummy(void)
{
return 0;
}

void  WarningIfOldDate(void)
{
   struct date current;
   getdate( &current );
   long moment = (long)current.da_year*12 + (long)current.da_mon;
   long delta = moment - BEGIN_DATE;
   if( delta > MAX_DELTA_DATE )
	Box(B_ONE_BUTTON,B_MESSAGE,
            " ��������� ������������! ���� ������",
            " ���������  ����������  ��������. ",
            " �������� ��� ���������� �� �������-",
            " ���� � ����� ������� � �������������.");
}

void  StopIfWrongVersion(void)
{
   int v = GetVersion();
   if(  v > MAX_VERSION || v < MIN_VERSION )
    { Box(B_ONE_BUTTON,B_MESSAGE,
       " �������� ������ ���������.",
       " �������� ���������� �",
       " �������������.");
      stop();
    }
}

int GetVersion()
{
   FILE* tmp = fopen( GetPath(DataFile), "r" );
   if( tmp == NULL )
     {   Rmessage( "�訡�� �� ����⨨ 䠩�� data.dat", LIGHTRED,CYAN,BLACK );
	 stop(); }
   int ver = fgetc( tmp );
   fclose( tmp );
   return ver;
}

int SetDefaultColors()
{
      int r=Box(B_TWO_BUTTON,B_MESSAGE,"   �� �����H�, ��� ������",
				       "������������ �������� �����?");
      if (r==1)  	return 0;

      VColors.ClrText       =  defClrText;
      VColors.ClrBack       =  defClrBack;
      VColors.ClrRef        =  defClrRef;
      VColors.ClrActRef     =  defClrActRef;
      VColors.ClrBackActRef =  defClrBackActRef;
      VColors.ClrBord       =  defClrBord;
      SwitchReset(mh6,0);
      SwitchReset(mh6,1);
      SwitchReset(mh6,2);
      SwitchReset(mh6,3);
      SwitchReset(mh6,4);
      SwitchReset(mh6,5);
      return OK;
}


void zast(void)
{
int i,l=19,ll=940,t=0;
int col1=LIGHTBLUE,col2=LIGHTGREEN,interv=170,shift=4;

setEGA();
PCXWrite(GetPath(ZastFile));
delay(500);

settextstyle(1,HORIZ_DIR,7);
Rbarc(0,0,130,768,CLOSE_DOT_FILL,BLUE);
Rbarc(895,0,1023,768,CLOSE_DOT_FILL,BLUE);

Rtextc(l+15,t+shift,"S",col1);
Rtextc(l+15,t+shift+interv,"T",col1);
Rtextc(l+15,t+shift+interv*2,"A",col1);
Rtextc(l+15,t+shift+interv*3,"R",col1);
Rtextc(l+10,t+0,"S",col2);
Rtextc(l+10,t+interv,"T",col2);
Rtextc(l+10,t+interv*2,"A",col2);
Rtextc(l+10,t+interv*3,"R",col2);

Rtextc(ll,t+shift,"D",col1);
Rtextc(ll+15,t+shift+interv,"I",col1);
Rtextc(ll,t+shift+interv*2,"C",col1);
Rtextc(ll,t+shift+interv*3,"T",col1);
Rtextc(ll-5,t,"D",col2);
Rtextc(ll-5+15,t+interv,"I",col2);
Rtextc(ll-5,t+interv*2,"C",col2);
Rtextc(ll-5,t+interv*3,"T",col2);

settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
Rtextc(12,712,"VERSION",BLUE);
Rtextc(10,710,"VERSION",CYAN);
Rtextc(35,742,VER,BLUE);
Rtextc(32,740,VER,CYAN);

Rtextc(910,712,"COPY  N",BLUE);
Rtextc(908,710,"COPY  N",CYAN);
Rtextc(950,742,COP,BLUE);
Rtextc(947,740,COP,CYAN);

for (i=0;i<30;i++)
   {
   delay(100);
   if (kbhit()!=0) { getsym(); return; }
   }
}



char *GetPath(const char *filename)
{
strcpy(Path,MainPath);
strcat(Path,filename);
return(Path);
}