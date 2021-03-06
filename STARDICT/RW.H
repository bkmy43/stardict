#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include "defines.h"
#include <graphics.h>
#include "graph.h"

//*****************    ���ਧ�஢����� �⥭��  ******************
class BufRead
     {
	 unsigned char*     FileBuf;    // ����� ��� �⥭��

       public:
	 int       hand ;      //  ����
	 int   wh;            // ��⨢��� �������� ����
		       //  ��� �㭪権, ����� �ᯮ�����  readHalf()
	 boolean   ReachEOF;        // ���⨣��� �� ����� 䠩��
	 int       CurPos;      // ⥪�饥 ��������� � �����e ��� �⥭��
	 int       UsedBufSize; // ����쪮 ���� ��⠭��� �� 䠩�� ��室���� � �����e ��� �⥭��
	 boolean   ErrorCode; // �뫠 �� ����饭� �訡�� �� �����


	 BufRead();
	 virtual void AllocMem(); // ��������� ������
	 virtual void FreeMem();  // �᢮������� ������
	 virtual ~BufRead();
	 void SetFileHandler( int h ) {  hand = h;  RenewBuf(); }
	 int RenewBuf();   // �-� �������� ����� ��� �⥭��
	 int  ReadByte( int* buf );   // ���. �����

	 void FF() { CurPos++; }
	 void Rew() { CurPos--; }
	 void bigRew() { lseek( hand, 0L, SEEK_SET );
			 ReachEOF = 0;    RenewBuf();  }
	 void SetError() { ErrorCode = 1 ; }

     };


//*****************    ���ਧ�஢����� ������  ******************
class BufWrite
     {
	 int       hand ;      //  ����
	 int       CurPos;      // ⥪�饥 ��������� � �����e ��� �����
	 unsigned char*     FileBuf;    // ����� ��� �����

       public:
	 int       UsedBufSize; // ����쪮 ���� �������� ����� � 䠩�
			       // ��室���� � �����e ��� �����
	 int   wh;            // ��⨢��� �������� ����
		       //  ��� �㭪権, ����� �ᯮ�����  writeHalf()
	 boolean   ErrorCode; // �뫠 �� ����饭� �訡�� �� �����

	 BufWrite();
	 virtual void AllocMem(); // ��������� ������
	 virtual void FreeMem();  // �᢮������� ������
	 virtual ~BufWrite();
	 void SetFileHandler( int h ) {  hand = h; }

	 int RenewBuf();   // �-� �������� ����� : ᯨ�뢠�� �� ��� ��� ᮤ�ন���

	 int  WriteByte( int info );  // ���ਧ�஢����� �����
	 void FF() { CurPos++; }
	 void Rew() { CurPos--; }
	 void SetError() { ErrorCode = 1 ; }

	 friend int writeHalf( BufWrite& bw, int info ); // �⮩
			// �㭪樨 ����室�� ����� � prived ��� �����
	 friend int SetTextEnd( BufWrite& bw );

     };


/*********************************************************************
	      ������樨 �㭪権 ��� �⥭��/�����
*********************************************************************/

// halfrw.c
int readHalf( BufRead& br, int* res );
int writeHalf( BufWrite& bw, int info );
int readByte( BufRead& brIn, int* ch ); // �-� �⠥� �� brIn ��� ���㡠��
				// � ��ꥤ���� �� � ch
int writeByte( BufWrite& bwOut, int ch ); // �-� �����뢠�� � bwOut ���
				     // ���㡠�� �� ch

// utilrw.c
int GoToChar( BufRead& brIn,char ch ); // �-� ���室�� � 䠩�� brIn �� ᨬ��� ch �����. ��� �訡��
int ReadInt( BufRead& brIn, int* res ); //  �-� �⠥� �� 䠩�� 2 ���� � �८�ࠧ�� �� � 楫��
int WriteInt( BufWrite& bwOut, int i );
int ReadString( BufRead& brIn, char* str );
int ReadName( BufRead& brIn, char* name ); // �-� �⠥� ��ப� � ⥪�饣� ��������� �� ᨬ��� '^'
int ReadLong( BufRead& brIn, long* res );

char UncodeChar(  int* buffer, int length );
// �-� �⠥� ������஢����� ��ப� � �����頥� NOK �� EOF
int ReadCodedLine( BufRead & brIn, char* str );
//  ��室�� � pbrIndex ����� � ������� ����஬ � �����. �� ���
int GetArtName( BufRead* pbrIndex, int num, char* name );
int GetArtNum( BufRead* pbrIndex, char* artname, int* pnum  );
int GetThNum( BufRead* pbrThemes, char* thname, int* pnum  );
int GetThName( BufRead* pbrThemes, char* thname, int num  );
int GetNextThNameNum( BufRead* pbrThemes, char* buffer, int* pnum );
int GetNextArtNameNum( BufRead* pbrIndex, char* buffer, int* pibuf );
int GetArtNamesList( BufRead* pbrIndex, char** names, int* pmaxnames );
int GetThNamesList( BufRead* pbrThemes, char** names, int* pmaxnames );
int GetHistoryList( BufRead* pbrIndex, char** names, int* qarts );
int GetArtNamesFromTh( int thnum, BufRead* pbrIndex, BufRead* pbrThemes,
		       char** names, int* pmaxnames );
