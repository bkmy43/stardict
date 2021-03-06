const int L_UP=-1;
const int L_DOWN=-2;

/**************************** SLIDEBAR ***********************************/
class SLIDEBAR
   {
   public:
      SLIDEBAR();
	void SetBarLeft(int l) { left=l; if (right!=-1) width=right-left; else if (width!=-1) right=left+width;  }
	void SetBarTop(int t) { top=t; if (bottom!=-1) height=bottom-top; else if (height!=-1) bottom=top+height; }
	void SetBarRight(int r) { right=r; if (left!=-1)  width=right-left; else if (width!=-1) left=right-width; }
	void SetBarBottom(int b) { bottom=b; if (top!=-1) height=bottom-top; else if (height!=-1) top=bottom-height; }
	void SetBarWidth(int w) { width=w; if (left!=-1) right=left+width; else if (right!=-1) left=right-width; }
	void SetBarHeight(int h) { height=h; if (top!=-1) bottom=top+height; else if (bottom!=-1) top=bottom-height; }

	void SetBarButtonHeight(int h) { button_height=h; dotted_bar_height=height-button_height*2; }
	void SetBarColors(int b, int d, int a, int but, int bord) { bar_color=b; dot_color=d; arrow_color=a; button_color=but; border_color=bord; }

	void DrawSlideBar();
	void DrawButton(int percent);
	int IfMyMouse(int x, int y);
	int DefinePercent(int y);


      ~SLIDEBAR() {}
   private:
      int left;
      int top;
      int right;
      int bottom;
      int width;
      int height;
      int button_height;
      int dotted_bar_height;
      int current_percent;

      int bar_color;
      int dot_color;
      int arrow_color;
      int button_color;
      int border_color;

   };


/******************************* LIST ************************************/
/*  ��p����p � SetDirection */
enum { L_HOR=0, L_VERT=1};

const int L_MAX_STRINGS=3000;

/* ⨯ ��p��p� */
enum { L_NO_BORD=0, L_SINGLE_BORD, L_DOUBLE_BORD };

/* ��p����p rezhim ��� SetColumns */
enum { L_Q_COLS=0, L_HOR_INTERVAL };

const int L_MAX_INS_FLAGS=20;
const int L_MAX_STATUS_TYPES=10;

/* ����p�� � ENTER_LIST::Manage */
const int GM_DONE=-2;

/* type � struct ABC_ORDER */
enum { CASE_SENSITIVE , NOT_CASE_SENSITIVE };

/* search_buffer_type � struct ABC_ORDER */
enum { VISIBLE_SEARCH_BUFFER , INVISIBLE_SEARCH_BUFFER };

struct STATUS_TYPE
   {
   int value;
   int color;
   int high_color;
   };

struct ABC_ORDER
   {
   int type;
   char *abc;
   int *index;
   char *search_buffer;
   int search_buffer_type;
   int search_buffer_length;
   char *search_buffer_buf;
   };


