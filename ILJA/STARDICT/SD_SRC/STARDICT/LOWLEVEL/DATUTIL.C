/********************************************************************
-- STARDICT Project --
                           datautil.c

�-�� ����������� ������� ������ � Dat-�������
(c) I.Vladimirsky 1997
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>

#include "datutil.h"
#include "ex.h"

extern filehand fhIndex;
extern filehand fhData;
extern filehand fhThemes;
extern filehand fhKeys;

// �-� ������ ��������� �����. ���������� NOK ���� ������ ��� ������� ���������
// �� SD. � filetype ��������� ������ ���� ���������.

void OpenDataFiles()
{
     fhThemes = open( "themes.dat", O_RDONLY | O_BINARY );
     fhIndex = open( "index.dat", O_RDONLY | O_BINARY );
     fhData = open( "data.dat", O_RDONLY | O_BINARY );
     fhKeys = open( "keyword.dat", O_RDONLY | O_BINARY );

     if( fhData == -1  )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_CANTOPEN_DATADAT, 0);
     if( fhIndex == -1  )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_CANTOPEN_INDEXDAT, 0);
     if( fhThemes == -1 )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_CANTOPEN_THEMESDAT, 0);
     if( fhKeys == -1  )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_CANTOPEN_KEYWORDDAT, 0);

//     CheckDataFiles();
}

int CheckDataFiles()
{
     int datver, forver;
     unsigned CRC;
     char type;

     BufRead brData, brIndex, brThemes, brKeys;
     brData.SetFileHandler( fhData );
     brIndex.SetFileHandler( fhIndex );
     brThemes.SetFileHandler( fhThemes );
     brKeys.SetFileHandler( fhKeys );

// data.dat
     if(ReadFileHeader(brData,&type,&forver,&datver,&CRC)!=OK || type!='D')
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_DATADAT);
     if( CRC != GetCRC( brData ) )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_CRC);
     if( datver != DATA_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_DATA_VERSION);
     if( forver != FORMAT_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_FORMAT_VERSION);
// index.dat
     if(ReadFileHeader(brIndex,&type,&forver,&datver,&CRC)!=OK || type!='I')
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_INDEXDAT);
     if( CRC != GetCRC( brIndex ) )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_CRC);
     if( datver != DATA_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_DATA_VERSION);
     if( forver != FORMAT_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_FORMAT_VERSION);
// themes.dat
     if(ReadFileHeader(brThemes,&type,&forver,&datver,&CRC)!=OK || type!='T')
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_THEMESDAT);
     if( CRC != GetCRC( brThemes ) )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_CRC);
     if( datver != DATA_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_DATA_VERSION);
     if( forver != FORMAT_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_FORMAT_VERSION);
// keyword.dat
     if(ReadFileHeader(brKeys,&type,&forver,&datver,&CRC)!=OK || type!='K')
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_KEYWORDDAT);
     if( CRC != GetCRC( brKeys ) )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_CRC);
     if( datver != DATA_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_DATA_VERSION);
     if( forver != FORMAT_VERSION )
         throw TSTDError("I\\"__FILE__, __LINE__, EC_WRONG_FORMAT_VERSION);

     return OK;
}
//*************************************************************************
void CloseDataFiles()
{
     close (fhThemes);
     close (fhIndex);
     close (fhData);
     close (fhKeys);
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

//*************************************************************************
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
// �-� ������ ��������� �����. ���������� NOK ���� ������ ��� �������
// ��������� �� SD. � filetype ��������� ������ ���� ���������.
// � pforver ������������ ������ ������� dat-����� (6 � 7 �����)
// � pdatver ������������ ������ ������ dat-����� (8 � 9 �����)

int ReadFileHeader(  BufRead & brIn, char* filetype, int* pforver,
                                          int* pdatver, unsigned* pCRC)
{
      int ibuf;

      brIn.ReadByte(&ibuf);   if( (char)ibuf != 'S' )   return NOK;
      brIn.ReadByte(&ibuf);   if( (char)ibuf != 'D' )   return NOK;
      brIn.ReadByte(&ibuf);

      *filetype = (char)ibuf;

      ReadInt(brIn, pCRC);

      ReadInt(brIn, pforver);
      ReadInt(brIn, pdatver);

      for( int i=0; i<11; i++ )
           brIn.ReadByte(&ibuf);

      return OK;
}

//*************************************************************************
// data.dat
int ReadCodedBody( BufRead & brIn, char* bigbuf, long maxbuf, unsigned* bodylen )
{
  int buffer[3]={0};
  ReadInt( brIn, bodylen );
  for( unsigned i=0; i< *bodylen; i++ )
    {
      if( i>=maxbuf )
         return EC_NOT_ENOUPH_BUF;

      readHalf( brIn, buffer );
      switch( buffer[0] )
         {
           case OFTEN_CODE:   readHalf( brIn, buffer+1 );
                              bigbuf[i] = UncodeChar( buffer, 2 );
                              break;

           case SELDOM_CODE:  readHalf( brIn, buffer+1 );
                              readHalf( brIn, buffer+2 );
                              bigbuf[i]  = UncodeChar( buffer, 3 );
                              break;
           default:           bigbuf[i] = UncodeChar( buffer, 1 );
         };

    }

  bigbuf[i]=0;
  return OK;
}
//**********************************************************************
char UncodeChar(  int* buffer, int length )
//   ������� �������� ������ ������. 1 2 ��� 3 ���������, � �� ����������
// ( length ).  ��������� ��� � ������ � ���������� ���� ������.
 {
          BYTE ch=0;
//-------------------------------------------------------------------------
          if( length == 1 )   // ���� ��� ������ 1 ����
                 switch( buffer[0] )
                        {
                          case 0:    return ' ';
                          case 1 :   return '�';
                          case 2 :   return '�';
                          case 3 :   return '�';
                          case 4 :   return '�';
                          case 5 :   return '�';
                          case 6 :   return '�';
                          case 7 :   return '�';
                          case 8 :   return '�';
                          case 9 :   return '�';
                          case 10:   return '�';
                          case 11:   return '�';
                          case 12:   return '�';
                          case 13:   return '�';
                        };
//-------------------------------------------------------------------------
          if( length == 2 )
                  switch( buffer[1] )
                         {
                          case  0:    return '�';
                          case  1:    return '�';
                          case  2:    return '�';
                          case  3:    return '�';
                          case  4:    return '�';
                          case  5:    return '�';
                          case  6:    return '�';
                          case  7:    return '�';
                          case  8:    return '�';
                          case  9:    return '�';
                          case 10:    return '�';
                          case 11:    return '�';
                          case 12:    return '.';
                          case 13:    return ',';
                          case 14:    return '�';
                          case 15:    return '�';
                         };
//-------------------------------------------------------------------------
          if( length == 3 )
                  {
                    ch = ch | (buffer[1] << 4);
                    ch = ch | buffer[2];
                    return ch;
                  }
//-------------------------------------------------------------------------
         return 0; // ���� �������� ����� ����� ��� ���������� �� ���������
                   // warning "Function should return a value
                   // �� ����� ���� return ���������� ������ � ���� ��������
                  // ������� �� ���������.
 }


//***************************************************************************
// index.dat
int GetNextArtNameNum( BufRead* pbrIndex, char* buffer, int* pibuf )
{
         BufRead& brIn = *pbrIndex ;
         long lbuf;

         if( ReadInt( brIn, pibuf ) != 1 || ReadName( brIn, buffer ) != 1
                         || ReadLong( brIn, &lbuf ) != 1 )
              return NOK;
#ifdef _STD_DEBUG
         char buf[255];
         sprintf( buf, "%s : %d", buffer, *pibuf );
         strcpy( buffer, buf );
#endif

         return OK;
}


//**************************************************************************
// themes.dat

int GetNextThNameNum( BufRead* pbrThemes, char* buffer, int* pnum )
{
        BufRead& brIn = *pbrThemes;
        int ibuf;
        if( ReadInt( brIn, pnum ) != 1 )
             return NOK;
        if( ReadName( brIn, buffer ) != 1 )
             return NOK;
        do
          if( ReadInt( brIn, &ibuf ) != 1 )
                return EC_WRONG_THEMESDAT;
        while( ibuf !=0 );

        return OK;
}
//***************************************************************************
// ��������������� �������, ���������� ������� ����� ����� ��������
// ��� ������� ���. ������� - 2 �����.
int GetThemesTableSize( BufRead* pbrThemes, WORD* ps )
{
       char type;
       int ibuf;
       char buffer[255];
       WORD size=0;
       BufRead& brIn = *pbrThemes;

       brIn.bigRew();
       ReadFileHeader(brIn,&type);
       if(type!='T')  return EC_WRONG_THEMESDAT;

       while( ReadInt( brIn, &ibuf ) == 1 )
       {
         size++; // ����� ��� ����� ����
         if( ReadName( brIn, buffer ) != 1 )
            return EC_WRONG_THEMESDAT;
         do
          {
            if( ReadInt( brIn, &ibuf ) != 1 )
                return EC_WRONG_THEMESDAT;
            size++;
          }
         while( ibuf !=0 );
       }
       *ps = size;
       return OK;
}
//***************************************************************************
/* ������� ������ ThemesTable -
   ������ ����� �����, ���������� ���������� � ������� ���� ���.
   ������: �����_����_1, �����_������_11, �����_������_12, ..., 0,
           �����_����_2, �����_������_21, �����_������_22, ..., 0,
                                                                                       .  .  .
           �����_����_n, �����_������_n1, �����_������_n2, ..., 0,
           0.
*/

