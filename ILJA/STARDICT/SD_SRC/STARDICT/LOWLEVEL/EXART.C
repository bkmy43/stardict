/********************************************************************
-- STARDICT Project --
                          exart.c

Функции базового класса TExArticle
И классов реализующих списки тем, статей и ключевых слов
(c) I.Vladimirsky 1997
********************************************************************/
#include <fcntl.h>
#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <io.h>


#include "exart.h"
#include "ex.h"

extern filehand fhIndex;
extern filehand fhData;
extern filehand fhThemes;
extern filehand fhKeys;

/*************************************************************************
                           КЛАСС  TExArticle
*************************************************************************/

TExArticle :: TExArticle( )
 {
    pbrIndex = pbrData = NULL;
    pbrIndex = new BufRead();
    pbrData = new BufRead();

    if( pbrIndex == NULL || pbrData == NULL)
         throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

    pbrIndex->SetFileHandler(fhIndex);
    pbrData->SetFileHandler(fhData);

    artnum = 0;
    bodylen=0;
    artbody = NULL;
 }

TExArticle :: TExArticle( int inumber )
 {
    pbrIndex = pbrData = NULL;
    pbrIndex = new BufRead();
    pbrData = new BufRead();

    if( pbrIndex == NULL || pbrData == NULL)
         throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

    pbrIndex->SetFileHandler(fhIndex);
    pbrData->SetFileHandler(fhData);

    artnum = 0;
    bodylen=0;
    artbody = NULL;

    ChangeArt(inumber);
 }

void TExArticle :: DelOldInfo()
{
    if(artbody!=NULL)   delete artbody;
    artbody=NULL;
    artnum=0;
    bodylen=0;
}

int TExArticle :: ChangeArt( int num )
 {
    long position;
    int res;

    DelOldInfo();

    if( (res = ReadNamePosition( num, &position )) != OK )
         throw TSTDError("I\\"__FILE__, __LINE__, (TSTDCode)res);

    if( (res=ReadBody( position )) != OK )
         throw TSTDError("I\\"__FILE__, __LINE__, (TSTDCode)res);

    return OK;
 }

TExArticle :: ~TExArticle(  )
 {
    if( pbrIndex != NULL) delete pbrIndex;
    if( pbrData != NULL) delete pbrData;
    DelOldInfo();
 }

//************************************************************************
int TExArticle :: ReadNamePosition ( int num, long* position )
// Ф-я читает из файла index.dat название статьи и адрес
// по которому ее текст можно найти в файле data.dat
 {
    BufRead& brIn = *pbrIndex ;
    brIn.bigRew( );

    // проверка заголовкa файла
    char filetype;
    ReadFileHeader( brIn, &filetype );
    if(filetype!='I')  return EC_WRONG_INDEXDAT;

    do
      if( ReadInt( brIn, &artnum ) != 1 || ReadName( brIn, artname ) != 1
                 || ReadLong( brIn, position ) != 1 )
                    return EC_WRONG_INDEXDAT;
    while( artnum != num );

#ifdef _STD_DEBUG
    char buf[255];
    sprintf( buf, "%s : %d", artname, artnum );
    strcpy( artname, buf );
#endif

    return OK;
 }

//************************************************************************
int TExArticle :: ReadBody( long position )
// Ф-я читает из файла data.dat текст статьи
{
  // Чтение текста статьи
    pbrData->ReachEOF = NO;
    pbrData->wh = 0;
    lseek( pbrData->hand, position, SEEK_SET );
    pbrData->RenewBuf();

    char* buffer = NULL;
    buffer = new char[MAX_BODY_SIZE];
    if( buffer == NULL )  return EC_NO_MEMORY;
    for(unsigned i=0; i < MAX_BODY_SIZE; i++ )   buffer[i]=0;

    int res;
    if ((res=ReadCodedBody( *pbrData, buffer, MAX_BODY_SIZE, &bodylen )) != OK )
        return res;

    // Сохранение прочитанного
    artbody = NULL;
    artbody = new char[ bodylen + 1 ];
    if( artbody == NULL )  return EC_NO_MEMORY;
    for(unsigned j=0; j<bodylen; artbody[j]=buffer[j], j++ );
    artbody[bodylen]=0;

    if( buffer != NULL )  delete buffer;
    return OK;
}