class LIST
   {
   public:
//      LIST(); // +++
      LIST(int size); // +++
      ~LIST();

	 void SetData(char *str[], int qstr, char *st);
	 // ��⠭�������� �᭮��� ����� - ���ᨢ ��p��, �� ������⢮,
	 // �� ����� (0 �᫨ ����� �� ���뢠����, � �᫨ ���뢠���� �. SetStatusType)
	 void SetLTCorner(int l, int t) { left=l; top=t; if (bottom!=-1) sl.SetBarHeight(bottom-top); }
	 // ��⠭�������� ���p������ ������ ��p孥�� 㣫� ��p���
	 void SetRBCorner(int r, int b) { right=r; bottom=b; if (top!=-1) sl.SetBarHeight(bottom-top);  }
	 // ��⠭�������� ���p������ �p����� ������� 㣫� ��p���
	 void SetColors(int g, int t) { ground_color=g;  text_color=t; }
	 // ��⠭�������� 梥� 䮭� � 梥� ��p��
	 void SetBorder(int type, int color) { border_type=type; border_color=color; }
	 // ��⠭�������� ⨯ ��p��p� (�.define) � ��� 梥�
	 void SetDirection(int d) { direction=d; }
	 // ��⠭�������� p�ᯮ������� ��p�� - �� ��p����⠫� ��� �� ��p⨪��� (�.define)
	 void SetABCOrder(int t, char *abc);
	 // H����������H�� ��H����; ��⠭�������� ��� ��䠢�⭮ 㯮p冷祭���
	 // ᯨ᪮� ⨯ (���⢨⥫쭮��� � p�����p� - �.define) � ��p���,
	 // ���p�� ������ ��䠢��� ��p冷� (p�������㥬 ALL_SYMBOLS �� UTIL.C)
	 void SetABCSearchBuffer(int t, int len);
	 // H����������H�� ��H����; ��⠭�������� ��� ��䠢�⭮ 㯮p冷祭���
	 // ᯨ᪮� ⨯ ���p� ����� (������ ��� ��� - �.define) � ��� �����

	 void SetColumns(int rezhim, int qcol);
	    // rezhim == L_Q_COLS (⮣�� qcols - ���-�� �⮫�殢)
	    //       ��� L_HOR_INTERVAL (⮣�� qcols - ���p��� ����� �⮫�栬�)
	 void SetMargins(int hor, int vert, int inter);
	 // ��⠭�������� ����� ᫥��-�p���, ����� ᢥp��-᭨��
	 // � ��p⨪���� ���p��� (�� ���� ��p��� �� ��p� ᫥���饩)
	 void SetStatusType(char v, int c);
	 // �ᯮ������ �⮫쪮 p��, ᪮�쪮 ������� p����稩 �� ������
	 // ������ char - ᨬ��� ��p������騩 ����� (��p��� ᨬ����� � ����p���
	 // ��p���� �������� � SetData) � 梥�, ���p� ��p��� �⮣� ����� p�������
	 void SetHelpItem(int h) { help_item_num=h; }
	 // ��⠭�������� ����� HELP�, ����� �㤥� ��뢠����

	 void SetBarWidth(int w)  { sl.SetBarWidth(w);  }
	 // ��⠭�������� �p��� �p���-��p�
	 void SetBarButtonHeight(int h) { sl.SetBarButtonHeight(h); }
	 // ��⠭�������� ����� ���㭪� �p���-��p�
	 void SetBarColors(int b, int d, int a, int but, int bord)
				  { sl.SetBarColors(b,d,a,but,bord); }
	 // ��⠭�������� 梥� �p���-��p� (䮭�, �祪, ���㭪�, ��p��p�)
	 void AllowEmptyList() { empty_list_allowed=YES; }
	 // �������� ᮧ����� ���⮣� ᯨ᪠
	 void CloseSearchBufferWindow();


   protected:

	 void DrawList();
	 void DrawScreen();
	 void DrawString(int n, int col);
	 void DefineStringCoordinates(int n, int *x, int *y);
//	 int GetStrLen(int n);
	 int  DefineMaxStringLength();
	 virtual int IfMyKey(int key, int mx, int my);
	 void beep();

	 int  GetNumInABC(int c); // ����p � abc_order.abc  ��� -1
	 int  FindStringAccordingToSearchBuffer(int search_from, int newsym);
	 int  IfABCOrderExists() { if (abc_order.abc!=0) return YES; return NO; }
	 void ClearSearchBuffer();
	 void OpenSearchBufferWindow();
	 void RepaintSearchBuffer();



	 int GetVertInterval() { return vert_interval; }
	 int GetDirection() { return direction; }
	 void DrawSlideBar() { sl.DrawSlideBar(); }
	 char *GetString(int n) { return strings[n]; }

	 virtual int Monitor()=0;
	 virtual int Manage(int key, int mx, int my)=0;



   private:
	 void SetBarTop(int t)    { sl.SetBarTop(t);    }
	 void SetBarBottom(int b) { sl.SetBarBottom(b); }
	 void SetBarRight(int r)  { sl.SetBarRight(r);  }

	 int  DefineStringColor(int n);
	 void CreateABCIndex();
	 void DefineSearchBufferCoordinates(int *l, int *t, int *r, int *b);


      /*+++++++++++++++++++ values +++++++++++++++++++*/
    protected:
      int qlines_per_page;     // set in SetMargins
      int qlines;              // count in DrawList
      int qstrings_per_page;   // count in DrawList
      int curline;
      int qstrings;            // set in SetData
      int qcolumns;            // set in SetColumns

      int ground_color;        // set in SetColors
      int text_color;          // set in SetColors


      SLIDEBAR sl;

//      char status[L_MAX_STRINGS];  // set in SetData +++
      char *status;                  // set in SetData +++
      struct STATUS_TYPE stp[L_MAX_STATUS_TYPES];
      int qstp;

      ABC_ORDER abc_order;
      int help_item_num;      // set in SetHelpItem

    private:


      int left,top,right,bottom; // set in Set**Corner
      int direction;             // set in SetDirection
	      // L_HOR L_VERT
      int hor_otstup;          // set in SetMargins
      int vert_otstup;         // set in SetMargins
      int hor_interval;        // set in  SetColumns
	     // �� ��砫� �⮫�� �� ��砫� ᫥���饣� �⮫��
      int vert_interval;       // set in SetMargins
	     // �� ������ �p����� ��p��� �� ��p孥��p����� ᫥���饩

//      char *strings[L_MAX_STRINGS];       // set in SetData //+++
      char **strings;       // set in SetData //+++

      int border_type;         // set in SetBorder
      int border_color;        // set in SetBorder

      int max_string_length;

      int insert_flags[L_MAX_INS_FLAGS];
      int slide_bar; // 0 - ���, 1 - ����

      int empty_list_allowed;

   };


