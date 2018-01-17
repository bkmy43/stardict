#include "thart.h"

TRef :: TRef( )
 {
   code=0;
   num=0;
 }

/*************************************************************************
			   КЛАСС  TArticle
*************************************************************************/

TArticle :: TArticle( BufRead  *Index, BufRead *Data )
 {
    pbrIndex = Index;
    pbrData = Data;
// обнуление всех указателей на строки
    for( int i=0; i< MAX_LINES; i++ )
	artline[ i ]= NULL ;
    artnum = 0;
 }

//************************************************************************
void TArticle :: DelOldInfo()
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
}

int TArticle :: ChangeArt( int num )
 {
    long position;
    int ec;

    //DelOldInfo(); //Внимание !!!  Память нужно освобождать на внешнем уровне

    if( (ec = ReadNamePosition( num, &position )) != OK )
	 return ec;

    if( (ec=ReadTextRef( position )) != OK )
	 return ec;
    return OK;
 }
//************************************************************************
TArticle :: ~TArticle(  )
 {
   // DelOldInfo();  // внимание !!! деструктор не освобождает память
 }

//************************************************************************
int TArticle :: ReadNamePosition ( int num, long* position )
// Ф-я читает из файла index.dat название статьи и адрес
// по которому ее текст можно найти в файле data.dat
 {
    BufRead& brIn = *pbrIndex ;
    brIn.bigRew( );

    do
      if( ReadInt( brIn, &artnum ) != 1 || ReadName( brIn, artname ) != 1
	  || ReadLong( brIn, position ) != 1 )
	    return WRONG_INDEXDAT;
    while( artnum != num );

    return OK;
 }

//************************************************************************
int TArticle :: ReadTextRef( long position )
// Ф-я читает из файла data.dat текст статьи и заполняет
// таблицу ссылок
{
    char buffer[120]={0};
  //--------------------------------------------------------
  // Чтение текста статьи
    pbrData->ReachEOF = NO;                // В начало файла  data.dat
    pbrData->wh = 0;
    lseek( pbrData->hand, position, SEEK_SET );
    pbrData->RenewBuf();

    for( int i=0; ReadCodedLine( *pbrData, buffer )==OK; i++ )
      {   // Сохранение прочитанного
	 artline[ i ] = new char[ strlen( buffer ) +1 ];
	 if( artline[ i ] == NULL )
	      gerrmes(NOMEM,"TArticle :: ChangeArt",0,SYSTEM_ERROR);
	 strcpy( artline[ i ] , buffer );
      }
  //--------------------------------------------------------
  // Заполнение таблицы ссылок
  int ibuf1=0, ibuf2=0, ec;
  for( i = 0; i< 256; i++ )
    {
      pbrData->ReadByte( &ibuf1 );
      ReadInt( *pbrData, &ibuf2 );

      if( ibuf1 == 0xFF )
	if( ibuf2 != artnum )
	   return WRONG_DATADAT;
	else
	   return OK;

      refTable[i].code = (char)ibuf1;
      refTable[i].num  = ibuf2;
    }
// раз мы вышли из цикла -> все 256 ссылок заняты
   gerrmes(NOMEM,"TArticle :: ChangeArt",1,SYSTEM_ERROR);
   return NOK;
}

