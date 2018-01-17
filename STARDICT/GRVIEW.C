#include "grview.h"
#include "list.hpp"
#include "help.h"
#include "box.hpp"

#ifdef _HISTORY
extern TViewerHistory Hist;
#endif
extern TGraphView* pViewer;
extern struct ViewerColors VColors;
void ClearScreen(void);
int prnerr(int ,int ,int ,int );



static char ALL_SYMBOLS[]=
   {" _-АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяabcdefghijklmnopqrstuvwxyz0123456789!?',.;~`@#$%^&*()|\\/=\"<>"};
//*********************************************************************
//  class TFullRef
//**********************************************************************
TFullRef :: TFullRef( char code, int num, int bl, int el,
		      int bc, int ec, TFullRef* apprev )
 : TRef( code, num )
 {
    begline = bl;
    endline = el;
    begchar = bc;
    endchar = ec;
    pnext = NULL;
    pprev = apprev;
 }

//***********************************************************************
// class TGraphView
//***********************************************************************
// вспомогательная ф-я
int ShowTheme( BufRead* pbrIndex, BufRead* pbrThemes, int thnum )
{
   Rbarc(0, 0, 1024, 20, SOLID_FILL, CYAN);
   char buffer[256];
   char thname[256];
   GetThName( pbrThemes, thname, thnum );
   sprintf( buffer, "     ТЕМА  \"%s\"", thname );
   Rtextc(0, 3, buffer, WHITE);

   char* names[256];
   int qarts=256;
   if( GetArtNamesFromTh(thnum, pbrIndex, pbrThemes, names, &qarts ) != OK )
       gerrmes(NOMEM,"ShowTheme()",0,SYSTEM_ERROR);

   int r;
   int left,top=100,right,bottom,vertint=12;
   ENTER_LIST el;
// найти наибольшую длину строки
   int maxwidth=0;
   for( int k=0; k<qarts; k++ )
    { for( int g=0; names[k][g]!=0; g++ );
      if( g > maxwidth ) maxwidth = g;
    }

   const LINE_WIDTH = maxwidth;
   const LINE_HEIGHT = 20;
   left=(1024-8*1.6*LINE_WIDTH-60)/2;
   right=(1024+8*1.6*LINE_WIDTH+60)/2;
   bottom=(qarts>16) ? 610 : top+qarts*(vertint+LINE_HEIGHT)+50;

   el.SetLTCorner(left,top);
   el.SetRBCorner(right,bottom);
   el.SetBarWidth(19);
//   el.SetColors(BLUE,LIGHTCYAN,CYAN,BLACK);
   el.SetColors(CYAN,BLUE,BLACK,CYAN);
   el.SetBorder(L_DOUBLE_BORD,WHITE);
   el.SetDirection(L_HOR);
   el.SetMargins(25,25,vertint);
   el.SetData(names,qarts,0);
   el.SetColumns(L_Q_COLS,1);
   el.SetColumns(L_Q_COLS,1);
   el.SetHelpItem(19);

   el.SetBarButtonHeight(25);
   el.SetBarColors(BLUE,LIGHTCYAN,WHITE,LIGHTGRAY,WHITE);
   el.SetABCOrder( NOT_CASE_SENSITIVE, ALL_SYMBOLS );
   el.SetABCSearchBuffer( VISIBLE_SEARCH_BUFFER, LINE_WIDTH );

   r=el.Monitor();

   int artnum;
   if( r!= GM_NOK )
	 GetArtNum( pbrIndex, names[r], &artnum );
   for( int i=0; i <qarts; i++ )
	 delete names[i];

   if( r == GM_NOK )     return 0;
   else	   	         return artnum;
}


