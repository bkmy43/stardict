#include <dir.h>

/* ���� �������� */
#define NO_BORD       0
#define SINGLE_BORD   1
#define DOUBLE_BORD   2
#define REDUCED_BORD  3

/* ��ࠬ��� ��� �㭪権 REnterString � REnterFilename (rezhim)
   � getsym_filtered (filter), ��।����騩 ⨯ �������� ��ப�
   ��� ᨬ���� */
#define TEXT           0    // �� ⥪��
#define DIGIT_INT      1    // 楫�� �᫮
#define DIGIT_REAL     2    // �᫮ � �窮�
#define FILENAME_PART  3    // ��� 䠩�� ��� ���७��
#define FILENAME_FULL  4    // ��� 䠩�� � ���७���
#define DATE           5    // ���

/* ���祭�� ��p����p� IfSave � REnterString */
#define  NO             0
#define  YES            1


/* �⠭����� ��� ������ */
#define GM_NOK       -1
#define GM_OK         0

/* ���ﭨ� ����� ��� */
#define GRON        0   // ����祭 � �����
#define GROFF      -1   // �몫�祭
#define GRHIDDEN    1   // ����祭, �� �� ����� (���⠭)

/* ⨯ ������ ��� �㭪樨 Ropen */
#define GM_DETECT     -1
#define GM_CGA         1
#define GM_EGA         3
#define GM_HERC        7
#define GM_ATT         8
#define GM_VGA         9



/******* ������ ����� �訡�� ��� �㭪樨 gerrmes (��ࠬ��� mesnum) ********/

#define GW_DOUBLEINIT   0
#define GW_NOINIT       1

#define NOMEM           2
#define BIGIMAGE        3

/* �訡�� ����譨�� */
#define NOSAVEMEM     100
#define LOWBUFFER     101
#define OPEN_ERROR    102
#define READ_ERROR    103
#define WRITE_ERROR   104
#define CREAT_ERROR   105
#define BAD_TYPE      106
#define KEY_BUFFER    107
#define NOLASTSTRING  108
#define MANYMENUS     109
#define MANYITEMS     110
#define NOMEMENOUGH   111
#define BADSTRLEN     112
#define CANTOUTTEXT   113
#define CANTSAVE      114
#define CANTRESTORE   115
#define CANTDRAW      116
#define CANTFIND      117
#define ALREADYACTIVE 118
#define BADITEMNUM    119
#define BADNAMELEN    120
#define BADITEMTYPE   121

#define BADMENUNUM    122
#define MANYAMENUS    123

#define FU_BADNUM     200
#define FU_NOCLEAR    201
#define FU_PRINTER    202
#define FU_SETFONT    203
#define FU_SETREF     204

#define GC_NOGRAPH     300
#define GC_BADKORD     301
#define GC_NOLINEINIT  302
#define GC_NOLINEMARK  303

#define FT_BADFORMAT   400
#define FT_BADPARAM    401

#define MN_OVERFLOW    500
#define MN_BADTYPE     501
#define MN_LONGSTRING  502

#define H_BADFILE      600
#define H_NOMEM        601
#define H_LONG         602
#define H_NOENTRYEND   603
#define H_MANYENTRIES  604
#define H_BADITEMNUM   605

#define FKEY_ERROR     700
#define FKEY_NOKEY     701
#define FKEY_PRESENT   702
#define FKEY_COLOR     703
#define FKEY_SWITCH    704

/* ⨯ �訡�� ��� �㭪樨 gerrmes (��ࠬ��� errtype)
   ⨯� ࠧ������� 梥⮬ �뢮������ ᮮ�饭��,
   � �� SYSTEM_ERROR �ந�室�� �����襭�� �ணࠬ�� */
#define SYSTEM_ERROR   0
#define COMMON_ERROR   1
#define MESSAGE        2
/*************************************************************/

