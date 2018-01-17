/********************************************************************
-- STARDICT Project --
Функции классов TArtR и CTag. Позволяют прочитать статью из файла данных
в формате RSTEXT, прверить ее правильность и записать ее в DAT файлы
(c) I.Vladimirsky 1997
********************************************************************/

#include <stdio.h>
#include <string.h>
//#include <stdio.h>

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
	   isBodyReady = YES;
	   break;

       case TAG_THEMEBEGIN :

	   ptag->ProceedTag( *pbrInput, bwTemp ); //прочитать имя
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
 return OK;
}

/*int TArtR :: test()
{
  int i;

  if( artnum == 0 )
      return ART_NUMBER_NOT_FOUND;
  if( artnum > 0xF00 )
      return WRONG_ART_NUMBER;
  if( strcmp( artname,"none" ) == 0 )
      return ART_NAME_NOT_FOUND;
  if( artline[0] == NULL )
      return TEXT_NOT_FOUND;
  // если такая статья уже существует
  if( ArtExists( artnum ) == 1 )
      return ART_EXISTS;

  if((i=TestThemes()) != OK )  return i;

  if(WorkMode != ART_READ_NOREFTEST)
   {
     if( (i=TestRefs1()) != OK )   return i;
     if( (i=TestRefs2()) != OK )   return i;
     if( (i=TestRefs3()) != OK )   return i;
   }

  return OK;
}


}
 */
int TArtR :: TestThemes()
// проверяет, все ли темы статьи существуют
{
  lseek( themes, 0L, SEEK_SET );
  BufRead brTh;
  brTh.SetFileHandler(  themes );
  int ibuf=0;
  char name[256] = {0};

  for( int i = 0; ths[i] != NULL; i++ )
// поиск в themes.dat данной темы
  {
	lseek( themes, 0L, SEEK_SET );
	brTh.RenewBuf();
	while( strcmp( ths[i], name) != 0 ) // пока имя не совпадает
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // считать номер темы
		 { CurError.ProceedError( TH_NOT_EXIST ); return NOK; }

	     if( :: ReadName( brTh, name ) != 1 )    // считать имя темы
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }

// прочитать эту тему до конца
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		 { CurError.ProceedError( ERROR_IN_TH ); return NOK; }
	   }
  }
  return OK;
}


#define NUMBER_MODE 0
#define NAME_MODE 1

/*int TArtR :: TestRefs3()
// ф-я проверяет все ли ссылки указывают на существующие объекты
{
 long prevpos; // для запоминания предыдущей позиции в файле
 int readmode = NUMBER_MODE;
 char str[MAX_CHARACTERS_IN_LINE];
 for( int k=0; refTable[k].code != 0; k++ )
  {
    // ссылка на себя
    if( refTable[k].num == artnum )
	return SELFREF;

    // проверить существует ли такой объект
    if( !(ArtExists( refTable[k].num ) == 1  ||
	  ThExists( refTable[k].num ) == 1) )
       {
	prevpos = ftell( fr );    // запомнить положение
	fseek( fr, 0L, SEEK_SET);
	readmode = NUMBER_MODE;
	while(1)
	 {
	  if( fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL )
	    { fseek( fr, prevpos, SEEK_SET);
	      return REF_TO_BAD_OBJECT;
	    }
	  if( readmode == NAME_MODE )
	    {
	      readmode = NUMBER_MODE;
	      continue;
	    }
	  if( str[0] == '#' && readmode == NUMBER_MODE )
	   {
	    if( atoi(str+1) == refTable[k].num )
	      break;
	    else
	      readmode = NAME_MODE;
	   }
	 }
	fseek( fr, prevpos, SEEK_SET);
       }
  }
 return OK;
} */

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

 while( ReadInt( br, &num ) == 1 )  // номер статьи прочитан
  {
    if( ::ReadName( br, name ) != 1 || ReadLong( br, &pos ) != 1 )
	  return NOK;

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
    lseek( themes, 0L, SEEK_SET );
    BufRead brTh;
    brTh.SetFileHandler(  themes );

    char* buffer = NULL;
    buffer = new char[100*1024];
    if( buffer == NULL )
       { CurError.ProceedError( NOT_ENOUPH_MEMORY ); return NOK; }
    int ibuf=0;
    char name[256] = {0};
    int i, j, k;

    for( i = 0; ths[i] != NULL; i++ )
     {
// поиск в themes.dat данной темы
	 while( strcmp( ths[i], name) != 0 ) // пока имя не совпадает
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // считать номер темы
		 { delete buffer; CurError.ProceedError( TH_NOT_EXIST ); return NOK; }
	     if( :: ReadName( brTh, name ) != 1 )    // считать имя темы
		 { delete buffer; return NOK; }
// прочитать эту тему до конца
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		     { delete buffer; return NOK; }
	   }
// Нужная тема найдена
// Сохранение файла с текущего положения до конца в переменной buffer
	 for( j=0; brTh.ReadByte( (int*)(buffer + j) ) == 1; j++ ) ;

	 lseek( themes, -j - 2 , SEEK_CUR );
	 WriteInt( *pbwTh, artnum );   // Записать ноиер статьи
	 WriteInt( *pbwTh, 0x0FFF );   // Запись кода конца

	 for( k=0; k<j; k++ )                // записать весь
	    pbwTh->WriteByte( buffer[k] );   // файл до конца

	 pbwTh->RenewBuf( );
	 lseek( themes, 0L, SEEK_SET  );  // перейти в начало файла
	 brTh.RenewBuf();
     }

    delete buffer;
    return OK;
}