/*************************************************************************
                           КЛАССЫ  TList
*************************************************************************/

// Базовый класс - элемент списка
TBaseListElem :: TBaseListElem( int a, char* str, TBaseListElem* apnext)
{
  num = a;

  name = NULL;
  name = new char[ strlen(str) +1 ];
  if( name == NULL )
     throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

  strcpy(name,str);

  pnext=apnext;
}

TBaseListElem :: ~TBaseListElem()
{
  num = 0;
  if( name != NULL ) delete name;
  if( pnext != NULL ) delete pnext;
}

//**********************************************************************
// Базовый класс - однонаправленный список
TBaseList :: ~TBaseList()
{
  if( pfirst != NULL ) delete pfirst;
  pfirst = NULL;
}

char* TBaseList :: GetNthName( int i ) // получить name i-го элемента списка
{
  if( i > ItemsNum ) return NULL;

  TBaseListElem* ptmp;
  int j=0;

  for( ptmp=pfirst; ptmp != NULL; ptmp=ptmp->pnext )
     if( j++ == i )
        return ptmp->name;

  return NULL;
}


int TBaseList :: GetNthNumber( int i ) // получить number i-го элемента списка
{
  if( i > ItemsNum ) return 0;

  TBaseListElem* ptmp;
  int j=0;

  for( ptmp=pfirst; ptmp != NULL; ptmp=ptmp->pnext )
     if( j++ == i )
        return ptmp->num;

  return 0;
}

char* TBaseList :: GetName( int num )
// получить имя соответствующее номеру num
{
  TBaseListElem* ptmp;

  for( ptmp=pfirst; ptmp != NULL; ptmp=ptmp->pnext )
     if( ptmp->num == num )
        return ptmp->name;

  return NULL;
}


int TBaseList :: GetNumber( char* str )
// получить номер соответствующий имени str
{
  TBaseListElem* ptmp;

  for( ptmp=pfirst; ptmp != NULL; ptmp=ptmp->pnext )
     if( strcmp(ptmp->name, str) == 0 )
        return ptmp->num;

  return 0;
}

int TBaseList :: AppendElem( int i, char* str )
// добавляет в конец списка элемент с параметрами (i,str)
{
  TBaseListElem* ptmp1;
  TBaseListElem* ptmp2=NULL;

  if( pfirst==NULL )
     {
       pfirst = new TBaseListElem( i, str, NULL );
       if( pfirst == NULL ) return NOK;
       ItemsNum++;
       return OK;
     }

  // the list has more then one element
  // найти последний элемент в списке
  for( ptmp1=pfirst; ptmp1->pnext != NULL; ptmp1=ptmp1->pnext );

 // if the list was empty
  ptmp2 = new TBaseListElem( i, str, NULL );
  if( ptmp2 == NULL ) return NOK;

  ptmp1->pnext = ptmp2;

  ItemsNum++;
  return OK;
}

//**********************************************************************
// Класс Список статей является потомком TBaseList
// Переопределяется функция  BuildList - данные читаются из index.dat
TArticleList :: TArticleList(int mode) : TBaseList()
{
   pbrIndex = NULL;
   pbrIndex = new BufRead();
   if( pbrIndex == NULL )
      throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

   pbrIndex->SetFileHandler( fhIndex );

   pfirst = NULL;

   if( mode == BUILD_LIST )  BuildList();
}

TArticleList :: ~TArticleList()
{
   if( pbrIndex != NULL ) delete pbrIndex;
}

void TArticleList :: BuildList()
{
   BufRead & br = *pbrIndex;

   br.bigRew();
   // проверка заголовкa файла
   char filetype;
   ReadFileHeader( br, &filetype );
   if(filetype!='I')
        throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_INDEXDAT);

   TBaseListElem* ptmp1=NULL;
   TBaseListElem* ptmp2=NULL;

   char buffer[MAX_NAME_LEN] = {0};
   int  ibuf;

   for( int i=0; GetNextArtNameNum( pbrIndex, buffer, &ibuf ) == OK; i++ )
     {
        if( i==0 )
          {
            pfirst = ptmp1 = new TBaseListElem(ibuf, buffer, NULL);
            if(pfirst == NULL)
              throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
          }
        else
          {
            ptmp2 = new TBaseListElem(ibuf, buffer, NULL);
            if( ptmp2 == NULL )
              throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
            ptmp1->pnext = ptmp2;
            ptmp1 = ptmp2;
          }
        ItemsNum++;
     }
}