/* ��� �㭪権 ⨯� IfMyKey / IfMyMouse */
const int NOT_MINE=-1;
const int MINE=0;

/* �� */
#define GW_ENG          0
#define GW_RUS          1
#define GW_GER          2

/* ⨯ �ਭ�� */
#define G_EPSON         0
#define G_IBM           1
#define G_IBM24         2
#define G_HYUNDAI       3
#define G_EPSON24       4

/**/
#define COL_MULTI       0
#define COL_MONO        1

/* ��� �㭪権 R...Fkey ࠡ����� � �������� */
#define MAX_FKEYS      20  // ���ᨬ��쭮� �����६����� ������⢮ ������
#define NOCOLOR        -1  // 梥� �� 㪠���

/* ��⨢�� ��� ��� ������ */
#define FKEY_OFF        0
#define FKEY_ON         1

/* ������� ����뢠��� ������ */
struct Rfunckey
   {
   int l,t,r,b;   // �࠭���
   int ot; // ���७�� ���� ���⢨⥫쭮�� � ��� �� �� ��஭� �� ot
   int fkey;  // ����� ������ � ᮮ⢥��⢨� � �㭪樥� getsym
        // (==getch,  �� � ������� ����� ������ 0 ᮮ⢥�����
        // �ਡ������� 1000)
   int col,fcol,shcol; // 梥� �������� ������, ᠬ�� ������, ⥭�
       // (�᫨ ⥭� �� �㦭� shcol==NOCOLOR)
   char *fkeyname; // ��ப� - �������� ������
   int active; // �. define FKEY_ON ��� FKEY_OFF
   };


/************************* �������� ������� *******************************/
/* ! �� ���न���� ��᮫���: 0-1023 �� ��ਧ��⠫�, 0-768 �� ���⨪��� */

void Ropen(int driver);
     /* ���樠������� ��䨪�
      driver - ⨯ ������ �. define */
void Rclose(void);
     /* ����뢠�� ��䨪� */
void Rexit(int cod);
     /* ����뢠�� ��䨪� � ��室�� �� �ணࠬ�� � ����� cod */

void Rclr(int col);
     /* ��頥� ��࠭, �������� ��� 梥⮬ col */
void Rcol(int col);
     /* ��⠭�������� 梥� */
void Rbcol(int col);
     /* ��⠭�������� 䮭 (��� �����஬� �ᥣ�� 0) */
void Rtext(int x, int y, char *s);
     /* �뢮��� ��ப� s � ���न���� x,y */
void Rtextc(int x, int y, char *s, int col);
     /* �뢮��� ��ப� s ��������� 梥� col � ���न���� x,y */
int Rtext_with_high_let(int x, int y, char *s, int hletnum, int hletcol);
     /* �뢮��� ��ப� s � ���न���� x,y
	ᨬ��� ��ப� ����� hletnum �뤥����� 梥⮬ hletcol;
	�����頥� GM_OK �� �ᯥ� ��� GM_NOK, �᫨ �� 墠⠥� ����� */
void Rtext_with_break(int x, int y, int interval, char *s);
     /* �뢮��� ��ப� s � ���न���� x,y
	����� ᨬ������ ��⠢��� �������⥫�� ���ࢠ� � interval (0-...)
	ᨬ����� */
void Rsym(int x, int y, char c);
     /* �뢮��� ᨬ��� c � ���न���� x,y */
void Rsymc(int x, int y, char c, int col);
     /* �뢮��� ᨬ��� c ��������� 梥� col � ���न���� x,y */
void Rdigit(int x, int y, int digit);
     /* �뢮��� 楫�� �᫮ digit � ���न���� x,y */
void Rdigitc(int x, int y, int digit, int col);
     /* �뢮��� 楫�� �᫮ digit ��������� 梥� col � ���न���� x,y */
