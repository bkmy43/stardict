/* **************************************************************************
                                 INSTALL.C
    v2.0
    Содержит реализацию инсталлятора StarDict и функцию WinMain.
    Классы:
        TInstallApp.
    (c) В.Перов.
************************************************************************** */

#include <windows.h>
#include <owl\gdiobjec.h>
#include <owl\decframe.h>
#include <owl\dialog.h>
#include <owl\profile.h>
#include <ddeml.h>
#include <dde.h>
#include <io.h>
#include <fcntl.h>
#include <dir.h>
#include <mem.h>
#include <stdio.h>

#pragma hdrstop

#include "sd_defs.h"
#include "instdef.h"
#include "code.h"
#include "sign.h"
#include "inst.h"
#include "ex.h"
#include "bd.h"
#include "indlg.h"

#define   STARDICT_VERSION     0x0002
#define   STEP_RECT                10
#define   NUM_RECT               (1000/STEP_RECT)
#define   BUF_SIZE             1500

#define  txtInstallPrefix       "INST"
#define  SD_X   30
#define  SD_Y   30

#define  stdFont_1      "benguiat.ttf"

extern "C" int AsmReadSignature(int drive, int track, int head);
extern "C" int AsmWriteSignature(int drive, int track, int head);

TSignature sign;
char buf_512[513];

char    UserName[100] = "",
        SDDirectory[256] = "C:\\STARDICT",
        SDGroupName[100] = "StarDict 2.0",
        RegNum[100] = "";


int     Drive = 0;
int     NumInst = 0;

extern int stdLanguage;

// Указатель на модуль (dll) языковых ресурсов :
TModule* resModule = NULL;
// Указатель на MainWindow:
TInstWindow* MWindow = NULL;

DEFINE_RESPONSE_TABLE1(TInstWindow, TFrameWindow)
    EV_WM_SIZE,
    EV_COMMAND(cmDoAllWork, CmDoAllWork),
    EV_COMMAND(cmQuit, CmQuit),
END_RESPONSE_TABLE;

