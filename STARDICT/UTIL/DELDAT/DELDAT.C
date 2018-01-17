#include "deldat.h"

filehand index, data, themes;
filehand nindex, ndata, nthemes;

int OpenDataFiles( void );
int CloseDataFiles( void );
int MakeBak();          // копирует dat-файлы в tmp-файлы
int SaveChanges();      // копирует tmp-файлы в dat-файлы

void main( void )
{
  int num, res;

  if( (res = OpenDataFiles()) != OK )
     { errres( res );  return;  }     // конец работы

  printf("********************************************************\n"
	 "   Данная программа уничтожает статью  т. е. убирает всю\n "
	 " информацию о ней из файлов с данными астрологической\n"
	 " энциклопедии.\n\n"
	 " Введите номер удаляемого объекта\n"
	 "( 0 для завершения программы)\n");
  scanf( "%d", &num );
  if( num == 0 )
     return; // конец работы

  printf( "*   создаю bak-файлы\n" );
  if ( (res=MakeBak()) != OK )
    {  errres( res ); return; }

  if( (res=DelArt( num )) != OK )
    {  errres( res );  return; }

  CloseDataFiles( );

  printf( "**  сохраняю изменения в dat-файлах\n" );
  if ( (res=SaveChanges()) != OK )
    {  errres( res ); return; }

  printf( "********************************************************\n" );
}
//*************************************************************************
int OpenDataFiles( void )
{
   _fmode = O_BINARY;
   themes = open( "themes.dat", O_RDWR | O_BINARY );
   index = open( "index.dat", O_RDWR | O_BINARY );
   data = open( "data.dat", O_RDWR | O_BINARY );
   nthemes = creat( "themes.tmp", S_IREAD|S_IWRITE );
   nindex = creat( "index.tmp", S_IREAD|S_IWRITE );
   ndata = creat( "data.tmp", S_IREAD|S_IWRITE );

   if( index == -1  || themes==-1  ||  data==-1  )
      return CAN_NOT_OPEN_DATA;

   if( nindex == -1  || nthemes==-1  || ndata==-1 )
      return CAN_NOT_CREATE;

   return OK;
}

int CloseDataFiles( void )
{
   close( themes );
   close( index );
   close( data );
   close( nthemes );
   close( nindex );
   close( ndata );
   return OK;
}
//************************************************************************
int MakeBak()          // копирует dat-файлы в tmp-файлы
{
  if ( copyfile("data.dat", "data.bak" ) != OK ||
       copyfile("index.dat", "index.bak" ) != OK ||
       copyfile("themes.dat", "themes.bak" ) != OK )
    return CAN_NOT_MAKE_BAK;
  return OK;
}

int SaveChanges()      // копирует tmp-файлы в dat-файлы
{
  if ( copyfile("data.tmp", "data.dat" ) != OK ||
       copyfile("index.tmp", "index.dat" ) != OK ||
       copyfile("themes.tmp", "themes.dat" ) != OK )
    return CAN_NOT_SAVE;
  return OK;
}
//*********************************************************************
void errres( int ec )
{
 switch( ec )
  {
   case ART_NOT_FOUND:    printf("ОШИБКА: статья не найдена\n"); break;
   case CAN_NOT_CREATE:   printf("ОШИБКА: не могу создать tmp-файл\n"); break;
   case CAN_NOT_MAKE_BAK: printf("ОШИБКА: не могу создать bak-файл\n"); break;
   case CAN_NOT_OPEN_DATA:printf("ОШИБКА: не могу открыть dat-файл\n"); break;
   case CAN_NOT_SAVE     :printf("ОШИБКА: не могу сохранить результат\n"); break;
  };
}