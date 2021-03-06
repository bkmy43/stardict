/********************************************************************
-- STARDICT Project --
���ᠭ�� �-�� � ����ᮢ ��� �����/�⥭��  � 䠩�� ����ﬨ �� ���-����.
�᭮���� �� �ᯮ�짮����� ���ਧ�樨 BufRead � BufWrite.
�ᯮ������� �� ����஢���� ⥪�⮢ � data.dat
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _RW_H
#define _RW_H

#include <stdio.h>
typedef  unsigned char boolean;
const STOP_CODE_FOUND = -2; // ��� �-�� GoToChar

// ࠧ��� 䠩������ ����
#define FILE_BUF_SIZE    1024
#define YES      1
#define NO       0

//*****************    ���ਧ�஢����� �⥭��  ******************
class BufRead
     {
       public:
	      int hand ;      //  ����
	      unsigned char* FileBuf;    // ����� ��� �⥭��

	      int wh;    // ��⨢��� �������� ����
		               //  ��� �㭪権, ����� �ᯮ�����  readHalf()
	      boolean ReachEOF;        // ���⨣��� �� ����� 䠩��
	      int CurPos;      // ⥪�饥 ��������� � �����e ��� �⥭��
         unsigned long CurLine; // ������ ��ப�
	      int UsedBufSize; // ����쪮 ���� ��⠭��� �� 䠩�� ��室���� � �����e ��� �⥭��
	      boolean ErrorCode; // �뫠 �� ����饭� �訡�� �� �����


     	 BufRead();
     	 ~BufRead();
       void SetFileHandler( int h ) {  hand = h;  RenewBuf(); }
       int RenewBuf();   // �-� �������� ����� ��� �⥭��
       int  ReadByte( int* buf );   // ���. �����

       void FF() { CurPos++; }
       void Rew() { CurPos--; }
       void SetError() { ErrorCode = 1 ; }
       void bigRew();
     };

//*****************    ���ਧ�஢����� ������  ******************
class BufWrite
     {
       public:
         int       hand ;      //  ����
         int       CurPos;      // ⥪�饥 ��������� � �����e ��� �����
         unsigned char*     FileBuf;    // ����� ��� �����

         int UsedBufSize; // ����쪮 ���� �������� ����� � 䠩�
                         // ��室���� � �����e ��� �����
         int wh;        // ��⨢��� �������� ����
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
              �㭪樨 ��� �⥭��/�����
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
int SeekSpaces( BufRead& brIn ); //�஬�⪠ �஡���� � '\n'
// �-� ���室�� � 䠩�� brIn �� n-� ᨬ��� 'ch' �����. ��� �訡��
int ReadInt( BufRead& brIn, int* res ); //  �-� �⠥� �� 䠩�� 2 ���� � �८�ࠧ�� �� � 楫��
int ReadInt( BufRead& brIn, unsigned* res );
int ReadIntFromChar( BufRead& brIn, int* res ); // ��⠥� ��ப� �� �஡���
                                              // � �८�ࠧ�� �� � int
int WriteInt( BufWrite& bwOut, int i );
int ReadString( BufRead& brIn, char* str );
int ReadName( BufRead& brIn, char* name ); // �-� �⠥� ��ப� � ⥪�饣� ��������� �� ᨬ��� '^'
int WriteString( BufWrite& bwOut, char* str );
int WriteName( BufWrite& bwOut, char* name ); // �-� �����뢠�� ��ப�, �����稢��� �� ᨬ����� '^'
int ReadLong( BufRead& brIn, long* res );
unsigned GetCRC(  BufRead& brIn  );

#endif