void Rdigit_right(int x, int y, int digit, int qsymbols);
     /* �뢮��� 楫�� �᫮ digit � ���न���� x,y
        � ��ࠢ�������� �� qsymbols �� �ࠢ��� ��� */
void Rline(int x1, int y1, int x2, int y2);
     /* �뢮��� ��१�� � ���न��⠬� x1,y1 - x2,x2 */
void Rlinec(int x1, int y1, int x2, int y2, int col);
     /* �뢮��� ��१�� � ���न��⠬� x1,y1 - x2,x2 梥� col */
void Rbar(int l, int t, int r, int b);
     /* �뢮��� ᯫ�譮� ��אַ㣮�쭨� */
void Rbarc(int l, int t, int r, int b, int fill, int col);
     /* �뢮��� ��אַ㣮�쭨� ��������� 梥� col �
        �������⥫� fill (�. define graphics.h) */
void RThickLinec(int x1, int y1, int x2, int y2, int col,
		 char style, char thickness);
     /* ���� ��१�� ��������� ⨯� � ⮫騭� (�. define graphics.h) */
void Rram(int l, int t, int r, int b);
     /* �뢮��� ࠬ�� */
void Rramc(int l, int t, int r, int b, int col);
     /* �뢮��� ࠬ��, ��������� 梥� col */
void Rcircle(int x, int y, int r);
     /* ���� ���㦭���� ࠤ��� r � 業�஬ � x,y */
void Rcirclec(int x, int y, int r, int col);
     /* ���� ���㦭���� ࠤ��� r � 業�஬ � x,y ��������� 梥� col */
void Rfillcircle(int x, int y, int r, int col);    /* NEW */
     /* �������� �p� 㪠����� 梥⮬ */
void Rfloodfill(int x, int y, int fill, int col, int bordercol); // NEW
     /* �������� p�����, ��p���祭�� 梥⮬ bordercol ᯮᮡ��
	fill � 梥⮬ col, ��稭�� � �窨 x,y */
void Rpoint_on_circle_coordinates(int *x, int *y, int xx, int yy,
				  int radius, double angle); /* NEW */
     /* �����뢠�� �⠭����� ���न���� ��� �窨 �� ���㦭���
	� 業�஬ xx,yy, ࠤ��ᮬ r � 㣫�� (᫥��, �� �ᮢ�� ��५��)
	angle */
void Rradius(int xx, int yy, double angle, int from, int to); /* NEW */
     /* ���� ��१�� ࠤ��� � 業�஬ xx,yy, 㣫�� angle ��
	from �� to */
void RMovingCircle(int x, int y, int srcRad, int destRad,
	int fcol, int col);
     /* ���� ����������� ���㦭���� � 業�஬ � x,y
        �� ��������� ࠤ��� srcRad �� ��������� ࠤ��� destRad
        梥� ���㦭��� col,  梥� 䮭� fcol */
void Rputpixel(int x, int y, int col);
     /* ���� ���ᥫ� ��������� 梥� col � �������� ���न���� x,y */
     /* �� �����஬��� ������ �� 㯮�ॡ���� ��� �����㭤� */
unsigned far Rgetpixel(int x, int y);
     /* �����頥� 梥� ���ᥫ� x,y � �������� ���न���� */
int RSwitchVisualPage(void);
     /* ����� ������� �p������ ��p����� (��� EGA)
	����p�頥� ����p (0 ��� 1) ⥪�饩 ������� �p���᪮� ��p����� */
int RSwitchActivePage(void);
     /* ����� ��⨢��� �p������ ��p����� (��� EGA)
	����p�頥� ����p (0 ��� 1) ⥪�饩 ��⨢��� �p���᪮� ��p����� */

/****************** ������� ��������������� ��������� *******************/
void Rconvert(int *rx, int *ry);
     /* ���������� ���न���� �� �⠭������ � ॠ��� */
void Rbackconvert(int *rx, int *ry);
     /* ���������� ���न���� �� ॠ���� � �⠭����� */

