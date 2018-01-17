#include <stdio.h>
#include <alloc.h>
#include <dos.h>
#include "ega.h"
#include "pcx.h"
#include "graph.h"
#include <graphics.h> // !!!!!!!!

#define BUFFLEN  640

#define MY_MOVE     0   // сдвиг ввеpх
#define MY_SHIFT    80  // сдвиг впpаво

void *memcpy(void *dest, const void *src, size_t n);  // !!!!!!!!!
char *save1,*save2,*save3; // !!!!!!!


typedef struct pcxheader
{
char manuf;
char hard;
char encod;
char bitpx;
int  x1;
int  y1;
int  x2;
int  y2;
int  hres;
int  vres;
char clrma[48];
char vmode;
char nplanes;
int  bplin;
int  palinfo;
int  shres;
int  svres;
char xtra[54];
}
PCXHEADER;

int op=REPLACE;
static PCXHEADER header;

void setEGA(void)
{
union REGS r;
r.h.ah=0;
r.h.al=0x10;
int86(0x10,&r,&r);
}

void setOperation(int operation) { op=operation; }

void openScreen(unsigned char color)
{
SETREG(GDC,GDCReset,color);
SETREG(GDC,GDCEnable,0x0F);
SETREG(GDC,GDCDataRotate,op<<3);
}

void closeScreen(void)
{
SETREG(GDC,GDCBitMask,0xff);
SETREG(GDC,GDCEnable,0x00);
SETREG(GDC,GDCDataRotate,REPLACE);
}


void writePixel(unsigned int x, unsigned int y)
{
unsigned char mask,c;
unsigned char far *memory;

mask=BITOF(x);
memory=MEMORYOF(x,y);

SETMASK(mask);
c=*memory;
*memory=0;

}

void EGAWriteLinePlanes(unsigned char *planes, int npl, int planeLen,
			int x, int y)
{
unsigned char *pl,mask;
unsigned char far *m;
unsigned char far *string_begin;
char c;
int k;
string_begin=MEMORYOF(x,y);

pl=planes;
m=string_begin;
mask=0xFF;

setOperation(REPLACE);
openScreen(0);
SETMASK(mask);
for (k=0;k<planeLen;k++)
   {
   c=*m; *m=0;
   m++;
   pl++;
   }

pl=planes;
m=string_begin;
mask=*pl;
setOperation(REPLACE);
openScreen(1);
SETMASK(mask);
for (k=0;k<planeLen;k++)
   {
   c=*m;
   *m=0;
   m++;
   pl++;
   mask=*pl;
   SETMASK(mask);
   }


if (npl>1)
   {
   pl=planes+planeLen;
   mask=*pl;
   m=string_begin;

   setOperation(OR);
   openScreen(2);
   SETMASK(mask);
   for (k=0;k<planeLen;k++)
      {
      c=*m;
      *m=0;
      m++;
      pl++;
      mask=*pl;
      SETMASK(mask);
      }
   }


if (npl>2)
   {
   pl=planes+2*planeLen;
   m=string_begin;
   mask=*pl;

   openScreen(4);
   SETMASK(mask);
   for (k=0;k<planeLen;k++)
      {
      c=*m;
      *m=0;
      m++;
      pl++;
      mask=*pl;
      SETMASK(mask);
      }
   }


if (npl>3)
   {
   pl=planes+3*planeLen;
   m=string_begin;
   mask=*pl;

   openScreen(8);
   SETMASK(mask);
   for (k=0;k<planeLen;k++)
      {
      c=*m;
      *m=0;
      m++;
      pl++;
      mask=*pl;
      SETMASK(mask);
      }
   }
setOperation(REPLACE);
openScreen(1);
closeScreen();
}


void setPalette(unsigned char i,
		unsigned char red,
		unsigned char green,
		unsigned char blue)
{
union REGS r;
r.h.ah=0x10;
r.h.al=1;
r.h.bl=i;
r.h.bh=(red%2)*32 + (red/2)*4 +
	 (green%2)*16 + (green/2)*2 +
	 (blue%2)*8  + (blue/2);
int86(0x10,&r,&r);
}