// *************************************************************
// *** class TInstWindow :
// *************************************************************
HDDEDATA FAR PASCAL _export TInstWindow :: CallBack( WORD wType,
        WORD /*wFmt*/, HCONV /*hConvX*/, HSZ /*hsz1*/,
        HSZ /*hsz2*/, HDDEDATA /*hData*/, DWORD /*dwData1*/,
        DWORD /*dwData2*/ )
{
   switch ( wType )
   {
      case XTYP_DISCONNECT:
         MWindow->hConv = (HCONV)NULL;
//         HandleOutput ( "The server forced a disconnect." );
         return ( (HDDEDATA) NULL );

      case XTYP_ERROR:
         break;

      case XTYP_XACT_COMPLETE:
         // compare transaction identifier, indicate transaction complete
         break;

   }
   return ( (HDDEDATA) NULL );
}
TInstWindow :: TInstWindow(LPSTR title)
            : TFrameWindow(NULL, NULL, NULL),
              TWindow(NULL, title),
              CallBackProc((FARPROC)CallBack)
{
    Attr.Style |= WS_TABSTOP;
    idInst = 0;
    pSD = new TBitmap(GetApplication()->GetInstance(), bmpSD);
    pSDmono = new TBitmap(GetApplication()->GetInstance(), bmpSDmono);
}
TInstWindow :: ~TInstWindow()
{
    if (pSD) delete pSD;
    if (pSDmono) delete pSDmono;
    if ( hConv != (HCONV)NULL ) {
        DdeDisconnect ( hConv );
        hConv = (HCONV)NULL;
    }
    DdeFreeStringHandle ( idInst, hszApplic );
    DdeFreeStringHandle ( idInst, hszTopic );
}
void TInstWindow :: SetupWindow()
{
    TFrameWindow :: SetupWindow();

    DdeInitialize ( (LPDWORD)&idInst, (PFNCALLBACK)(FARPROC)CallBackProc,
                     APPCMD_CLIENTONLY, 0L ) ;

    hszApplic = DdeCreateStringHandle ( idInst, "PROGMAN", CP_WINANSI );
    hszTopic = DdeCreateStringHandle ( idInst, "PROGMAN", CP_WINANSI );

//    ::SetWindowPos(HWindow, (HWND)-1, 0, 0, 0, 0,
//                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); // always topmost

    PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0L);
    PostMessage(WM_COMMAND, cmDoAllWork, 0L);
}
void TInstWindow :: EvSize(uint sizeType, TSize& size)
{
    if (sizeType == SIZE_MAXIMIZED) {
        Attr.W = size.cx;
        Attr.H = size.cy;
    }
    TFrameWindow :: EvSize(sizeType, size);
}
void TInstWindow :: GetWindowClass(WNDCLASS& one)
{
    TFrameWindow :: GetWindowClass(one);
    one.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
}
void TInstWindow :: Paint(TDC& dc, BOOL b, TRect& r)
{
    TFrameWindow :: Paint(dc, b, r);

    for (int i=0; i<NUM_RECT; ++i)
        dc.FillRect(0, i*STEP_RECT, Attr.W, i*STEP_RECT+STEP_RECT,
                    TColor(0, 0, 90+i*((255-90)/NUM_RECT)));

    TMemoryDC memdc(dc);
    memdc.SelectObject(*pSDmono);
    dc.BitBlt(SD_X, SD_Y, pSDmono->Width(), pSDmono->Height(), memdc, 0, 0, SRCAND);
    memdc.SelectObject(*pSD);
    dc.BitBlt(SD_X, SD_Y, pSD->Width(), pSD->Height(), memdc, 0, 0, SRCPAINT);
    memdc.RestoreBitmap();
}
void TInstWindow :: CmQuit()
{
    PostMessage(WM_COMMAND, CM_EXIT, 0);
}
// ----------------------------------------------------- commands :
void TInstWindow :: CreateGroupAndElements()
{
    hConv = DdeConnect(idInst, hszApplic, hszTopic, 0);

    char buff[400];

    sprintf(buff, "[CreateGroup(\"%s\",0)]"
                  "[AddItem(%sstardict.exe,StarDict 2.0 for Windows)]"
                  "[AddItem(%sstardict.hlp,StarDict 2.0 for Windows help file)]"
                  "[AddItem(%sreadme.txt,Read me file)]",
                   SDGroupName, SDDirectory, SDDirectory, SDDirectory);

    DdeClientTransaction((unsigned char*)buff, strlen(buff)+1, hConv,
                          NULL, CF_TEXT, XTYP_EXECUTE, 1000, 0);


    if (hConv) {
        DdeDisconnect(hConv);
        hConv = (HCONV)NULL;
    }
}
void TInstWindow :: AddFonts()
{
    char path[160];
    sprintf(path, "%s%s", SDDirectory, stdFont_1);
    AddFontResource(path);
}
void TInstWindow :: CmDoAllWork()
{
    try {
        AskLanguage();
        ShowPlans();
        AskDriveLetter();
        ReadSignature();

        AskUserInfo(); // UserName, Directory, GroupName.
        CopyStarDictFiles();
        CreateGroupAndElements();

        ChangeNumInstallation(); // to diskette
        RegisterStarDict();
        AddFonts();
        BringWindowToTop();
        ShowFinishDialog();

        PostMessage(WM_COMMAND, CM_EXIT, 0);
    }
    catch (TSTDError s) {
        s.ProcessException();
        PostMessage(WM_COMMAND, CM_EXIT, 0);
    }
}
void TInstWindow :: ShowFinishDialog()
{
    TBaseDialog*  d = new TBaseDialog(this, dlgFinish, NULL, bmpTriangle);
    d->Execute();
}
void TInstWindow :: AskLanguage()
{
    TAskLangDialog* p = new TAskLangDialog(this, dlgAskLang, NULL, bmpTriangle);
    if (p->Execute() != IDOK)
        throw TSTDError("", __LINE__, IC_CANCELINSTALL, 0);

    try {
        switch (stdLanguage) {
            case LANG_RUSSIAN :  resModule = new TModule("instrus.dll", TRUE);
                                 break;

            case LANG_ENGLISH :
            default :            resModule = new TModule("insteng.dll", TRUE);
                                 break;
        }
    }
    catch (TModule::TXInvalidModule) {
        throw TSTDError("V\\" __FILE__, __LINE__, EC_NODLL, 0);
                        /*"Cannot find resource library"*/
    }
}
void TInstWindow :: AskUserInfo()
{
    TAskInfoDialog* p = new TAskInfoDialog(this, dlgAskInfo, NULL, bmpTriangle);
    if (p->Execute() != IDOK)
        throw TSTDError("", __LINE__, IC_CANCELINSTALL, 0);
    UpdateWindow();
}
void TInstWindow :: AskDriveLetter()
{
    TAskDriveLetterDialog* p = new TAskDriveLetterDialog(this, dlgAskDriveLetter, NULL, bmpTriangle);
    if (p->Execute() != IDOK)
        throw TSTDError("", __LINE__, IC_CANCELINSTALL, 0);
    UpdateWindow();
    dfree free_str;
    getdfree(Drive+1, &free_str);
}
void TInstWindow :: ShowPlans()
{
    TBaseDialog* d = new TBaseDialog(this, dlgPlans, NULL, bmpTriangle);
    if (d->Execute() != IDOK)
        throw TSTDError("", __LINE__, IC_CANCELINSTALL, 0);
}
void TInstWindow :: ChangeNumInstallation()
{
    --sign.numInst;
    --NumInst;
    sign.CRC = CountCRCword((unsigned char*)&sign.progVersion, sizeof(TSignature)-2-2);
    AsmWriteSignature(Drive, TRACK_NUMBER_first, HEAD_NUMBER_first);
}
void TInstWindow :: CopyOneFile(find_t* FoundFile)
{
    char name[15];
    strcpy(name, FoundFile->name);
    strupr(name);
    if (strstr(name, txtInstallPrefix) == name)
        return;

    char* buffer = new char[BUF_SIZE];
    char fname_in[40], fname_out[200];

    strcpy(fname_out, SDDirectory);
    strcat(fname_out, FoundFile->name);
    strcpy(fname_in, FoundFile->name);

    char ch_ = FoundFile->name[strlen(FoundFile->name)-2];
    char ch__ = FoundFile->name[strlen(FoundFile->name)-3];
    switch (ch_) {
        case 'X':       if (ch__ == 'E')
                             ch_ = 'E';
                        else ch_ = 'T';
                        break;
        case 'T':       ch_ = 'F'; break;
        case 'A':       ch_ = 'T'; break;
        case 'L':       if (ch__ == 'D')
                             ch_ = 'L';
                        else ch_ = 'P';
                        break;
        case 'O':       ch_ = 'S'; break;
        default:        ch_ = FoundFile->name[strlen(FoundFile->name)-1]; break;
    }
    fname_out[strlen(fname_out)-1] = ch_;

    int fout = _rtl_creat(fname_out, 0),
        fin  = open(fname_in, O_RDONLY | O_BINARY);

    if ( fout < 0 || fin <0)
        throw TSTDError("", __LINE__, EC_DISKERROR, 0);

    int bytes_read = 0;
    do {
        bytes_read = read(fin, buffer, BUF_SIZE);
        if (bytes_read <0) throw TSTDError("", __LINE__, EC_DISKERROR, 0);
        if (bytes_read ==0 ) break;
        int wrt = write(fout, buffer, bytes_read);
        if (wrt <0) throw TSTDError("", __LINE__, EC_DISKERROR, 0);
    } while (bytes_read >0);

    ftime f; getftime(fin, &f); setftime(fout, &f);
    close(fin);
    close(fout);
    delete buffer;
}
void TInstWindow :: CopyStarDictFiles()
{
    TBaseDialog* SDDialogBox = new TBaseDialog(this, dlgCopying, NULL, bmpTriangle);
    SDDialogBox->Create();

    char dr[sizeof(SDDirectory)];
    strcpy(dr, SDDirectory);
    dr[strlen(dr)-1] = '\0';
    mkdir(dr);
    char filename[40];
    strcpy(filename, "*.*");
    find_t FoundFile;
    _dos_findfirst(filename, _A_NORMAL | _A_RDONLY | _A_ARCH, &FoundFile);

    CopyOneFile(&FoundFile);
    while ( _dos_findnext(&FoundFile) == 0)
        CopyOneFile(&FoundFile);

// decoding stardict.in_ :
    char* buffer = new char[BUF_SIZE];
    char* buffer2 = new char[BUF_SIZE];

    char fname_in[200], fname_out[200];
    strcpy(fname_in, SDDirectory);
    strcat(fname_in, "stardict.in_");
    strcpy(fname_out, SDDirectory);
    strcat(fname_out, "stardict.ini");

    int fout = _rtl_creat(fname_out, 0),
        fin  = open(fname_in, O_RDONLY | O_BINARY);

    if (fin < 0 || fout < 0)
        throw TSTDError("", __LINE__, EC_DISKERROR, 0);

    int bytes_read = 0;
    do {
        bytes_read = read(fin, buffer, BUF_SIZE);
        if (bytes_read <0)
            throw TSTDError("", __LINE__, EC_DISKERROR, 0);
        if (bytes_read ==0 ) break;
        UnCodeStringShifting(buffer2, buffer, bytes_read);
        int wrt = write(fout, buffer2, bytes_read);
        if (wrt <0) throw TSTDError("", __LINE__, EC_DISKERROR, 0);
    } while (bytes_read >0);

    ftime f; getftime(fin, &f); setftime(fout, &f);
    close(fin); close(fout);
    remove(fname_in);
    flushall();

    delete buffer;
    delete buffer2;
    delete SDDialogBox;
    UpdateWindow();
}
void TInstWindow :: RegisterStarDict()
{
    char section[20], key[10], ideal[10];

    strcpy(section, "Com");  strcpy(key, "V"); strcpy(ideal, "0x");
    strcat(section, "pat");  strcat(key, "B"); strcat(ideal, "01");
    strcat(section, "ibi");  strcat(key, "V"); strcat(ideal, "11");
    strcat(section, "lity"); strcat(key, "I");

    ::WriteProfileString(section, key, ideal);

    char pt[200];
    strcpy(pt, SDDirectory);
    strcat(pt, txtIniFileName);

    TProfile pr(txtIniSectionAppl, pt);
    pr.WriteString(txtIniSectionAppl_UN, UserName);
    pr.WriteString(txtIniSectionAppl_RN, RegNum);
    char unc[100], rnc[100];;
    CodeRegNum(unc, RegNum, sizeof(unc));
    CodeUserName(rnc, UserName, sizeof(rnc));

    pr.WriteString(txtIniSectionAppl_RNcoded, unc);
    pr.WriteString(txtIniSectionAppl_UNcoded, rnc);
    pr.WriteInt(txtIniSectionAppl_Inst, NumInst);
    pr.WriteInt(txtIniSectionAppl_Lang, stdLanguage);
}
void TInstWindow :: ReadSignature()
{
    memset(&sign, 0, sizeof(TSignature));
    memset(buf_512, 0, sizeof(buf_512));

    if (AsmReadSignature(Drive, TRACK_NUMBER_first, HEAD_NUMBER_first) != 0)
        throw TSTDError("", __LINE__, EC_DISKERROR, 0);
    memcpy(&sign, buf_512, sizeof(TSignature));

    int CRC = CountCRCword((unsigned char*)&sign.progVersion, sizeof(TSignature)-2-2);
    if (CRC != sign.CRC)
        throw TSTDError("", __LINE__, IC_WRONGSIGNATURE, 0);
    if (sign.chProgram[0] != 'S' || sign.chProgram[1] != 'D')
        throw TSTDError("", __LINE__, IC_WRONGSIGNATURE, 0);
    if (sign.progVersion != STARDICT_VERSION)
        throw TSTDError("", __LINE__, IC_WRONGVERSION, 0);
    if (sign.numInst <= 0)
        throw TSTDError("", __LINE__, IC_NOMOREINST, 0);
    char RNcoded[100];
    CodeRegNum(RNcoded, sign.RegNum, sizeof(RNcoded));
    if (strcmp(RNcoded, sign.RegNumCoded) != 0)
        throw TSTDError("", __LINE__, IC_WRONGSIGNATURE, 0);

    strcpy(RegNum, sign.RegNum);
    NumInst = sign.numInst;
}
// ********************************************************************
// *** class TInstallApp :
// ********************************************************************
TInstallApp :: TInstallApp(LPSTR t)/*, HINSTANCE hInstance,
                             HINSTANCE hPrevInstance, LPSTR lpCmdLine,
                             int nCmdShow)*/
              : TApplication(t)//, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
{
    MWindow = NULL;
}
TInstallApp :: ~TInstallApp()
{
    if (resModule) delete resModule; resModule = NULL;
}
void TInstallApp :: InitMainWindow()
{
    EnableCtl3d(TRUE);

    MainWindow = new TInstWindow("StarDict v2.0");
    MainWindow->SetIcon(this, icoInstall);
}
void TInstallApp :: InitInstance()
{
    if (hPrevInstance) { // вытаскиваем на поверхность предыдущее приложение
            HWND hwnd = FindWindow(txtMainWindowClassName, NULL);
            if (hwnd) {
                hwnd = GetLastActivePopup(hwnd);
                BringWindowToTop( hwnd );
                ShowWindow(hwnd, SW_RESTORE);
            }
            PostAppMessage(GetCurrentTask(), WM_QUIT, 0, 0); // второе (это) приложение закрываем
    }
    else {
        TApplication :: InitInstance();
        randomize();
    }
}
// **************************************************************************
int OwlMain(int, char*[])
{
    TInstallApp ap ("InstallApp");
    int result = ap.Run();

    if (resModule) delete resModule; resModule=NULL;

    return result;
}

