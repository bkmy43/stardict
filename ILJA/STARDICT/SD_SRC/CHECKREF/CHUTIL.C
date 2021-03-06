/********************************************************************
-- STARDICT Project --
�ணࠬ�� CheckRef
�������⥫�� �⨫��� ��� �஢�ન 楫��⭮�� ��뫮�

(c) I.Vladimirsky 1997
********************************************************************/
#include <stdio.h>
#include "chutil.h"

// ����� ��室��� 䠩���
extern char InNames[MAX_IN_FILES][MAX_FILE_NAME_LEN];

// ���� ��� ��� ��室��� 䠩���
extern BufRead* pbrIn[MAX_IN_FILES];

// � �㤥� ����ᥭ� ���ଠ�� ��� ��� �ந��襤�� �訡���
extern TError CurError;

//********************************************************************
// �㭪�� �������� ���ᨢ, ᮤ�ঠ騩 ����� ��� ��������� ��⥩
void GetAllNumbers( int * AllNumbers, int* size )
{
   int s=0;
   *size = 0;

   for( int i=0; i< MAX_IN_FILES && pbrIn[i] != NULL; i++ )
    {
      printf("      * ����祭�� ᯨ᪠ ����஢ ��⥩ �� 䠩�� %s.\n", InNames[i]);
      (pbrIn[i])->bigRew();
      GetNumbersFromFile( pbrIn[i], AllNumbers + *size, &s );
      *size += s;
    }
}

//********************************************************************
// �㭪�� �������� � ���ᨢ ����� ᮤ�ঠ���� � ������ 䠩�� ��⥩
void GetNumbersFromFile( BufRead* pbrIn, int * AllNumbers, int * size)
{
   *size = 0;
   int ibuf;

   while( GoToChar( *pbrIn, '<' ) != EOF )
    {
       SeekSpaces(*pbrIn);
       if(  pbrIn->ReadByte( &ibuf ) != 1 )
           CurError.ProceedError( WRONG_TAG );

       if( (char)ibuf != '#' )
           continue;
       SeekSpaces(*pbrIn);
       if( ReadIntFromChar( *pbrIn, AllNumbers + *size ) != 1 )
           CurError.ProceedError( WRONG_TAG );
       (*size)++;
    }
}

//********************************************************************
// �஢�ઠ ��� ��뫮� �� ��� 䠩���
void CheckAllRefs( int * AllNumbers, int size )
{
   for( int i=0; i< MAX_IN_FILES && pbrIn[i] != NULL; i++ )
    {
      printf("      * �஢�ઠ ��뫮� � 䠩�� %s.\n", InNames[i]);
      (pbrIn[i])->bigRew();
      CheckRefsInFile( pbrIn[i], AllNumbers, size );
    }
}

//********************************************************************
//�஢�ઠ ��� ��뫮� � 䠩��
void CheckRefsInFile( BufRead* pbrIn, int * AllNumbers, int size)
{
   int ibuf;

   while( GoToChar( *pbrIn, '<' ) != EOF )
    {
       SeekSpaces(*pbrIn);
       if(  pbrIn->ReadByte( &ibuf ) != 1 )
           CurError.ProceedError( WRONG_TAG );

       if( (char)ibuf != 'L' )
           continue;
       SeekSpaces(*pbrIn);
       if( ReadIntFromChar( *pbrIn, &ibuf ) != 1 )
           CurError.ProceedError( WRONG_TAG );

       for( int i=0; i<size, AllNumbers[i] != ibuf; i++ )
       if( i== size )
         {
            CurError.line = pbrIn->CurLine;
            CurError.ProceedError( REF_TO_BAD_OBJECT );
            CurError.line = 0;
         }

    }
}

