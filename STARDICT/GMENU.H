#include <dos.h>
#include <conio.h>

#define  MAX_MENUS   20     /* ���ᨬ��쭮� ������⢮ ���� */
#define  MAX_ITEMS   20     /* ���ᨬ��쭮� ������⢮ ITEM�� */
#define  MAX_STRINGS 20     /* ����. ���-�� ��ਠ�⮢ �� ��४��祭��
							      SWITH-ITEMa*/
#define  DESTROYED_MENU  0  /* �ਧ��� ��ᮧ������� ���� */
#define  LASTDRIVE   'C'    /* ���ᨬ���� �ࠩ� ��� ��࠭���� */

/* ���� �������� */
#define NO_BORD       0
#define SINGLE_BORD   1
#define DOUBLE_BORD   2
#define REDUCED_BORD  3

/* ���� ������ */
#define GM_NOK       -1
#define GM_OK         0
#define GM_OVERFLOW  -2
#define GM_NOT_MEM   -3
#define GM_ESC       -2

/*����� ���������� ���� */
#define GM_SAVE       0
#define GM_NOSAVE     1

/* ���� ITEM�� */
#define  MENU_ITEM      0
#define  FUNC_ITEM      1
#define  YES_NO_ITEM    2
#define  SWITCH_ITEM    3
#define  STRING_ITEM    4
#define  FILENAME_ITEM  5
#define  FUNCKEY_ITEM   6  /* CPP */

/* �������� YESNOHANDLER� */
#define  NO             0
#define  YES            1

/* �������� FILEMASK */
#define  GM_EXTENSION    0
#define  GM_NOEXTENSION  1

/* ���� �������� ����� */
#define TEXT             0
#define DIGIT_INT        1
#define DIGIT_REAL       2

/* ��������� ������-������ */
#define  STAT_HEIGHT 1.5  /* ���� ����� ��ப� �� �⭮襭�� � LINE_HEIGHT */
#define  STAT_LEFT          0
#define  STAT_TOP           769-(int)(LINE_HEIGHT*STAT_HEIGHT)
#define  STAT_RIGHT         1024
#define  STAT_BOTTOM        769
#define  STAT_NOTINIT       -1      /* �� ���樠����஢��� */
#define  STAT_NOTACTIVE      0      /* �� ��⨢�� */
#define  STAT_ACTIVE         1      /* ��⨢�� */


struct STRINGS
   {
   char *str0,*str1,*str2;
   };

struct GMENU
   {
   int    top, left, right, bottom;   /* ���न���� */
   int    qitems;                     /* ���-�� ITEM�� � ���� */
   struct GITEM *GItems[MAX_ITEMS];
   char   menutype;   /* ⨯ ���� - ���⨪��쭮� ��� ��ਧ��⠫쭮� : V or H */
   int    bordtype;   /* ⨯ ����� - �. define */
   char   *bordname0, *bordname1, *bordname2, *bordname;
		      /* ��������� �� ����� */
   int    lang;       /* �� - 0,1,2 */

		   /* ����� */
   int    groundcolor, groundbordcolor, bordcolor,
				  /* 䮭 ����, 䮭 �����, 梥� ����� */
	  barcolor, highbarcolor, /* 䮭 ITEM� � 䮭 �뤥������� ITEM� */
	  textcolor, hightextcolor, /* 梥� �뢮������ ⥪�� - �뤥�������
				       � ���뤥������� */
	  highletcolor, /* 梥� �뤥������ �㪢� */
	  hidebarcolor, hidetextcolor, /* 䮭 � ⥪�� ���⠭���� ITEM� */
	  hidehightextcolor,  /* ⥪�� ���⠭���� �뤥������� ITEM� */
	  str_color,str_groundcolor,str_curscolor;
	      /* ⮫쪮 ��� STRING_ITEM  - 梥� ।����㥬�� ��ப� */

   int    otstup, interval; /* ����� �� ��� � ���ࢠ� ����� ITEM��� */
   int    freq;             /* ���� ��㪠 */
   int    curitem;          /* ⥪�騩 ITEM */
   int    menucall;         /* nandler �� �맢��饥 ���� (��� -1) */
   signed long  savepos,    /* 0 - ... ��� ����� (ᬥ饭�� �� �⭮襭�� �
			       savebuf � �����);
			  -1,-2 etc. - ����� 䠩�� �� ��࠭���� �� ��᪥ */
		savesize;    /* ࠧ��� � ����� */
   int	  savehandler;       /* �᫨ �� ��࠭﫮�� == 0; ���� 1 */
   int	  statcolor,statgroundcolor; /* 梥� ⥪�� � 䮭� ����� ��ப� */
   int    exitkey;          /* ������ ��室� �� ����, �� 㬮�砭�� - ESC,
			       0 - �� ������⢨� ��室� */
   };

