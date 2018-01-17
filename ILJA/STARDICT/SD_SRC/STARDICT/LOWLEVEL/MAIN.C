/********************************************************************
-- STARDICT Project --
                             main.c

Тестовые функции
(c) I.Vladimirsky 1997
********************************************************************/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "rw.h"
#include "defines.h"
#include "exart.h"

typedef int filehand;

filehand fhIndex, fhThemes, fhData, fhKeys;


//*********************************************************************
int main( )
   {
      OpenDataFiles( );

      TKeywordList* keylist = new TKeywordList();
      TArticleList* artlist = keylist->GetNthArtList(3);

      printf("\n");
      for(int i=0; i < artlist->GetItemsNum(); i++)
        {
           printf("Article N %d. NAME \"%s\"\n", artlist->GetNthNumber(i),
                                     artlist->GetNthName(i) );
        }

      delete artlist;
      delete keylist;

      CloseDataFiles();

      getch();
      return 1 ;
  }

//***********************************************************************