int ChooseTheme( BufRead *pbrThemes )
{
   char* names[300];
   int qths=300;
   if( GetThNamesList(pbrThemes, names, &qths ) != OK )
       gerrmes(NOMEM,"TGraphView :: ChooseTheme",0,SYSTEM_ERROR);

   Rbarc(0, 0, 1024, 20, SOLID_FILL, CYAN);
   Rtextc(0, 3, "     СПИСОК ТЕМ", WHITE);

   int r;
   int left,top=100,right,bottom,vertint=12;
   ENTER_LIST el;

// найти наибольшую длину строки
   int maxwidth=0;
   for( int k=0; k<qths; k++ )
    { for( int g=0; names[k][g]!=0; g++ );
      if( g > maxwidth ) maxwidth = g;
    }

   const LINE_WIDTH = maxwidth;
   const LINE_HEIGHT = 20;
   left=(1024-8*1.6*LINE_WIDTH-60)/2;
   right=(1024+8*1.6*LINE_WIDTH+60)/2;
   bottom=(qths>16) ? 610 : top+qths*(vertint+LINE_HEIGHT)+50;

   el.SetLTCorner(left,top);
   el.SetRBCorner(right,bottom);
   el.SetBarWidth(19);
//   el.SetColors(BLUE,LIGHTCYAN,CYAN,BLACK);
   el.SetColors(CYAN,BLUE,BLACK,CYAN);
   el.SetBorder(L_DOUBLE_BORD,WHITE);
   el.SetDirection(L_HOR);
   el.SetMargins(25,25,vertint);
   el.SetData(names,qths,0);
   el.SetColumns(L_Q_COLS,1);
   el.SetHelpItem(18);

   el.SetBarButtonHeight(25);
   el.SetBarColors(BLUE,LIGHTCYAN,WHITE,LIGHTGRAY,WHITE);
   el.SetABCOrder( NOT_CASE_SENSITIVE, ALL_SYMBOLS );
   el.SetABCSearchBuffer( VISIBLE_SEARCH_BUFFER, LINE_WIDTH );

   r=el.Monitor();

   int thnum;

   if( r!= GM_NOK )
	 GetThNum( pbrThemes, names[r], &thnum );
   for( int i=0; i <qths; i++ )
	 delete names[i];

   pViewer->show(); // чтобы стереть списк
   if( r == GM_NOK )     return 0;
   else	   	      return thnum;

}

//**************************************************************************

TGraphView :: TGraphView( BufRead * Index, BufRead *Data, BufRead *Themes,
     int GDr, int fsy, int LMLet, int RMLet, int UMStr, int DMStr, int mm ):
 TArticle( Index, Data ),
 TextScreen ( GDr, fsy, LMLet, RMLet+2, UMStr, DMStr )
{
   references = NULL;
   ActiveRef = NULL;
   MAX_LINES_ON_SCREEN = TextScreen :: LINES_ON_SCREEN;
   fline = 0;
   pbrThemes = Themes;
   MouseMode = mm;
   FkeysStatus = OFF;


   int delta = ( DownMarginStrings == 0 || UpMarginStrings == 0
		 || LeftMarginLetters == 0 ) ? 0:1;
// инициализация скроллера
   int x1=MAX_X - RMLet*8 - SCROLLER_WIDTH  +  1;
   int y1=UMStr*fsy-delta;
   int x2 = MAX_X - RMLet*8  +  1;
   int y2=MAX_Y - DMStr*fsy+delta;
   Rbackconvert( &x1, &y1 ); // из реальных в стандартные
   Rbackconvert( &x2, &y2 ); // из реальных в стандартные

   pScr = NULL;
   pScr = new TVScroller(x1, y1, x2, y2, DARKGRAY, WHITE, LIGHTGRAY);
   if( pScr == NULL )   VErrorReact( NO_MEMORY );

}

TGraphView :: ~TGraphView()
{
   if( pScr != NULL )   delete pScr;
   DelOldInfo();
}

void  TGraphView :: DelOldInfo()
{
// удаление всех строк
    for( int i=0; i< MAX_LINES && artline[i]!=NULL; i++ )
	delete artline[i];
// обнуление всех указателей на строки
    for( i=0; i< MAX_LINES; i++ )
	artline[ i ]= NULL ;
// обнуление всех кодов ссылок
    for( i=0 ; i <  256; i++ )
	refTable[i].clear();
// удаление таблицы ссылок
     if( references != NULL )
       {
	  delete references;
	  references = NULL;
       }
}

// составляет таблицу ссылок и преобразует строки к нормальному виду
int TGraphView :: setrefs( void )
 {
    int bc, ec, bl, el;
    char chcode;
    int num;

    if( references != NULL )
       {
	delete  references;
	references = NULL;
       }
    LINES_IN_TEXT = 0;
    TFullRef* plast=NULL;
    TFullRef* pr=NULL;
    for( int l=0; l< MAX_LINES && artline[l] != NULL; l++ )
     {
       for( int i= 0; artline[l][i] != '\0'; i++ )
	     if(  artline[l][i] == '&' )
	       {
		 bl = l;
		 bc = i;
		 artline[l][i] = ' ';
		 chcode =  artline[l][++i];
		 artline[l][i] = ' ';
		 while( artline[l][i] != '&' )
		   if( artline[l][i++] == '\0')
		     { if(  artline[l++] == NULL )
			   return NOK;
		       else
			   i=0;
		       LINES_IN_TEXT++;
		     }

		 artline[l][i] = ' ';
		 el = l;
		 ec = i-1;
     // нахождение нужной ссылки в таблице
		 for( int x=0; refTable[x].code != chcode; x++ )
		    if( refTable[x].code == 0 )
		       return NOK;
		 num = refTable[x].num;

		 pr = NULL;
		   pr = new TFullRef( chcode, num, bl, el, bc+1, ec+1, plast );
		 if( pr == NULL )  VErrorReact(NO_MEMORY);

		 if( plast != NULL )   plast->pnext = pr;
		 plast = pr;
		 if( references == NULL ) references = pr;
	       }
       LINES_IN_TEXT++;
     }
   return OK;
}