void PCXGetSize(char *fname, int *x1, int *x2, int *y1, int *y2)
{
FILE *fp;
int len=sizeof(PCXHEADER);

fp=fopen(fname,"rb");
if (fp==NULL) { printf("\nCan't open file %s.",fname); return; }

fread(&header,len,1,fp);

*x1=header.x1;
*x2=header.x2;
*y1=header.y1;
*y2=header.y2;

fclose(fp);
}


void PCXSetPalette(char *fname)
{
FILE *fp;
int len=sizeof(PCXHEADER);
int r;

fp=fopen(fname,"rb");
if (fp==NULL) { printf("\nCan't open file %s.",fname); return; }

fread(&header,len,1,fp);

for (r=0;r<16;r++)
   {
   setPalette(r,
	      header.clrma[3*r  ]/64,
	      header.clrma[3*r+1]/64,
	      header.clrma[3*r+2]/64  );
   }
fclose(fp);
}


void PCXWrite(char *fname)
{
FILE *fp;
int k,n,y,l,npl,rest,rep_count,len=sizeof(PCXHEADER);
unsigned char *planes,*buffer,*b,c,bits;
char bufr[512];

fp=fopen(fname,"rb");
if (fp==NULL)  { printf("\nCan't open file %s.",fname); return; }
if (setvbuf(fp, bufr, _IOFBF, 512) != 0)
   printf("\nFailed to set up buffer for input file");


fread(&header,len,1,fp);

buffer=(unsigned char *)farmalloc(BUFFLEN);
if (buffer==NULL) { printf("\nNot enough memory"); return; }

planes=(unsigned char *)farmalloc(header.nplanes*header.bplin);
if (planes==NULL) { printf("\nNot enough memory"); return; }
rest=fread(buffer,1,BUFFLEN,fp);
b=buffer;

npl=header.nplanes;
// for (y=0;y<MAX_Y;y++)
for (y=0;y<=MAX_Y+MY_MOVE;y++)
   {
   n=header.nplanes*header.bplin;
   for (k=0;k<n;k+=rep_count)
      {
      if (rest==0)
	 {
	 rest=fread(buffer,1,BUFFLEN,fp);
	 b=buffer;
	 }
      c=b[0]; b++; rest--;
      if ((c&0xC0)==0xC0)
	 {
	 rep_count=(c&0x3F);
	 if (rest==0)
	    {
	    rest=fread(buffer,1,BUFFLEN,fp);
	    b=buffer;
	    }
	  bits=(*b); b++; rest--;
	  }
      else { rep_count=1; bits=c; }
      for (l=0;l<rep_count;l++) planes[k+l]=bits;
      }
//   EGAWriteLinePlanes(planes,npl,80,0,y);
   if (y<MY_MOVE)  EGAWriteLinePlanes(planes,npl,80,MY_SHIFT,0);
      else EGAWriteLinePlanes(planes,npl,80,MY_SHIFT,y-MY_MOVE);
   //if (y==0) memcpy(save1,planes,4*80);// !!!!!!!
   }
setOperation(REPLACE);
openScreen(1);
closeScreen();

farfree(planes);
farfree(buffer);
fclose(fp);
}


/****************** write ****************************/
void EGAReadLine (unsigned char *planes, int planeLen, int x, int y)
{
int kPlane,k;
unsigned char far *memory;
//unsigned char far *mb;
unsigned char far *s_begin;
/*
memory=MEMORYOF(x,y);

s_begin=memory;

for (kPlane=0; kPlane<4; kPlane++)
   {
   SETREG(GDC,GDCReadMapSel,kPlane);
   memory=s_begin;

   for (k=0;k<planeLen;k++)
      {
      unsigned char c;
      c=*memory;
      *planes=c;
      memory++;
      planes++;
      }
   }*/ /* for(kPlane) */

for (k=0;k<planeLen*4;k++) planes[k]=0;
for (k=0;k<planeLen*8;k++)
   {
   unsigned char c;
   c=getpixel(x+k,y);
   planes[k/8]+=(c%2)<<(7-k%8);
   planes[k/8+80]+=(c/2%2)<<(7-k%8);
   planes[k/8+160]+=(c/4%2)<<(7-k%8);
   planes[k/8+240]+=(c/8%2)<<(7-k%8);
   }

}


