/* **************************************************************************
                                 EXCEPT.C
    v2.0
    Содержит реализацию классов исключений, выбрасываемых интерфейсными
    функциями StarDict.
    Классы:
    (c) В.Перов.
************************************************************************** */

#include <windows.h>
#include <stdio.h>

#pragma hdrstop

#include "ex.h"

// **************************************************************************
// *** class TSTDError :
// **************************************************************************
TSTDError :: TSTDError(char* txtFN, int nL, TSTDCode CC, int fC,
                       char* txtEC)

{
    txtFileName = txtFN;
    txtErrorMessage = NULL;
    txtErrorClass = txtEC;

    numLine = nL;
    CCode = CC;
    fCritical = fC;
}
int TSTDError :: ProcessException()
{
    char buf[1000];
    txtErrorMessage = GetErrorMessage();

    sprintf(buf, "%s (error code = %3d)\n"
                 "in file \"%s\", line number = %4d.\n\n"
                 "Please, write down this information and apply to StarDict team.",

            txtErrorMessage, CCode, txtFileName, numLine);

    if (fCritical) {
        ::MessageBox(NULL, buf, txtErrorClass, MB_OK | MB_ICONSTOP);
        return IDCANCEL;
    }
    else {
        return ::MessageBox(NULL, buf, txtErrorClass, MB_OKCANCEL | MB_ICONSTOP);
    }
}
char* TSTDError :: GetErrorMessage()
{
    return "AAAAAAAAAaaaaaaaaaaaaaahhhhh!!";
}

