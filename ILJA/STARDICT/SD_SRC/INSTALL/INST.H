/* ***********************************************************************
                               INST.H
    v1.0
    �������� �������� ������� ��� ������������ StarDict.
    ������:
        TInstWindow,
        TInstallApp.
    (c) �.�����.
*********************************************************************** */

#if !defined (_INST_H)
#define _INST_H

#define txtMainWindowClassName        "InstMainWindow"

extern TModule* resModule;

/* -----------------------------------------------------------------------
 ������� ������ ��� ������������ StarDict :
----------------------------------------------------------------------- */

class TInstWindow : public TFrameWindow { // ����, �������������� ������� ���� � ��.

        DWORD          idInst;  /*  Instance of app for DDEML       */
        HSZ            hszApplic;
        HSZ            hszTopic;
        HCONV          hConv;    /*Handle of established conversation*/

        TBitmap*  pSD, *pSDmono;

        void    AskLanguage();
        void    AskUserInfo(); // UserName, Directory, GroupName.
        void    AskDriveLetter();
        void    ShowPlans();
        void    ReadSignature();
        void    ChangeNumInstallation();
        void    CopyStarDictFiles();
        void    CopyOneFile(find_t*);
        void    CreateGroupAndElements();
        void    RegisterStarDict();
        void    ShowFinishDialog();
        void    AddFonts();

    public :

        TInstWindow(LPSTR);
        virtual ~TInstWindow();
        LPSTR GetClassName() { return txtMainWindowClassName; };

        void GetWindowClass(WNDCLASS&);
        void SetupWindow();
        void Paint(TDC&, BOOL, TRect&);

// *********** ������� ����:

        void CmQuit();
        void CmDoAllWork();
        void EvSize(uint sizeType, TSize& size);

        static HDDEDATA FAR PASCAL _export CallBack(WORD, WORD, HCONV, HSZ, HSZ,
                                                HDDEDATA, DWORD, DWORD);
        TProcInstance CallBackProc;

        DECLARE_RESPONSE_TABLE(TInstWindow);
};

class TInstallApp : public TApplication {

    public :

        TInstallApp(LPSTR t);//, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
        ~TInstallApp();

        void InitInstance();
        void InitMainWindow();
};

#endif // of _INST_H