void TGraphView::InitViewerKeys( void )
{
 RInitFkey(30, 705, 0, " UP ", UP, BLUE,LIGHTGRAY,DARKGRAY);
 RInitFkey(94, 705, 0, " DOWN ", DOWN, BLUE,LIGHTGRAY,DARKGRAY);
 RInitFkey(183, 705, 0, " PGUP ", PGUP, BLUE,LIGHTGRAY,DARKGRAY);
 RInitFkey(271, 705, 0, " PGDN ", PGDN, BLUE,LIGHTGRAY,DARKGRAY);
 RInitFkey(359, 705, 0, " HOME ", HOME, BLUE,LIGHTGRAY,DARKGRAY);
 RInitFkey(447, 705, 0, " END ", END, BLUE,LIGHTGRAY,DARKGRAY);

 RInitFkey(537, 705, 0, " ENTER ", ENTER, BLACK,LIGHTGRAY,DARKGRAY);
 RInitFkey(639, 705, 0, " ALT-F1 ", ALT_F1, BLACK,LIGHTGRAY,DARKGRAY);

 RInitFkey(767, 705, 0, " F1 - HELP ", F1, RED,LIGHTGRAY,DARKGRAY);
 RInitFkey(921, 705, 0, " F10 ", F10, RED,LIGHTGRAY,DARKGRAY);
}

void TGraphView::SwitchViewerKeys()
{
// если это начало
// и если нельзя перейти на предыдущую ссылку
 if( fline == 0 )
  {
   RSwitchFkey( PGUP , FKEY_OFF);
   RSwitchFkey( HOME , FKEY_OFF);
   if(  ActiveRef == NULL  || ActiveRef->pprev == NULL
       || ActiveRef->pprev->begline < fline  )
	 RSwitchFkey( UP , FKEY_OFF);
   else
	 RSwitchFkey( UP , FKEY_ON);
  }
 else
  {
   RSwitchFkey( PGUP , FKEY_ON);
   RSwitchFkey( HOME , FKEY_ON);
   RSwitchFkey( UP , FKEY_ON);
  }
//если это конец
// и если нельзя перейти на следующую ссылку
 if( artline[ fline + MAX_LINES_ON_SCREEN ] == 0 )
  {
   RSwitchFkey( PGDN , FKEY_OFF);
   RSwitchFkey( END , FKEY_OFF);
   if( ActiveRef == NULL || ActiveRef->pnext==NULL ||
       ActiveRef->pnext->endline > fline+MAX_LINES_ON_SCREEN )
	 RSwitchFkey( DOWN , FKEY_OFF);
   else
	 RSwitchFkey( DOWN , FKEY_ON);
  }
 else
  {
   RSwitchFkey( PGDN , FKEY_ON);
   RSwitchFkey( END , FKEY_ON);
   RSwitchFkey( DOWN , FKEY_ON);
  }

 if( ActiveRef==NULL || ActiveRef->begline < fline ||
     ActiveRef->endline > fline + MAX_LINES_ON_SCREEN - 1 )
	 RSwitchFkey( ENTER , FKEY_OFF);
 else
	 RSwitchFkey( ENTER , FKEY_ON);
#ifdef _HISTORY
 if( Hist.hnum == 0 )
	 RSwitchFkey( ALT_F1 , FKEY_OFF);
 else
	 RSwitchFkey( ALT_F1 , FKEY_ON);
#endif
 FkeysStatus = ON;

}

void TGraphView::SwitchViewerKeysOff()
{
 RSwitchFkey( F10 , FKEY_OFF);
 RSwitchFkey( F1 , FKEY_OFF);
 RSwitchFkey( ALT_F1 , FKEY_OFF);
 RSwitchFkey( ENTER , FKEY_OFF);
 RSwitchFkey( END , FKEY_OFF);
 RSwitchFkey( HOME , FKEY_OFF);
 RSwitchFkey( PGDN , FKEY_OFF);
 RSwitchFkey( PGUP , FKEY_OFF);
 RSwitchFkey( DOWN , FKEY_OFF);
 RSwitchFkey( UP , FKEY_OFF);
}

void TGraphView::RemoveViewerKeys()
{
 RRemoveFkey(F10);
 RRemoveFkey(F1);
 RRemoveFkey(ALT_F1);
 RRemoveFkey(ENTER);
 RRemoveFkey(END);
 RRemoveFkey(HOME);
 RRemoveFkey(PGDN);
 RRemoveFkey(PGUP);
 RRemoveFkey(DOWN);
 RRemoveFkey(UP);
 FkeysStatus = OFF;
}