/**************** ����������� ������� ��� ������ ����� *******************/
void RPutLine(int x, int y, char *s, int SymWidth, int col);
     /* �뢮��� �� ��࠭ ��ப� s ��������� 梥� col � ���न���� x,y
        � ������� ���ࢠ��� ����� �㪢��� SymWidth (� �⠭������
        �������) */
void RPutLineClear(int x, int y, char *s, int SymWidth, int col);
     /* �뢮��� �� ��࠭ ��ப� s ��������� 梥� col � ���न���� x,y
        � ������� ���ࢠ��� ����� �㪢��� SymWidth (� �⠭������
        �������), ���⨢ ��� ��� ����࠭�⢮ � ᮮ⢥��⢨�
        � ⥪�騬 �����㭤��  */
void RPutLineWithHighSym(int x, int y, char *s, int SymWidth, int col,
			 int SymNum, int highcol);
     /* �뢮��� �� ��࠭ ��ப� s ��������� 梥� col � ���न���� x,y
        � ������� ���ࢠ��� ����� �㪢��� SymWidth (� �⠭������
        �������), ���⨢ ��� ��� ����࠭�⢮ � ᮮ⢥��⢨�
        � ⥪�騬 �����㭤�� � ���ᢥ稢�� ᨬ��� SymNum (����� �
        ��ப�) ������� 梥⮬ highcol */
void RPutLineWithCursor(int x, int y, char *s, int SymWidth, int col,
			 int SymNum, int highcol, int bcol);
     /* �뢮��� �� ��࠭ ��ப� s ��������� 梥� col � ���न���� x,y
        � ������� ���ࢠ��� ����� �㪢��� SymWidth (� �⠭������
        �������) � ����஬ 梥� highcol ��� ᨬ����� ����� SymNum
        ⥪�騩 䮭 �����㭤� ������ 㪠�뢠���� � bcol (! ᠬ ��
        �� ��⠭����������) */
void RPaintString(char *str, int n, int qstr_on_page, int first,
		  int top, int left, int interval,
		  int groundcolor, int textcolor, int highcolor,
                  int cur);  /* �᫨ cur==-1, � 䮭 */
     /* �뢥�� (��� ᮮ⢥��⢥��� �� �뢥��) ��ப� str ��
        ᯨ᪠ ��ப, �᫨
          - �� ����� � ᯨ᪥ n,
          - �ᥣ� �� ��࠭�� ����頥��� qstr_on_page ��ப
          - ��ࢠ� ��ப� �� ��࠭�� ����� ����� first
          - ���孨� ���� 㣮� ��࠭��� ����� ���न���� top,left
          - ���ࢠ� ����� ��ப��� interval (� �⠭������ �������)
          梥� �����㭤� groundcolor,
          梥� ��ப� �᫨ ��ப� �뤥������ (n==cur) highcolor, ����
             textcolor � ��㣮� ��砥


/************** ������� ���������� ����� ������ � ������ *******************/
void Rgetimage(int l, int t, int r, int b, char *buf);
     /* ��������� � buf (! �� ������ �।���⥫쭮 �뤥���� ������)
        ��אַ㣮�쭨� ��࠭� � ���न��⠬� l,t,r,b */
void Rputimage(int l, int t, char *buf, int op);
     /* �뢥�� �� � buf �� ��࠭ ��אַ㣮�쭨� � ���न���� l,t
        (���孨� ���� 㣮�), op == ᮮ⢥�����饬� ��ࠬ���� ��
        putimage */
unsigned far Rimagesize(int l, int t, int r, int b);
     /* ������� � ����� ������, ����室���� ��� ��࠭���� ��אַ㣮�쭨��
        ��࠭� � ���न��⠬� l,t,r,b */