struct GITEM
   {
   int itemtype;                 /* ⨯ ITEM� - �. define */
   char hideitem;                 /* 'H' ��� 'N' - ���⠭�� ��� ��� ITEM */
   char *str0,*str1,*str2,*str;  /* ⥪�� ITEM� */
   int length;                   /* ����� ��ப� */
   int hlet0,hlet1,hlet2,hlet;   /* �뤥����� �㪢� (! ��।����� �㪢�,
				    �� �࠭����� ����� �� ��砫� str)*/
   char *statstr0,*statstr1,*statstr2,*statstr;  /* ⥪�� ����� ��ப�
				    statstr==NULL, �᫨ �����-��ப� ��� */
   int menuhandler;              /* � MENU - handler MENU
				    � SWITCH - ������⢮ ��ப
				    � STRING - ⨯ ��ப�
				    � FILENAME - ⨯ ����� 䠩��: � ���-
				      ७��� ��� ��� */
				 /* � FUNCKEY - �㭪樮���쭠� ������,
				      �� ���p�� p����p�� item */
   int *mainhandler;              /* � YESNO - handler YES/NO */
				  /* � SWITCH - ⥪��� ��ப� */
   int (* funchandler)();         /* � FUNC � MENU - �㭪�� */
   char *string;                  /* � STRING - ��ப�,
				     � FILENAME - ��� 䠩�� */
   char *mask;                    /* � FILENAME - 蠡��� ��� ���᪠ 䠩�� */
   int menuclose;                 /* ᪮�쪮 ���� ����뢠�� ��᫥ �믮������
				     ITEM�  (� MENU �� ࠡ�⠥�) */
   int menurepaint;
	     /* � FUNC - ᪮�쪮 ���� ������ �� �६�  �믮������ �㭪樨
		� SWITCH - 0 - OK; -1 - �� �뫮 ��⠭���� ��᫥���� ��ப� */
   int strlength;     /* � SWITCH � STRING - ���ᨬ��쭠� ����� ��ப� ᢨ�� */
   int helpitem;              /* ����� HELP� �� item (-1 �� ������⢨�) */
   struct STRINGS strings[MAX_STRINGS];  /* � SWITCH - 㪠��⥫� �� ��ப� */
   };

/************************ ������� ������� ************************/
int InitMenu(int *menuhandler,
	     int left, int top, char menutype, int lang,
	     int bordtype, char *bordname0, char *bordname1, char *bordname2,
	     int otstup, int interval, int freq,
	     int groundcolor, int groundbordcolor, int bordcolor,
	     int barcolor, int highbarcolor, int hidebarcolor,
	     int textcolor, int hightextcolor, int hidetextcolor,
	     int hidehightextcolor, int highletcolor);
  /* ���樫������ ����, �뤥��� ������
     ������ ���� ��ࢮ� �㭪樥� ����譨�� (�᫨ ⮫쪮 �� �ᯮ������
	SetSaveBufferSize - ⮣�� ��� ��ࢠ�)
     ������ - GM_OK ��� ��� �訡�� */

void DestroyMenu(int menuhandler);
  /* 㭨�⮦��� ����, �᢮������� ������
     �ᯮ������ � DestroyAllMenus */
void DestroyAllMenus(void);
  /* 㭨�⮦��� �� ����, �᢮������� ������
     ! ������ ���� ��᫥���� �㭪樥� ����譨�� */

int InitMenuItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int newmenuhandler, int (* funchandler)(), int helpitem);
  /* ���樠������ MENU ITEM�
     newmwnuhandler - handler ��뢠����� ����,
     funchandler - ���� �㭪樨, �믮��塞�� ��। �맮��� ����
     ������ - handler ITEM� ��� ��� �訡�� */

int InitYesNoItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int *yesnohandler, int menuclose, int helpitem);
  /* ���樠������ YES_NO ITEM�
     yesnohandler - 㪠��⥫� �� ��६�����, ������� ���祭�� YES ��� NO
		    (�. define ), � ��� �� ����頥��� १����;
     menuclose - ᪮�쪮 �஢��� ���� ����뢠�� ��᫥ �믮������ ITEM�;
     ������ - handler ITEM� ��� ��� �訡�� */

int InitSwitchItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int *curstr, int menuclose, int helpitem);
  /* ���樠������ SWITCH ITEM�
     yesnohandler - 㪠��⥫� �� ����� ⥪�饩 ��ப� ᢨ��
     menuclose - ᪮�쪮 �஢��� ���� ����뢠�� ��᫥ �믮������ ITEM�;
     ������ - handler ITEM� ��� ��� �訡�� */

int InitFuncItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int (* funchandler)(),
		 int menuclose, int menurepaint, int helpitem);
  /* ���樠������ FUNC ITEM�
     funchandler - ���� ��뢠���� �㭪樨
     menuclose - ᪮�쪮 �஢��� ���� ����뢠�� ��᫥ �믮������ ITEM�;
     menurepaint - ᪮�쪮 ���� ������ ��। �믮������� �㭪樨,
		   ����⠭������� ��᫥ �� �����襭��
     ������ - handler ITEM� ��� ��� �訡�� */
           	
int InitStringItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 char *string, int strlength, int strtype,
		 int menuclose, int helpitem);
  /* ���樠������ STRING ITEM�
     ! ࠧ�襭 ⮫쪮 � ���⨪����� ����;
     string - 㪠��⥫� �� �������� ��ப�
     strlength - ���ᨬ��쭠� ����� �������� ��ப�
     strtype - ⨯ �������� ��ப� (�.define)
     menuclose - ᪮�쪮 �஢��� ���� ����뢠�� ��᫥ �믮������ ITEM�;
     helpitem - ����� HELP� �� item (-1 �� ������⢨�)
     ������ - handler ITEM� ��� ��� �訡�� */

int InitFilenameItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 char *mask, char *filename, int filemask,
		 int menuclose, int helpitem);
  /* ���樠������ FUNC ITEM�
     mask - 蠡��� ��� ���᪠ (���ਬ��: "*.CFG");
     filename - ��� 䠩��
     filemask - �뢮���� ��� 䠩�� � ���७��� ��� ��� (�. ��䠩��)
     menuclose - ᪮�쪮 �஢��� ���� ����뢠�� ��᫥ �믮������ ITEM�;
     ������ - GM_OK ��� ��� �訡�� */


int InitFuncKeyItem(int menuhandler,
		 char *str0, char *str1, char *str2,
		 int hlet0, int hlet1, int hlet2,
		 int funckey,
		 int menuopen, /* int menurepaint,*/ int helpitem);
  /* ���樠������ FUNCKEY ITEM�
     funckey - ������, ����p�頥��� ����
     menuclose - ᪮�쪮 ���� ��⠢���� ��p��묨 ��᫥ �믮������ ITEM�
		 (-1), �᫨ ����� �� ���� ���p뢠��;
     ������ - handler ITEM� ��� ��� �訡�� */
/* ! ������ �� ०��� ���譥�� �ࠢ����� ����: define OUTER_OPERATION_MODE */


int AddStringToItem(int menuhandler, char *str0, char *str1, char *str2);
    /* �������� ��ப� ᢨ�� � SWITCH ITEM�
       ������ ��室����� �����।�⢥��� ��᫥ ���樠����樨 ITEM�
       �ᯮ������ � AddLastStringToItem
       ������ - GM_OK */

int AddLastStringToItem(int menuhandler, char *str0, char *str1, char *str2);
    /* �������� ��᫥���� �� ��ப ᢨ�� � SWITCH ITEM�
       ������ �����稢��� ᮡ�� �� AddStringToItem
       ������ - GM_OK ��� ��� �訡��
    */

void SetEnterStringColors(int menuhandler,
		     int color, int groungcolor, int curscolor);
    /* ��⠭����� � ���� 梥� ��� ����� ��ப�
       ���᫥�� ⮫쪮 �� ����稨 � ���� STRING ITEM�
       color        - 梥� ��ப�
       groundcolor  - 䮭 ��ப�
       curscolor    - 梥� ⥪�饩 �㪢�
    */

int ActivateMenu(int menuhandler);
    /* ��⨢������ � ���� ����
       �����頥� GM_OK  */

