/* ***********************************************************************
                                BD.H
    ������:
        TExFont,

*********************************************************************** */

#if !defined(_BD_H)
#define _BD_H

extern TModule* resModule;
extern BOOL fShowTriangles;

// -----------------------------------------------------------------
// �����, ����������� �������� ������ � ��� �����.
// -----------------------------------------------------------------
class TExFont : public TFont {

    public :
        int lpSizes[256];    // ������� ���� ������������� ������.
        int SpaceLen;        // ������ �������.
        int chHeight;       // ������ �����.
        int chMaxWidth, chAveWidth;
        int chExternal, chInternal;
        int chAscent, chDescent;
        TColor Color;

        TExFont(LOGFONT*, TColor);
        ~TExFont() {}

        int     CountLengthInPixels(LPSTR txt); // ������� ����� ������ � ��������
        void    InitAllWithFonts();
};

// -----------------------------------------------------------------
//  ������� ������:
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