int GetThemesTable( BufRead* pbrThemes, int* ThemesTable )
{
  pbrThemes->bigRew();

  // �������� ��������a �����
  char filetype;
  WORD i=0;
  ReadFileHeader( *pbrThemes, &filetype );
  if(filetype!='T')  return EC_WRONG_THEMESDAT;

  char buffer[256];

  while( ReadInt( *pbrThemes, ThemesTable + i ) == 1  ) // ��������� ����� ����
       {
          // ��������� ��� ����
          if( ReadName( *pbrThemes, buffer ) != 1 )
                  return EC_WRONG_THEMESDAT;

          // ��������� ������ ������ ����
          do
           {
             i++;
             if( ReadInt( *pbrThemes, ThemesTable + i ) != 1 )
                    return EC_WRONG_THEMESDAT;
           }
          while( ThemesTable[i] != 0  );
          i++;
       }

  return OK;      // ���� ��� ����������
}

//**************************************************************************
// keyword.dat

int GetNextKeyNameNum( BufRead* pbrKeys, char* buffer, int* pnum )
{
        BufRead& brIn = *pbrKeys;
        int ibuf;
        if( ReadInt( brIn, pnum ) != 1 )
                 return NOK;
        if( ReadName( brIn, buffer ) != 1 )
                 return NOK;
        do
          if( ReadInt( brIn, &ibuf ) != 1 )
                 return EC_WRONG_KEYWORDDAT;
        while( ibuf !=0 );

        return OK;
}
//***************************************************************************
// ��������������� �������, ���������� ������� ����� ����� ��������
// ��� ������� ���. ������� - 2 �����.
int GetKeywordsTableSize( BufRead* pbrKeys, WORD* ps )
{
       char type;
       int ibuf;
       char buffer[255];
       WORD size=0;
       BufRead& brIn = *pbrKeys;

       brIn.bigRew();
       ReadFileHeader(brIn,&type);
       if(type!='K')  return EC_WRONG_KEYWORDDAT;

       while( ReadInt( brIn, &ibuf ) == 1 )
       {
         size++; // ����� ��� ����� keyword'�
         if( ReadName( brIn, buffer ) != 1 )
            return EC_WRONG_KEYWORDDAT;
         do
          {
            if( ReadInt( brIn, &ibuf ) != 1 )
                return EC_WRONG_KEYWORDDAT;
            size++;
          }
         while( ibuf !=0 );
       }
       *ps = size;
       return OK;
}

