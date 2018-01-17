#include  "app.h"
extern filehand in, index, themes, data ;
char ALL_SYMBOLS[]={" _-АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяabcdefghijklmnopqrstuvwxyz0123456789!?',.;~`@#$%^&*()|\\/=\"<>"};


// Ф-ии для работы с файлами
// для appdata

//*********************************************************************
int ReadTextToInt(  BufRead& brIn, int* res )
// Ф-я читает из файла три char`овых байта и преобразует их в целое
// возвр. код ошибки
  {
     int b1,b2,b3 ;

     if (  brIn.ReadByte( &b1 ) != 1 || brIn.ReadByte(  &b2 ) != 1
	   || brIn.ReadByte( &b3 ) != 1 )
	      return 0;

     if (  CharToInt( (char)b1,  &b1 )!=1 || CharToInt( (char)b2,  &b2 )!=1
	   || CharToInt( (char)b3, &b3)!=1 )
	      return 0;

     *res =  16*16*b1 + 16*b2 + b3;

     return 1;
  }
//***************************************************************************
int CharToInt( char ch, int* res )
  {
    switch ( ch )
     {
	case '0' :   *res = 0; break;
	case '1' :   *res = 1; break;
	case '2' :   *res = 2; break;
	case '3' :   *res = 3; break;
	case '4' :   *res = 4; break;
	case '5' :   *res = 5; break;
	case '6' :   *res = 6; break;
	case '7' :   *res = 7; break;
	case '8' :   *res = 8; break;
	case '9' :   *res = 9; break;
	case 'A' :   *res = 10; break;
	case 'B' :   *res = 11; break;
	case 'C' :   *res = 12; break;
	case 'D' :   *res = 13; break;
	case 'E' :   *res = 14; break;
	case 'F' :   *res = 15; break;

	default : return 0;
     };

    return 1;
  }
//*********************************************************************
int ReadCompName ( BufRead& brIn, char* name )
// Ф-я читает строку с текущего положения по символ ']'
// и сравнивает его с name
 {
   char buf[256]={0};

   ReadName( brIn, buf );
   return !strcmp( name,buf );
 }

//***********************************************************************
int codeChar( char ch, int* buffer )
//   Функция получает символ и переводит его в условный код,
// при этом часто встречающиеся в тексте символы ( русские буквы ) коди-
// руются 4 битами, более редкие 8 битами и, наконец, самые редкие 12
// битами. Функция возвращает количество полубайт в закодированном символе.
 {
      switch( ch )
	{
//-------------------------------------------------------------------------
	   case ' ':  buffer[0] = 0   ; return 1;
	   case 'в':  buffer[0] = 1   ; return 1;
	   case 'е':  buffer[0] = 2   ; return 1;
	   case 'и':  buffer[0] = 3   ; return 1;
	   case 'к':  buffer[0] = 4   ; return 1;
	   case 'л':  buffer[0] = 5   ; return 1;
	   case 'а':  buffer[0] = 6   ; return 1;
	   case 'н':  buffer[0] = 7   ; return 1;
	   case 'о':  buffer[0] = 8   ; return 1;
	   case 'п':  buffer[0] = 9   ; return 1;
	   case 'р':  buffer[0] = 10  ; return 1;
	   case 'с':  buffer[0] = 11  ; return 1;
	   case 'т':  buffer[0] = 12  ; return 1;
	   case 'м':  buffer[0] = 13  ; return 1;
//-------------------------------------------------------------------------
	   case 'б':  buffer[0] = OFTEN_CODE; buffer[1]= 0   ; return 2;
	   case 'г':  buffer[0] = OFTEN_CODE; buffer[1]= 1   ; return 2;
	   case 'д':  buffer[0] = OFTEN_CODE; buffer[1]= 2    ; return 2;
	   case 'з':  buffer[0] = OFTEN_CODE; buffer[1]= 3    ; return 2;
	   case 'й':  buffer[0] = OFTEN_CODE; buffer[1]= 4    ; return 2;
	   case 'у':  buffer[0] = OFTEN_CODE; buffer[1]= 5    ; return 2;
	   case 'х':  buffer[0] = OFTEN_CODE; buffer[1]= 6    ; return 2;
	   case 'ц':  buffer[0] = OFTEN_CODE; buffer[1]= 7    ; return 2;
	   case 'ч':  buffer[0] = OFTEN_CODE; buffer[1]= 8    ; return 2;
	   case 'ы':  buffer[0] = OFTEN_CODE; buffer[1]= 9    ; return 2;
	   case 'ь':  buffer[0] = OFTEN_CODE; buffer[1]= 10   ; return 2;
	   case 'я':  buffer[0] = OFTEN_CODE; buffer[1]= 11   ; return 2;
	   case '.':  buffer[0] = OFTEN_CODE; buffer[1]= 12   ; return 2;
	   case ',':  buffer[0] = OFTEN_CODE; buffer[1]= 13   ; return 2;
	   case 'ю': buffer[0] = OFTEN_CODE; buffer[1]= 14   ; return 2;
	   case '\n': buffer[0] = OFTEN_CODE; buffer[1]= 15   ; return 2;
//-------------------------------------------------------------------------
	   default: buffer[0] = SELDOM_CODE;
		    buffer[1] = ch >> 4;
		    buffer[2] = ch&0x0F;
		    return 3;
//-------------------------------------------------------------------------
	};

 }
