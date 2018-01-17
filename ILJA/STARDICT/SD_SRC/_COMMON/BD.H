/* ***********************************************************************
                                BD.H
    Классы:
        TExFont,

*********************************************************************** */

#if !defined(_BD_H)
#define _BD_H

extern TModule* resModule;
extern BOOL fShowTriangles;

// -----------------------------------------------------------------
// Класс, описывающий свойства шрифта и сам шрифт.
// -----------------------------------------------------------------
class TExFont : public TFont {

    public :
        int lpSizes[256];    // размеры букв используемого шрифта.
        int SpaceLen;        // ширина пробела.
        int chHeight;       // высота буквы.
        int chMaxWidth, chAveWidth;
        int chExternal, chInternal;
        int chAscent, chDescent;
        TColor Color;

        TExFont(LOGFONT*, TColor);
        ~TExFont() {}

        int     CountLengthInPixels(LPSTR txt); // подсчет длины текста в пикселах
        void    InitAllWithFonts();
};

// -----------------------------------------------------------------
//  Базовый диалог:
// -----------------------------------------------------------------
class TBaseDialog : public TDialog {

        TExFont* tipFont;
        TBitmap* triangle;

    public :

        TBaseDialog(TWindow*, TResId, TExFont*, TResId bmp, int fUseGlobalModule = 0);
        virtual ~TBaseDialog();
        virtual void SetupWindow();
        virtual void EvPaint();
        void CmHelp();

        DECLARE_RESPONSE_TABLE(TBaseDialog);
};


#endif
