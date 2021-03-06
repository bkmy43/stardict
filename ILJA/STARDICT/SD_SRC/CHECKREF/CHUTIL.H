/********************************************************************
-- STARDICT Project --
�ணࠬ�� CheckRef
���ᠭ�� �㭪権 ��� �஢�ન 楫��⭮�� ��뫮�

(c) I.Vladimirsky 1997
********************************************************************/
#include "defines.h"
#include "rw.h"
#include "errors.h"

// �㭪�� �������� ���ᨢ, ᮤ�ঠ騩 ����� ��� ��������� ��⥩
void GetAllNumbers( int * AllNumbers, int* size );

// �㭪�� �������� � ���ᨢ ����� ᮤ�ঠ���� � ������ 䠩�� ��⥩
void GetNumbersFromFile( BufRead* pbrIn, int * AllNumbers, int * size);

// �஢�ઠ ��� ��뫮� �� ��� 䠩���
void CheckAllRefs( int * AllNumbers, int size );

//�஢�ઠ ��� ��뫮� � 䠩��
void CheckRefsInFile( BufRead* pbrIn, int * AllNumbers, int size);