void SetSaveBufferSize(int percents, int minimum);
    /* ��⠭�������� ࠧ���� ���� ��� ��࠭���� ����
       percents - ࠧ���� ���� � ��業�� �� ᢮������ �����
       minimum  - ��������� ࠧ��� ����
       ! ������ ���� ᠬ�� ��ࢮ� �㭪樥� ����譨��
       �� 㬮�砭�� ࠧ��� ���� - 120 Kb */

int MoveMenu(int menuhandler, int left, int top);
    /* ��⠭�������� ���न���� ����
       ���ਬ����� � ��⨢���� ����
       �����頥� GR_OK ���
		  GR_NOK, �᫨ ���� 㦥 ��⨢��� */

void SetStatusColor(int menuhandler, int col, int bkcol);
   /* ��⠭�������� ��� ���� 梥� ⥪�� � 䮭� �����-��ப� */

void AddStatus(int menuhandler, int itemhandler,
	      char *str0, char *str1, char *str2);
   /* ���樠������� � ITEM� �����-��ப� */

void HideItem(int menuhandler, int itemhandler);
   /* ������ ITEM ������㯭� */

void RebirthItem(int menuhandler, int itemhandler);
   /* ������ ITEM ����㯭� */

int SwitchReset(int menuhandler, int itemhandler);
   /* �����뢠�� � SWITCH_ITEM ��ப� ᮮ⢥�������� ��६�����
   (�.�. *mainhandler )
   menuhandler, itemhandler - ����� ���� � item��
   ���ࢠ� GM_OK ��� ��� �訡��
   ����室��� �� �����।�⢥���� ��������� ��६����� SWITCH_ITEM�
	 ��� �ᯮ�짮����� ����
    */

void SetExitKey(int menuhandler, int key);
   /* �����砥� ������� ����� ESC, �� ���p�� �㤥� �����⢫�����
      ��室 �� ����   */


/*********************** ���������� ������� **************************/

int AddStringToStringItem(char *str, char *string, int strlength);
    /* ������� � �㦭�� ���� ��ப� ITEM� str �������� ��ப� string
       strlength - ���ᨬ��쭠� ����� ��ப� ᢨ��;
       ��뢠���� �� InitStringItem � ActivateMenu;
       ������ - GM_OK ��� ᮮ�饭�� �� �訡�� */

int DrawMenu(int menuhandler, int saveoption);
    /* ���� �������� ����, ��࠭�� ��� ��� ��� (�. define)
    �ᯮ������ � ActivateMenu, SaveLevels
    �����頥� GM_OK ��� ��� �訡�� */

int DrawItem(int menuhandler, int itemhandler);
    /* ���� ITEM � �������� ����,
    �ᯮ������ � DrawMenu, ActivateMenu
    �����頥� GM_OK */

int DefineItemPosition(int menuhandler, int itemhandler,
		       int *l, int *t, int *r, int *b);
    /* ��।���� ����ਭ��� ITEM� �� ��࠭�
    �ᯮ������ � DrawItem, ActivateMenu � MouseOperate
    �����頥� GM_OK */

int WhichItemPosition(int menuhandler, int x, int y);
    /* ��।���� � ����� ITEM ������� ���� �������� ���न���� x,y
       �ᯮ������ � MouseOperate
       �����頥� ����� ITEM� ��� -1 */

int DrawBordStr(int left, int top, int right, int bordylength,
		int bordgroundcolor, int bordcolor, char *str);
    /* ���� ����� ���� � �뢮��� �� ���� ��ப�
       �ᯮ������ � DrawMenu
       �����頥� GM_OK */

int GetStrSumWidth(int menuhandler, int item);
    /* ��।���� �㬬���� ����� ITEM�� � ���� (��� H-����)
       �ᯮ������ � DrawItem � DefineItemPosition
       �����頥� ����� */

int GetBordLength(int menuhandler, int *bordxlength, int *bordylength);
    /* ��⠭�������� ࠧ���� ����� �� ��ਧ��⠫� � ���⨪��� �
       ����ᨬ��� �� ��� ⨯�
       �ᯮ������ � DrawItem � DefineItemPosition
       �����頥� GM_OK */

int YesNoSet(char *str, int *yesnohandler, int lang);
    /* ������� � �㦭�� ���� ��ப� ITEM� Yes ��� No �� ᮮ⢥�����饬
       �몥 (��ࠬ��� - lang) � ᮮ⢥��⢨� � yesnohandler (�� ����� ���);
       ��뢠���� �� InitYesNoItem � ActivateMenu;
       ������ - GM_OK ��� ᮮ�饭�� �� �訡�� */