/********************** ������� ������ ��������� *************************/
int  gerrmes(int mesnum, char *proc, int errnum, int errtype);
     /* �뤠�� ᮮ�饭�� � ᮮ⢥��⢨� � mesnum (�. define)
        � proc  㪠�뢠����  ����⠭�  - ��� ��楤���,  ��� ��������
        �訡��, errnum - ����� ᮮ�饭�� (�� ��砩, �᫨ � ����� ��-
        楤�� ��᪮�쪮 ����⨯��� ᮮ�饭��), errtype - ⨯ �訡��
        (�.  define);
        �����頥� �ᥣ�� 0 */
int  Rmessage(char *str, int col, int fcol, int bordcol);
     /* �뢮��� ��ப� str, 梥� col �� 䮭� 梥� fcol � ����஬
        梥� bordcol � ���� ������ ������ ��� ���
        �����頥� ������ ᨬ��� (��� -1/-2 �� ����⨨ �����/�ࠢ��
        ������ ���) */
void termes(char *str, char *func);
     /* �����蠥� �ணࠬ��, �뤠� �।���⥫쭮 ᮮ�饭�� str
        �� �訡�� � �㭪樨 func */


/*************** ������� ��������� ���� � ���������� ***********************/
int  getsym_filtered(int filter);
     /* ���� ���� �� �㤥� ����� ������ ����᪠���� ��ࠬ��஬ filter
        (�. define),
        �����頥� ����� ����⮩ ������ � ᮮ⢥��⢨� �
        �㭪樥� getsym (==getch,  �� � ������� ����� ������
        0 ᮮ⢥����� �ਡ������� 1000) */
int  getsym(void);
     /* ���� ���� �� �㤥� ����� ������,
        �����頥� ����� ����⮩ ������ (==getch, �� � ������� ����� ������
        0 ᮮ⢥����� �ਡ������� 1000) */

void RbarcM(int l, int t, int r, int b, int fill, int col);
     /* = Rbarc, �� ���� ������ �� �६� �ᮢ���� */
void RsymcM(int x, int y, char c, int col);
     /* = Rsymc, �� ���� ������ �� �६� �ᮢ���� */
void RtextcM(int x, int y, char *s, int col);
     /* = Rtextc, �� ���� ������ �� �६� �ᮢ���� */
void RramcM(int l, int t, int r, int b, int col);
     /* = Rramc, �� ���� ������ �� �६� �ᮢ���� */

int  Rmouse_set(void);
     /* ���樠����஢��� ����
        �����頥� GRON �� 㤠�, GROFF �� ��㤠� */
int  Rmouse_status(int *x, int *y);
     /* ��।����� ���न���� (ॠ���, � �� �⠭����� !!!)
        ��� �� ��᫥���� ����⨨
	������:  0 - �� �뫠 �����
                  1 - �뫠 ����� ����� ������
                  2 - �뫠 ����� �ࠢ�� ������
     */
void Rmouse_hide(void);
     /* ���� ��設��� ��५�� � ��࠭� */
void Rmouse_show(void);
     /* �������� ��設��� ��५�� �� ��࠭� */
void Rmouse_page(int page);
     /* �����뢠�� ��設�� ��p� �� �p���᪮� ��p���� page */

int Revent(int *x, int *y);
    /* �����頥� ������� �������
       ��� -1, �᫨ ������ ���� - ⮣�� � x � y - ���न���� (�⠭�����)
               ��᫥����� ������ ���
       �롮� ����� ������ (�. �㭪樨 R...Fkey) ��ࠡ��뢠����,
               ��� ����⨥ ᮮ⢥�����饩 ������
    */

