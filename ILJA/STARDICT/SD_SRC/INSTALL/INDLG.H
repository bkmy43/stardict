/* ****************************************************************************
                             INDLG.H
    v1.0
    Содержит описания классов диалоговых окон для инсталлятора.
    Классы:
        TAskLangDialog.
**************************************************************************** */

#if !defined (_INDLG_H)
#define _INDLG_H

class TAskInfoDialog : public TBaseDialog {

    public :

        TAskInfoDialog(TWindow*, TResId, TExFont*, TResId bmp);
        virtual void SetupWindow();
        BOOL DiskLargeEnough();

        void CmOk();

        DECLARE_RESPONSE_TABLE(TAskInfoDialog);
};

class TAskLangDialog : public TBaseDialog {

    public :

        TAskLangDialog(TWindow*, TResId, TExFont*, TResId bmp);
        virtual void SetupWindow();

        void CmOk();

        DECLARE_RESPONSE_TABLE(TAskLangDialog);
};

class TAskDriveLetterDialog : public TBaseDialog {

    public :

        TAskDriveLetterDialog(TWindow*, TResId, TExFont*, TResId bmp);
        virtual void SetupWindow();

        void CmOk();

        DECLARE_RESPONSE_TABLE(TAskDriveLetterDialog);
};

#endif


