/********************************************************************
-- STARDICT Project --
Функции классов TArtR и CTag. Позволяют прочитать статью из файла данных
в формате RSTEXT, прверить ее правильность и записать ее в DAT файлы
(c) I.Vladimirsky 1997
********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "app.h"

extern filehand in, index, themes, data, tmp1, tmp2 ;
extern WorkMode;

extern TError CurError;
TArtR :: TArtR( BufRead* apbrInput, BufWrite* abwIndex, BufWrite* abwTh,
		BufWrite* abwData, long aposition )
{
  pbrInput = apbrInput;
  pbwIndex = abwIndex;
  pbwTh = abwTh;
  pbwData = abwData;

  position = aposition;

  artname = new char[256];

// обнуление всех кодов ссылок
  for( int i=0 ; i <  256; i++ )
	refTable[i] = 0;

// Создание нового темпового файла
  if ( (tmp1 = _creat("temp1.tmp", FA_ARCH | O_BINARY)) == -1 )
	CurError.ProceedError( CAN_NOT_CREATE_TEMP );
  tmp1 = open( "temp1.tmp", O_RDWR | O_BINARY );
  bwTemp.SetFileHandler(  tmp1 );

  for( i=0; i<256; i++ )
      ths[i] = NULL;

  CurError.code = NONE;
}

TArtR :: ~TArtR( )
{
  int i;
  delete artname;
  bwTemp.RenewBuf();
  for( i=0; i<256; i++ )
   if( ths[i] != NULL )
     {
       delete ths[i];
       ths[i] = 0;
     }
  CurError.code = NONE;
  close( tmp1 );
}

void TArtR :: ClearAll()
{
// удаление всех строк
 for( int i=0; i<256 && ths[i]!= NULL; i++ )
	delete ths[i];
// обнуление всех указателей на строки
 for( i=0; i<256; i++ )
	ths[i] = NULL;
// обнуление всех кодов ссылок
 for( i=0 ; i <  256; i++ )
	refTable[i] = 0;

 artnum = 0;
 strcpy( artname, "None" );

 bwTemp.RenewBuf();

 CurError.code = NONE;

  close( tmp1 );
// Создание нового темпового файла
  if ( (tmp1 = _creat("temp1.tmp", FA_ARCH | O_BINARY)) == -1 )
	CurError.ProceedError( CAN_NOT_CREATE_TEMP );
  bwTemp.SetFileHandler(  tmp1 );
}

int TArtR :: ReadInfo()
{
 ClearAll();

 int isNumberReady=NO;
 int isNameReady=NO;
 int isBodyReady=NO;

 char buffer[1024];
 int NumberOfThemes = 0 ; // Сколько тем прочтено

 TCTag* ptag = NULL;
 while( ! (isNumberReady==YES && isNameReady==YES &&  isBodyReady==YES) )
  {
     if (ptag != NULL) { delete ptag; ptag = NULL; }
     ptag = ProceedUntilTag_buf( *pbrInput, buffer, 1024 ); // найти тэг
// если исходный файл закончился
     if( ptag == NULL && isNumberReady==NO &&
	       isNameReady==NO &&  isBodyReady==NO )
	      return STOP_READ;

     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка

     switch ( ptag->TGCode )
      {
       case TAG_NAMEBEGIN :

	   if(isNameReady == YES ) //имя уже было прочтено
	      if( isNumberReady == YES )  CurError.ProceedError(BODY_NOT_FOUND);
	      else                        CurError.ProceedError(ART_NUMBER_NOT_FOUND);

	   ptag->ProceedTag( *pbrInput, bwTemp ); //прочитать имя
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	   strcpy( artname, ((TCTagName *)ptag)->str );
	   isNameReady = YES;
	   break;

       case TAG_NUMBER :

	   if(isNumberReady == YES ) //номер уже было прочтено
	      if( isNameReady == YES )  CurError.ProceedError(BODY_NOT_FOUND);
	      else                      CurError.ProceedError(ART_NAME_NOT_FOUND);

	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	   artnum = ((TCTagNum*)ptag)->artnum;
	   isNumberReady = YES;
	   break;

       case TAG_BODYBEGIN :

	   if(isBodyReady == YES ) //тело уже было прочтено
	      if( isNumberReady == YES )  CurError.ProceedError(ART_NAME_NOT_FOUND);
	      else                        CurError.ProceedError(ART_NUMBER_NOT_FOUND);

	   ptag->ProceedTag( *pbrInput, bwTemp ); //прочитать тело
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
           // !!!!!!!!!!!!!!!!!!!!!!!!!!!!
           for( int i=0; i< ((TCTagBody*)ptag)->KeywordsNum; i++ )
               if(  AddArtToKeyword(artnum, ((TCTagBody*)ptag)->KeywordsTable[i])
                      == KEYWORD_NOT_EXISTS )
                {
                  CreateNewKeyword(((TCTagBody*)ptag)->KeywordsTable[i]);
                  AddArtToKeyword(artnum, ((TCTagBody*)ptag)->KeywordsTable[i]);
                }

           // !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	   isBodyReady = YES;
	   break;

       case TAG_THEMEBEGIN :

	   ptag->ProceedTag( *pbrInput, bwTemp ); //прочитать название темы
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	   ths[NumberOfThemes] = new char[strlen( ((TCTagTheme*)ptag)->str ) +1 ];
	   strcpy( ths[NumberOfThemes], ((TCTagTheme*)ptag)->str );
	   NumberOfThemes++;
	   break;

       default :
	   CurError.ProceedError(FIRST_LEVEL_WRONG_TAG);
      };
  }

 if (ptag != NULL) { delete ptag; ptag = NULL; }

 bwTemp.RenewBuf();

 if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
 return OK;
}

void TArtR :: Data2Win1251()
{
 Dos2Win1251( artname );

 for( int i=0; ths[i]!=NULL; i++ )
    Dos2Win1251( ths[i] );
}

int TArtR :: test()
{
  int i;

  if( artnum == 0 )
     {  CurError.ProceedError(ART_NUMBER_NOT_FOUND); return NOK; }
  if( artnum > 20000 )
     {  CurError.ProceedError(WRONG_ART_NUMBER); return NOK; }
  if( strcmp( artname,"none" ) == 0 )
     {  CurError.ProceedError(ART_NAME_NOT_FOUND); return NOK; }
  // если такая статья уже существует
  if( ArtExists( artnum ) == 1 )
     {  CurError.ProceedError(ART_EXISTS); return NOK; }

  if( TestThemes() != OK )  return NOK;

  return OK;
}

int TArtR :: TestThemes()
// проверяет, все ли темы статьи существуют
{
  lseek( themes, 0L, SEEK_SET );
  BufRead brTh;
  brTh.SetFileHandler(  themes );

 // проверка заголовкa файла
 char filetype;
 ReadFileHeader( brTh, &filetype );
 if(filetype!='T') { CurError.ProceedError(ERROR_IN_THEMES); return NOK; }

  int ibuf=0;
  char name[256] = {0};

  for( int i = 0; ths[i] != NULL; i++ )
// поиск в themes.dat данной темы
  {
	lseek( themes, 0L, SEEK_SET );
	brTh.RenewBuf();
	ReadFileHeader( brTh, &filetype );
	while( strcmp( ths[i], name) != 0 ) // пока имя не совпадает
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // считать номер темы
		 { CurError.ProceedError( TH_NOT_EXIST ); return NOK; }

	     if( :: ReadName( brTh, name ) != 1 )    // считать имя темы
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }

// прочитать эту тему до конца
             do
		if( ReadInt( brTh, &ibuf ) != 1 )
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }
	     while(  ibuf != 0 );
	   }
  }
  return OK;
}


#define NUMBER_MODE 0
#define NAME_MODE 1


/*************************************************************************
		     ЗАПИСЬ ИНФОРМАЦИИ В ФАЙЛЫ ДАННЫХ
*************************************************************************/
int TArtR :: TransferInfo()
{
   if( TransferIndex() != OK )
    { CurError.ProceedError(ERROR_IN_INDEX ); return NOK; }
   if( TransferThemes() != OK )
    { CurError.ProceedError(ERROR_IN_THEMES ); return NOK; }
   if( TransferData() != OK )
    { CurError.ProceedError(ERROR_IN_DATA ); return NOK; }

   pbwIndex->RenewBuf();
   pbwTh->RenewBuf();
   pbwData->RenewBuf();
   position = filelength( data );
   return OK;
}