/***************** ������� ��� ������ � �������� ***************************/
void RInitFkey(int x, int y, int otstup, char *fkeyname, int fkey,
	       int col, int fcol, int shadowcol);
     /* ᮧ���� �� ��࠭� ������ � ���न��⠬� x,y ������ ���孥�� 㣫�
        otstup - ���७�� ���� ���⢨⥫쭮�� � ������ ��� �� �ᥬ
                 ���ࠢ����� (� �⠭������ �������)
        fkeyname - ��ப� - �������� ������
        fkey - ����� ������ �� getsym (�. define � keys.h)
        col, fcol, shadowcol - 梥� ��������, 䮭� � ⥭� ᮮ⢥��⢥���
                   �᫨ ⥭� �� �㦭�, 㪠�뢠�� NOCOLOR
     */


void RSwitchFkey(int fkey, int rezhim);
     /* ��ॢ���� ������ (fkey - ����� ������) � ᮮ⢥�����騩 ०��
     rezhim: FKEY_ON - ࠡ�祥 ���ﭨ�;
             FKEY_OFF - ��ࠡ�祥 ���ﭨ� */

void RRemoveFkey(int fkey);
     /* 㤠��� ������ (� ��࠭�� ������ � ��࠭� ���������
        ᠬ����⥫쭮) (fkey - ����� ������) */

void RSetFkey(int l, int t, int r, int b, int ot, int fkey,
	      char *fkeyname, int col, int fcol, int shcol);
     /* �㭪�� ����������� �ᯮ�짮����� - ᠬ����⥫쭮 �� �ᯮ������ !
        ����頥� � ���� ������� � �� ���न����
        �� ��࠭� ��� �ᯮ�짮����� ����� */


/*********************** ���� ������ � ���������� *********************/
char *REnterString(char *str, int x, int y, int length, int SymWidth,
		  int col, int bcol, int curscol, int rezhim, int IfSave);
     /* ������ ��ப� �� ����� str (! �� ������ �뤥���� ������)
        ���न���� ���� ����� - x,y
        length - ���ᨬ��쭠� ����� �������� ��ப�
        SymWidth -  ����ﭨ� ����� ᨬ������ (� �⠭������ �������)
        bcol, col, curscol - 梥� 䮭�, ��ப�, ����� ᮮ⢥��⢥���
	rezhim - ⨯ ��ப� (�. define)
	IfSave - YES ��� NO (�. define) ��p����� � ����⠭��������
		 �� ���p⮥ ���� �� ��p��� */

char *REnterStringBox(char *str, int left, int top, int length, int SymWidth,
		  int col, int bcol, int curscol, int rezhim,
		  int boxcol, int bordcol, int mescol,
		  char *mes1, char *mes2);

int IfDate(char *str);
     /* ��।���� ���室�� ��� ��� ��ப� ��� �ଠ� ���� � ᮮ⢥��⢥���
       �����頥� GM_OK ��� GM_NOK */
int  days_in_month (int m, int y);
     /* �����頥� ������⢮ ���� � ����� m (1-12) ���� y */
int  REnterFilename(char *name, int x, int y, int rezhim,
		   int col, int bcol, int curscol);
     /* ������ ��� 䠩�� � ���७��� (rezhim==FILENAME_FULL) ���
        ��� (FILENAME_PART) �� ����� name
        ���न���� ���� ����� - x,y
        bcol, col, curscol - 梥� 䮭�, ��ப�, ����� ᮮ⢥��⢥���
	������  0 - OK;  -1 - �訡�� */

/**************************** FONTER *************************************/
#define FU_OK         0
#define FU_NOFILE    -1
#define FU_CANTOPEN  -2
#define FU_BADFILE   -3
#define FU_NOMEM     -4
#define FU_CANTREAD  -5

#define FU_SMALLFONT   8
#define FU_LARGEFONT  32

#define FU_EPSON      0
#define FU_IBM        1

#define FU_SYSTEMSYMBOL '~'

#define FUB(s) r=biosprint(0,s,0); if ((r & 0x01)||(r & 0x08)||(r & 0x020)||(r & 0x080)==1) return(-1)

