/********************************************************************
-- STARDICT Project --
Дополнительные утилиты работы с файлами с помощью BufRead и BufWrite
структур. Описания BufRead и BufWrite см. в rw.h.
(c) I.Vladimirsky 1997
********************************************************************/

#include  "app.h"

extern filehand in, index, themes, data, keys ;
char ALL_SYMBOLS[]={" _-└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀ABCDEFGHIJKLMNOPQRSTUVWXYZрстуфхцчшщъыьэюяЁёЄєЇїЎў°∙·√№¤■ abcdefghijklmnopqrstuvwxyz0123456789!?',.;~`@#$%^&*()|\\/=\"<>"};
char DOS_SYMBOLS[]={"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя"};
char WIN1251_SYMBOLS[]={"└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀рстуфхцчшщъыьэюяЁёЄєЇїЎў°∙·√№¤■ "};

int members[MAX_KEYWORDS_IN_ART];

extern data_version;
extern format_version;

// Ф-ии для работы с файлами
// для appdata

//*********************************************************************
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
           case 'т':  buffer[0] = 1   ; return 1;
           case 'х':  buffer[0] = 2   ; return 1;
           case 'ш':  buffer[0] = 3   ; return 1;
           case 'ъ':  buffer[0] = 4   ; return 1;
           case 'ы':  buffer[0] = 5   ; return 1;
           case 'р':  buffer[0] = 6   ; return 1;
           case 'э':  buffer[0] = 7   ; return 1;
           case 'ю':  buffer[0] = 8   ; return 1;
           case 'я':  buffer[0] = 9   ; return 1;
           case 'Ё':  buffer[0] = 10  ; return 1;
           case 'ё':  buffer[0] = 11  ; return 1;
           case 'Є':  buffer[0] = 12  ; return 1;
           case 'ь':  buffer[0] = 13  ; return 1;
//-------------------------------------------------------------------------
           case 'с':  buffer[0] = OFTEN_CODE; buffer[1]= 0   ; return 2;
           case 'у':  buffer[0] = OFTEN_CODE; buffer[1]= 1   ; return 2;
           case 'ф':  buffer[0] = OFTEN_CODE; buffer[1]= 2    ; return 2;
           case 'ч':  buffer[0] = OFTEN_CODE; buffer[1]= 3    ; return 2;
           case 'щ':  buffer[0] = OFTEN_CODE; buffer[1]= 4    ; return 2;
           case 'є':  buffer[0] = OFTEN_CODE; buffer[1]= 5    ; return 2;
           case 'ї':  buffer[0] = OFTEN_CODE; buffer[1]= 6    ; return 2;
           case 'Ў':  buffer[0] = OFTEN_CODE; buffer[1]= 7    ; return 2;
           case 'ў':  buffer[0] = OFTEN_CODE; buffer[1]= 8    ; return 2;
           case '√':  buffer[0] = OFTEN_CODE; buffer[1]= 9    ; return 2;
           case '№':  buffer[0] = OFTEN_CODE; buffer[1]= 10   ; return 2;
           case ' ':  buffer[0] = OFTEN_CODE; buffer[1]= 11   ; return 2;
           case '.':  buffer[0] = OFTEN_CODE; buffer[1]= 12   ; return 2;
           case ',':  buffer[0] = OFTEN_CODE; buffer[1]= 13   ; return 2;
           case '■':  buffer[0] = OFTEN_CODE; buffer[1]= 14   ; return 2;
           case '¤': buffer[0] = OFTEN_CODE; buffer[1]= 15   ; return 2;
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
/*int SetTextEnd( BufWrite& bw )
// Ф-я устанавливает последними тремя полубайтами в тексте FFF

 {
    BYTE ch=TAG_ENDOFARTBODY;

    if( !writeHalf( bw, 15 ) )  // F
          return 0;
    if( !writeHalf( bw, ch>>4 ) )
          return 0;
    if( !writeHalf( bw, ch&0x0F ) )          // F
          return 0;

    if( bw.wh==1 )                // Если в конце стоит первая половина байта
        writeHalf( bw, 15 );      // Дописать байт до конца

    return 1;
 }
  */
