/* **************************************************************************
                                 EXCEPT.C
    v2.0
    �������� ���������� ������� ����������, ������������� �������������
    ��������� StarDict.
    ������:
    (c) �.�����.
************************************************************************** */

#if defined(_Windows) || defined(__WIN32__)
#include <windows.h>
#endif

#include <stdio.h>

#pragma hdrstop

#include "ex.h"

int stdLanguage = LANG_ENGLISH;

// **************************************************************************
// *** class TSTDError :
// **************************************************************************
TSTDError :: TSTDError(char* txtFN, int nL, TSTDCode CC, int fC,
                       char* txtEC)

{
    txtFileName = txtFN;
    txtErrorMessage = NULL;
    txtErrorMessageRus = NULL;
    txtErrorClass = txtEC;

    numLine = nL;
    CCode = CC;
    fCritical = fC;
}

int TSTDError :: ProcessException()
{
    GetErrorMessage();
// ----------------------- Windows part:
#if defined(_Windows) || defined(__WIN32__)
    char buf[1000];
    int a;
    if (fCritical) {
        sprintf(buf, "%s (error code = %3d)\n"
                     "in file \"%s\", line number = %4d;\n\n"
                     "%s (��� ������ = %3d)\n"
                     "� ����� \"%s\", ������ = %4d.",
        txtErrorMessage,    CCode, txtFileName, numLine,
        txtErrorMessageRus, CCode, txtFileName, numLine);

        ::MessageBox(NULL, buf, txtErrorClass, MB_OK | MB_ICONSTOP);
        a = IDCANCEL;
    }
    else {
        char* c;
        if (stdLanguage == LANG_RUSSIAN) {
             sprintf(buf, "%s", txtErrorMessageRus);
             c = "������";
        }
        else {
            sprintf(buf, "%s", txtErrorMessage);
            c = "Error";
        }
        ::MessageBox(NULL, buf, c, MB_OK | MB_ICONSTOP);
        a = IDCANCEL;
    }
//    ::InvalidateRect(NULL, NULL, FALSE);
    return a;
}
// --------------------------- DOS part:
#else
    printf("\n* %s (error code = %3d)\n"
           "  in file \"%s\", line number = %4d;\n\n"
           "* %s (��� �訡�� = %3d)\n"
           "  � 䠩�� \"%s\", ����� ��ப� = %4d.",
    txtErrorMessage,    CCode, txtFileName, numLine,
    txtErrorMessageRus, CCode, txtFileName, numLine);

    return -1;
}
#endif  // of __WIN32__ || _Windows

void TSTDError :: GetErrorMessage()
{
    char* e, *r;
    switch (CCode) {

        case IC_TOOLOWDISKSPACE  :  r = "�� ��������� ���� ����� ������������ �����\n"
                                        "��� ��������� ���������.";
                                    e = "The disk You have choosen for installation\n"
                                        "is not large enough.";
                                    break;

        case IC_USERNAMETOOSHORT :  r = "�� ������� ������� �������� ��� ������������.";
                                    e = "You gave too short User Name.";
                                    break;

        case IC_NOMOREINST :        r = "���������� ����������� ����������� ���������.\n"
                                        "��� ������� ���������� ��� ���� ����� ���������.";
                                    e = "Sorry, the number of allowed installations is out."
                                        "You should buy another copy.";
                                    break;

        case IC_WRONGSIGNATURE  :   r = "�������� ��� ����������� �������� �������.";
                                    e = "Wrong key diskette.";
                                    break;

        case IC_WRONGVERSION  :     r = "�� ��������� ���������� StarDict ����� ������� ������.\n";
                                        "�������������� ������������� ���������� �����������.";
                                    e = "You are trying to install StarDict of later version.\n"
                                        "Use the corresponding installation program";
                                    break;

        case IC_CANCELINSTALL :     r = "����������� �������� �������������.";
                                    e = "Installation process is cancelled by user.";
                                    break;

        case EC_NOLICENSE     :     r = "�� ����������� �������������������� ������ ���������.";
                                    e = "This copy of the programm is not registered.";
                                    break;
        case EC_DISKERROR     :     r = "������ ��� ������ � ������";
                                    e = "Disk error";
                                    break;
        case EC_NODLL          :    r = "�� ������� ��������� ����� ��� ������ ���������.\n";
                                        "��������� � ������� ������ � ������� �������� ���������."
                                        "������ ���������� ������ ��������� � ����� READ.ME.";
                                    e = "Cannot find nessesary files needed for program.\n"
                                        "Please, check their existance in program work directory.\n"
                                        "You can find files list in file READ.ME.";
                                    break;
        case EC_WRONGUSERNAME   :   r = "��� ������������ ��������� ��������.\n"
                                        "����������, ����������������� ���������.";
                                    e = "Program user name is changed.\n"
                                        "Please, re-install programm.";
                                    break;
        case EC_WRONGREGNUM     :   r = "��������������� ����� ��������� �������.\n"
                                        "����������, ����������������� ���������.";
                                    e = "Program registration number is changed.\n"
                                        "Please, re-install programm.";
                                    break;

        case EC_ERRORFONTTAG    :   r = "��������� ������: �������� ������ ���� ����� ������";
                                    e = "Internal: bad article font tags format";
                                    break;

        case EC_NOSUCHARTICLE   :   r = "��������, � ������� �� ���������� ����� ������";
                                    e = "Sorry, there is no this article in dictionary";
                                    break;

        case EC_WRONGREGINFO:       e = "Program registration information is corrupted.\n"
                                        "Please, re-install programm.";
                                    r = "";
                                    break;

//
// Errors from Ilia:
//
#include "..\errors.c"

        default :                   r = "����������� ������";
                                    e = "Unknown error";
                                    break;
    }
    txtErrorMessage = e;
    txtErrorMessageRus = r;
}

