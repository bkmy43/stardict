#include "rw.h"
#include <fcntl.h>
#include <sys/stat.h>

const OFTEN_CODE  = 0xE;
const SELDOM_CODE = 0xF;

const OK		 =  0;
const ART_NOT_FOUND      = -2;
const CAN_NOT_CREATE     = -3;
const CAN_NOT_MAKE_BAK   = -4;
const CAN_NOT_OPEN_DATA  = -5;
const CAN_NOT_SAVE  = -6;

char UncodeChar(  int* buffer, int length );
int copyfile(char *fromfile, char *tofile);
void errres( int ec );

typedef int filehand;

extern  filehand index;
extern  filehand data;
extern  filehand themes;
extern  filehand nindex;
extern  filehand  ndata;
extern  filehand  nthemes;

int DelArt( int num );