//**********************************************************************
// Класс Список тем является потомком TBaseList
// Переопределяется функция  BuildList - данные читаются из themes.dat
// Функция GetNthArtList(int n) строит список статей соответствующих теме
// стоящей в списке под номером n.

TThemeList :: TThemeList() : TBaseList()
{
        pbrThemes = NULL;
        pbrThemes = new BufRead();
        if( pbrThemes == NULL )
              throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
        pbrThemes->SetFileHandler( fhThemes );

        AllArtList = NULL;
        AllArtList = new TArticleList();  // создать список всех статей
        if( AllArtList == NULL )
              throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

        ThemesTable = NULL;

        BuildThemesTable();

        ThArtList = NULL;

        BuildList();
}

TThemeList :: ~TThemeList()
{
        if( pbrThemes != NULL ) delete pbrThemes;

        if( AllArtList != NULL ) {delete AllArtList; AllArtList=NULL;}

        if( ThArtList != NULL ) {delete ThArtList; ThArtList=NULL; }

        if( ThemesTable != NULL )  {delete [] ThemesTable; ThemesTable=NULL; }
}


/* функция строит ThemesTable -
        массив целых чисел, содержащий информацию о составе всех тем.
        формат: номер_темы_1, номер_статьи_11, номер_статьи_12, ..., 0,
                          номер_темы_2, номер_статьи_21, номер_статьи_22, ..., 0,
                                                                                        .  .  .
                          номер_темы_n, номер_статьи_n1, номер_статьи_n2, ..., 0,
                          0.
*/

int TThemeList :: BuildThemesTable()
{
        int res;
        ThemesTable = NULL;

        WORD TableSize;
        res = GetThemesTableSize( pbrThemes, &TableSize );
        if( res != OK )
              throw TSTDError("I\\"__FILE__, __LINE__,(TSTDCode)res);

        ThemesTable = new int[TableSize+1];
        if( ThemesTable == NULL )
              throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

        res = GetThemesTable( pbrThemes, ThemesTable );
        if( res != OK )
              throw TSTDError("I\\"__FILE__, __LINE__,(TSTDCode)res);

        ThemesTable[TableSize] = 0;
        return OK;
}

void TThemeList :: BuildList()
{
        BufRead & br = *pbrThemes;

        br.bigRew();
        // проверка заголовкa файла
        char filetype;
        ReadFileHeader( br, &filetype );
        if(filetype!='T')
              throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_THEMESDAT);

        TBaseListElem* ptmp1=NULL;
        TBaseListElem* ptmp2=NULL;

        char buffer[MAX_NAME_LEN] = {0};
        int  ibuf;

        for( int i=0; GetNextThNameNum( pbrThemes, buffer, &ibuf ) == OK; i++ )
                {
                  if( i==0 )
                   {
                    pfirst = ptmp1 = new TBaseListElem(ibuf, buffer, NULL);
                    if( pfirst == NULL )
                       throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
                   }
                  else
                   {
                    ptmp2 = new TBaseListElem(ibuf, buffer, NULL);
                    if( ptmp2 == NULL )
                       throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
                    ptmp1->pnext = ptmp2;
                    ptmp1 = ptmp2;
                   }
                  ItemsNum++;
                }
}

// функция строит список статей из N-ой темы и возвращает указатель
// на него
TArticleList* TThemeList :: GetNthArtList( int n )
{
        if( ThArtList != NULL ) { delete ThArtList; ThArtList = NULL; }

        ThArtList = new TArticleList( NOT_BUILD_LIST );

// find theme
        int NeededNumber = GetNthNumber(n);
        for( WORD l=0; ThemesTable[l] != NeededNumber; l++ )
          {
                  if( n == ThemesTable[l] ) break;

                  while( ThemesTable[l] != 0 )
                                 l++;
          }

// get article list
        while( ThemesTable[++l] != 0 )
                ThArtList->AppendElem( ThemesTable[l],
                             AllArtList->GetName(ThemesTable[l]));

        return ThArtList;
}


