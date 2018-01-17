/* Параметры для Box() */
enum {B_ONE_BUTTON,B_TWO_BUTTON};
enum {B_ERROR,B_MESSAGE};

int Box(int type, int pal,
	      char *s0=0, char *s1=0, char *s2=0, char *s3=0, char *s4=0,
	      char *s5=0, char *s6=0, char *s7=0, char *s8=0, char *s9=0);


const int B_MAX_MES_STRINGS=10;
const int B_MAX_BOX_KEYS=10;

/* значение additional key */
const int NO_KEY=-1;

/* значение hor/vert _justifying_type */
const int CENTER_JUSTIFIED=0;
const int EDGE_JUSTIFIED=1;

/* значение ifsave в Draw */
const int B_SAVE=0;
const int B_NOT_SAVE=1;


class BOX_KEY
   {
   public:
      BOX_KEY();
	 void SetLTBCorner(int l, int t, int b);
	 void SetKey(char *s, int h, int akey);
	 int  GetLength();
	 int  IfMyKey(int r, int mx, int my);
	 void Draw(int gcol, int textcol, int highletcol, int shcol, int hor_otstup);
      ~BOX_KEY() {}
   private:
      int left,top,right,bottom;
      char *str;
      int highletnum;
      int additional_key;
   };

class BOX
   {
   public:
      BOX();
	void InitBox();
	   // остается все кpоме стpок
	void DeleteKeys() { qkeys=0; allkeyslength=0; }
	  // удаляются только KEYS

	void SetHorizontal(int horjust, int xx, int otstup, int keyjust, int keyminint, int mesjust);
	void SetVertical(int vertjust, int yy, int topmesotstup, int mesint, int keybototstup);
	void SetKeyMargins(int hor, int vert);
	void SetMessageString(char *s);
	void SetColors(int gcol, int bcol, int mcol);
	void SetShadow(int shhor, int shvert, int shcol);
	void SetBoxKey(char *s, int hletnum, int akey);
		  // отдельно кля каждого
	void SetBoxKeyColors(int gcol, int tcol, int hletcol, int shcol,
		     int hgcol, int htcol);  // общие для всех


	int  Draw(int ckey, int ifsave);


      ~BOX() {}


  private:
	void DrawKey(int n);
	int  DefineMaxMessageLength();
	void CountBoxParams();
	void DrawMessage(int n);
	int  DefineKeyLeft(int n);
	int  Monitor();

   private:
      /*  SetMessageString */
      char *mes[B_MAX_MES_STRINGS];
      int qmes;

      /* SetBoxKey */
      BOX_KEY bk[B_MAX_BOX_KEYS];
      int qkeys;

      /* SetHorizontal */
      int hor_justifying_type;
      int x;
      int hor_otstup;
      int key_justifying_type;
      int key_min_interval;
      int mes_justifying_type;

      /* SetVertical */
      int vert_justifying_type;
      int y;
      int top_mes_otstup;
      int mes_interval;
      int key_bottom_otstup;

       /* SetKeyMargins */
      int hor_key_otstup;
      int vert_key_otstup;

      int left,top,right,bottom; // вычисляются в Draw
      int curkey;
      int allkeyslength;

      /* SetColors */
      int groundcol;
      int bordcol;
      int mescol;

      /* SetShadow */
      int shadow_hor_otstup;
      int shadow_vert_otstup;
      int shadowcol;

      /* SetBoxKeyColors */
      int key_groundcol;
      int key_textcol;
      int key_highletcol;
      int key_shadowcol;
      int key_highgroundcol;
      int key_hightextcol;
   };