int TGraphView :: ChangeArt( int num )
{
  int ec;
#ifdef _HISTORY
  if( artnum != 0 )
     Hist.AppendLast( artnum, fline, ActiveRef->code );
  Hist.curnum = Hist.hnum-1;
  Hist.count = 0;
#endif

  DelOldInfo();
  if( (ec = TArticle::ChangeArt( num )) != OK )
	VErrorReact(ec);
  setrefs();

  ActiveRef = references; // если NULL то в данной статье нет ссылок
  fline=0;
  return OK;
}

#ifdef _HISTORY
void TGraphView :: Return( )  // возврат к предыдущей статье
{
  if( Hist.hnum == 0 ) return;

  if( artnum != 0 && Hist.count++ == 0)
    {  Hist.AppendLast( artnum, fline, ActiveRef->code );
       Hist.curnum = Hist.hnum-2;  }
  else
    {  if( Hist.curnum == 0 )  Hist.curnum = Hist.hnum-1;
       else                    Hist.curnum--;  }

  int ec;
  DelOldInfo();
  TViewerInfo & info  =  Hist.history[ (Hist.curnum) ];
  if( (ec = TArticle::ChangeArt( info.number )) != OK )
	VErrorReact(ec);
  setrefs();
  fline = info.fline;
  TFullRef* pr;
  for( pr=references; pr->code!=info.ActiveRefCode; pr=pr->pnext );
  ActiveRef = pr;
}
#endif

void TGraphView :: SetUserColors()
{
      colors.ClrText       =  VColors.ClrText;
      colors.ClrBack       =  VColors.ClrBack;
      colors.ClrRef        =  VColors.ClrRef;
      colors.ClrActRef     =  VColors.ClrActRef;
      colors.ClrBackActRef =  VColors.ClrBackActRef;
      colors.ClrBord       =  VColors.ClrBord;
}

/***************************************************************************
	*******************  ВНЕШНИЕ ФУНКЦИИ ************************
**************************************************************************/
int TGraphView :: view()
 {
    int res;
    show();
    NameNumOut();
    if( MouseMode == ON )   Rmouse_show();
    while( ( res=keyanalisys() ) != ASK_QUIT )
     {
       if( res == ASK_GO_TO_REF )
// активная сылка не уехала за экран
	if( ActiveRef->begline >= fline &&
	  ActiveRef->endline <= fline + MAX_LINES_ON_SCREEN - 1 )
	   if( ActiveRef->num < 0x0F00 )
	     {
	       ChangeArt( ActiveRef->num );
	       ClearClient( );
	       NameNumOut();
	       show();
	     }
	   else   return ActiveRef->num; // возвр. запрос на показ темы
#ifdef _HISTORY
       if( res == ASK_RETURN )
	 {
	  Return( );
	  ClearClient( );
	  NameNumOut();
	  show();
	 }
#endif
     }
    if( MouseMode == ON )   Rmouse_hide();
    return 0;
 }


void TGraphView :: show() // печать строк с fline
{
   SetUserColors();
   if( artnum == 0 ) { ClearScreen(); return; }
   ClearClient( );
// рамка
   DrawBord();
   if( MouseMode == ON )   Rmouse_hide();
   for( int i=fline; i < fline + MAX_LINES_ON_SCREEN &&
	artline[i] != NULL; i++ )
	  PutLine( i - fline, artline[i] );
   InsRefs();
   if( MouseMode == ON )   Rmouse_hide();
   pScr->Draw();
   pScr->SetPos( (float)fline/LINES_IN_TEXT );
   if( MouseMode == ON )   Rmouse_show();
}

