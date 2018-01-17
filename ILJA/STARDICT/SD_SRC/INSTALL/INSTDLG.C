/* *************************************************************************
                                INSTDLG.C
    v1.0
    Реализация классов окон диалога в инсталляторе.
    Классы:
        TAskLangDialog.
************************************************************************* */

#include <windows.h>
#include <owl\gdiobjec.h>
#include <owl\dialog.h>

#pragma hdrstop

#include "instdef.h"
#include "code.h"
#include "ex.h"
#include "bd.h"
#include "indlg.h"

extern int stdLanguage;

extern char    UserName[100],
               SDDirectory[256],
               SDGroupName[100],
               RegNum[100];

extern int     Drive, NumInst;

DEFINE_RESPONSE_TABLE1(TAskLangDialog, TBaseDialog)
    EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TAskDriveLetterDialog, TBaseDialog)
    EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TAskInfoDialog, TBaseDialog)
    EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

// ***************************************************************************
// *** class TAskLangDialog :
// ***************************************************************************
TAskLangDialog :: TAskLangDialog(TWindow* parent, TResId id, TExFont* tip, TResId bmp)
             : TBaseDialog(parent, id, tip, bmp, 1)
{
    stdLanguage = LANG_RUSSIAN;
}
void TAskLangDialog :: SetupWindow()
{
    TBaseDialog :: SetupWindow();

    CheckDlgButton(idRussianLang, stdLanguage==LANG_RUSSIAN);
    CheckDlgButton(idEnglishLang, stdLanguage==LANG_ENGLISH);
}
void TAskLangDialog :: CmOk()
{
    if (IsDlgButtonChecked(idRussianLang) == 1)
         stdLanguage = LANG_RUSSIAN;
    else stdLanguage = LANG_ENGLISH;

    TBaseDialog :: CmOk();
}
// ***************************************************************************
// *** class TAskDriveLetterDialog :
// ***************************************************************************
TAskDriveLetterDialog :: TAskDriveLetterDialog(TWindow* parent, TResId id, TExFont* tip, TResId bmp)
             : TBaseDialog(parent, id, tip, bmp)
{
    Drive = 0;
}
void TAskDriveLetterDialog :: SetupWindow()
{
    TBaseDialog :: SetupWindow();

    CheckDlgButton(idDriveA, Drive==0);
    CheckDlgButton(idDriveB, Drive==1);
}
void TAskDriveLetterDialog :: CmOk()
{
    if (IsDlgButtonChecked(idDriveA) == 1)
         Drive = 0;
    else Drive = 1;

    TBaseDialog :: CmOk();
}
// ***************************************************************************
// *** class TAskInfoDialog :
// ***************************************************************************
TAskInfoDialog :: TAskInfoDialog(TWindow* parent, TResId id, TExFont* tip, TResId bmp)
             : TBaseDialog(parent, id, tip, bmp)
{
    strcpy(UserName, "");
    strcpy(SDDirectory, "C:\\STARDICT\\");
    strcpy(SDGroupName, "StarDict 2.0");
}
void TAskInfoDialog :: SetupWindow()
{
    TBaseDialog :: SetupWindow();

    SetDlgItemText(idUserName, UserName);
    SetDlgItemText(idSDDirectory, SDDirectory);
    SetDlgItemText(idGroupName, SDGroupName);
    SetDlgItemInt(idNumInst, NumInst);
    SetDlgItemText(idRegNum, RegNum);

//    ::EnableWindow(GetDlgItem(idNumInst), FALSE);
//    ::EnableWindow(GetDlgItem(idRegNum), FALSE);
}
void TAskInfoDialog :: CmOk()
{
    GetDlgItemText(idUserName, UserName, sizeof(UserName));
    GetDlgItemText(idSDDirectory, SDDirectory, sizeof(SDDirectory));
    GetDlgItemText(idGroupName, SDGroupName, sizeof(SDGroupName));

    while (SDDirectory[strlen(SDDirectory)-1] == '\x20')
        SDDirectory[strlen(SDDirectory)-1] = '\0';
    if (SDDirectory[strlen(SDDirectory)-1] != '\\')
        strcat(SDDirectory, "\\");
    SetDlgItemText(idSDDirectory, SDDirectory);

    if (strlen(UserName) < CODE_MIN_STRINGLEN) {
        TSTDError s("", __LINE__, IC_USERNAMETOOSHORT, 0);
        s.ProcessException();
        ::SetFocus(GetDlgItem(idUserName));
        SendDlgItemMessage(idUserName, EM_SETSEL, (WPARAM)0, (LPARAM)strlen(UserName));
        return;
    }

    if (DiskLargeEnough() != TRUE) {
        TSTDError s("", __LINE__, IC_TOOLOWDISKSPACE, 0);
        s.ProcessException();
        ::SetFocus(GetDlgItem(idSDDirectory));
        SendDlgItemMessage(idSDDirectory, EM_SETSEL, (WPARAM)0, (LPARAM)strlen(SDDirectory));
        return;
    }

    TBaseDialog :: CmOk();
}
BOOL TAskInfoDialog :: DiskLargeEnough()
{
    dfree s;
    char one_little_big_char = (unsigned char) (*SDDirectory);
    if (one_little_big_char > 'Z') {
        one_little_big_char -= ('z'-'Z'); // 32
    }
//    getdfree((*SDDirectory - 'A')+1, &s);
    getdfree(( one_little_big_char - 'A')+1, &s);
    if (((s.df_avail*( s.df_sclus*s.df_bsec/1024)) / 1024) < 2)
         return FALSE;
    else return TRUE;
}