class VIEW_LIST: public LIST
   {
   public:
      VIEW_LIST::VIEW_LIST(int size=L_MAX_STRINGS):LIST(size) {}
      ~VIEW_LIST() {}
      int Monitor();
      int Manage(int key, int mx, int my);
   };


class ENTER_LIST: public LIST
   {
   public:
//      ENTER_LIST() { curstring=-1; high_ground_color=-1; high_text_color=-1; }
      ENTER_LIST::ENTER_LIST(int size=L_MAX_STRINGS):LIST(size)
	     { curstring=-1; high_ground_color=-1; high_text_color=-1; }
      void SetColors(int g, int t, int hg, int ht)
		       { LIST::SetColors(g,t); high_ground_color=hg;  high_text_color=ht;  }
      void SetStatusType(char v, int c, int hc);

      int Monitor();
      int IfMyKey(int key, int mx, int my);
      int Manage(int key, int mx, int my);

      void DrawList() { LIST::DrawList(); curstring=0; }
      void DrawScreen() { LIST::DrawScreen(); DrawStringFull(curstring,'V'); }
      void DrawScreen(int rezhim) { LIST::DrawScreen(); DrawStringFull(curstring,rezhim); }
		// rezhim == 'V', 'N'
      void GetStringField(int n, int *left, int *top, int *right, int *bottom);
      void DrawStringFull(int n, int rezhim);  // rezhim == 'N' ��� 'V' ��ଠ���/�뤥�����
      int  IfMyMouse(int x, int y);
      int  DefineStringColor(int n, char rezhim);

      ~ENTER_LIST() {}
   private:
      void IfMyMouse(int n, int *x, int *y);
      int  LineOfString(int n);
      int  LineOfCurstring() { return LineOfString(curstring); }
      int  IfStringIsOnPage(int n);



   private:
      int curstring;
      int high_ground_color;   // set in SetColors
      int high_text_color;     // set in SetColors
   };


int ftoupper(int c);  // � GMENU.C