int PCXSave(char *fname)
{
FILE *fp;
int y,r;
int len=sizeof(PCXHEADER);

unsigned char far *buffer;
unsigned char far *mb;

unsigned char far *pack_buffer;
unsigned char far *mpb;

int buf_full;
int pack_count;
unsigned char rep;
unsigned char cur_byte;
   int i; // !!!!!!!!!!!!

fp=fopen(fname,"wb");
if (fp==NULL)  { gerrmes(CREAT_ERROR,"PCXSave",0,COMMON_ERROR); return -1; }

header.manuf=0xA;
header.hard=5;
header.encod=1;
header.bitpx=1;
header.x1=0;
header.y1=0;
header.x2=MAX_X;
header.y2=MAX_Y;
header.hres=MAX_X+1;
header.vres=MAX_Y+1;
header.vmode=0;
header.nplanes=4;
header.bplin=80;
header.palinfo=1;

r=fwrite(&header,1,len,fp);
if (r!=len) { gerrmes(WRITE_ERROR,"PCXSave",1,COMMON_ERROR); return -1; }

buffer=(unsigned char far *) farmalloc(4*80);
if (buffer==NULL) gerrmes(NOMEM,"PCXSave",1,SYSTEM_ERROR);
pack_buffer=(unsigned char far *) farmalloc(4*80);
if (pack_buffer==NULL) gerrmes(NOMEM,"PCXSave",1,SYSTEM_ERROR);

for (y=0;y<=MAX_Y;y++)
   {
   mb=buffer;
   mpb=pack_buffer;

   EGAReadLine(mb,80,0,y);
   //if (y==0) memcpy(save2,mb,4*80); // !!!!!!!!!!!!
   if (y%10==0) { sound(300+y*10); delay(20); nosound(); }  // !!!!!!!!!!!!

   mb=buffer;
   mpb=pack_buffer;
   buf_full=4*80;
   pack_count=0;

   while(1)
      {
      if (buf_full==0) break;
      cur_byte=*mb++;
      rep=1;
      buf_full--;

      while ((*mb==cur_byte)&&(buf_full>0))
	 {
	 mb++;
	 rep++;
	 buf_full--;
	 if (buf_full==0) break;
	 if (rep==63) break;
	 } /* while */

      if (rep>1)
	 {
	 *mpb=(0xC0 | rep);  mpb++;
	 *mpb=cur_byte;      mpb++;
	 pack_count+=2;
	 }
      else if (cur_byte<0xC0)
	 {
	 *mpb=cur_byte;      mpb++;
	 pack_count++;
	 }
      else
	 {
	 *mpb=0xC1;          mpb++;
	 *mpb=cur_byte;      mpb++;
	 pack_count+=2;
	 }
      } /* while(1) */
   r=fwrite(pack_buffer,1,pack_count,fp);
   if (r!=pack_count)
      {
      gerrmes(WRITE_ERROR,"PCXSave",2,COMMON_ERROR);
      farfree(buffer);
      farfree(pack_buffer);
      return -1;
      }

   } /* for */

fclose(fp);
farfree(buffer);
farfree(pack_buffer);
return 0;
}

/*
void main()
{
int i;
save1=(char *)farmalloc(4*80);
save2=(char *)farmalloc(4*80);
save3=(char *)farmalloc(4*80);
for (i=0;i<4*80;i++) { save1[i]='S'; save2[i]='S'; save3[i]='S'; }

setEGA();
Ropen(GM_EGA);
PCXSetPalette("test.pcx");
PCXWrite("test.pcx");
PCXSave("teto.pcx");
cleardevice();
memcpy(save3,save1,4*80);
PCXWrite("teto.pcx");
getch();
Rclose();
printf("\n**********\n");
for (i=0;i<4*80;i++)  printf("%2x",save3[i]);
for (i=0;i<4*80;i++)   printf("%2x",save2[i]);
for (i=0;i<4*80;i++)   printf("%2x",save1[i]);

getch();
}

*/