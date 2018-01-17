
#define REPLACE         0x00
#define AND             0x01
#define OR              0x02
#define XOR             0x03

#define MAX_X            639
#define MAX_Y            349
#define SYM_X              8
#define SYM_Y             14
#define MAX_COL           80

#define BYTEOF(x,y)       ( (y)*MAX_COL + ((x)/8) )
#define BITOF(x)          ( 0x80 >> ((x)&0x7) )
#define RIGHTBITOF(x,y)   ( 0xFF >> ((x)&0x7) )
#define LEFTBITOF(x,y)    ( 0xFF << (7-(x)&0x7) )


#define EgaBase            0xA0000000L


#define MEMORYOF(x,y) (unsigned char far*)( EgaBase + (long)BYTEOF((x),(y)) )

#define SETREG(c,r,v)  outportb((c),(r)); outportb((c+1),(v))


#define   GDC                   0x3CE
#define   GDCReset              0x00
#define   GDCEnable             0x01
#define   GDCColorCompare       0x02
#define   GDCDataRotate         0x03
#define   GDCReadMapSel         0x04E
#define   GDCMode               0x05
#define   GDCMisc               0x06
#define   GDCColorDC            0x07
#define   GDCBitMask            0x08

#define SETMASK(m)  SETREG(GDC,GDCBitMask,(m))