int TGraphView::keyanalisys( )
{
 if( FkeysStatus == OFF )
    InitViewerKeys();
 SwitchViewerKeys();
 int x,y, key;
 key = Revent( &x, &y);
 if( key == -1 )
    // проверить скроллер
  if( (key=pScr->Analisys( x, y )) == 1 ) // не область скроллера
     {
       Rconvert( &x, &y );
       // проверить все ссылки не на них ли щелкнули мышью
       for( TFullRef* pr=references; pr != NULL; pr=pr->pnext )
	{
	  for( int j=pr->begline; j<= pr->endline; j++ )
	  {
	   if( j <fline || j>fline + MAX_LINES_ON_SCREEN - 1 )
	      continue;
	   int b = ( j==pr->begline ) ? pr->begchar : 0 ;
	   int e = ( j==pr->endline ) ? pr->endchar : strlen( artline[j] );
	   int x1, x2, y1, y2;
	   GetTextRect(j-fline,b,e,&x1,&y1,&x2,&y2);
	   if( x>=x1 && x<=x2 && y>=y1 && y<=y2 )
	      if( pr==ActiveRef )
		{ RemoveViewerKeys();
		  return ASK_GO_TO_REF; }
	      else
		{ UnInsActRef(); ActiveRef = pr; InsActRef();
		  return 1; }
	  }
	}

       return 1;
     }

// обработка клавиатуры
 switch( key  )
  {
   case UP:
   case LEFT:
   // если активная ссылка не первая и предыдущая есть на экране
	 if( ( ActiveRef != NULL ) && (ActiveRef->pprev != NULL )
	     && ( ActiveRef->pprev->begline >= fline))
	       {
		     UnInsActRef();
		     ActiveRef=ActiveRef->pprev;
		     InsActRef();
	       }
	 else
	       moveup();
	 break;

   case DOWN:
   case RIGHT:
   // если активная ссылка не последняя и следующая есть на экране
	 if( ( ActiveRef != NULL ) && ActiveRef->pnext!=NULL &&
	     ( ActiveRef->pnext->endline < fline+MAX_LINES_ON_SCREEN ) )
	       {
		     UnInsActRef();
		     ActiveRef=ActiveRef->pnext;
		     InsActRef();
	       }

	 else
	     movedown();
	 break;

   case PGDN:  pagedown(); break;
   case PGUP:  pageup(); break;
   case HOME:  gotoline( 0 ); break;
   case END:   if( LINES_IN_TEXT > MAX_LINES_ON_SCREEN )
		  gotoline(LINES_IN_TEXT - MAX_LINES_ON_SCREEN );
	       break;
   case ENTER: if( ActiveRef != NULL )
		  { RemoveViewerKeys();
		    return ASK_GO_TO_REF;  }   break;
#ifdef _HISTORY
   case ALT_F1:  RemoveViewerKeys();
		 return ASK_RETURN;
#endif
   case F10:     SwitchViewerKeysOff();
		 RemoveViewerKeys();
		 return ASK_QUIT;
   case F1:      HShowHelp(21);  break;
   default :     return 1;
  }
  return 1;
}

void TGraphView :: gotoline( int linenum )
{
     fline = linenum ;
// активизвровать ближайшую ссылку
     if( ActiveRef != NULL )
       {
	  for(TFullRef* pr=references; pr->begline < fline ; pr=pr->pnext)
	       if( pr->pnext == NULL )  break;
	  ActiveRef = pr;
       }
     show();
}

void TGraphView :: pagedown()
 {
   if( artline[ fline + MAX_LINES_ON_SCREEN ] != NULL )
     {
	fline += MAX_LINES_ON_SCREEN ;
// активизвровать ближайшую ссылку
	if( ActiveRef != NULL )
	  {
	     for(TFullRef* pr=references; pr->begline < fline ; pr=pr->pnext)
		  if( pr->pnext == NULL )  break;
	     ActiveRef = pr;
	  }
	show();
     }
 }

void TGraphView :: pageup()
 {
   if( fline == 0 )
	return;
   if( fline - MAX_LINES_ON_SCREEN >= 0 )
	fline-=MAX_LINES_ON_SCREEN;
   else
	fline = 0;
// активизвровать ближайшую ссылку
   if( ActiveRef != NULL )
     {
	for(TFullRef* pr=references; pr->begline < fline ; pr=pr->pnext)
	  if( pr->pnext == NULL )  break;
	ActiveRef = pr;
     }
   show();
 }

void TGraphView :: movedown()
 {
   if( artline[ fline + MAX_LINES_ON_SCREEN ] != 0 ) // если на экране нет
     {						 // последней строки
	if( MouseMode == ON )   Rmouse_hide();
	// ф-я класса TextScreen
	ScrollUp( artline[ fline + MAX_LINES_ON_SCREEN ] );
	fline++;
	InsRefs(IN_SINGLE_LINE, fline + MAX_LINES_ON_SCREEN - 1 );
	if( MouseMode == ON )   Rmouse_hide();
	pScr->SetPos((float)fline/LINES_IN_TEXT);
	if( MouseMode == ON )   Rmouse_show();
     }
 }

void TGraphView :: moveup()
 {
   if( fline != 0 )
   {
	if( MouseMode == ON )   Rmouse_hide();
	ScrollDown( artline[ --fline ] ); // ф-я класса TextScreen
	InsRefs( IN_SINGLE_LINE, fline );
	if( MouseMode == ON )   Rmouse_hide();
	pScr->SetPos((float)fline/LINES_IN_TEXT);
	if( MouseMode == ON )   Rmouse_show();
   }
 }

void TGraphView :: NameNumOut( void )  // информация о статье
 {
   if( MouseMode == ON )   Rmouse_hide();
   Rbarc(LeftMarginLetters*8, 0, 1024-(RightMarginLetters-1)*8,
	 20, SOLID_FILL, colors.ClrBackActRef);
   char buffer[256];
   sprintf( buffer, "      СТАТЬЯ  \"%s\"", artname );
   Rtextc(0, 3, buffer, colors.ClrRef);
   if( MouseMode == ON )   Rmouse_show();
 }

