#include <stdio.h>
#include <string.h>
//#include <stdio.h>

#include "app.h"
extern filehand in; //, index, themes, data ;
extern WorkMode;

TArtR :: TArtR( FILE* afr, FILE* afw )
{
  fr = afr;
  fw = afw;

  artname = new char[256];
// обнуление всех указателей на строки
  for( int i=0; i<= MAX_LINES; i++ )
      artline[ i ]= NULL ;
  for( i=0; i<256; i++ )
      ths[i] = NULL;
}

TArtR :: ~TArtR( )
{
  int i;
  delete artname;
  for( i=0; i<MAX_LINES; i++ )
   if( artline[i] != NULL )
     {
       delete artline[i];
       artline[i] = 0;
     }
  for( i=0; i<256; i++ )
   if( ths[i] != NULL )
     {
       delete ths[i];
       ths[i] = 0;
     }
}

void TArtR :: ClearAll()
{
// удаление всех строк
 for( int i=0; i< MAX_LINES && artline[i]!=NULL; i++ )
	delete artline[i];
 for( i=0; i<256 && ths[i]!= NULL; i++ )
	delete ths[i];
// обнуление всех указателей на строки
 for( i=0; i< MAX_LINES; i++ )
	artline[ i ]= NULL ;
 for( i=0; i<256; i++ )
	ths[i] = NULL;
// обнуление всех кодов ссылок
 for( i=0 ; i <  256; i++ )
	refTable[i].clear();

 artnum = 0;
 strcpy( artname, "None" );
}