void FUOn(void);
void FUOff(void);
void FUSetCurrentFont(struct FUFONT *f);                       // FONT
void FUSetCurrentRef(struct FUREF *f);                            // REF
void FUSetRef(struct FUREF *f, int qsym, char *num, char *sym);   // REF
int FUIfinRef(int sym);                                           // REF
/* �᫨ ASCII ᨬ��� �������� � FUCurrentRef, �����頥��� ��� ����� �
   ����,   �᫨ ���, � -1 */
void FUPutSym(int x, int y, int num, int fonttype);            // FONT
void FUPutSymc(int x, int y, int num, int fonttype, int col);  // FONT
void FUtext(int x, int y, char *s);                            // FONT  REF
char *FUConvertC(char *s);                                        // REF
int  FUPrintSym(int num, int printer);                         // FONT
     /* ������ - 0-OK, -1 -�訡�� */
char *FULoadFont(struct FUFONT *font, int fonttype);
void FUReleaseFont(struct FUFONT *f);

void RPutSym(int x, int y, char *buf, int num, int fonttype);
int  RPrintSym(char *buf, int num, int printer);

struct FUFONT
   {
   char filename[MAXPATH];
   char *buf;
   int qsym;
   };

struct FUREF
   {
   char *num; // ����� � ����
   char *sym; // ����� ASCII ᨬ����
   int qsym;
   };


/*************************** GWAIT *******************************/
#ifndef  _TIME_T
#define  _TIME_T
typedef long time_t;
#endif

struct GWAIT
{
char *buf;
int l,t,r,b;
int curdigit;
int fcol,pcol,tcol;
time_t time;
char elapstime[6],estimtime[6];
};

void GWaitInit(int l, int t, int lang, int srcdigit,
	       int col, int fcol, int rcol, int pcol, int tcol);
void GWaitChange(int digit);
void GWaitClose(void);


void Rsound(int freq, int millisec);
     /* ������ ��� ����� freq ������� millisec �����ᥪ㭤 */


/**************************** CURSOR *************************************/
/* �㭪樨 ����७���� ���짮�����. ���� gcursor.c */

#define HWING        5
#define VWING        5

#define GC_OFF      -1
#define GC_ON        0
#define GC_HIDDEN    1

#define MAX_MARKERS 20

struct GCURSOR
   {
   int  status;
   int  x,y,l,t,r,b,col;
   unsigned save[HWING*2+VWING*2+1];
   };

/* �� � ��᮫���� ���न���� */

void GCInit(int l, int t, int r , int b, int col);
   /* ���樠������ �����, ��⠭���������� �࠭��� ��।������� � 梥� */
void GCSet(int x, int y);
   /* ��⠭���������� ��������� �����, ����� �뢮����� �� ��࠭ */
void GCShow(void);
   /* ����� �뢮����� �� ��࠭ */
void GCHide(void);
   /* ����� ������� */
void GCputpixel(int x, int y, int col);
int  GCgetpixel(int x, int y);
void GCMove(int direction);
   /* ����� ��।�������� � ᮮ⢥�����饬 ���ࠢ����� */
void GCInfo(void);
   /* �뢮��� �� ��࠭ ���ଠ�� � ����� */
void GCMark(void);
   /* ��४�訢��� ���ᥫ� � ����訢���� 梥� */
void GCClearMark(void);
   /* ��頥� �� ��થ�� */
int GCDefineMarker(int *l, int *t, int *r, int *b, int col);
   /* ��।���� ॠ��� (�� ��᮫��� !!!) ���न���� �� ��� ��થࠬ
      ��������� 梥�
      ������ -  0 - �᫨ �� 㤠筮
		-1 - ��થ� �� ������
   */
void GCMarkLineInit(int col);
void GCMarkLine(void);
void GCRestoreLine(void);
void GCDeleteLines(void);
void GCGetMarkersInfo(int col);
void GCMarkLineExit(void);

/**************************/  /* NEW */
double fsin(double x);
double fcos(double x);