void TGraphView :: InsRefs( int mode, int nline )
{
   char* buf = new char[150];
   if( MouseMode == ON )   Rmouse_hide();
   for( TFullRef* pr =references ; pr != NULL; pr=pr->pnext )
    {
       if( pr == ActiveRef)
	 {
	      InsActRef();
	      if( MouseMode == ON )   Rmouse_hide();
	      continue;
	 }
       for( int j=pr->begline; j<= pr->endline; j++ )
	 {
	   if( j <fline || j>fline + MAX_LINES_ON_SCREEN - 1 )
	      continue;
	   if( mode == IN_SINGLE_LINE && j!=nline)
	      continue;
	   int b = ( j==pr->begline ) ? pr->begchar : 0 ;
	   int e = ( j==pr->endline ) ? pr->endchar: strlen( artline[j] );
	   for( int q=0; q< strlen(artline[j]); q++ )
		buf[q] = (q>=b && q<=e) ? artline[j][q]:' ';
	   buf[q] = '\0';
	   PutLine( j-fline, buf, colors.ClrRef, APPEND );
	 }
    }
   if( MouseMode == ON )   Rmouse_show();
   delete buf;
}

void TGraphView ::InsActRef()    //выделяет в тексте активную ссылку
{
   if( ActiveRef == NULL ) // ссылки нет
	return;
   if( MouseMode == ON )   Rmouse_hide();
   char* buf = new char[150];
   for( int j=ActiveRef->begline; j<= ActiveRef->endline; j++ )
	 {
	   if( j <fline || j>fline + MAX_LINES_ON_SCREEN - 1 )
	      continue;
	   int b = ( j==ActiveRef->begline ) ? ActiveRef->begchar : 0 ;
	   int e = ( j==ActiveRef->endline ) ? ActiveRef->endchar : strlen( artline[j] );
	   for( int q=0; q< strlen(artline[j]); q++ )
		buf[q] = (q>=b && q<=e) ? artline[j][q]:' ';
	   buf[q] = '\0';
	   PutTextBackground( j-fline, b+1, e+2, colors.ClrBackActRef );
	   PutLine( j-fline, buf, colors.ClrActRef, APPEND );
	 }
   if( MouseMode == ON )   Rmouse_show();
   delete buf;
}

int TGraphView :: ChooseArt( void )
{
   char* names[3000];
   int qarts=3000;
   if( GetArtNamesList(pbrIndex, names, &qarts ) != OK )
       gerrmes(NOMEM,"TGraphView :: ChooseArt",0,SYSTEM_ERROR);

   Rbarc(0, 0, 1024, 20, SOLID_FILL, colors.ClrBackActRef);
   Rtextc(0, 3, "     СПИСОК СТАТЕЙ", colors.ClrRef);

   int r;
   int left,top=100,right,bottom,vertint=12;
   ENTER_LIST el;

// найти наибольшую длину строки
   int maxwidth=0;
   for( int k=0; k<qarts; k++ )
    { for( int g=0; names[k][g]!=0; g++ );
      if( g > maxwidth ) maxwidth = g;
    }

   const LINE_WIDTH = maxwidth;
   const LINE_HEIGHT = 20;
   left=(1024-8*1.6*LINE_WIDTH-60)/2;
   right=(1024+8*1.6*LINE_WIDTH+60)/2;
   bottom=(qarts>16) ? 610 : top+qarts*(vertint+LINE_HEIGHT)+50;

   el.SetLTCorner(left,top);
   el.SetRBCorner(right,bottom);
   el.SetBarWidth(19);
//   el.SetColors(BLUE,LIGHTCYAN,CYAN,BLACK);
   el.SetColors(CYAN,BLUE,BLACK,CYAN);
   el.SetBorder(L_DOUBLE_BORD,WHITE);
   el.SetDirection(L_HOR);
   el.SetMargins(25,25,vertint);
   el.SetData(names,qarts,0);
   el.SetColumns(L_Q_COLS,1);
   el.SetHelpItem(17);

   el.SetBarButtonHeight(25);
   el.SetBarColors(BLUE,LIGHTCYAN,WHITE,LIGHTGRAY,WHITE);
   el.SetABCOrder( NOT_CASE_SENSITIVE, ALL_SYMBOLS );
   el.SetABCSearchBuffer( VISIBLE_SEARCH_BUFFER, LINE_WIDTH );

   r=el.Monitor();

   int artnum;

   if( r!= GM_NOK )
	 GetArtNum( pbrIndex, names[r], &artnum );
   for( int i=0; i <qarts; i++ )
	 delete names[i];

   if( r == GM_NOK )     return 0;
   else	   	         return artnum;
}

