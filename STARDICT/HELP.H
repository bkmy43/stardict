#include <stdio.h>

#define MAX_HELPSTRINGS       50
#define MAX_HELPSTRINGLENGTH  80
#define MAX_HELPENTRIES       10
#define MAX_HELPITEMS       1000
#define MAX_MEMITEMS          30
#define ITEM_MARKER          '@'
#define ENTRY_BEGIN_MARKER   '~'
#define ENTRY_END_MARKER     '^'

struct HELPENTRY
   {
   int x,y,l;  /* координаты текста ! */
   int left,top,right,bottom;
   int itemnum;
   char str[MAX_HELPSTRINGLENGTH];
   };

struct HELPITEM
   {
   FILE *f;
   int left;
   int top;
   int right;
   int bottom;
   int interval;
   int length;
   int qstr;
   int textcolor;
   int groundcolor;
   int entrycolor;
   int highentrycolor;
   int highentrygroundcolor;
   int bordcolor;
   int hotkeycolor;
   int qentries;
   signed long *index;
   struct HELPENTRY HelpEntry[MAX_HELPENTRIES];
   char *str[MAX_HELPSTRINGS];
   int nstr;
   int qpages;
   int mem[MAX_MEMITEMS];
   };



int HInitHelp(char *filename, int left, int top, int interval, int length,
	      int qstr,
	      int textcolor, int groundcolor, int entrycolor,
	      int highentrycolor, int highentrygroundcolor,
	      int bordcolor, int hotkeycolor);
void HCloseHelp(void);
void HShowHelp(int n);
void HLoadHelp(int n);
void HShowHelpPage(int pagenum);
void HShowHelpString(int x, int y, char *str);
void HAddEntry(int pos, int l, int x, int y, char *str);
void HShowEntry(int n, char rezhim);    /* H/N high/normal */
void HAddMem(int n);