//*********************************************************************
void CreateDat()
{
   int f;

   if( (f=open( "index.dat", O_RDWR | O_BINARY )) == -1 )
    {
      printf( "Не могу открыть файл INDEX.DAT --- СОЗДАЮ НОВЫЙ !!!\n");
      _creat("index.dat",FA_ARCH | O_BINARY);
      printf("Версия DAT файла - %d\n", data_version);
      printf( "Нажмите любую клавишу\n\n");
      getch();
    }
   else
     close(f);

   if( (f=open( "data.dat", O_RDWR | O_BINARY )) == -1 )
    {
      printf( "Не могу открыть файл DATA.DAT --- СОЗДАЮ НОВЫЙ !!!\n");
      _creat("data.dat",FA_ARCH | O_BINARY);
      printf("Версия DAT файла - %d\n", data_version);
      printf( "Нажмите любую клавишу\n\n");
      getch();
    }
   else
     close(f);

   if( (f=open( "themes.dat", O_RDWR | O_BINARY )) == -1 )
    {
      printf( "Не могу открыть файл THEMES.DAT --- СОЗДАЮ НОВЫЙ !!!\n");
      _creat("themes.dat",FA_ARCH | O_BINARY);
      printf("Версия DAT файла - %d\n", data_version);
      printf( "Нажмите любую клавишу\n\n");
      getch();
    }
   else
     close(f);

   if( (f=open( "keyword.dat", O_RDWR | O_BINARY )) == -1 )
    {
      printf( "Не могу открыть файл KEYWORD.DAT --- СОЗДАЮ НОВЫЙ !!!\n");
      _creat("keyword.dat",FA_ARCH | O_BINARY);
      printf("Версия DAT файла - %d\n", data_version);
      printf( "Нажмите любую клавишу\n\n");
      getch();
    }
   else
     close(f);
}

int CreateBakFiles( void )
 {
   if(    copyfile( "index.dat","index.bak") != 0
       || copyfile("themes.dat", "themes.bak") != 0
       || copyfile("data.dat","data.bak") != 0
       || copyfile("keyword.dat","keyword.bak") != 0 )
       {
          printf( "ОШИБКА: Ошибка при создании bak-файла.\n");
          return 0;
       }
   else
       return 1;
 }
//*************************************************************************
int WriteFileHeader(  BufWrite & bwOut, char filetype )
// ф-я записывает в файл заголовок вида
// [ "sd" - 2b ][ type (d,i,t,k) - 1b][ CRC - 2b ][ версия формата - 2b ]
// [ версия данных - 2b ][ дата dd-mm-yyyy - 4b ][ reserved - 7b ]
{
   struct date cur_date;

   bwOut.WriteByte((int)'S');
   bwOut.WriteByte((int)'D');
   bwOut.WriteByte((int)filetype);

   WriteInt(bwOut, 0);  // CRC

   WriteInt(bwOut, format_version);
   WriteInt(bwOut, data_version);

// дата
   getdate( &cur_date );
   bwOut.WriteByte((int)cur_date.da_day);
   bwOut.WriteByte((int)cur_date.da_mon);
   WriteInt(bwOut, cur_date.da_year);

   for(int i=0; i<7; i++)
     bwOut.WriteByte(0);

   bwOut.RenewBuf();
   return OK;
}
//*************************************************************************
// Ф-я читает заголовок файла. Возвращает NOK если первые два символа заголовка
// не SD. В filetype заносится третий байт заголовка.