// записывает данные о статье в index.dat, причем статьи располагаются в
// алфавитном порядке
int TArtR :: TransferIndex( )
{
 const ART_HAS_BEEN_WRITTEN             = 1;
 const ART_HAS_NOT_BEEN_WRITTEN      = 0;
 int ind = open( "index.tmp", O_RDWR | O_BINARY );

 if( ( tmp2 = _creat("temp2.tmp", FA_ARCH | O_BINARY)) == -1 )
    { CurError.ProceedError(CAN_NOT_CREATE_TEMP); return NOK; }

 BufRead br;
 BufWrite bw;
 br.SetFileHandler(ind);
 bw.SetFileHandler(tmp2);
 int num;
 char name[256];
 long pos;
 int mode = ART_HAS_NOT_BEEN_WRITTEN;

 // проверка заголовкa файла
 char filetype;
 ReadFileHeader( br, &filetype );
 if(filetype!='I') { CurError.ProceedError(ERROR_IN_INDEX); return NOK; }

 // запись заголовка файла в новый файл
 WriteFileHeader( bw, 'I' );

 while( ReadInt( br, &num ) == 1 )  // номер статьи прочитан
  {
    if( ::ReadName( br, name ) != 1 || ReadLong( br, &pos ) != 1 )
	  { CurError.ProceedError(ERROR_IN_INDEX); return NOK; }

    if( mode == ART_HAS_NOT_BEEN_WRITTEN ) // статья еще не была записана
       if( StrAlphComp(name, artname) == 2 )  // по алфавиту artname < name
	   { // запись данных о нашей статье
		// запись номера статьи
		WriteInt(  bw, artnum );
		// запись названия статьи
		for(int i=0; artname[ i ] != 0; i++ )
		    bw.WriteByte( artname[i] );
		bw.WriteByte( '^' );
		// запись позиции статьи в файле данных
		bw.WriteByte( position >> 24 );
		bw.WriteByte( position >> 16 );
		bw.WriteByte( position >> 8 );
		bw.WriteByte( position & 0x000000FF );
		mode = ART_HAS_BEEN_WRITTEN;
	   }
    // записать прочтенную информацию
    WriteInt(  bw, num );
    for(int i=0; name[ i ] != 0; i++ )
	 bw.WriteByte( name[i] );
    bw.WriteByte( '^' );
    bw.WriteByte( pos >> 24 );
    bw.WriteByte( pos >> 16 );
    bw.WriteByte( pos >> 8 );
    bw.WriteByte( pos & 0x000000FF );
  }
// в файле больше нет статей
 if( mode == ART_HAS_NOT_BEEN_WRITTEN ) // статья еще не была записана
    { // запись данных о нашей статье
     // запись номера статьи
	WriteInt(  bw, artnum );
	// запись названия статьи
	for(int i=0; artname[ i ] != 0; i++ )
	    bw.WriteByte( artname[i] );
	bw.WriteByte( '^' );
	// запись позиции статьи в файле данных
	bw.WriteByte( position >> 24 );
	bw.WriteByte( position >> 16 );
	bw.WriteByte( position >> 8 );
	bw.WriteByte( position & 0x000000FF );
	mode = ART_HAS_BEEN_WRITTEN;
    }

 bw.RenewBuf();
 close( ind );
 close( tmp2 );
 copyfile("temp2.tmp", "index.tmp");
 return OK;
}

