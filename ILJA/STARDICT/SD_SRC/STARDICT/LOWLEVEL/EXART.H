/********************************************************************
-- STARDICT Project --
								 exart.h

�������� �������� ������ TExArticle
�������� ������� TBaseList, TArticleList, TThemeList
��������� �������� ������ ������, ��� � keyword'��
� �������� � ����
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _TEXART_H
#define _TEXART_H

#include "datutil.h"

class TExArticle       // ������
    {
       protected :
          char artname[MAX_NAME_LEN];          // �������� ������
          int artnum;                // ����� ������
			 char* artbody;            // ���� (����� ������)
			 unsigned bodylen;            //  ����� ������ ������
			 BufRead *pbrIndex;      // ��������� �� ������ ������
			 BufRead *pbrData;      // � �������

			 virtual void DelOldInfo();
			 // �-� ������ �������� ������ N num � �����
			 // �� �������� �� ����� ����� ����� � ����� data.dat
			 int ReadNamePosition ( int num, long* position );
			 // �-� ������ ����� ������ name � �����
			 // �� �������� �� ����� ����� ����� � ����� data.dat
			 int ReadNumPosition ( char* name, long* position );

		 public:
			 TExArticle( );  /* === Function Can Throw Exeptions === */
			 TExArticle( int inumber );  /* === Function Can Throw Exeptions === */
			 virtual ~TExArticle( );

			 char* GetArtName() { return artname; };
			 int GetArtNum() { return artnum; };
			 unsigned GetArtBodyLen() { return bodylen; };
			 char* GetArtBody(){ return artbody; };

			 // �-� ������ ���������� �
			 // ������ N num � ��������� ������ ������
			 // � ������ ������ ���������� �� ���
          /* === Function Can Throw Exeptions === */
			 virtual int ChangeArt( int num );

			 // �-� ������ ����� ������
			 int ReadBody( long position );
	 };


// ������� ����� - ������� ������
class TBaseListElem
		 {
			 public:
				int num;
				char* name;

				TBaseListElem* pnext;

            /* === Function Can Throw Exeptions === */
				TBaseListElem( int a, char* str, TBaseListElem* apnext=NULL);
				~TBaseListElem();
				TBaseListElem* GetNext() { return pnext; };
		  };

// ������� ����� - ���������������� ������
class TBaseList
		  {
			 protected:
				int ItemsNum;
				TBaseListElem* pfirst;
				virtual void BuildList()=0;  // ��������� �������� ������ �����������
												// �������. ���������������� � �����������
												// �� ���������� ����������� ������
				int AppendElem( int i, char* str ); // ��������� � ����� ������
												  // ������� � ����������� (i,str)

			 public:
				TBaseList() {pfirst=NULL; ItemsNum=0; };
				~TBaseList();
				int GetItemsNum() { return ItemsNum; }
				char* GetNthName( int n ); // �������� name n-�� �������� ������
				int GetNthNumber( int n ); // �������� number n-�� �������� ������
				char* GetName( int num ); // �������� ��� ��������������� ������ num
				int GetNumber( char* str ); // �������� ����� ��������������� ����� str
		  };

// ����� ������ ������ �������� �������� TBaseList
// ���������������� �������  BuildList - ������ �������� �� index.dat
class TArticleList : public TBaseList
		  {
			 protected:
				BufRead* pbrIndex;

            /* === Function Can Throw Exeptions === */
				virtual void BuildList();

			 public:
            /* === Function Can Throw Exeptions === */
				TArticleList( int mode=BUILD_LIST); // �������� BuildList
				~TArticleList( );
				  // �������� ��� ������ ��������������� ������ num
				char* GetArticleName( int num ) { return GetName(num); };
				 // �������� ����� ������ � ������ "str"
				int GetArticleNumber( char* str ) { return GetNumber(str); };

				friend class TThemeList;
				friend class TKeywordList;
		  };

// ����� ������ ��� �������� �������� TBaseList
// ���������������� �������  BuildList - ������ �������� �� themes.dat
class TThemeList : public TBaseList
		  {
			 protected:
				BufRead* pbrThemes;
				TArticleList* AllArtList; // ������ ���� ������ ��������
												  // � ������������

 // ������ ����� �����, ���������� ���������� � ������� ���� ���.
 // ������: �����_����_1, �����_������_11, �����_������_12, ..., 0,
 //         �����_����_2, �����_������_21, �����_������_22, ..., 0,
 //                               .  .  .
 //         �����_����_n, �����_������_n1, �����_������_n2, ..., 0,
 //         0

				int* ThemesTable;
            /* === Function Can Throw Exeptions === */
				int  BuildThemesTable();
            /* === Function Can Throw Exeptions === */
				virtual void BuildList();

			 public:
				TArticleList* ThArtList;  // ������ ������ �� ������ ����
					  // �������� �������� GetNthArtList �� ������ AllArtList

            /* === Function Can Throw Exeptions === */
				TThemeList();
				~TThemeList( );
 			   // �������� ��� ���� ��������������� ������ num
				char* GetThemeName( int num ) { return GetName(num); };
				 // �������� ����� ���� � ������ "str"
				int GetThemeNumber( char* str ) { return GetNumber(str); };

				// ������� ������ ������ ������ �� N-�� ���� � ����������
				// ��������� �� ����
				TArticleList* GetNthArtList( int n );
		  };

// ����� ������ keyword'�� �������� �������� TBaseList
// ���������������� �������  BuildList - ������ �������� �� keyword.dat
class TKeywordList : public TBaseList
		  {
			 protected:
				BufRead* pbrKeys;
				TArticleList* AllArtList; // ������ ���� ������ ��������
												  // � ������������

 // ������ ����� �����, ���������� ���������� � ������� ���� keyword'��.
 // ������: �����_keyword'�_1, �����_������_11, �����_������_12, ..., 0,
 //         �����_keyword'�_2, �����_������_21, �����_������_22, ..., 0,
 //                               .  .  .
 //         �����_keyword'�_n, �����_������_n1, �����_������_n2, ..., 0,
 //         0

				int* KeywordsTable;
            /* === Function Can Throw Exeptions === */
				int  BuildKeywordsTable();
            /* === Function Can Throw Exeptions === */
				virtual void BuildList();

			 public:
				TArticleList* KeyArtList;  // ������ ������ � ������� �����������
													// ������ keyword
					  // �������� �������� GetNthArtList �� ������ AllArtList

            /* === Function Can Throw Exeptions === */
				TKeywordList();

				~TKeywordList( );
				// �������� ��� keyword ��������������� ������ num
				char* GetThemeName( int num ) { return GetName(num); };
				// �������� ����� keyword � ������ "str"
				int GetThemeNumber( char* str ) { return GetNumber(str); };

				// ������� ������ ������ ������ �� N-��� keyword'� � ����������
				// ��������� �� ����
				TArticleList* GetNthArtList( int n );
		  };

#endif

