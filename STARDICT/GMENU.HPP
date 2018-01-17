
/* eventtype */
const int E_KEY=0;   // нажата клавиша
const int E_LEFT=-1;  // нажата левая кнопка мыши
const int E_RIGHT=-2; // нажата пpавая кнопка мыши

const int MAX_LOG_ITEMS=100;

const int LI_ACTIVATED=-1;
const int LI_IS_ACTIVE=-2;

/* MenuStatus */
const char MS_CURRENT='C';
const char MS_VISUAL='V';
const char MS_PASSIVE='P';

/* param для GMMENU::Operate */
const char ACTIVATE='C';
const char ASK='A';
const char MANAGE='M';

/* rezhim для FindNewItem */
const int NEXT=0;
const int PREV=1;
const int LAST=2;
const int FIRST=3;

class GMEVENT
   {
   public:
      GMEVENT() { eventtype=-1; x=-1; y=-1; }
      void SetEvent(int key) { eventtype=E_KEY; x=key; y=-1; }
      void SetEvent(int button, int xx, int yy) { eventtype=button; x=xx; y=yy; }
      GMEVENT& WaitEvent();
      int Type() { return eventtype; }
      int X() { return x; }
      int Y() { return y; }
      ~GMEVENT() {}
   private:
      int eventtype;
      int x;   // ключ пpи KEY_PRESSED или кооpдината по х для мыши
      int y;   // кооpдината по у для мыши
   };

class GMMENU
   {
   public:
      GMMENU();
      int Operate(int mhandler);
      int Operate(char param, GMEVENT& e);
      int CloseLastMenu();
      ~GMMENU() {}

   private:
      int MenuLog[MAX_LOG_ITEMS];
      int q;
      char LastCurrent; // 'Y' 'N'
      char PullDown; // 'Y' 'N'

      /*++++++++++++++++++++++++++*/
      int OperateNotLast(int mhandler, GMEVENT& e);
      int OperateLastNotCurrent(int mhandler, GMEVENT& e);
      int OperateLastCurrent(int mhandler, GMEVENT& e);
      int ManageItem(int mhandler, int itemhandler);
      int IfMenuIsOwner(int mhandler, char menustatus, GMEVENT& e);
      int FindOwner(GMEVENT& e);
   };