int TGraphView :: ChooseArtFromHistory( void )
{
   char* names[MAX_HISTORY];
   int qarts=MAX_HISTORY;
   if( GetHistoryList(pbrIndex, names, &qarts ) != OK )
       gerrmes(NOMEM,"TGraphViewer :: ChooseArtFromHistory",0,SYSTEM_ERROR);

   Rbarc(0, 0, 1024, 20, SOLID_FILL, colors.ClrBackActRef);
   Rtextc(0, 3, "     ИСТОРИЯ", colors.ClrRef);

   int r;
   int left,top=100,right,bottom,vertint=12;
   ENTER_LIST el;

// найти наибольшую длину строки
   int maxwidth=0;
   for( int k=0; k<qarts; k++ )
    { for( int g=0; names[k][g]!=0; g++ );
      if( g > maxwidth ) maxwidth = g;
    }

   const LINE_WIDTH = maxwidth;
   const LINE_HEIGHT = 20;
   left=(1024-8*1.6*LINE_WIDTH-60)/2;
   right=(1024+8*1.6*LINE_WIDTH+60)/2;
   bottom=(qarts>16) ? 610 : top+qarts*(vertint+LINE_HEIGHT)+50;

   el.SetLTCorner(left,top);
   el.SetRBCorner(right,bottom);
   el.SetBarWidth(19);
//   el.SetColors(BLUE,LIGHTCYAN,CYAN,BLACK);
   el.SetColors(CYAN,BLUE,BLACK,CYAN);
   el.SetBorder(L_DOUBLE_BORD,WHITE);
   el.SetDirection(L_HOR);
   el.SetMargins(25,25,vertint);
   el.SetData(names,qarts,0);
   el.SetColumns(L_Q_COLS,1);
   el.SetHelpItem(20);

   el.SetBarButtonHeight(25);
   el.SetBarColors(BLUE,LIGHTCYAN,WHITE,LIGHTGRAY,WHITE);
   el.SetABCOrder( NOT_CASE_SENSITIVE, ALL_SYMBOLS );
   el.SetABCSearchBuffer( VISIBLE_SEARCH_BUFFER, LINE_WIDTH );

   r=el.Monitor();

   int artnum;

   if( r!= GM_NOK )
	 GetArtNum( pbrIndex, names[r], &artnum );
   for( int i=0; i <qarts; i++ )
	 delete names[i];

   if( r == GM_NOK )     return 0;
   else	   	         return artnum;
}

void TGraphView :: UnInsActRef()
{
   if( ActiveRef == NULL ) // ссылки нет
	return;
   if( MouseMode == ON )   Rmouse_hide();
   char* buf = new char[150];
   for( int j=ActiveRef->begline; j<= ActiveRef->endline; j++ )
	 {
	   if( j <fline || j>fline + MAX_LINES_ON_SCREEN - 1 )
	      continue;
	   int b = ( j==ActiveRef->begline ) ? ActiveRef->begchar : 0 ;
	   int e = ( j==ActiveRef->endline ) ? ActiveRef->endchar: strlen( artline[j] );
	   for( int q=0; q< strlen(artline[j]); q++ )
		buf[q] = (q>=b && q<=e) ? artline[j][q]:' ';
	   buf[q] = '\0';
	   PutTextBackground( j-fline, b+1, e+2, colors.ClrBack );
	   PutLine( j-fline, buf, colors.ClrRef, APPEND );
	 }
   if( MouseMode == ON )   Rmouse_show();
   delete buf;
}


void TGraphView :: ClearClient( )
{
   if( MouseMode == ON ) Rmouse_hide();
   int x1=  LeftMarginLetters*8;
   int y1=  UpMarginStrings*LineSizeY;
   int x2 = MAX_X - RightMarginLetters*8;
   int y2 = MAX_Y - DownMarginStrings*LineSizeY;
   Rbackconvert( &x1, &y1 ); // из реальных в стандартные
   Rbackconvert( &x2, &y2 ); // из реальных в стандартные
   Rbarc( x1, y1, x2, y2, SOLID_FILL, colors.ClrBack);
   if( MouseMode == ON ) Rmouse_show();
}

void TGraphView :: DrawBord()
{
    if( DownMarginStrings == 0 || UpMarginStrings == 0
	|| LeftMarginLetters == 0 )
	  return;
    int x1=LeftMarginLetters*8-2;
    int x2=MAX_X - RightMarginLetters*8+2;
    int y1=UpMarginStrings*LineSizeY-2;
    int y2=MAX_Y-DownMarginStrings*LineSizeY+2;
    Rbackconvert( &x1, &y1 ); // из реальных в стандартные
    Rbackconvert( &x2, &y2 ); // из реальных в стандартные
    Rcol(colors.ClrBord);

    if( MouseMode == ON )   Rmouse_hide();
    Rline(x1,y1,x2,y1);
    Rline(x1,y2,x2,y2);
    Rline(x1,y1,x1,y2);
    if( MouseMode == ON )   Rmouse_show();

}

