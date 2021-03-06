/* ***************************************************************************
                                MAKE.C
    �������� ����ਡ�⨢� ��� ��⠭���� StarDict.
    (c) �. ��஢.
*************************************************************************** */

#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <errno.h>
#include <io.h>
#include <mem.h>
#include <fcntl.h>

#pragma hdrstop

#include "code.h"
#include "sign.h"

#define     txtInstallPrefix      "INST"

extern "C" int AsmFormatTrack(int drive, int track, int head);
extern "C" int AsmWriteSignature(int drive, int track, int head);
extern "C" int AsmReadSignature(int drive, int track, int head);

TSignature   sign;
int track = TRACK_NUMBER_first;

unsigned char buf_512[512];

int     fIgnoreCopy = 0;

char    tDrive[10],
        tDirectory[128],
        tRegNum[50],
        tINIfile[50],
        tNumInst[10];

int     Drive = 0;
int     NumInst = 4;
char    RNcoded[60];

unsigned char buffer[10000];

struct  TDEr {
    char* text;

    TDEr(char* t) { text = t; }
};

struct TMelody {
    int   freq, len;
};

TMelody arMelody[] = { {500, 1}, {600, 1}, {700, 1}, {800, 1}, {900, 1}  };
#define BASE_LEN       80

// ***********************************************************
void PrintDone()
{
    printf(" �믮�����.\n");
}

void Melody()
{
    for (int i=0; i<5; ++i) {
        sound(arMelody[i].freq);
        delay(arMelody[i].len*BASE_LEN);
        nosound();
    }
}
void InterpretParameters()
{
    Drive = toupper(tDrive[0]) - 'A';
    if (Drive != 0 && Drive != 1)
        throw TDEr("������� ����ୠ� �㪢� ��᪮����. �㪢� ������ ���� ���� A:, ���� B:");
    NumInst = atoi(tNumInst);
    if (NumInst <= 0)
       throw TDEr("����୮� ���祭�� �᫠ ���⠫��権.");

    CodeRegNum(RNcoded, tRegNum, sizeof(RNcoded) );
}

void ShowCredits()
{
    printf("%s", "CRTDISTR: ᮧ����� ����ਡ�⨢� ��� ��⠭���� �ணࠬ�� StarDict v2.0.\n"
                 "(c) ���� ��஢, 1997.\n"
                 "��ࠬ����:\n"
                 "-d <���ன�⢮ � ���⠫��樮���� ��᪥⮩:>\n"
                 "-c <��⠫�� � 䠩���� ����ਡ�⨢�>\n"
                 "-r <ॣ����樮��� �����>\n"
                 "-n <�᫮ ���⠫��権>\n"
                 "-i <��� ���䨣��樮����� 䠩��, � ����� ����뢠���� �਩�� �����>\n"
                 "-9 :����易⥫�� ��ࠬ���, �������� ����஢���� �� ��᪥��\n\n"
    );
}

void GetParameters(int a, char* b[])
{
    track = TRACK_NUMBER_first;
    for (int i=1; i<a; ) {
        if (*b[i] == '-') {
            switch (*(b[i]+1)) {
                case 'd':       strcpy(tDrive, b[i+1]); i+=2; break;
                case 'c':       strcpy(tDirectory, b[i+1]); i+=2; break;
                case 'r':       strcpy(tRegNum, b[i+1]); i+=2; break;
                case 'n':       strcpy(tNumInst, b[i+1]); i+=2; break;
                case 'i':       strcpy(tINIfile, b[i+1]); i+=2; break;
                case '9':       fIgnoreCopy = 1; i++; break;
                case 't':       track = 79; i++; break;

                default :       throw TDEr("������ ��ࠬ����.");
            }
        }
        else throw TDEr("������ ��ࠬ����.");
    }
    if ( * (tDirectory + strlen(tDirectory) -1) != '\\')
        strcat(tDirectory, "\\");
}

