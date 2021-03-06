#include <stdio.h>
#include <dos.h>
#include <io.h>

typedef  unsigned char boolean;
const STOP_CODE_FOUND = -2; // ��� �-�� GoToChar

boolean const  YES = 1;
boolean const  NO  = 0;
int const  FILE_BUF_SIZE = 1024;

//*****************    ���ਧ�஢����� �⥭��  ******************
class BufRead
     {
	 int       hand ;      //  ����
	 unsigned char*     FileBuf;    // ����� ��� �⥭��

       public:
	 int   wh;            // ��⨢��� �������� ����
		       //  ��� �㭪権, ����� �ᯮ�����  readHalf()
	 boolean   ReachEOF;        // ���⨣��� �� ����� 䠩��
	 int       CurPos;      // ⥪�饥 ��������� � �����e ��� �⥭��
	 int       UsedBufSize; // ����쪮 ���� ��⠭��� �� 䠩�� ��室���� � �����e ��� �⥭��
	 boolean   ErrorCode; // �뫠 �� ����饭� �訡�� �� �����


	 BufRead();
	 ~BufRead();
	 void SetFileHandler( int h ) {  hand = h;  RenewBuf(); }
	 int RenewBuf();   // �-� �������� ����� ��� �⥭��
	 int  ReadByte( int* buf );   // ���. �����

	 void FF() { CurPos++; }
	 void Rew() { CurPos--; }
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
	 ~BufWrite();
	 void SetFileHandler( int h ) {  hand = h; }

	 int RenewBuf();   // �-� �������� ����� : ᯨ�뢠�� �� ��� ��� ᮤ�ন���

	 int  WriteByte( int info );
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

int GoToChar( BufRead& brIn, char ch, char stopcode = EOF );
// �-� ���室�� � 䠩�� brIn �� ᨬ��� ch �����. ��� �訡��
// ���� �����⢫���� �� �� ��� ���� �� �������� stopcode,
// ��祬 �᫨ stopcode �� 㪠��� �� �맮�� � �� 㬮�砭�� �� = EOF
int GoToNthChar( int n, BufRead& brIn, char ch );
// �-� ���室�� � 䠩�� brIn �� n-� ᨬ��� 'ch' �����. ��� �訡��
int ReadInt( BufRead& brIn, int* res ); //  �-� �⠥� �� 䠩�� 2 ���� � �८�ࠧ�� �� � 楫��
int WriteInt( BufWrite& bwOut, int i );
int ReadString( BufRead& brIn, char* str );
int ReadName( BufRead& brIn, char* name ); // �-� �⠥� ��ப� � ⥪�饣� ��������� �� ᨬ��� '^'
int ReadLong( BufRead& brIn, long* res );