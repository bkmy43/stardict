/********************************************************************
-- STARDICT Project --
main file �ணࠬ�� CheckRef
�ணࠬ�� �஢���� 楫��⭮���
䠩��� Stardict
(c) I.Vladimirsky 1997
********************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <dos.h>

#include "defines.h"
#include "rw.h"
#include "errors.h"
#include "chutil.h"

// ����� ��� ��������� ��ꥪ⮢
int AllNumbers[ MAX_OBJECTS ];

// ����� ��室��� 䠩���
char InNames[MAX_IN_FILES][MAX_FILE_NAME_LEN];
// ���� ��� ��� ��室��� 䠩���
BufRead* pbrIn[MAX_IN_FILES];

// � �㤥� ����ᥭ� ���ଠ�� ��� ��� �ந��襤�� �訡���
TError CurError;

void BeginMessages( void );
void EndMessages( void );
int  OpenFiles( int argc, char* argv[]  );
void CloseInFiles();
// chutil.c

//*********************************************************************
int main( int argc, char* argv[] )
   {
      int res;
// ᮮ�饭�� user'�
      BeginMessages( );
      if( argc == 1 )
          return OK;

      textcolor(GREEN);

      if( OpenFiles(  argc, argv ) != 1 )
      	 return NOK;

      printf("   * ����祭�� ᯨ᪠ ��������� ��⥩.\n");
      int s;
      GetAllNumbers( AllNumbers,  &s );

      printf("\n   * �஢�ઠ 楫��⭮�� ��뫮�.\n");
      CheckAllRefs( AllNumbers, s );

      CloseInFiles();

// ᮮ�饭�� user'�
      EndMessages( );

      return 1 ;
  }

//***********************************************************************
// ᮮ�饭��, �뤠����� user'� � ��砫� ࠡ���

void BeginMessages( )
{
      textcolor(WHITE);
      cprintf(
	      "    ������ �ணࠬ�� �஢���� 楫��⭮��� ��४����� ��뫮�\n\r"
         "    � ⥪��� ��⥩ � 䠩��� ������ ���஫����᪮� ��横�������\n\r"
         "    STARDICT ���ᨨ 2.0.\n\r"

	      "   ������  1.0  �� 15.12.97\n\r\n\r"
	      "   ���⠪��:  \n\r"
         "     CHECKREF <infile1> [<infile2> ... <infileN>]\n\r\n\r");
      textcolor(LIGHTGRAY);
      cprintf("\n\r");

}
//***********************************************************************
// ᮮ�饭��, �뤠����� user'� � ���� ࠡ���
void EndMessages( )
{
      sound(200); delay(20); nosound();
      sound(500); delay(20); nosound();
      sound(900); delay(20); nosound();

      textcolor(WHITE);

      cprintf( "   �஢�ઠ 楫��⭮�� ��뫮� �����襭�.\n\r");

      textcolor(LIGHTGRAY);
      cprintf("\n\r");
}
//*************************************************************************
int OpenFiles( int argc, char* argv[] )
{
      filehand fhIn;
      int i;

      if( argc > MAX_IN_FILES + 1 )
              CurError.ProceedError(TOO_MANY_ARGUMENTS);

      for( i=0; i< MAX_IN_FILES; i++ )
       {
         pbrIn[i] = NULL;
         strcpy(InNames[i], "None");
       }

      for( i=0; i < argc-1; i++ )
       {
         fhIn = open( argv[i+1], O_RDONLY | O_BINARY );
         if( fhIn == -1 )     CurError.ProceedError(CAN_NOT_OPEN_IN);
         pbrIn[i] = new BufRead();
         if(pbrIn == NULL)     CurError.ProceedError(NOT_ENOUPH_MEMORY);
         pbrIn[i]->SetFileHandler( fhIn );
         strcpy(InNames[i], argv[i+1]);
       }

      return 1;
}

void CloseInFiles()
{
      for( int i=0; i < MAX_IN_FILES && pbrIn[i] != NULL; i++ )
       {
          close(pbrIn[i]->hand);
          strcpy(InNames[i], "None");
          delete pbrIn[i];
       }
}