int ReadFileHeader(  BufRead & brIn, char* filetype )
{
   int ibuf;

   brIn.ReadByte(&ibuf);   if( (char)ibuf != 'S' )   return NOK;
   brIn.ReadByte(&ibuf);   if( (char)ibuf != 'D' )   return NOK;
   brIn.ReadByte(&ibuf);

   *filetype = (char)ibuf;

   for( int i=0; i<17; i++ )
      brIn.ReadByte(&ibuf);

   return OK;
}


//*************************************************************************

int CreateTmpFiles( void )
 {
   if(  copyfile("data.dat","data.tmp") != 0  ||
        copyfile("index.dat","index.tmp") != 0  ||
        copyfile("themes.dat","themes.tmp") != 0 ||
        copyfile("keyword.dat","keyword.tmp") != 0 )
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
       || copyfile("data.tmp","data.dat") != 0
       || copyfile("keyword.tmp","keyword.dat") != 0  )
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

// читать заголовок
    char filetype;
    if( ReadFileHeader( brIndex, &filetype ) == NOK ) return NOK;
    if( filetype != 'I' ) return NOK;

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

// читать заголовок
    char filetype;
    if( ReadFileHeader( brTh, &filetype ) == NOK ) return NOK;
    if( filetype != 'T' ) return NOK;

    while( num != number )
     {
       if( ReadInt( brTh, &num ) != 1  )     // считать номер темы
           return 0;
       ReadName( brTh, name );

       do
          ReadInt( brTh, &ibuf );
       while(  ibuf != 0 );        // прочитать эту тему до конца

     }

    return 1;
 }

//***************************************************************
void strtrim(char* str) //ф-я убирает пробелы в конце строки
{
  for(int i=(strlen(str)-1); str[i]=='\n' || str[i]=='\r' || str[i]==' '; i--)
         str[i]=0;
}

//******************************************************************
//перекодировка строки из дос кодировки в Win1251
void Dos2Win1251(char* str)
{
  int i,j;

  for( i=0; str[i]!=0; i++ )
    {
      for( j=0; DOS_SYMBOLS[j] != 0 && str[i] != DOS_SYMBOLS[j]; j++ );
      if( DOS_SYMBOLS[j]==0 )  continue;  //если этот символ не русская буква
      str[i] = WIN1251_SYMBOLS[j];
    }
}

//перекодировка символа из дос кодировки в Win1251
void Dos2Win1251(char& ch)
{
   for( int j=0; DOS_SYMBOLS[j] != 0 && ch != DOS_SYMBOLS[j]; j++ );

   if( DOS_SYMBOLS[j]!=0 )    //если этот символ - русская буква
      ch = WIN1251_SYMBOLS[j];
}

//перекодировка символа из дос кодировки в Win1251
void Dos2Win1251(int& i)
{
   char ch = (char)i;
   Dos2Win1251( ch );
   i = (int)ch;
}

//**************************************************************************
unsigned GetCRC(  BufRead& brIn  )
{
    unsigned CRC=0;
    int i;
    int ibuf;

    brIn.bigRew();

    for( int j=0; j<CRC_POSITION+2; j++ )
        brIn.ReadByte( &ibuf);

    while( ReadInt( brIn, &i) == 1 )
     {
       asm {
              MOV AX,CRC
              ROL AX, 1
              MOV CRC,AX
           };

       CRC= CRC + (unsigned)i;
     }

    return CRC;
}