//***************************************************************************
/* ������� ������ KeywordsTable -
        ������ ����� �����, ���������� ���������� � ������� ���� ���.
        ������: �����_keyword'�_1, �����_������_11, �����_������_12, ..., 0,
                �����_keyword'�_2, �����_������_21, �����_������_22, ..., 0,
                                                                                        .  .  .
                �����_keyword'�_n, �����_������_n1, �����_������_n2, ..., 0,
                0.
*/

int GetKeywordsTable( BufRead* pbrKeys, int* KeywordsTable )
{
  pbrKeys->bigRew();

  // �������� ��������a �����
  char filetype;
  WORD i=0;
  ReadFileHeader( *pbrKeys, &filetype );
  if(filetype!='K')  return EC_WRONG_KEYWORDDAT;

  char buffer[256];

  while( ReadInt( *pbrKeys, KeywordsTable + i ) == 1  ) // ��������� ����� ����
       {
          // ��������� ��� keyword'�
          if( ReadName( *pbrKeys, buffer ) != 1 )
                  return EC_WRONG_KEYWORDDAT;

          // ��������� ������ ������ keyword'�
          do
           {
             i++;
             if( ReadInt( *pbrKeys, KeywordsTable + i ) != 1 )
                    return EC_WRONG_KEYWORDDAT;
           }
          while( KeywordsTable[i] != 0  );
          i++;
       }

  return OK;      // ���� �������� ���� ����������
}


