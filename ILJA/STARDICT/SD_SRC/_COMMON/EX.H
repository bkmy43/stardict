/* **************************************************************************
                                 EX.H
    v2.0
    �������� �������� ������� ����������, ������������� ��������� StarDict.
    � ������������ ���������� ������ ��������� � ����� ����� ���������,
    ����������� �� ������ �����. ("I\\", "V\\").
    ������:
        TSTDError.
    (c) �.�����.
************************************************************************** */

#if !defined (_EX_H)
#define _EX_H

enum { LANG_ENGLISH = 0, LANG_RUSSIAN  };

enum TSTDCode {

        EC_NOERROR = 0,
        EC_NOLICENSE,
        EC_WRONGDATFILE,
        EC_DISKERROR,
        EC_NODLL,
        EC_WRONGREGINFO,
        EC_WRONGUSERNAME,
        EC_WRONGREGNUM,
        EC_ERRORFONTTAG,
        EC_NOSUCHARTICLE,


        IC_CANCELINSTALL,   // codes of installation
        IC_WRONGSIGNATURE,
        IC_WRONGVERSION,
        IC_NOMOREINST,
        IC_USERNAMETOOSHORT,
        IC_TOOLOWDISKSPACE,

        EC_WRONG_INDEXDAT,
        EC_WRONG_DATADAT,
        EC_WRONG_THEMESDAT,
        EC_WRONG_KEYWORDDAT,
        EC_WRONG_CRC,
        EC_WRONG_DATA_VERSION,
        EC_WRONG_FORMAT_VERSION,
        EC_CANTOPEN_INDEXDAT,
        EC_CANTOPEN_DATADAT,
        EC_CANTOPEN_THEMESDAT,
        EC_CANTOPEN_KEYWORDDAT,
        EC_NO_MEMORY,

        EC_NOT_ENOUPH_BUF
};

struct TSTDError {

        char*       txtErrorClass; // ����� ������ (������)
                                   // ���� "������ ������ � ������" � �.�.

        char*       txtErrorMessage; // ����� ������, �� ������� � ���������� �������.
        char*       txtErrorMessageRus; // ����� ������, �� ������� � ���������� �������.

        char*       txtFileName; // ��� CPP-����� �������, � ������� ��������� ������.
        int         numLine;     // ����� ������ � �����, ��� ��������� ������.

        TSTDCode    CCode;      // ��� ������;
        int         fCritical;  // ���� - ���� 1, �� �������� ���������� ���������,
                                // ����� - ����� ����������.

        TSTDError(char* txtFN, int nL, TSTDCode CC, int fC = 1,
                  char* txtEC = "Error: ������");

        void  GetErrorMessage();
        int   ProcessException(); // �������� ���� ������� � �.�.
};

#endif