//**************************************************************************
int SetCRC(  filehand fhIn  )
{

  BufRead brIn;
  brIn.SetFileHandler(fhIn);

  unsigned CRC = GetCRC( brIn  );

  filehand fh = brIn.hand;
  lseek( fh, CRC_POSITION , SEEK_SET);
  if( write( fh, &CRC, 2 ) != 2 ) return NOK;

  return OK;
}
//**************************************************************************
// добавить к теме новую статью
int AddArtToTheme( int artnum, char* thname )
{
    int ibuf=0;
    char name[256] = {0};
    int i;
    filehand tmp2;

// подготовка буфера для чтения из themes файла
    lseek( themes, 0L, SEEK_SET );
    BufRead brTh;
    brTh.SetFileHandler(  themes );

 // проверка заголовкa файла themes
    char filetype;
    ReadFileHeader( brTh, &filetype );
    if(filetype!='T') return ERROR_IN_THEMES;

// подготовка temp-файла
    if( ( tmp2 = _creat("temp2.tmp", FA_ARCH | O_BINARY)) == -1 )
        return CAN_NOT_CREATE_TEMP;
    BufWrite bwTmp;
    bwTmp.SetFileHandler(tmp2);
    WriteFileHeader(  bwTmp, 'T' );

// поиск в themes.dat данной темы
   while( strcmp( thname, name) != 0 ) // пока имя не совпадает
     {
       if( ReadInt( brTh, &ibuf ) != 1  )     // считать номер темы
           return TH_NOT_EXIST;
       WriteInt( bwTmp, ibuf );    // записать номер темы

       if( :: ReadName( brTh, name ) != 1 )    // считать имя темы
           return ERROR_IN_THEMES;
       WriteName( bwTmp, name);  // записать имя темы

// прочитать эту тему до конца
       do
          {
            if( ReadInt( brTh, &ibuf ) != 1 )
               return ERROR_IN_THEMES;
            WriteInt( bwTmp, ibuf );
          }
       while(  ibuf != 0 );
     }

// Нужная тема найдена
   bwTmp.UsedBufSize-=2;   // ненужный ноль был записан
   bwTmp.CurPos-=2;

   WriteInt( bwTmp, artnum );   // Записать номер добавляемой статьи
   WriteInt( bwTmp, 0 );   // Запись кода конца

// записать весь файл до конца
   while( brTh.ReadByte( &ibuf ) == 1 )
       bwTmp.WriteByte( ibuf );

   bwTmp.RenewBuf();
   close(tmp2);
   close( themes );

   copyfile( "temp2.tmp", "themes.tmp");
   themes = open( "themes.tmp", O_RDWR | O_BINARY );

   return OK;
}

int CreateNewTheme( int thnum, char* thname )
{
 const TH_HAS_BEEN_WRITTEN              = 1;
 const TH_HAS_NOT_BEEN_WRITTEN          = 0;

 filehand thm = open( "themes.tmp", O_RDWR | O_BINARY );
 _fmode = O_BINARY;
 int tmp = creat("temp2.tmp", S_IREAD|S_IWRITE );
 BufRead br;
 BufWrite bw;
 br.SetFileHandler(thm);
 bw.SetFileHandler(tmp);

// проверка заголовкa файла themes
 char filetype;
 ReadFileHeader( br, &filetype );
 if(filetype!='T') return ERROR_IN_THEMES;

// запись заголовка в файл themes
 WriteFileHeader( bw, 'T' );

 int num;
 char name[256];
 int  members[256];
 int mode = TH_HAS_NOT_BEEN_WRITTEN;

 while( ReadInt( br, &num ) == 1 )  // номер темы прочитан
  {
    // прочитать тему
    for( int i=0; i<256; i++)  members[i]=0 ;
    if( ReadName( br, name ) != 1 )  return NOK;

    for( i=0; ; i++ )
      {
        if( ReadInt( br, members+i ) != 1 )   return NOK;
        if( members[i] == 0 )                 break;
      }

    if( mode == TH_HAS_NOT_BEEN_WRITTEN ) // тема еще не была записана
       if( StrAlphComp(name, thname) == 2 )  // по алфавиту thname < name
            {  // запись данных о нашей теме
             if( WriteInt( bw, thnum ) != 1 )   return NOK;
             if( WriteName( bw, thname ) != 1 ) return NOK;
             WriteInt( bw, 0 );
             mode = TH_HAS_BEEN_WRITTEN;
            }

    // записать прочтенную информацию
    if( WriteInt( bw, num ) != 1 )   return NOK;
    if( WriteName( bw, name ) != 1 ) return NOK;
    for( i=0; members[i] != 0; i++ )
         if( WriteInt( bw, members[i] ) != 1 )   return NOK;
    WriteInt( bw, 0 );
  }
// в файле больше нет тем
 if( mode == TH_HAS_NOT_BEEN_WRITTEN ) // статья еще не была записана
    { // запись данных о нашей теме
      if( WriteInt( bw, thnum ) != 1 )   return NOK;
      if( WriteName( bw, thname ) != 1 ) return NOK;
      WriteInt( bw, 0 );
      mode = TH_HAS_BEEN_WRITTEN;
    }

 bw.RenewBuf();
 close( thm );
 close( tmp );
 copyfile("temp2.tmp", "themes.tmp");

 return OK;
}

