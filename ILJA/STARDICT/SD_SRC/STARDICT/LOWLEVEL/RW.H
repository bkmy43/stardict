/********************************************************************
-- STARDICT Project --
								 rw.h

�������� �-�� � ������� ��� ������/������  � ����� �������� �� ���-�����.
�������� �� ������������� ����������� BufRead � BufWrite.
������������ ��� ����������� ������� � data.dat
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _RW_H
#define _RW_H


typedef  unsigned char boolean;
const STOP_CODE_FOUND = -2; // ��� �-�� GoToChar

// ������ ��������� ������
#define FILE_BUF_SIZE    1024
#define YES      1
#define NO       0

//*****************    ���������������� ������  ******************
class BufRead
     {
       public:
	      int hand ;      //  ����
	      unsigned char* FileBuf;    // ������ ��� ������

	      int wh;    // �������� �������� �����
		               //  ��� �������, ������� ����������  readHalf()
	      boolean ReachEOF;        // ��������� �� ����� �����
	      int CurPos;      // ������� ��������� � ������e ��� ������
	      int UsedBufSize; // ������� ���� ��������� �� ����� ��������� � ������e ��� ������
	      boolean ErrorCode; // ���� �� �������� ������ ��� ������


     	 BufRead();
     	 ~BufRead();
       void SetFileHandler( int h ) {  hand = h;  RenewBuf(); }
       int RenewBuf();   // �-� ��������� ������ ��� ������
       int  ReadByte( int* buf );   // ���. ������

       void FF() { CurPos++; }
       void Rew() { CurPos--; }
       void SetError() { ErrorCode = 1 ; }
       void bigRew();
     };

//*****************    ���������������� ������  ******************
class BufWrite
     {
         int       hand ;      //  ����
         int       CurPos;      // ������� ��������� � ������e ��� ������
         unsigned char*     FileBuf;    // ������ ��� ������

       public:
         int UsedBufSize; // ������� ���� ��������� ������ � ����
                         // ��������� � ������e ��� ������
         int wh;        // �������� �������� �����
                       //  ��� �������, ������� ����������  writeHalf()
         boolean   ErrorCode; // ���� �� �������� ������ ��� ������

         BufWrite();
         ~BufWrite();
         void SetFileHandler( int h ) {  hand = h; }

         int RenewBuf();   // �-� ��������� ������ : ��������� �� ���� ��� ����������

         int  WriteByte( int info );
         void FF() { CurPos++; }
         void Rew() { CurPos--; }
         void SetError() { ErrorCode = 1 ; }

         friend int writeHalf( BufWrite& bw, int info ); // ����
                        // ������� ��������� ������ � prived ����� ������
         friend int SetTextEnd( BufWrite& bw );

     };

/*********************************************************************
              ���������� ������� ��� ������/������
*********************************************************************/

// halfrw.c
int readHalf( BufRead& br, int* res );
int writeHalf( BufWrite& bw, int info );
int readByte( BufRead& brIn, int* ch ); // �-� ������ �� brIn ��� ���������
                                // � ���������� �� � ch
int writeByte( BufWrite& bwOut, int ch ); // �-� ���������� � bwOut ���
                                     // ��������� �� ch

// utilrw.c

int GoToChar( BufRead& brIn, char ch, char stopcode = EOF );
// �-� ��������� � ����� brIn �� ������ ch �����. ��� ������
// ����� �������������� �� ��� ��� ���� �� ���������� stopcode,
// ������ ���� stopcode �� ������ ��� ������ �� �� ��������� �� = EOF
int GoToNthChar( int n, BufRead& brIn, char ch );
int SeekSpaces( BufRead& brIn ); //�������� �������� � '\n'
// �-� ��������� � ����� brIn �� n-� ������ 'ch' �����. ��� ������
int ReadInt( BufRead& brIn, int* res ); //  �-� ������ �� ����� 2 ����� � ����������� �� � �����
int ReadInt( BufRead& brIn, unsigned* res );
int ReadIntFromChar( BufRead& brIn, int* res ); // ������ ������ �� �������
                                              // � ����������� �� � int
int WriteInt( BufWrite& bwOut, int i );
int ReadString( BufRead& brIn, char* str );
int ReadName( BufRead& brIn, char* name ); // �-� ������ ������ � �������� ��������� �� ������ '^'
int ReadLong( BufRead& brIn, long* res );

#endif