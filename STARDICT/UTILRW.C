#include "rw.h"
#ifdef _HISTORY
#include "history.h"
TViewerHistory Hist;
#endif
int GoToChar( BufRead& brIn,char ch )
// Ф-я переходит в файле brIn на символ ch возвр. код ошибки
  {
     int res=0;
     int ec;

     while(  res != ch  )
        {
           ec=brIn.ReadByte( &res );

           if ( ec == -1 || ec == EOF )
               return ec;
        }
    return 1;
  }
//***********************************************************************

int ReadInt( BufRead& brIn, int* res )
//  Ф-я читает из файла 2 байта и преобразует их в целое
  {
     int b1,b2;

     if (  brIn.ReadByte(  &b1 ) != 1 || brIn.ReadByte(  &b2 ) != 1 )
            return 0;

     *res = ( b1 << 8)|b2 ;
     return 1;
  }
//**************************************************************************
int WriteInt( BufWrite& bwOut, int i )
  {
     int b1,b2;

     if( i<0 || i> 4095 )
         return 0;

     b1 = i>>8;
     b2 = i&0x00FF;


     if (  bwOut.WriteByte(  b1 ) != 1 || bwOut.WriteByte(  b2 ) != 1 )
              return 0;

     return 1;
  }
//***************************************************************************
int ReadString( BufRead& brIn, char* str )
  {
    int i;

    for(  i=0 ; ; i++ )
       {
         if( brIn.ReadByte( (int*)(str + i) )!=1 )
            return 0;

         if ( str[i] == '\r' )
               break;
       }

    str[i]=0;

    return 1;

  }
//****************************************************************************
int ReadName( BufRead& brIn, char* name )
// Ф-я читает строку с текущего положения по символ '^'
  {
    int i;
    name[0] = 0 ;

    for(  i=0 ; name[ i-1 ] != '^' ; i++ )
        if( brIn.ReadByte( (int*)(name + i) ) != 1 )
            return 0;


    name[i-1]=0;

    return 1;

 }

//*********************************************************************
int ReadLong( BufRead& brIn, long* res )
//  Ф-я читает из файла 4 байта и преобразует их в long
  {
     int b1, b2, b3, b4;

     if (  brIn.ReadByte(  &b1 ) != 1 || brIn.ReadByte(  &b2 ) != 1 ||
           brIn.ReadByte(  &b3 ) != 1 || brIn.ReadByte(  &b4 ) != 1  )
            return 0;

     *res = ((long)b1<<24) | ((long)b2<<16) | ((long)b3 << 8) | (long)b4 ;
     return 1;
  }

//*************************************************************************
char UncodeChar(  int* buffer, int length )
//   Функция получает буффер содерж. 1 2 или 3 полубайта, и их количество
// ( length ).  Переводит его в символ и возвращает этот символ.
 {
     char ch=0;
//-------------------------------------------------------------------------
     if( length == 1 )   // если это символ 1 типа
       switch( buffer[0] )
         {
           case 0:    return ' ';
           case 1 :   return 'в';
           case 2 :   return 'е';
           case 3 :   return 'и';
           case 4 :   return 'к';
           case 5 :   return 'л';
           case 6 :   return 'а';
           case 7 :   return 'н';
           case 8 :   return 'о';
           case 9 :   return 'п';
           case 10:   return 'р';
           case 11:   return 'с';
           case 12:   return 'т';
           case 13:   return 'м';
         };
//-------------------------------------------------------------------------
      if( length == 2 )
        switch( buffer[1] )
          {
           case  0:    return 'б';
           case  1:    return 'г';
           case  2:    return 'д';
           case  3:    return 'з';
           case  4:    return 'й';
           case  5:    return 'у';
           case  6:    return 'х';
           case  7:    return 'ц';
           case  8:    return 'ч';
           case  9:    return 'ы';
           case 10:    return 'ь';
           case 11:    return 'я';
           case 12:    return '.';
           case 13:    return ',';
           case 14:    return 'ю';
           case 15:    return '\n';
          };
//-------------------------------------------------------------------------
      if( length == 3 )
               {
                 if( buffer[1]==0xF && buffer[2]==0xF )
                     return EOF;
                 ch = ch | (buffer[1] << 4);
                 ch = ch | buffer[2];
                 return ch;
               }
//-------------------------------------------------------------------------
    return 0; // этот оператор нужен чтобы при компиляции не выдавался
              // warning "Function should return a value
              // на самом деле return произойдет раньше и этот оператор
              // никогда не вызовется.
 }

//***************************************************************************
int ReadCodedLine( BufRead & brIn, char* str )
// возвращает NOK если встречает конец текста
{
  int buffer[3]={0};
  int ibuf;
  for( int i=0; ; i++ )
    {   readHalf( brIn, buffer );
        switch( buffer[0] )
         {
          case OFTEN_CODE:   readHalf( brIn, buffer+1 );
                             str[i] = UncodeChar( buffer, 2 );
                             break;

          case SELDOM_CODE:  readHalf( brIn, buffer+1 );
                             readHalf( brIn, buffer+2 );
                             str[i]  = UncodeChar( buffer, 3 );
                             break;

          default:           str[i] = UncodeChar( buffer, 1 );
         };

        if(  str[i]  == (unsigned char)EOF )
          {  if( brIn.wh == 1 ) // Если текст заканчивается в первой
               { brIn.wh = 0;    // половине байта
                 brIn.ReadByte( &ibuf);
               }
             return NOK;
          }
        if(  str[i] == '\n' )
            { str[i] = 0;  return OK; }
    }
}
//**********************************************************************
int GetArtName( BufRead* pbrIndex, int num, char* name )
{
    BufRead& brIn = *pbrIndex ;
    brIn.bigRew( );
    int ibuf;
    long lbuf;
    do
      if( ReadInt( brIn, &ibuf ) != 1 || ReadName( brIn, name ) != 1
          || ReadLong( brIn, &lbuf ) != 1 )
            return NOK;
    while( ibuf != num );
    return OK;
}