//**************************************************************************+
int CreateNewKeyword( char* keyname )
{
 const KEY_HAS_BEEN_WRITTEN              = 1;
 const KEY_HAS_NOT_BEEN_WRITTEN          = 0;

 int keynum = 0; // Всем новым keyword'ам присваивается номер 0.
                 // впоследствие их нужно перенумеровать функцией
                 // RenumberKeywords();

 _fmode = O_BINARY;
 int tmp = creat("temp2.tmp", S_IREAD|S_IWRITE );

 BufWrite bw;
 bw.SetFileHandler(tmp);

// подготовка буфера для чтения из keyword файла
 lseek( keys, 0L, SEEK_SET );
 BufRead br;
 br.SetFileHandler( keys );
// проверка заголовкa файла keyword
 char filetype;
 ReadFileHeader( br, &filetype );
 if(filetype!='K') return ERROR_IN_KEYS;

// запись заголовка в файл keyword
 WriteFileHeader( bw, 'K' );

 int num;
 char name[256];

 int mode = KEY_HAS_NOT_BEEN_WRITTEN;

 while( ReadInt( br, &num ) == 1 )  // номер keyword прочитан
  {
    // прочитать keyword
    for( int i=0; i<MAX_KEYWORDS_IN_ART; i++)  members[i]=0 ;
    if( ReadName( br, name ) != 1 )  return NOK;

    if( strcmp( name, keyname ) == 0 )
     {
         bw.RenewBuf();
         close( tmp );
         return OK;  // такой keyword уже есть
     }

    for( i=0; ; i++ )
      {
        if( ReadInt( br, members+i ) != 1 )   return NOK;
        if( members[i] == 0 )                 break;
      }

    if( mode == KEY_HAS_NOT_BEEN_WRITTEN ) // keyword еще не был записан
       if( StrAlphComp(name, keyname) == 2 )  // по алфавиту keyname < name
            {  // запись данных о keyword
             if( WriteInt( bw, keynum ) != 1 )   return NOK;
             if( WriteName( bw, keyname ) != 1 ) return NOK;
             WriteInt( bw, 0 );
             mode = KEY_HAS_BEEN_WRITTEN;
            }

    // записать прочтенную информацию
    if( WriteInt( bw, num ) != 1 )   return NOK;
    if( WriteName( bw, name ) != 1 ) return NOK;
    for( i=0; members[i] != 0; i++ )
         if( WriteInt( bw, members[i] ) != 1 )   return NOK;
    WriteInt( bw, 0 );
  }

// в файле больше нет keyword'ов
 if( mode == KEY_HAS_NOT_BEEN_WRITTEN ) // статья еще не была записана
    { // запись данных о нашей теме
      if( WriteInt( bw, keynum ) != 1 )   return NOK;
      if( WriteName( bw, keyname ) != 1 ) return NOK;
      WriteInt( bw, 0 );
      mode = KEY_HAS_BEEN_WRITTEN;
    }

 bw.RenewBuf();
 close( keys );
 close( tmp );
 copyfile("temp2.tmp", "keyword.tmp");
 keys = open( "keyword.tmp", O_RDWR | O_BINARY );
 return OK;
}

