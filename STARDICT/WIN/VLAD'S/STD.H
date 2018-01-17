/* ***********************************************************************
                               STD.H
    v2.0
    Содержит описания классов для базовых окон StarDict.
    (c) В.Перов.
*********************************************************************** */

#if !defined (_STD_H)
#define _STD_H

// сообщение для перерисовки всех дочерних окон (шрифт изменился)
#define     wmInvalidate        (WM_USER + 1)

// Используется при регистрации главного окна StarDict и для
// предотвращения запуска второго приложения.
#define     txtMainWindowClassName      "StarDictMainWin"

// Имя файла, хранящего опции для StarDict.
#define     txtIniFileName              "stardict.ini"
// *** Секции файла опций :
// Секция размера и положения на экране:
#define     txtIniSectionSize           "Size"
#define     txtIniSectionSize_Max          "Max"
#define     txtIniSectionSize_X            "X"
#define     txtIniSectionSize_Y            "Y"
#define     txtIniSectionSize_W            "Width"
#define     txtIniSectionSize_H            "Height"
// Секция описания шрифтов окон ссылок:
#define     txtIniSectionFont           "Font"
#define     txtIniSectionFont_Face         "Face"
#define     txtIniSectionFont_Charset      "Charset"
#define     txtIniSectionFont_Size         "Point"
#define     txtIniSectionFont_Color        "Color"

/* -----------------------------------------------------------------------
 Оконные классы для StarDict :
----------------------------------------------------------------------- */
class TSmartButton : public TButton { // Кнопка с улучшенным шрифтом

    public :

        TSmartButton(TWindow* parent, int id, LPSTR title,
                     int x, int y, int w, int h, BOOL isDefault = FALSE)
                     : TButton(parent, id, title, x, y, w, h, isDefault)
                     {}

        void SetupWindow() {
            SetWindowFont( (HFONT)::GetStockObject(ANSI_VAR_FONT), TRUE);
        }
};

class TStarDictClient;

struct TExArticle {
        LPSTR   Title;
        int     numArticle;
        LPSTR   article;

        TExArticle() {
                 numArticle = 0;
                 Title = "Main contents";
                 CreateString();
        }

        ~TExArticle() { delete article; }

    private :

        void CreateString();
};

class TTipButton : public TButtonGadget { // bitmap-кнопка с всплывающей подсказкой.

        static int lpSizes[256];    // размеры букв используемого шрифта.
        static int SpaceLen;        // ширина пробела.
        static int tipHeight;       // высота буквы.

        LPSTR   txtTip;             // текст подсказки.
        int     tipLength;          // длина подсказки в пикс., с учетом 2ух пробелов.
        TBitmap*   ScreenShot;      // фотография экрана при печати подсказки.
        TRect   rShot;              // координаты подсказки на экране.

        int     CountLengthInPixels(); // подсчет длины текста в пикселах
        void    GetScreenShot(TDC& dc, TRect& r);
        void    ReturnScreenShot(); 

    public :

        TTipButton(TResId resource, int id);
        ~TTipButton();

        static void InitAllWithFonts();

        virtual void MouseEnter(uint, TPoint&);
        virtual void MouseLeave(uint, TPoint&);
        virtual void LButtonDown(uint, TPoint&);
};

class TArticleWindow : public TMDIChild { // Окно, содержащее текст статьи.

    public :
        TExArticle* pArticle;

        TArticleWindow(TStarDictClient& parent, TExArticle* artLink);
        virtual ~TArticleWindow();

        void GetWindowClass(WNDCLASS&);
        void SetupWindow();
        virtual void Paint(TDC&, BOOL, TRect&);

        void EvSize(uint, TSize&);
        LPARAM EvInvalidate(WPARAM, LPARAM);

        DECLARE_RESPONSE_TABLE(TArticleWindow);
};

class TStarDictClient : public TMDIClient { // Окно, обрабатывающее команды меню и пр.

        friend class TArticleWindow;

    protected :

        TFont* pFont;
        TChooseFontDialog :: TData data;
        BOOL isWindowOpened;

        void WriteProfileFont();
        void ReadProfileFont();

    public :

        TStarDictClient();
        virtual ~TStarDictClient();

        void GetWindowClass(WNDCLASS&);
        void SetupWindow();
        virtual TArticleWindow* InitChild();

// *********** Команды меню:

        void CmAbout();
        void CmQuitDict();
        void CmShowContents();
        void CmShowOptions();
        void CmNewWindow();
        void CmCascade();
        void CmCloseAll();

        void CeCascade(TCommandEnabler&);
        void CeCloseAll(TCommandEnabler&);

        DECLARE_RESPONSE_TABLE(TStarDictClient);
};

class TSDecoratedMDIFrame : public TDecoratedMDIFrame { // Главное окно программы.

        int isMaximized;

    public :

        TSDecoratedMDIFrame(LPCSTR title, TResId id, TStarDictClient& client);
        ~TSDecoratedMDIFrame();

        virtual void SetupWindow();
        LPSTR GetClassName() { return txtMainWindowClassName; };

        void EvSize(uint sizeType, TSize& size);
        DECLARE_RESPONSE_TABLE(TSDecoratedMDIFrame);
};

class TStarDictApp : public TApplication {

        TStarDictClient* stdClient;
        TStatusBar* statusBar;
        TControlBar* controlBar;
        TTextGadget* sbtxtStatus;

        void InitializeBars(TDecoratedMDIFrame*);

    public :

        TStarDictApp(LPSTR t, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
                 : TApplication(t, hInstance, hPrevInstance, lpCmdLine, nCmdShow) {}

        TStarDictApp(LPSTR t) : TApplication(t) {}

        void InitInstance();
        void InitMainWindow();
};

#endif // of _STD_H

