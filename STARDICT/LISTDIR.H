#define D_MAXFILES  200


struct LIST_ENTER_ITEM
   {
   int left;
   int top;
   int right;
   int bottom;
   int interval;
   int otstupx;
   int qstr;
   int first;
   int curstr;
   };

int ShowList(char *str[], int qstr, int left, int top, int right, int bottom,
	      int bordtype, int interval, int otstupx,
	      int groundcolor, int textcolor, int highcolor, int bordcolor,
	      int helpitem, int ListParam);
void PaintString(char *str[], int n, int qstr_on_page, int first,
	      int top, int left, int interval,
	      int groundcolor, int textcolor, int highcolor,
	      int cur);  /* если cur==-1, то фон */
void InitListEnterItem(int left, int top, int  right, int bottom,
	      int interval, int otstupx, int qstr, int first);
int  SetListEnterItem(int mx, int my);
char *ReducePath(char *path, int maxlen);
int  SelectFile(char *filemask, char path[MAXPATH], char *rezfile,
		int left, int top, int maxstr);
int  SelectDrive(int left, int top, int maxstr);

int  Dget(char *str, char *path, char *filemask, int *qdir);
int  Dgetfiles(char *str, char *path, char *mask, char type);

void ClearListDirScreen(int x, int y, int wid, int hei);

char *ldirmes(int n);

int ListDir(char *filemask, char path[MAXPATH], char *rezfile, int x, int y);

/******************** CFG *********************/
#define FNAM                      33
#define FTYP                      28

//#define HELP_ON

int LoadCfgData(void);
int SelectCfgFile(char *cfgname);
int ShowCfgList(char *cfgname, int left, int top, int interval,
		int otstupx, int groundcolor, int textcolor, int bordcolor,
		int highgroundcolor, int hightextcolor);
void ShowCfgString(char *str, int cur, int left, int top, int interval,
		   int otstupx, int groundcolor, int textcolor);

char *listmes(int n);


void HShowHelp(int n);
void ClearScreen(void);
