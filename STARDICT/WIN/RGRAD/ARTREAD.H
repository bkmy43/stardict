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
	  FILE* fr;
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
//	  int TestRefs(); // �-� �஢���� �� �� ��ꥪ�� �� ����� ��뫠���� ��������
	  int TestThemes(); // �� �� ⥬� ��������

	  int TransferInfo( ); // ��ᯥ��� ��뢠�� ᫥���騥 �㭪樨
	  int TransferData( );
	  int TransferThemes( );
	  int TransferIndex( );
    };

/*class ThemeR
    {
	  FILE* fr;
	  BufWrite* pbwTh;
       public :
	  char* thname;
	  int thnum;

	  ThemeR( FILE* afr, BufWrite* abwTh);
	  ~ThemeR( );
	  void ClearAll();
	  int ReadInfo();      // ��ᯥ���
	  int test(); // �஢���� �ࠢ��쭮��� ���⠭��� ���ଠ樨

	  int TransferInfo( );
    };
*/
#endif