void CreateIniFile()
{
    printf("* �⥭�� �������������� ���䨣��樮����� 䠩��.\n"
           "  ������ � 䠩� ॣ����樮����� ����� �\n"
           "  ����饭�� ��� � ��⠫�� ����ਡ�⨢�.");

    FILE* fin = fopen(tINIfile, "rt");
    if (fin == NULL)
        throw TDEr("���������� ���� 㪠����� 䠩� ���䨣��樨.");

    char buf[200];
    char buf_out[200];
    strcpy(buf, tDirectory);
    strcat(buf, "stardict.in_");
    FILE* fout = fopen(buf, "wb");

    if (fout == NULL)
        throw TDEr("���������� ᮧ���� ���䨣��樮��� 䠩� STARDICT.INI.");

    static char* aNames[] = { "RegNum=", "Back=" };
    static char* aVal[]   = { tRegNum, RNcoded };

    int count =0;
    while (!feof(fin)) {
        if (fgets(buf, sizeof(buf) -1, fin) == NULL) break;
        strcat(buf, "\n");
        buf[strlen(buf)-2] = '\r';
        for (int i=0; i<2; ++i) {
            char* beg = strstr(buf, aNames[i]);
            if (beg != NULL) {
                ++count;
                buf[strlen(buf)-2]='\0';
                strcat(buf, aVal[i]);
                strcat(buf, "\n");
                break;
            }
        }
        CodeStringShifting(buf_out, buf, strlen(buf));
        fwrite(buf_out, strlen(buf), 1, fout);
    }
    if (count != 2)
        throw TDEr("�� ������� ������� �� ����室���� ���祩 � ���䨣��樮���� 䠩��.");

    flushall();
    fclose(fin);
    fclose(fout);
    PrintDone();
}