//**************************************************************************
int AddArtToKeyword(int artnum, char * keyname)
{
    int ibuf=0;
    char name[256] = {0};
    int i;
    filehand tmp2;

// подготовка буфера для чтения из keyword файла
    lseek( keys, 0L, SEEK_SET );
    BufRead br;
    br.SetFileHandler( keys );

 // проверка заголовкa
    char filetype;
    ReadFileHeader( br, &filetype );
    if(filetype!='K') return ERROR_IN_KEYS;

// подготовка temp-файла
    if( ( tmp2 = _creat("temp2.tmp", FA_ARCH | O_BINARY)) == -1 )
        return CAN_NOT_CREATE_TEMP;
    BufWrite bw;
    bw.SetFileHandler(tmp2);
    WriteFileHeader(  bw, 'K' );

// поиск в keyword.dat данной темы
   while( strcmp( keyname, name) != 0 ) // пока имя не совпадает
     {
       if( ReadInt( br, &ibuf ) != 1  )     // считать номер
        {
          bw.RenewBuf();
          close(tmp2);
          return KEYWORD_NOT_EXISTS;
        }

       WriteInt( bw, ibuf );    // записать номер

       if( :: ReadName( br, name ) != 1 )    // считать имя
           return ERROR_IN_KEYS;
       WriteName( bw, name);  // записать имя

// прочитать keyword до конца
       do
          {
            if( ReadInt( br, &ibuf ) != 1 )
               return ERROR_IN_KEYS;
            if( ibuf == artnum && strcmp( keyname, name) == 0)
               {
                bw.RenewBuf();    // такая статья уже записана
                close(tmp2);     // в данном keyword'е
                return OK;
               }
            WriteInt( bw, ibuf );
          }
       while(  ibuf != 0 );
     }

// Нужный keyword найден
   bw.UsedBufSize-=2;   // ненужный ноль был записан
   bw.CurPos-=2;

   WriteInt( bw, artnum );   // Записать номер добавляемой статьи
   WriteInt( bw, 0 );   // Запись кода конца

// записать весь файл до конца
   while( br.ReadByte( &ibuf ) == 1 )
       bw.WriteByte( ibuf );

   bw.RenewBuf();
   close(tmp2);
   close( keys );

   copyfile( "temp2.tmp", "keyword.tmp");
   keys = open( "keyword.tmp", O_RDWR | O_BINARY );

   return OK;
}

//***************************************************************************
int RenumberKeywords()
{
    int ibuf=0;
    char name[256] = {0};

    filehand tmp2;

// подготовка буфера для чтения из keyword файла
    lseek( keys, 0L, SEEK_SET );
    BufRead br;
    br.SetFileHandler( keys );

 // проверка заголовкa
    char filetype;
    ReadFileHeader( br, &filetype );
    if(filetype!='K') return ERROR_IN_KEYS;

// подготовка temp-файла
    if( ( tmp2 = _creat("temp2.tmp", FA_ARCH | O_BINARY)) == -1 )
        return CAN_NOT_CREATE_TEMP;
    BufWrite bw;
    bw.SetFileHandler(tmp2);
    WriteFileHeader(  bw, 'K' );

   for( int num=25001; ReadInt( br, &ibuf ) == 1 ; num++ )
     {
       WriteInt( bw, num );    // записать номер

       if( :: ReadName( br, name ) != 1 )    // считать имя
           return ERROR_IN_KEYS;
       WriteName( bw, name);  // записать имя

       do
          {
            if( ReadInt( br, &ibuf ) != 1 )
               return ERROR_IN_KEYS;
            WriteInt( bw, ibuf );
          }
       while(  ibuf != 0 );
     }

   bw.RenewBuf();
   close(tmp2);
   close( keys );

   copyfile( "temp2.tmp", "keyword.tmp");
   keys = open( "keyword.tmp", O_RDWR | O_BINARY );

   return OK;
}