int TArtR :: TransferData( )
{
   int refnum;
   BufRead brTmp;
   int ibuf,j;
   int buffer[3]={0};
   int length=0;

   brTmp.SetFileHandler(  tmp1 );

// текст статьи сейчас находится в brTemp;
// записать тело статьи в dat-файл, кодируя его
   while( readByte(brTmp, &ibuf) == 1 )
    {
     length = codeChar( (char)ibuf, buffer );
     for( j=0; j<length; j++ )
	{
	  if( writeHalf( *pbwData, buffer[j] ) == 0 )
	     CurError.ProceedError( ERROR_WRITING );
	}
    }

   SetTextEnd( *pbwData );
   WriteInt( *pbwData, artnum );

   pbwData->RenewBuf();
   return OK;
 }


/************************************************************************
                           ФУНКЦИИ КЛАССА ThemeR
************************************************************************/
/*ThemeR :: ThemeR( FILE* afr, BufWrite* abwTh)
{
 fr = afr;
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
 int count = 0;  // счетчик знаков '#'
 char str[MAX_CHARACTERS_IN_LINE];
 int ec, FirstLine = YES;
 long prevpos; // для запоминания предыдущей позиции в файле
 ClearAll();
// читать строки до конца статьи
 do
  {
    prevpos = ftell( fr );    // запомнить положение
    if (fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL) // конец файла
	if( FirstLine == YES ) // если чтение темы только что начато
	    return STOP_READ; // в файле больше нет тем
	else
	    break;    // чтение закончено
    FirstLine = NO; // первая строка уже прочитана
    if(  str[ strlen(str) - 1 ] == '\n' )
	str[ strlen(str) - 1 ] = 0; // убрать \n

    if( str[0]  == '#')
      {
	if( ++count == 1 )
	  if( (thnum = atoi( str + 1 )) < 0xF00 || thnum<0 )
	      return WRONG_TH_NUMBER;

	if( count == 2 )
	  strcpy( thname, str+1 );
      }
  }
 while( !( str[0] == '#' && count == 3 )); // если '#' встречена в третий раз
					  // т. е. это первая строка следующей статьи

 if( str[0] == '#' )  // была прочитана "чужая" строка
    fseek( fr, prevpos, SEEK_SET);

 if( (ec = test()) != OK )
    return ec;
 else
    return OK;

}

int ThemeR :: test()  // проверяет правильность прочитанной информации
{
  if( thnum == 0 )
     return TH_NUMBER_NOT_FOUND;
  if( strcmp( thname, "None") == 0 )
     return TH_NAME_NOT_FOUND;

  if( ThExists( thnum ) == 1 )
      return TH_EXISTS;

  return OK;
}

int ThemeR :: TransferInfo( )
// сортирует в алфавитном порядке
{
 const TH_HAS_BEEN_WRITTEN              = 1;
 const TH_HAS_NOT_BEEN_WRITTEN          = 0;
 int thm = open( "themes.tmp", O_RDWR | O_BINARY );
 _fmode = O_BINARY;
 int tmp = creat("temp2.tmp", S_IREAD|S_IWRITE );
 BufRead br;
 BufWrite bw;
 br.SetFileHandler(thm);
 bw.SetFileHandler(tmp);
 int num;
 char name[256];
 int  members[256];
 int mode = TH_HAS_NOT_BEEN_WRITTEN;

 while( ReadInt( br, &num ) == 1 )  // номер темы прочитан
  {
    // прочитать тему
    if( ::ReadName( br, name ) != 1 )
	  return NOK;
    for( int i=0; i<256; i++)  members[i]=0 ;
    for( i=0; ; i++ )
      { if( ReadInt( br, members+i ) != 1 )
	   return NOK;
	if( members[i] == 0x0FFF )
	   {  members[i] = 0;
	      break;
	   }
      }

    if( mode == TH_HAS_NOT_BEEN_WRITTEN ) // тема еще не была записана
       if( StrAlphComp(name, thname) == 2 )  // по алфавиту thname < name
	    { // запись данных о нашей теме
	      // запись номера темы
	      WriteInt( bw, thnum );
	      // запись названия темы
	      for(i=0; thname[ i ] != 0; i++ )
		    bw.WriteByte( thname[i] );
	      bw.WriteByte( '^' );

	      WriteInt( bw, 0x0FFF );
	      mode = TH_HAS_BEEN_WRITTEN;
            }
    // записать прочтенную информацию
    if( WriteInt( bw, num ) != 1 )   return NOK;
    for( i=0; name[i] != 0; i++ )
          bw.WriteByte( name[i] );
    bw.WriteByte( '^' );
    for( i=0; members[i] != 0; i++ )
         if( WriteInt( bw, members[i] ) != 1 )   return NOK;
    WriteInt( bw, 0x0FFF );
  }
// в файле больше нет тем
 if( mode == TH_HAS_NOT_BEEN_WRITTEN ) // статья еще не была записана
    { // запись данных о нашей теме
      // запись номера темы
      WriteInt(  bw, thnum );
      // запись названия темы
      for( int i=0; thname[ i ] != 0; i++ )
            bw.WriteByte( thname[i] );
      bw.WriteByte( '^' );

      WriteInt( bw, 0x0FFF );
      mode = TH_HAS_BEEN_WRITTEN;
    }

 bw.RenewBuf();
 close( thm );
 close( tmp );
 copyfile("temp2.tmp", "themes.tmp");
 return OK;
}

  */