void ShowBaseData()
{
//    clrscr();
    printf("\n*** �஢���� �ࠢ��쭮��� ��室��� ������:\n\n"
          "* �������樮��� �����:%s\n"
          "* ��室��� �᫮ ���⠫��権: %2d\n"
          "* ��⠫�� � 䠩���� ����ਡ�⨢�:%s\n"
          "* ��᪮���, �� ����� �㤥� ����ᠭ ����ਡ�⨢:%s\n\n"
          "* �஢�ਢ, ������ ���� �������, ��᫥ 祣� ��筥��� �����\n"
          "  ����஢���� 䠩��� ����ਡ�⨢� �� ��᪥��.\n\n",
          tRegNum, NumInst, tDirectory, tDrive
    );
    asm {
        mov     ah, 0x08
        int     0x21
    }
}
void CopyFile(find_t* FoundFile)
{
    printf("* ����஢���� 䠩��:  %-12s\t\t", FoundFile->name);

    if (fIgnoreCopy == 1) {
        PrintDone();
        return;
    }

    char fname_out[40];
    char fname_in[200];

    strcpy(fname_out, tDrive);
    strcat(fname_out, FoundFile->name);

    char name[15];
    strcpy(name, FoundFile->name); strupr(name);

    if (strstr(name, txtInstallPrefix) != name)
        fname_out[strlen(fname_out)-1] = '_';

    strcpy(fname_in, tDirectory);
    strcat(fname_in, FoundFile->name);

    int fout = _rtl_creat(fname_out, 0),//open(fname_out, O_RDWR | O_TRUNC | O_CREAT | O_BINARY),
        fin  = open(fname_in, O_RDONLY | O_BINARY);

    if ( fout < 0)
        throw TDEr("���������� ᪮��஢��� 䠩� �� ��᪥��.");

    if ( fin < 0)
        throw TDEr("�訡�� �� ����⨨ 䠩��� ����ਡ�⨢�.");

    long len = filelength(fin);
    long done =0;
    if (len==0) len ++;
    int bytes_read = 0;
    do {
        bytes_read = read(fin, buffer, sizeof(buffer));
        if (bytes_read <0) throw TDEr("�訡�� �� �⥭�� ��᪠.");
        if (bytes_read ==0 ) break;
        int wrt = write(fout, buffer, bytes_read);
        if (wrt <0) throw TDEr("�訡�� �� ����� � 䠩�.");

        done += bytes_read;
        if (bytes_read > 0) {
            gotoxy(wherex()-4, wherey());
            printf("%3d%%", done*100/len);
        }

    } while (bytes_read >0);

    ftime f; getftime(fin, &f); setftime(fout, &f);
    close(fin);
    close(fout);

    PrintDone();
}
void CopyDistrib()
{
    char filename[200];
    strcpy(filename, tDirectory);
    if (filename[strlen(filename)] != '\\')
        strcat(filename, "\\");
    strcat(filename, "*.*");

    find_t FoundFile;

    int res = _dos_findfirst(filename, _A_NORMAL | _A_RDONLY | _A_ARCH, &FoundFile);
    if (res != 0)
        throw TDEr("��� 䠩��� ��� ����஢����.");

    CopyFile(&FoundFile);
    while ( _dos_findnext(&FoundFile) == 0)
        CopyFile(&FoundFile);

    flushall();
    printf("\n* ����� ����ਡ�⨢� �ᯥ譮 ᪮��஢���.\n");
}
void FormatTrack(int Drive, int t, int h)
{
    printf("* ��ଠ�஢���� ��⠩��� ��஦�� ���祢�� ��᪥��.");
    int a=AsmFormatTrack(Drive, t, h);
    if (a) {
        char b[100];
        sprintf(b,"�訡�� N %4xh �� �ଠ�஢����.", a);
        throw TDEr(b);
    }

    PrintDone();
}
void ReadAndTestSignature(int Drive, int t, int h )
{
    printf("* �⥭�� � �஢�ઠ �����஭��� ������ �� ���祢�� ��᪥�.");

    if (AsmReadSignature(Drive, t, h))
        throw TDEr("�訡�� �� �⥭�� ������ �� ���祢�� ��᪥�.");

    unsigned char* p = (unsigned char*) buf_512; // here we have signature.
    unsigned char* ff = (unsigned char*) &sign;

    for (int i=0; i<sizeof(TSignature); ++i)
        if ( *p++ != *ff++)
            throw TDEr("������� �뫠 ����ᠭ� � �訡����. �஢���� ��᪮���.");

    TSignature* z = (TSignature*) buf_512;
    unsigned int crc = CountCRCword((unsigned char*)&z->progVersion, sizeof(TSignature)-2-2);
    if (sign.CRC != crc)
            throw TDEr("�訡�� ����஫쭠� �㬬� ������. �஢���� ������.");

    PrintDone();
}
void WriteSignature(int Drive, int t, int h)
{
    printf("* ������ �����஭��� ������ �� ���祢�� ��᪥��.");
    int a= AsmWriteSignature(Drive, t, h);
    if (a) {
        char b[100];
        sprintf(b, "�訡�� N %4xh �� ����� �����஭��� ������ �� ���祢�� ��᪥��.", a);
        throw TDEr(b);
    }

    PrintDone();
}
void CreateSignature()
{
    memset(&sign, 0, sizeof(TSignature));
    strcpy(sign.chProgram, "SD");

    sign.progVersion = 0x0002; // v2.00
    sign.dataVersion = 0;
    sign.numInst = NumInst;
    strcpy(sign.RegNum, tRegNum);
    strcpy(sign.RegNumCoded, RNcoded);

    sign.CRC = CountCRCword((unsigned char*)&sign.progVersion, sizeof(TSignature)-2-2);
}
int main(int a, char* b[])
{
    ShowCredits();

    try {
/*        a = 5*2+2;
        b[1] = "-9";
        b[2] = "-d";
        b[3] = "a:\\";
        b[4] = "-c";
        b[5] = "d:\\projects\\";
        b[6] = "-r";
        b[7] = "SD-1234-11A";
        b[8] = "-n";
        b[9] = "5";
        b[10] = "-i";
        b[11] = "stardict.zzz";
*/
        if (a< 5*2+1)
             throw TDEr("�������筮 ��ࠬ��஢ ��� �ନ஢���� ����ਡ�⨢�.\n");

        GetParameters(a, b);
        InterpretParameters();
        CreateIniFile();
        ShowBaseData();
        CopyDistrib();

        CreateSignature();
        FormatTrack(Drive, track/*TRACK_NUMBER_first*/, HEAD_NUMBER_first );
        WriteSignature(Drive, track/*TRACK_NUMBER_first*/, HEAD_NUMBER_first );
        ReadAndTestSignature(Drive, track/*TRACK_NUMBER_first*/, HEAD_NUMBER_first );
    }
    catch (TDEr s) {

        printf("\x07\n%s\n", s.text);
        printf("\npress almost any key to continue.\n");
        getch();
        return 1;

    }

    Melody();
    printf("\n"
           "* ����ਡ�⨢ �ணࠬ�� StarDict v2.0 ��९�ᠭ �� ��᪥��.\n"
           "* �믮����� ᫥���騥 ����⢨�:\n"
           " - �� 㪠������� ॣ����樮����� ������ ��ନ஢��� ������ �\n"
           "   ���䨣��樮���� 䠩�� �ணࠬ�� StarDict;\n"
           " - �� 䠩�� ����ਡ�⨢� ᪮��஢��� �� 㪠�������� ��⠫��� ��\n"
           "   㪠����� ���, ������ ���������� ���䨣��樮��� 䠩�;\n"
           " - ��ନ஢��� �����஭��� �������, �࠭���� �� ���⠥��� ��஦��\n"
           "   ��᪥�� (� ������ �࠭���� �᫮ ��⠢���� ���⠫��権 �\n"
           "   ॣ����樮��� �����);\n"
           " - �� ������� ����ᠭ� �� ��� � ������ �஢�७�.\n\n"
           "* ��.\n\n"
    );


    return 0;
}