int GetArtNum( BufRead* pbrIndex, char* artname, int* pnum  )
{
    BufRead& brIn = *pbrIndex ;
    brIn.bigRew( );
    char buffer[256];
    long lbuf;
    do
      if( ReadInt( brIn, pnum ) != 1 || ReadName( brIn, buffer ) != 1
          || ReadLong( brIn, &lbuf ) != 1 )
            return NOK;
    while( strcmp(artname, buffer) != 0);
    return OK;
}

int GetThNum( BufRead* pbrThemes, char* thname, int* pnum  )
{
   pbrThemes->bigRew( );
   char buffer[256];
   do
      if( GetNextThNameNum( pbrThemes, buffer, pnum) != OK )
           return NOK;
   while( strcmp(thname, buffer) != 0);

   return OK;
}

int GetThName( BufRead* pbrThemes, char* thname, int num  )
{
   pbrThemes->bigRew( );
   int ibuf;
   do
      if( GetNextThNameNum( pbrThemes, thname, &ibuf) != OK )
           return NOK;
   while( ibuf != num );

   return OK;
}

int GetNextThNameNum( BufRead* pbrThemes, char* buffer, int* pnum )
{
   BufRead& brIn = *pbrThemes;
   int ibuf;
   if( ReadInt( brIn, pnum ) != 1 )
       return NOK;
   if( ReadName( brIn, buffer ) != 1 )
       return NOK;
   ibuf = 0;
   while( ibuf !=0x0FFF )
     if( ReadInt( brIn, &ibuf ) != 1 )
          return WRONG_TH;
   return OK;
}

int GetNextArtNameNum( BufRead* pbrIndex, char* buffer, int* pibuf )
{
    BufRead& brIn = *pbrIndex ;
    long lbuf;

    if( ReadInt( brIn, pibuf ) != 1 || ReadName( brIn, buffer ) != 1
          || ReadLong( brIn, &lbuf ) != 1 )
               return NOK;
    return OK;
}

int GetArtNamesList( BufRead* pbrIndex, char** names, int* pmaxnames )
// в maxnames возвращается сколько имен было прочитано
{
    char buffer[256];
    int ibuf;
    pbrIndex->bigRew( );

    for( int i=0; i< *pmaxnames; i++ )
      {
        if( GetNextArtNameNum( pbrIndex, buffer, &ibuf ) != OK )
                break;
        names[i] = NULL;
        names[i] = new char[ strlen( buffer ) + 1 ];
        if( names[i] == NULL )   return NO_MEMORY;
        strcpy( names[i], buffer );
      }
    *pmaxnames = i;
    return OK;
}

int GetThNamesList( BufRead* pbrThemes, char** names, int* pmaxnames )
// в maxnames возвращается сколько имен было прочитано
{
    char buffer[256];
    int ibuf;
    pbrThemes->bigRew( );

    for( int i=0; i< *pmaxnames; i++ )
      {
        if( GetNextThNameNum( pbrThemes, buffer, &ibuf ) != OK )
                break;
        names[i] = NULL;
        names[i] = new char[ strlen( buffer ) + 1 ];
        if( names[i] == NULL )   return NO_MEMORY;
        strcpy( names[i], buffer );
      }
    *pmaxnames = i;
    return OK;
}

#ifdef _HISTORY
int GetHistoryList( BufRead* pbrIndex, char** names, int* qarts )
{
    char buffer[256];

    for( int i=0; i< Hist.hnum; i++ )
      {
        GetArtName( pbrIndex, Hist.history[i].number, buffer );
        names[i] = NULL;
        names[i] = new char[ strlen( buffer ) + 1 ];
        if( names[i] == NULL )   return NO_MEMORY;
        strcpy( names[i], buffer );
      }
    *qarts = i;
    return OK;
}
#endif

int GetArtNamesFromTh( int thnum, BufRead* pbrIndex, BufRead* pbrThemes,
                   char** names, int* pmaxnames )
{
  pbrThemes->bigRew();
  char buffer[256];
  int n,i;
  int buf[256];
  do
   {
     if( ReadInt( *pbrThemes, &n ) != 1 )
        return WRONG_TH;
     if( ReadName( *pbrThemes, buffer ) != 1 )
        return WRONG_TH;
     for( i = 0; i<256; i++ )
      {
       if( ReadInt( *pbrThemes, buf+i ) != 1 )
           return WRONG_TH;
       if( buf[i] ==0x0FFF )    break;
      }
   }
  while(  n != thnum );  // тема найдена

  pbrIndex->bigRew();
  for( int j=0; GetNextArtNameNum( pbrIndex, buffer, &n ) == OK ; )
    for( i=0; buf[i] != 0x0FFF && i<256; i++ )
         if( buf[i]==n )
          {
             names[j] = NULL;
             names[j] = new char[ strlen( buffer ) + 1 ];
             if( names[j] == NULL )   return NO_MEMORY;
             strcpy( names[j], buffer );
             j++;
          }

  *pmaxnames = j;
  return OK;
}