int TArtR :: TransferThemes( )
{
    int res;

    for( int i = 0; ths[i] != NULL; i++ )
      if( (res = AddArtToTheme( artnum, ths[i] )) != OK )
          {  CurError.ProceedError( res ); return NOK; }

    return OK;
}
//**************************************************************************
int TArtR :: TransferData( )
{
   int refnum;
   BufRead brTmp;
   int ibuf,j;
   int buffer[3]={0};
   int length=0;

   bwTemp.RenewBuf();
   close(tmp1);
   tmp1 = open( "temp1.tmp", O_RDWR | O_BINARY );

   brTmp.SetFileHandler(  tmp1 );
// Записать в data.dat длину данной статьи в байтах.
// Она меньше 32000 - можно использовать для ее хранения два байта
   WriteInt( *pbwData, (unsigned)filelength(tmp1) );
// текст статьи сейчас находится в brTmp;
// записать тело статьи в dat-файл, кодируя его
   while( readByte(brTmp, &ibuf) == 1 )
    {
     char ch=(char)ibuf;
     length = codeChar( ch, buffer );
     for( j=0; j<length; j++ )
	   {
	     if( writeHalf( *pbwData, buffer[j] ) == 0 )
	       CurError.ProceedError( ERROR_WRITING );
	   }
    }

   if (pbwData->wh == 1 )
 // если последний символ записан в первой половине байта дополнить байт
      if( writeHalf( *pbwData, 0 ) == 0 )
            CurError.ProceedError( ERROR_WRITING );

   WriteInt( *pbwData, artnum );

   pbwData->RenewBuf();
   return OK;
 }


