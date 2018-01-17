#include <graphics.h>
#include "grview.h"
#include "graph.h"

#ifdef _HISTORY
   extern TViewerHistory* pHist;
#endif

BufRead* pIndex;
BufRead* pData;
BufRead* pThemes;

int OpenDataFiles( void );
void ActivateViewer( TGraphView *pv );

void main( void )
 {
   if( OpenDataFiles() != 1 )
     return;

   Ropen( GM_EGA );
   int mm=MOUSE_OFF;
// мышь удачно установлена
   if( Rmouse_set() == GRON )	mm = MOUSE_ON;

#ifdef _HISTORY
   pHist = new TViewerHistory;
#endif

  // TGraphView* pv = new TGraphView( pIndex, pData, pThemes, EGA,
    //				    16, 0, 0, 0, 0, ON );
   //int num=pv->ChooseArt();
 //  if( num == NOK )  return;
  // pv->ChangeArt(2);
  // ActivateViewer(pv);
  ShowTheme( pIndex, pThemes, 3842 );
#ifdef _HISTORY
   delete pHist;
#endif
   Rclose();
   delete pIndex;
   delete pData;
   delete pThemes;
 }
//***************************************************
void ActivateViewer( TGraphView *pv )
{
    int thnum;
    int artnum;

    while( (thnum=pv->view() ) != 0 )
     {
       #ifdef _HISTORY
       pHist->Clear();
       #endif
       if( (artnum = ShowTheme( pIndex, pThemes, thnum )) != 0 )
	   pv->ChangeArt( artnum );
     }
}

//**********************************************************************
int OpenDataFiles( void )
 {
   _fmode = O_BINARY;
   int themes, index, data;

   themes = open( "themes.dat", O_RDWR | O_BINARY );
   index = open( "index.dat", O_RDWR | O_BINARY );
   data = open( "data.dat", O_RDWR | O_BINARY );

   if( index == -1  )
     {
	printf("ОШИБКА: Ошибка при открытии файла index.dat \n" );
	return 0;
     }

   if(  themes==-1  )
     {
	printf("ОШИБКА: Ошибка при открытии файла themes.dat \n" );
	return 0;
     }

   if(  data==-1  )
     {
	printf("ОШИБКА: Ошибка при открытии файла data.dat \n" );
	return 0;
     }

   pIndex = new BufRead;
   pData = new BufRead;
   pThemes = new BufRead;

   pIndex->SetFileHandler( index );
   pData->SetFileHandler( data );
   pThemes->SetFileHandler( themes );

  return 1;
 }
