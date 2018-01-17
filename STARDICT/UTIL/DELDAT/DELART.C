#include "deldat.h"

int DelArt( int num )
 {
    int ibuf=0;
    char buffer[100];
    long lbuf=0, pos;
    float len=0;    // длинна текста статьи в data.dat

    BufRead  brIndex;
    BufRead  brData;
    BufRead  brThemes;
    BufWrite bwNewIndex;
    BufWrite bwNewData;
    BufWrite bwNewThemes;

    brIndex.SetFileHandler( index );
    brData.SetFileHandler( data );
    brThemes.SetFileHandler( themes );
    bwNewIndex.SetFileHandler( nindex );
    bwNewData.SetFileHandler( ndata );
    bwNewThemes.SetFileHandler( nthemes );

    // Считывание адреса удаляемой статьи
    while( 1 )
      {
	if( ReadInt( brIndex, &ibuf ) != 1 )
	   return ART_NOT_FOUND;

	ReadName( brIndex, buffer );
	ReadLong( brIndex, &lbuf );
	if( ibuf == num )
	   {
	     pos = lbuf;
	     break;
	   }
      }

    lseek( index, 0L, SEEK_SET );
    brIndex.RenewBuf();
    brIndex.ReachEOF = NO;


    // Удаление статьи из файла themes.dat
    while( ReadInt( brThemes, &ibuf ) == 1  )
      {
	ReadName( brThemes, buffer );
	WriteInt( bwNewThemes, ibuf );
	WriteName( bwNewThemes, buffer );
	ibuf=0;
	while( 1 )
	  {
	    ReadInt( brThemes, &ibuf );
	    if( ibuf != num )
		WriteInt( bwNewThemes, ibuf );

	    if( ibuf == 0x0FFF )
	      break;
	  }
      }

  // Удаление статьи из файла data.dat
    int i=0;
    ibuf=0;
    while( i != pos )
       {
	   brData.ReadByte( &ibuf );
	   i++;
	   bwNewData.WriteByte( ibuf );
       }


    int buf[3];
    char ch=0;
    while( readHalf( brData, buf ) != EOF )
	{
	  switch( buf[0] )
	    {
	      case OFTEN_CODE:   readHalf( brData, buf+1 );
				 ch = UncodeChar( buf, 2 );
				 len+=1;
				 break;

	      case SELDOM_CODE:  readHalf( brData, buf+1 );
				 readHalf( brData, buf+2 );
				 ch = UncodeChar( buf, 3 );
				 len+=1.5;
				 break;

	      default:           ch  = UncodeChar( buf, 1 );
				 len+=0.5;
	    };

	  if( ch == EOF )         // если обнаружен конец текста
	     {
	       if( brData.wh == 1 )
		 {
		   brData.FF();
		   len+=0.5;
		 }
	       break;
	     }
	}

    i = ibuf = 0;
    do
      {
	brData.ReadByte( &i );
	ReadInt( brData, &ibuf );
	len+=3;
      }
    while( i != 0xFF );

    ibuf=0;
    while( brData.ReadByte( &ibuf ) == 1 )
	bwNewData.WriteByte( ibuf );

  // Удаление статьи из файла index.dat
    while( ReadInt( brIndex, &ibuf ) == 1  )
      {
	ReadName( brIndex, buffer );
	ReadLong( brIndex, &lbuf );

	if( ibuf != num )
	  {
	    WriteInt( bwNewIndex, ibuf );
	    WriteName( bwNewIndex, buffer );
	    if( lbuf > pos )    // если эта статья стоит в data.dat
		lbuf-=(long)len;       // после удаляемой
	    WriteLong( bwNewIndex, lbuf );
	  }
      }

  bwNewIndex.RenewBuf();
  bwNewData.RenewBuf();
  bwNewThemes.RenewBuf();
  return OK;
 }