int TArtR :: ReadInfo()
{
 char str[MAX_CHARACTERS_IN_LINE];
 int count = 0;  // счетчик знаков '#'
 int FirstLine = YES;
 int ec;
 long prevpos; // для запоминания предыдущей позиции в файле
 ClearAll();
// читать строки до конца статьи
 do
  {
    prevpos = ftell( fr );    // запомнить положение
    if (fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL) // конец файла
	if( FirstLine == YES ) // если чтение статьи только что начато
	    return STOP_READ; // в файле больше нет статей
	else
	    break;    // чтение закончено
    FirstLine = NO; // первая строка уже прочитана
    if(  str[ strlen(str) - 1 ] == '\n' )
	str[ strlen(str) - 1 ] = 0; // убрать \n

    switch( str[0] )
	  { case '#':   if( ++count == 1 )
			  if((ec = ReadNumber( str + 1 )) != OK )
			     return ec;
			if( count == 2 )
			  if((ec = ReadName( str + 1 )) != OK )
			     return ec;
			break;
	    case '@':   ReadTheme( str + 1 ); break;
	    case '$':   if((ec = ReadRef( str + 1 )) != OK )
			     return ec;
			break;
	    case '~':   if((ec = ReadText( str )) != OK )
			     return ec;
			break;
	  };
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

int TArtR :: test()
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
//  if( ArtExists( artnum ) == 1 )
//      return ART_EXISTS;

//  if((i=TestThemes()) != OK )  return i;

//  if(WorkMode != ART_READ_NOREFTEST)
//   {
     if( (i=TestRefs1()) != OK )   return i;
     if( (i=TestRefs2()) != OK )   return i;
//     if( (i=TestRefs3()) != OK )   return i;
//   }

  return OK;
}

int TArtR :: ReadNumber( char *  str )
{
  artnum = atoi( str );
  if ( artnum == 0 )  return WRONG_ART_NUMBER;
  return OK;
}

int TArtR :: ReadName( char * str )
{
  strcpy( artname, str );
  return OK;
}

int TArtR :: ReadTheme( char * str )
{
  for( int i=0; ths[i] != NULL; i++ ); // найти последнюю незанятую тему
  ths[i] = new char[ strlen( str ) + 1];
  if( ths[i] == NULL )
     return NOT_ENOUGH_MEMORY;
  strcpy( ths[i], str );
  return OK;
}

int TArtR :: ReadText( char * str )
{
  int lastline = NO; // это еще не последняя строка
  int firstline = YES;

  for( int i=0; str[0] != '^' && lastline != YES; i++)
   {
// если в первой строке нет текста
    if( firstline == YES )
     {
      if( strlen(str) == 1 )
       {
	if ((fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL)
	     || str[0] =='#')
	    return TEXT_END_NOT_FOUND;
	}
      else
	str[0] = ' ';
      firstline = NO;
     }

// если это последняя строка
    if( str[ strlen(str) - 1 ] == '^')
       { lastline = YES;
	 str[ strlen(str) - 1 ] = 0;
       }
// добавление строки
    artline[i] = new char[ strlen( str ) + 1];
    if( artline[i] == NULL )
	 return NOT_ENOUGH_MEMORY;
    strcpy( artline[i], str );
    if ((fgets( str, MAX_CHARACTERS_IN_LINE, fr ) == NULL) || str[0] =='#')
	 return TEXT_END_NOT_FOUND;
// убрать \n
    if(  str[ strlen(str) - 1 ] == '\n' )
	str[ strlen(str) - 1 ] = 0;
   }
  return OK;
}

int TArtR :: ReadRef( char* str )
{
    for( int i=0; refTable[i].code != 0; i++ );  // найти последнюю ссылку
    refTable[i].code = str[0];
    refTable[i].num  = atoi( str + 2 );

    return OK;
}
  /*
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
		  return TH_NOT_EXIST;
	     if( :: ReadName( brTh, name ) != 1 )    // считать имя темы
		   return ERROR_IN_TH;
// прочитать эту тему до конца
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		     return ERROR_IN_TH;
	   }
  }
  return OK;
}
*/

#define IN_THE_REFTEXT      0
#define OUT_OF_THE_REFTEXT  1
#define NOT_FOUND           0
#define FOUND               1

int TArtR :: TestRefs1()
// ф-я проверяет все ли ссылки из таблицы встречаются в тексте
{
  int ReadMode = OUT_OF_THE_REFTEXT;
  int result = NOT_FOUND;

  for( int k=0; refTable[k].code != 0; k++ )
   { // просмотреть весь техт статьи
    ReadMode = OUT_OF_THE_REFTEXT;
    result = NOT_FOUND;
    for( int i=0; artline[i] != NULL && result == NOT_FOUND; i++ )
       for( int j=0; artline[i][j] != 0 && result == NOT_FOUND; j++ )
	   if( artline[i][j] == '&' )
	    {// если найдено начало ссылки
	     if( ReadMode == OUT_OF_THE_REFTEXT )
	      {
		ReadMode = IN_THE_REFTEXT;
		if( artline[i][j+1] == refTable[k].code )
		       result = FOUND;
	      }
	     // если найден конец ссылки
	     else ReadMode = OUT_OF_THE_REFTEXT;
	    }

    if(  result == NOT_FOUND )  return BAD_REF_IN_TABLE;
   }
  return OK;
}

int TArtR :: TestRefs2()
// ф-я проверяет все ли ссылки из текста встречаются в таблице
{
  int ReadMode = OUT_OF_THE_REFTEXT;
  // просмотреть весь техт статьи
  for( int i=0; artline[i] != NULL; i++ )
    for( int j=0; artline[i][j] != 0 ; j++ )
	{
	   if( artline[i][j] == '&')
	     {
		if( ReadMode == OUT_OF_THE_REFTEXT )  // если найдено начало ссылки
		 {
		  ReadMode = IN_THE_REFTEXT;
		  // искать эту ссылку в таблице
		  for( int k=0; refTable[k].code != 0; k++ )
		     if( refTable[k].code == artline[i][j+1] )
			break;
		  if(  refTable[k].code == 0 )  return BAD_REF_IN_TEXT;
		 }
		// если найден конец ссылки
		else  ReadMode = OUT_OF_THE_REFTEXT;
	     }
	}
  return OK;
}
#define NUMBER_MODE 0
#define NAME_MODE 1

/*
int TArtR :: TestRefs3()
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
}
*/
int TArtR :: GetRefNumber( char refcode)
{
    for(int i=0; refTable[i].code != 0; i++)
       if( refTable[i].code == refcode )
	   return refTable[i].num;

    return NOK;
}

#define OUT_OF_REF  0
#define INSIDE_REF  1

//минимальное кол-во пробелов в первой строке параграфа
#define    PARAGRAPH_SPACE_NUM 3
//минимальное кол-во пробелов в центрованной строке
#define    CENTERLINE_SPACE_NUM 10


int TArtR :: ConvertLine(int num, int & mode)
{
   char buffer[1024] = {0};
   char buffer2[16] = {0};

   int isChanges = NO;
   int i, j;
   i = j = 0;
// вставка тэга параграфа.
   for( ; artline[num][i] == ' '; i++);
// теперь i - количество пробелов в начале строки
   if( i >= PARAGRAPH_SPACE_NUM )
     {
	isChanges = YES;
	buffer[j++] =   TG_BEGIN_BULLET;
	buffer[j++] = CTG_PARAGRAPH;

	if(i >= CENTERLINE_SPACE_NUM )
	  { buffer[j++] = ' ';
	    buffer[j++] = '1'; }

	buffer[j++] = TG_END_BULLET;
     }

   for( ; artline[num][i] != 0; i++, j++)
      if( artline[num][i] == '&' )
	 {
	  isChanges = YES;
	  if( mode == OUT_OF_REF)
	     {
	       mode = INSIDE_REF;
	       buffer[j] = TG_BEGIN_BULLET;
	       sprintf( buffer2, "%d", GetRefNumber( artline[num][++i]) );
	       buffer[++j]=CTG_LINK;
	       buffer[++j]=0;
	       strcat( buffer, buffer2 );
	       j+=strlen(buffer2);
	       buffer[j]=TG_END_BULLET;
	     }
	  else
	     {
	       buffer[j] =   TG_BEGIN_BULLET;
	       buffer[++j] = TG_CLOSE;
	       buffer[++j] = CTG_LINK;
	       buffer[++j] = TG_END_BULLET;
	       mode = OUT_OF_REF;
	     }

	 }
      else
	  buffer[j] = artline[num][i];

   buffer[j]=0;

   if(isChanges == YES)
    {
       delete artline[num];
       artline[num] = new char[strlen(buffer)+1];
       strcpy(artline[num], buffer);
    }

   return mode;
}
/*************************************************************************
		     ЗАПИСЬ ИНФОРМАЦИИ В ФАЙЛЫ ДАННЫХ
*************************************************************************/
int TArtR :: WriteInfo()
{
// запись номера статьи
   WriteCTag(fw, CTG_NUMBER, artnum);
   NewLine(fw);

// запись имени статьи
   WriteCTag(fw, CTG_NAME);
   fprintf(fw, "%s", artname);
   WriteCTagEnd(fw, CTG_NAME);
   NewLine(fw);

// запись тем статьи
   for(int i=0; ths[i]!=NULL; i++)
    {
      WriteCTag(fw, CTG_THEME);
      fprintf(fw, "%s", ths[i]);
      WriteCTagEnd(fw, CTG_THEME);
      NewLine(fw);
    }

// запись текста статьи
   NewLine(fw);
   WriteCTag(fw, CTG_BODY);
   int mode = OUT_OF_REF; // с начала первой строки не может идти ссылка
   for(i=0; artline[i] != NULL; i++)
     {
       mode = ConvertLine(i, mode);
       fprintf( fw, "%s\n", artline[i] );
     }
   WriteCTagEnd(fw, CTG_BODY);
   NewLine(fw);

   NewLine(fw);
   return OK;
}

/*
// записывает данные о статье в index.dat, причем статьи располагаются в
// алфавитном порядке
int TArtR :: TransferIndex( )
{
 const ART_HAS_BEEN_WRITTEN		= 1;
 const ART_HAS_NOT_BEEN_WRITTEN      = 0;
 int ind = open( "index.tmp", O_RDWR | O_BINARY );
 _fmode = O_BINARY;
 int tmp = creat("temp.tmp", S_IREAD|S_IWRITE );
 BufRead br;
 BufWrite bw;
 br.SetFileHandler(ind);
 bw.SetFileHandler(tmp);
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
 close( tmp );
 copyfile("temp.tmp", "index.tmp");
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
       return NOT_ENOUGH_MEMORY;

    int ibuf=0;
    char name[256] = {0};
    int i, j, k;

    for( i = 0; ths[i] != NULL; i++ )
     {
// поиск в themes.dat данной темы
	 while( strcmp( ths[i], name) != 0 ) // пока имя не совпадает
	   {
	     if( ReadInt( brTh, &ibuf ) != 1  )     // считать номер темы
		{ delete buffer; return TH_NOT_EXIST ; }
	     if( :: ReadName( brTh, name ) != 1 )    // считать имя темы
		{ delete buffer; return ERROR_IN_TH; }
// прочитать эту тему до конца
	     ibuf = 0;
	     while(  ibuf != 0x0FFF )
		if( ReadInt( brTh, &ibuf ) != 1 )
		     { delete buffer; return ERROR_IN_TH; }
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
   int i=0;
   int refnum;

   for( i= 0; artline[i] != NULL; i++ )
      WriteCodedString( *pbwData, artline[i] );

   SetTextEnd( *pbwData );

   for( i = 0; refTable[i].code != 0; i++ )
      {
	 pbwData->WriteByte( refTable[i].code );
	 WriteInt( *pbwData, refTable[i].num );
      }

   pbwData->WriteByte( 0xFF );
   WriteInt( *pbwData, artnum );
   return OK;
 }
 */

/************************************************************************
			   ФУНКЦИИ КЛАССА ThemeR
************************************************************************/
ThemeR :: ThemeR( FILE* afr, FILE* afw)
{
 fr = afr;
 fw = afw;
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

//  if( ThExists( thnum ) == 1 )
 //     return TH_EXISTS;

  return OK;
}

/*
int ThemeR :: TransferInfo( )
// сортирует в алфавитном порядке
{
 const TH_HAS_BEEN_WRITTEN		= 1;
 const TH_HAS_NOT_BEEN_WRITTEN          = 0;
 int thm = open( "themes.tmp", O_RDWR | O_BINARY );
 _fmode = O_BINARY;
 int tmp = creat("temp.tmp", S_IREAD|S_IWRITE );
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
 copyfile("temp.tmp", "themes.tmp");
 return OK;
}

*/