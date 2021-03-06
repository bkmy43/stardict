/********************************************************************
-- STARDICT Project --
���ᠭ�� �-�� � ����ᮢ TArtR � CTag. ��������� ������ ����� ��
䠩�� ������ � �ଠ� RSTEXT, �ࢥ��� �� �ࠢ��쭮��� � �������
�� � DAT 䠩��.
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _ARTREAD_H
#define _ARTREAD_H

// ����� ᮤ�ন� �-�� ��������騥 ����� �� ��室���� 䠩�� ���ଠ�� �
// ����, �஢���� �� � ������ � 䠩�� ������
class TArtR
    {
	  BufRead*  pbrInput;
	  BufWrite* pbwIndex;
	  BufWrite* pbwTh;
	  BufWrite* pbwData;
	  BufWrite  bwTemp; // � �㤥� ����ᠭ ⥪�� ����.
			   // ��᫥ �஢�ન �� ��������� � data.dat
	  long position; // ���� � 䠩�� data.dat

       public :
	  char* artname;          // ��������
	  int artnum;
	  int refTable[256];    // ������ ��뫮� �� ��㣨� ���� � ⥬�
	  char* ths[256];         // ���� � ����� �⭮���� ������ �����

	  TArtR( BufRead* pbrInput, BufWrite *bwIndex, BufWrite *bwTh,
		 BufWrite *bwData, long position );
	  ~TArtR( );
	  void ClearAll();

	  int ReadInfo();
          void Data2Win1251(); // ��४���஢�� � Win1251

//	  int TestRefs(); // �-� �஢���� �� �� ��ꥪ�� �� ����� ��뫠���� ��������
	  int TestThemes(); // �� �� ⥬� ��������
	  int test(); // ��뢠�� �� ����
	  int TransferInfo( ); // ��ᯥ��� ��뢠�� ᫥���騥 �㭪樨
	  int TransferData( );
	  int TransferThemes( );
	  int TransferIndex( );
    };

class ThemeR
    {
	  BufRead*  pbrInput;
	  BufWrite* pbwTh;
       public :
	  char* thname;
	  int thnum;

	  ThemeR( BufRead* pbrInput, BufWrite* abwTh);
	  ~ThemeR( );
	  void ClearAll();
	  int ReadInfo();      // ��ᯥ���
          void Data2Win1251( ); // ��४���஢�� � Win1251
	  int test(); // �஢���� �ࠢ��쭮��� ���⠭��� ���ଠ樨

	  int TransferInfo( );
    };


#endif