#ifdef _PRINT_ALLOWED
int TGraphView :: PrintArt()
{
   if( fprintf(stdprn, "АСТРОЛОГИЧЕСКАЯ ЭНЦИКЛОПЕДИЯ STARDICT\r\n"
	      "Copyright 1998 Fedor Rojanski\r\n"
	      "СТАТЬЯ:\"%s\"\r\n\r\n",artname) == EOF )
	return NOK;

   for( int i=0; artline[i] != NULL; i++ )
      if( fprintf(stdprn, "%s\r\n", artline[i]) == EOF )
	return NOK;

   if(fprintf(stdprn,"%c", 12 ) == EOF)
     return NOK;

   return OK;
}

int TGraphView :: PrintArtToFile( char* fname)
{
   char filename[15];
   strcpy( filename, fname );
   strcat( filename,".txt");

   FILE *f;
   if( (f = fopen(filename, "a+")) == NULL )
     return NOK;

   if( fprintf(f, "АСТРОЛОГИЧЕСКАЯ ЭНЦИКЛОПЕДИЯ STARDICT\r\n"
	      "Copyright 1998 Fedor Rojanski\r\n"
	      "СТАТЬЯ:\"%s\"\r\n\r\n",artname) == EOF )
      return NOK;

   for( int i=0; artline[i] != NULL; i++ )
     if(fprintf(f, "%s\r\n", artline[i]) == EOF)
      return NOK;

   if(fprintf(f, "\r\n") == EOF)
      return NOK;

   fclose(f);

   return OK;

}
#endif
//************************************************************************

TViewerHistory :: TViewerHistory()
{
	curnum = hnum = count = 0;
}

void TViewerHistory :: Clear()
{
	curnum = hnum = count = 0;
}

TViewerHistory :: ~TViewerHistory()
{
	Clear();
}

void TViewerHistory :: AppendLast( int n, int fl, int arc)
//добавить инф. о статье ( теме )
{
	for( int i=0; i<hnum; i++ )
// закоментированное условие позаволяет заносить одну и ту же
// статью в историю при условии несовпадения активной сылки или позиции
//	   if( history[i].number == n && history[i].fline == fl &&
//	       history[i].ActiveRefCode == arc )
	   if( history[i].number == n )
		{
		   for( int j=i; j<hnum-1; j++ )
		     {
		      history[j].number=history[j+1].number;
		      history[j].fline=history[j+1].fline;
		      history[j].ActiveRefCode=history[j+1].ActiveRefCode;
		     }
		   hnum--;
		   break;
		}

	if( hnum == MAX_HISTORY )
	  {
	    for( int i=0; i<hnum-1; i++ )
	     {
	      history[i].number=history[i+1].number;
	      history[i].fline=history[i+1].fline;
	      history[i].ActiveRefCode=history[i+1].ActiveRefCode;
	     }
	    history[hnum-1].number=n;
	    history[hnum-1].fline=fl;
	    history[hnum-1].ActiveRefCode=arc;
	  }

	else
	  {
	    hnum++;
	    history[hnum-1].number=n;
	    history[hnum-1].fline=fl;
	    history[hnum-1].ActiveRefCode=arc;
	  }
}

//*************************************************************************

void TGraphView :: VErrorReact( int ec )
{
   char* mes;
   if( MouseMode == ON )   Rmouse_hide();
   switch( ec )
      {
	case WRONG_INDEXDAT:  mes = "Ошибка в файле index.dat "; break ;
	case WRONG_DATADAT :  mes = "Ошибка в файле data.dat " ; break ;
	case NO_MEMORY     :  clrscr(); mes = "Недостаточно памяти" ; break ;
	case WRONG_TH      :  mes = "Файл themes.dat испорчен"; break;
      };

   Rmessage( mes, LIGHTRED,CYAN,BLACK );
   exit( NOK );
}


char TryMore,PrinterReady;

int PreparePrinter()
/* return:  0 - OK; -1 - don't try */
{
ret:
TryMore='N';
PrinterReady='Y';
harderr(prnerr);
fputc('\n',stdprn);
if (PrinterReady!='Y')
   {
   if (TryMore=='Y') goto ret;
   else return(-1);
   }
TryMore='0';
return(0);
}


//#pragma warn -par
int prnerr(int ,int ,int ,int )
{
int r;

if (TryMore=='0')   Box(B_ONE_BUTTON,B_MESSAGE,"СБОЙ НА ПРИНТЕРЕ...");
else
   {
   PrinterReady='N';
   r=Box(B_TWO_BUTTON,B_MESSAGE,"ПРИНТЕР НЕ ГОТОВ. ПРОДОЛЖАТЬ ?");
   if (r==0) TryMore='Y';
   }
hardretn(0);
return(0);
}