/************************************************************************
			   ФУНКЦИИ КЛАССА ThemeR
************************************************************************/

ThemeR :: ThemeR( BufRead* apbrInput, BufWrite* abwTh)
{
 pbrInput = apbrInput;
 pbwTh = abwTh;
 thnum = 0;
 thname = new char[256];
 strcpy( thname, "None");
}

ThemeR :: ~ThemeR( )
{
 delete thname;
}

void ThemeR :: ClearAll()
{
 thnum = 0;
 strcpy( thname, "None");
}

int ThemeR :: ReadInfo()      // диспетчер
{
 ClearAll();

 int isNumberReady=NO;
 int isNameReady=NO;

 char buffer[1024];

 TCTag* ptag = NULL;

 while( ! (isNumberReady==YES && isNameReady==YES) )
  {
     if (ptag != NULL) { delete ptag; ptag = NULL; }
     ptag = ProceedUntilTag_buf( *pbrInput, buffer, 1024 ); // найти тэг
// если исходный файл закончился
     if( ptag == NULL && isNumberReady==NO &&  isNameReady==NO )
	      return STOP_READ;

     if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка

     switch ( ptag->TGCode )
      {
       case TAG_NUMBER :

	   if(isNumberReady == YES ) //номер уже был прочтено
		CurError.ProceedError(TH_NAME_NOT_FOUND);
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	   thnum = ((TCTagNum*)ptag)->artnum;
	   isNumberReady = YES;
	   break;

       case TAG_NAMEBEGIN :

	   if(isNameReady == YES ) //имя уже было прочтено
		CurError.ProceedError(TH_NUMBER_NOT_FOUND);
	   ptag->ProceedTag( *pbrInput, *pbwTh ); //прочитать имя
	   if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка
	   strcpy( thname, ((TCTagName *)ptag)->str );
	   isNameReady = YES;
	   break;

       default :
	   CurError.ProceedError(WRONG_TAG_IN_TH_MODE);
      };
  }

 if (ptag != NULL)     { delete ptag; ptag = NULL; }

 if(CurError.code != NONE )  return NOK; // если зарегистрирована ошибка return NOK;
 return OK;
}

void ThemeR :: Data2Win1251( ) // перекодировка в Win1251
{
  Dos2Win1251( thname );
}

int ThemeR :: test()  // проверяет правильность прочитанной информации
{
  if( ThExists( thnum ) == 1 )
     {  CurError.ProceedError(TH_EXISTS); return NOK; }
  return OK;
}

int ThemeR :: TransferInfo( )
// Записывает информацию о новой теме в файл
// themes.dat при этом сортирует темы в алфавитном порядке
{
 if ( CreateNewTheme( thnum, thname ) != OK )
     return NOK;

 return OK;
}

