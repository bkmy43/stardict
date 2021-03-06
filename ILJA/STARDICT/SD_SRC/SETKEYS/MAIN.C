/********************************************************************
-- STARDICT Project --
main file �ணࠬ�� SetKeys.
�ணࠬ�� ���⠢��� ���� ���祢�� ᫮�  � ��室���
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
#include "keys.h"

filehand fhIn, fhOut, fhKeys;

BufRead brIn;
BufWrite bwOut;

// � �㤥� ����ᥭ� ���ଠ�� ��� ��� �ந��襤�� �訡���
TError CurError;

void BeginMessages( void );
void EndMessages( void );
int  OpenFiles( int argc, char* argv[]  );

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

      TKeysList* pkeys = new TKeysList();

/*      SetKeywordInText("�ਢ�� �� ���� test � ��祣� �����...\
       ᠬ� ����� test\n ���� test. �� �� �㬠��, �� �� ��-�\
       �஬� test'�", "test" ); */

      char* buffer=NULL;
      buffer = new char[ MAX_BODY_SIZE ];
      if( buffer == NULL ) CurError.ProceedError( NOT_ENOUPH_MEMORY );

      char* BodyWithKeys=NULL;
      int i,j=1;

      while( ReadUntilBodyBegin( brIn , buffer, MAX_BODY_SIZE ) == OK )
       {
         printf( "   ��ࠡ��뢠� %d �����\n", j++ );
         for( i=0; i<=MAX_BODY_SIZE && buffer[i]!=0; i++)
             bwOut.WriteByte(buffer[i]);
         ReadUntilBodyEnd( brIn , buffer, MAX_BODY_SIZE );

         pkeys->SeekKeyfileToBegin();  // �⬮��� 䠩� �����म� � ��砫�
         while( pkeys->BuildList() == OK )  // ���� ᫥���騩 ����� keyword'��
          {
            for( i=0; i< pkeys->GetItemsNum(); i++ )
              {
                char* keyword = pkeys->GetNthStr(0); // �᭮���� �ଠ
                char* keyform = pkeys->GetNthStr(i); // �������⥫쭠� �ଠ
                BodyWithKeys = SetKeywordInText( buffer, keyword, keyform );
                strcpy( buffer, BodyWithKeys );
                delete BodyWithKeys;
              }
            pkeys->ClearList();
          }
         // ������� ⥫� ���� � ���⠢����묨 keyword'���
         for( i=0; i<=MAX_BODY_SIZE && buffer[i]!=0; i++)
             bwOut.WriteByte(buffer[i]);
       }

      if( buffer!=NULL ) delete buffer;
      brIn.RenewBuf();
      bwOut.RenewBuf();

      close( fhIn );
      close( fhOut );
      close( fhKeys );

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
	      "    ������ �ணࠬ�� ���⠢��� ���� ���祢�� ᫮� � ⥪���\n\r"
              "   ��⥩ � 䠩��� ������ ���஫����᪮� ��横�������\n\r"
              "   STARDICT ���ᨨ 2.0.\n\r"

	      "   ������  1.0  �� 1.12.97\n\r\n\r"
	      "   ���⠪��:  \n\r"
              "     SETKEYS <䠩�_ᯨ᮪_���祢��_᫮�> <infile> <outfile>\n\r\n\r");
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

      cprintf( "   ���� ���祢�� ᫮� �ᯥ譮 ���⠢���� � ��室��� 䠩��\n\r");

      textcolor(LIGHTGRAY);
      cprintf("\n\r");
}
//*************************************************************************
int OpenFiles( int argc, char* argv[] )
{
      if( argc != 4 )     CurError.ProceedError(WRONG_ARGUMENTS);

      fhIn = open( argv[argc-2], O_RDWR | O_BINARY );
      if( fhIn == -1 )     CurError.ProceedError(CAN_NOT_OPEN_IN);
      fhKeys = open( argv[argc-3], O_RDWR | O_BINARY );
      if( fhKeys == -1 )     CurError.ProceedError(CAN_NOT_OPEN_KEYS);
      fhOut = _creat( argv[argc-1], FA_ARCH | O_BINARY);
      if( fhOut == -1 )     CurError.ProceedError(CAN_NOT_CREATE_OUT);

      brIn.SetFileHandler( fhIn );
      bwOut.SetFileHandler( fhOut );

      return 1;
}