int SwitchSet(char *str, struct STRINGS *strings, int strlength, int lang);
    /* ������� � �㦭�� ���� ��ப� ITEM� str ��ப� strings ᢨ�� ��
       ᮮ⢥�����饬 �몥 (��ࠬ��� - lang);
       strlength - ���ᨬ��쭠� ����� ��ப� ᢨ��;
       ��뢠���� �� InitSwitchItem � ActivateMenu;
       ������ - GM_OK ��� ᮮ�饭�� �� �訡�� */

void SetFilename(char *str, char *path, int filemask);
    /* ������� � ����� ��ப� FILENAME_ITEM� ��� 䠩�� �� ���
       str - ��ப�
       path - ����
       filemask - �뢮���� ��� 䠩�� � ���७��� ��� ��� (�. ��䠩��)
       ��뢠���� �� InitFilenameItem � ActivateMenu;
       �� �訡�� �뤠�� ᮮ�饭�� */

int  InitSaveBuf(void);
    /* ���樠������� ���� ��� ��࠭���� ����
       �ᯮ������ � InitMenu
       �����頥� GM_OK ��� ��� �訡�� */

int  SaveMenu(signed long *savepos, long *savesize, int *savehandler,
	     int left, int top, int right, int bottom);
    /* ��࠭�� ���� � ��࠭� � ����� ��� �� ��᪥
       �ᯮ������ � DrawMenu
       �����頥� GM_OK ��� ��� �訡�� */

int  RestoreMenu(int menuhandler);
    /* ���⠭�������� ���� �� ��࠭
       �ᯮ������ � RestoreLevels ��� ActivateMenu
       �����頥� GM_OK ��� ��� �訡�� */

int  MemoryMove(signed long srcpos, signed long srcsize);
    /* ᤢ����� ���� � ���� �� 㤠����� ������-���� �� ���
       �ᯮ������ � RestoreMenu
       �����頥� GM_OK */

int  FindNextMenu(signed long menupos);
    /* ��室�� ᫥���饥 ����� ������� ���� � ����;
       �ᯮ������ � MemoryMove
       �����頥� handler, �᫨ ��� - � -1 */

int  SaveBufferOnDisk(int menuhandler);
    /* ��࠭�� ���� �� ��᪥
       �ᯮ������ � SaveMenu � RestoreMenu
       �����頥� GM_OK ��� ��� �訡�� */

int  LastSaveBufCoord(signed long *begin, signed long *end);
    /* �����頥� ��砫��� � ������� ������ ����, ��࠭������ � �����,
       �ᯮ������ � SaveMenu � RestoreMenu,
       �᫨ ��祣� �� ��࠭﫮��, �����頥� -1, ���� 㪠��⥫� �� ����,
       ��࠭����� ��᫥����  */

int  GetSwapFileName(char *mask, int drive, int firstlet, char *filenam);
    /* � ᮮ⢥��⢨� � ��᪮� � ����஬ �ࠩ�� (0-) ᮧ���� 㭨���쭮�
       ��� 䠩�� (firstlet - c ����� ����樨 � ��᪥ ��稭����� �����),
       ����祭��� ��� �����뢠���� � filenam,
       �ᯮ������ � SaveBufferOnDIsk
       ������ - ����� 䠩�� (1-99) ��� 0 �� ��㤠� */

int  TakeSwapFileName(int menuhandler, char *mask, int firstlet, char *filenam);
    /* �� ��᪥ � menuhabdler ��।���� ��� 䠨�� � ���஬ �뫮 ��࠭���
       ����, ����祭��� ��� �����뢠���� � filenam,
       �ᯮ������ � RestoreMenu
       ������ - GM_OK, �᫨ ⠪�� 䠩� � ��ࠢ�� ����;
		 GM_NOK - � ��⨢��� ��砥  */

int  RestoreLevels(int menuhandler, int levels);
    /* ����⠭�������� �� ��࠭� ����, ��稭�� � menuhandler (- �� handler)
       � ������⢥ levels,
       �ᯮ������ � ActivateMenu
       ������ -  handler ��᫥����� ���⠭��������� ����  ���
		  -1, �᫨ ��祣� �� ����⠭���������� */