//**********************************************************************
// Класс Список ключевых слов является потомком TBaseList
// Переопределяется функция  BuildList - данные читаются из keyword.dat
// Функция GetNthArtList(int n) строит список статей, в которых встречается
// keyword стоящий в списке под номером n.

TKeywordList :: TKeywordList() : TBaseList()
{
        pbrKeys = NULL;
        pbrKeys = new BufRead();
        if( pbrKeys == NULL )
          throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
        pbrKeys->SetFileHandler( fhKeys );

        AllArtList = NULL;
        AllArtList = new TArticleList();  // создать список всех статей
        if( AllArtList == NULL )
          throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

        KeywordsTable = NULL;
        BuildKeywordsTable();

        KeyArtList = NULL;

        BuildList();
}

TKeywordList :: ~TKeywordList()
{
        if( pbrKeys != NULL ) delete pbrKeys;

        if( AllArtList != NULL ) {delete AllArtList; AllArtList=NULL;}

        if( KeyArtList != NULL ) {delete KeyArtList; KeyArtList=NULL; }

        if( KeywordsTable != NULL )  {delete [] KeywordsTable; KeywordsTable=NULL; }
}


/* функция строит KeywordsTable -
        массив целых чисел, содержащий информацию о составе всех тем.
        формат: номер_keyword'а_1, номер_статьи_11, номер_статьи_12, ..., 0,
                          номер_keyword'а_2, номер_статьи_21, номер_статьи_22, ..., 0,
                                                                                        .  .  .
                          номер_keyword'а_n, номер_статьи_n1, номер_статьи_n2, ..., 0,
                          0.
*/

int TKeywordList :: BuildKeywordsTable()
{
        int res;
        KeywordsTable = NULL;

        WORD TableSize;
        res = GetKeywordsTableSize( pbrKeys, &TableSize );
        if( res != OK )
              throw TSTDError("I\\"__FILE__, __LINE__,(TSTDCode)res);

        KeywordsTable = new int[TableSize+1];
        if( KeywordsTable == NULL )
              throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);

        res = GetKeywordsTable( pbrKeys, KeywordsTable );
        if( res != OK )
              throw TSTDError("I\\"__FILE__, __LINE__,(TSTDCode)res);

        KeywordsTable[TableSize] = 0;
        return OK;
}

void TKeywordList :: BuildList()
{
        BufRead & br = *pbrKeys;

        br.bigRew();
        // проверка заголовкa файла
        char filetype;
        ReadFileHeader( br, &filetype );
        if(filetype!='K')
          throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_KEYWORDDAT);

        TBaseListElem* ptmp1=NULL;
        TBaseListElem* ptmp2=NULL;

        char buffer[MAX_NAME_LEN] = {0};
        int  ibuf;

        for( int i=0; GetNextKeyNameNum( pbrKeys, buffer, &ibuf ) == OK; i++ )
            {
                  if( i==0 )
                   {
                     pfirst = ptmp1 = new TBaseListElem(ibuf, buffer, NULL);
                     if( pfirst == NULL )
                       throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
                   }
                  else
                   {
                     ptmp2 = new TBaseListElem(ibuf, buffer, NULL);
                     if( ptmp2 == NULL )
                       throw TSTDError("I\\"__FILE__, __LINE__, EC_NO_MEMORY);
                     ptmp1->pnext = ptmp2;
                     ptmp1 = ptmp2;
                   }
                  ItemsNum++;
            }
}

// функция строит список статей из N-ого keyword'а и возвращает указатель
// на него
TArticleList* TKeywordList :: GetNthArtList( int n )
{
        if( KeyArtList != NULL ) { delete KeyArtList; KeyArtList = NULL; }

        KeyArtList = new TArticleList( NOT_BUILD_LIST );

// find keyword
        int NeededNumber = GetNthNumber(n);
        for( WORD l=0; KeywordsTable[l] != NeededNumber; l++ )
          {
                  if( n == KeywordsTable[l] ) break;

                  while( KeywordsTable[l] != 0 )
                                 l++;
          }

// get article list
        while( KeywordsTable[++l] != 0 )
                KeyArtList->AppendElem( KeywordsTable[l],
                                                                                          AllArtList->GetName(KeywordsTable[l]));

        return KeyArtList;
}