//*************************************************************************
int WriteCodedString( BufWrite &bw, char* str )
{
     int buffer[3]={0};
     int i, j, length;
     int end = NO;

     for( i=0; end != YES; i++ )
	{
	  if( str[i] == 0 )
	      end = YES;
	  // закодировать символ
	  length = codeChar( ((end == YES) ? '\n' : str[i]), buffer );
	  for( j=0; j<length; j++ )
	      {
		if( writeHalf( bw, buffer[j] ) == 0 )
		      return 0;
	      }
	 }

     return 1;
}
//*************************************************************************
int GetLetterNumber( char c )
{
   for( int i=0; ALL_SYMBOLS[i] != 0 && ALL_SYMBOLS[i]!=c; i++ );
   return i;
}
//*************************************************************************
int StrAlphComp(char* str1, char* str2)
// сравнивает строки и возвращает номер той из них,
// которая по алфавиту первая
{
   int n1, n2;
   for( int i=0; ; i++ )
     {
	if( str1[i] == '\0' )  return 1;
	if( str2[i] == '\0' )  return 2;
	n1= GetLetterNumber(str1[i]);
	n2= GetLetterNumber(str2[i]);
	if( n1 > n2 )  return 2;
	if( n1 < n2 )  return 1;
     }
}
//*************************************************************************
int SetTextEnd( BufWrite& bw )
// Ф-я устанавливает последними тремя полубайтами в тексте FFF

 {
    if( !writeHalf( bw, 15 ) )  // F
	  return 0;
     if( !writeHalf( bw, 15 ) )  // F
	  return 0;
    if( !writeHalf( bw, 15 ) )          // F
	  return 0;

    if( bw.wh==1 )                // Если в конце стоит первая половина байта
	writeHalf( bw, 15 );      // Дописать байт до конца

    return 1;
 }

//*********************************************************************
void CreateDat()
{
   int f;

   if( (f=open( "index.dat", O_RDWR | O_BINARY )) == -1 )
    {
      printf( "Не могу открыть файл INDEX.DAT --- СОЗДАЮ НОВЫЙ !!!\n");
      _creat("index.dat",FA_ARCH | O_BINARY);
      printf( "Нажмите любую клавишу\n");
      getch();
    }
   close(f);

   if( (f=open( "data.dat", O_RDWR | O_BINARY )) == -1 )
    {
      int ver;
      printf( "Не могу открыть файл DATA.DAT --- СОЗДАЮ НОВЫЙ !!!\n");
      _creat("data.dat",FA_ARCH | O_BINARY);
      close(f);

      FILE* tmp = fopen( "data.dat", "w" );
      printf( "Необходимо задать версию ( 0< x <256 )\n" );
      scanf("%d",&ver);
      fputc( ver, tmp);
      fclose( tmp);

      printf( "Нажмите любую клавишу\n");
      getch();
    }

   if( (f=open( "themes.dat", O_RDWR | O_BINARY )) == -1 )
    {
      printf( "Не могу открыть файл THEMES.DAT --- СОЗДАЮ НОВЫЙ !!!\n");
      _creat("themes.dat",FA_ARCH | O_BINARY);
      printf( "Нажмите любую клавишу\n");
      getch();
    }
   close(f);

}

int CreateBakFiles( void )
 {
   if(    copyfile( "index.dat","index.bak") != 0
       || copyfile("themes.dat", "themes.bak") != 0
       || copyfile("data.dat","data.bak") != 0  )
       {
	  printf( "ОШИБКА: Ошибка при создании bak-файла.\n");
	  return 0;
       }
   else
       return 1;
 }
//*************************************************************************
int CreateTmpFiles( void )
 {
   if(  copyfile("data.dat","data.tmp") != 0  ||
	copyfile("index.dat","index.tmp") != 0  ||
	copyfile("themes.dat","themes.tmp") != 0 )
       {
	  printf( "ОШИБКА: Ошибка при создании tmp-файла.\n");
	  return 0;
       }
   return 1;
 }
//*************************************************************************
int SaveChanges( void )
 {
   if(    copyfile( "index.tmp","index.dat") != 0
       || copyfile("themes.tmp", "themes.dat") != 0
       || copyfile("data.tmp","data.dat") != 0  )
       {
	  printf( "ОШИБКА: Ошибка при сохранении результата работы.\n");
	  return 0;
       }
   else
       return 1;
 }
//*************************************************************************
int ArtExists( int number )
 {
    int num = 0;
    long pos;
    char name[50];
    BufRead brIndex;

    lseek( index, 0L, SEEK_SET );
    brIndex.SetFileHandler( index );

    while( num != number )
      {
	if( ReadInt( brIndex, &num  ) != 1 )
	   return 0;
	ReadName( brIndex, name );
	ReadLong( brIndex, &pos );
      }

    return 1;
 }
//***************************************************************
int ThExists( int number )
 {
    int ibuf, num = 0;
    long pos;
    char name[50];
    BufRead brTh;

    lseek( themes, 0L, SEEK_SET );
    brTh.SetFileHandler( themes );

    while( num != number )
     {
       if( ReadInt( brTh, &num ) != 1  )     // считать номер темы
	   return 0;
       ReadName( brTh, name );
       ibuf = 0;
       while(  ibuf != 0x0FFF )        // прочитать эту тему до конца
	   ReadInt( brTh, &ibuf );

     }

    return 1;
 }