int  SaveLevels(int srcmenuhandler, int destmenuhandler);
    /* ���� ����, ��࠭�� ��, ��稭�� � srcmenuhandler (- �� handler)
       � �� destmenuhandler (- �� ⮦� handler);
       �ᯮ������ � ActivateMenu
       ������ GM_OK ��� ��� �訡�� */

int  GetUpperMenuHandler(int menuhandler, int qlevels);
    /* ��।���� handler ���� �� qlevels �஢��� ����� ��᮪���,
       祬 menuhandler,
       �ᯮ������ � ActivateMenu � MouseOperate
       �����頥� - handler �� ���� ��� -1 �� ������⢨� ⠪����� */

int  IfHighLet(int let, int menuhandler);
    /* �����頥� ����� ITEM� ��� -1, �᫨ �� �㪢� � ���� �� ������⢮����
       �ᯮ������ � ActivateMenu  */

int  SetHighLet(int menuhandler, int itemhandler);
    /* ��।���� ����� ᨬ���� ����� �뤥������ �㪢� ��� ������� ITEM�
       �ᯮ������ �� ��� Init****Item
       �����頥� ����� ��� -1, �᫨ ⠪�� �㪢� ��������� */

void AddKeyToBuf(int c);
    /* �����뢠�� ᨬ��� � KeyBuffer
       �ᯮ������ � ActivateMenu � MouseOperate */

int  GetKeyFromBuf(void);
    /* ���� ᨬ��� �� KeyBuffer
       �ᯮ������ � GetEvent */

int  GetEvent(void);
    /* �����頥� ᨬ��� �� ���� KeyBuffer ��� �᫨ ��� ���,
       � � ����������
       �ᯮ������ � ActivateMenu */

int MouseOperate(int x, int y);
   /* ��뢠�� ॠ��� �ॡ㥬�� �� ����⨨ ��� � �窥 x,y
      �ᯮ������ � GetEvent
      ������ -  0, �᫨ x,y �� �⭮���� � ����譨��
		 1, �᫨ �⭮����	 */

int ShowStatus(int menuhandler, int itemhandler);
   /* �뢮��� �� ��࠭ �����-��ப�
      �ᯮ������ � DrawItem
      ������ GM_OK */

int SaveStatusScreen(void);
    /* ��࠭�� ��࠭ ��� �����-��ப��
       �ᯮ������ � ShowStatus
       ������ GM_OK ��� GM_NOK, �᫨ stathandler �� ᮮ⢥�����騩 */

int RestoreStatusScreen(void);
    /* ����⠭�������� ��࠭ ��� �����-��ப��
       �ᯮ������ � ShowStatus, ActivateMenu � RestoreMenu
       ������ GM_OK ��� GM_NOK, �᫨ stathandler �� ᮮ⢥�����騩 */

int MoveItemBar(int mhandler, int olditem, int newitem);
  /* ��।������ ����� ���� � ��ண� itema �� ����,
     ��⠭�������� ->curitem
     ����p�� - GM_OK �p� �ᯥ� ��� GM_NOK �p� ������ */
/* ! ������ �� ०��� ���譥�� �ࠢ����� ����: define OUTER_OPERATION_MODE */

int FindNewItem(int mhandler, int olditem, int rezhim);
  /* ��� ᮮ⢥�����騩 ०��� �����⠭�� item
     ����p�� - ����p ���������� item� ��� GM_NOK �p� ������
     rezhim - �. define (FIRST,LAST,NEXT,PREV) */
/* ! ������ �� ०��� ���譥�� �ࠢ����� ����: define OUTER_OPERATION_MODE */

int IfFuncKey(int key, int menuhandler);
  /* �����頥� ����� FUNCKEY ITEM�, ���஬� ᮮ⢥����� �㭪樮���쭠�
     ������  ��� -1, �᫨ �� ������ � ���� �� ������⢮���� */
/* ! ������ �� ०��� ���譥�� �ࠢ����� ����: define OUTER_OPERATION_MODE */






void menusound(int menuhandler);
int  getsym(void);
void badkey(void);
void msound(int fr);
int  ftoupper(int c);

//int  getch(void);
//void sound(unsigned frequency);
//void nosound(void);
//void delay(unsigned milliseconds);

int test(void);
int test1(void);

/****************************** UTIL ***************************************/
int writemem(char *flname, char *buf, long bufsize);
int readmem(char *flname, char *buf);
int FindFreeDrive(long freespace);

