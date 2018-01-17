#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>

#define QDIRS      11
#define CFGSIZE    18

#define YES         0
#define NO         -1

#define PSWNUM1     1
#define PSWNUM2     2

//#define B_DRIVE            1
#define INSTALL_BYTE       0x135
#define INSTALLATION_BYTE  0x136
#define LEGAL              8


int DefineHardData(unsigned int *tim, unsigned int *dirnum);
int IfSameHard(int tim, int dirnum);
int UpdatePsw(char *cfgname, int pswnum1, unsigned int value1,
			     int pswnum2, unsigned int value2);
int ReadPsw(char *cfgname, int pswnum1, unsigned int *value1,
			   int pswnum2, unsigned int *value2);
signed long DiskSpace(int drive);
signed long AllFilesSpace(char drive);
int ReadBoot(int drive, int byte);
int WriteBoot(int drive, int byte, char value);
void MakeKeyDisk(int qcopies);


int CUR_DRIVE;


int DefineHardData(unsigned int *tim, unsigned int *dirnum)
// return:  0 - OK; -1 - bad
{
int i;
struct ffblk ff;
char *dirs[QDIRS]={"DOS","NC","WINDOWS","WIN95","WW",
		   "WINWORD","NU","ANTI","SYS","TOOLS",
		   "WIN"};
char path[MAXPATH];


for (i=0;i<QDIRS;i++)
   {
   sprintf(path,"C:\\%s",dirs[i]);
   if (findfirst(path,&ff,FA_DIREC)==0)
     {
     *dirnum=i;
     *tim=ff.ff_ftime;
     return 0;
     }
   sprintf(path,"C:\\%s","CONFIG.SYS");
   if (findfirst(path,&ff,FA_ARCH)==0)
     {
     *dirnum=QDIRS;
     *tim=ff.ff_ftime;
     return 0;
     }
   }
return -1;
}


int IfSameHard(int tim, int dirnum)
// return:  0 - OK; -1 - bad
{
struct ffblk ff;
char *dirs[QDIRS]={"DOS","NC","WINDOWS","WIN95","WW",
		   "WINWORD","NU","ANTI","SYS","TOOLS",
		   "WIN"};
char path[MAXPATH];


if (dirnum==QDIRS)
   {
   sprintf(path,"C:\\%s","CONFIG.SYS");
   if (findfirst(path,&ff,FA_ARCH)==0  && tim==ff.ff_ftime) return 0;
   }
else
   {
   sprintf(path,"C:\\%s",dirs[dirnum]);
   if (findfirst(path,&ff,FA_DIREC)==0  && tim==ff.ff_ftime) return 0;
   }
return -1;
}


int UpdatePsw(char *cfgname, int pswnum1, unsigned int value1,
			     int pswnum2, unsigned int value2)
{
struct ftime ft;
int f,r;
unsigned int *buf;

buf=(unsigned int *)farmalloc(CFGSIZE);
if (buf==NULL) { printf ("\nNot enough memory!");  return -1; }

f=open(cfgname,O_RDONLY | O_BINARY);
if (f==-1)
   {
   printf ("\nCan't open file %s",cfgname);
   farfree(buf);
   return -1;
   }
r=read(f,buf,CFGSIZE);
getftime(f,&ft);
close(f);

buf[pswnum1]=value1;
buf[pswnum2]=value2;

f=_creat(cfgname,FA_ARCH | O_BINARY);
if (f==-1)
   {
   printf("\nCan't write on diskette");
   farfree(buf);
   return -1;
   }
r=write(f,buf,CFGSIZE);
setftime(f,&ft);
close(f);


farfree(buf);
return 0;
}


int ReadPsw(char *cfgname, int pswnum1, unsigned int *value1,
			   int pswnum2, unsigned int *value2)
{
int f,r;
unsigned int *buf;

buf=(unsigned int *)farmalloc(CFGSIZE);
if (buf==NULL) { printf ("\nNot enough memory!");  return -1; }

f=open(cfgname,O_RDONLY | O_BINARY);
if (f==-1)
   {
   printf ("\nCan't open file %s",cfgname);
   farfree(buf);
   return -1;
   }
r=read(f,buf,CFGSIZE);
if (r!=CFGSIZE) { printf("\nFile damaged!"); return -1; }
close(f);

*value1=buf[pswnum1];
*value2=buf[pswnum2];

farfree(buf);
return 0;
}


signed long DiskSpace(int drive)
     /*  0 - default; 1 - A etc.;  on error returns  0 */
{
struct dfree df;
getdfree(drive,&df);
if (df.df_sclus==0xFFFF) return (-1);
return((signed long) df.df_avail * (signed long)df.df_bsec
				* (signed long)df.df_sclus);
}

signed long AllFilesSpace(char drive)
     /*  'A', 'B' etc. */
{
char path[MAXPATH]="A:\\*.*";
struct ffblk ff;
long signed p=0l;

path[0]=drive;
if (findfirst(path,&ff,FA_DIREC)==0)
   p+=(ff.ff_fsize/1024l)*1024l+1024l;
else return p;

while( findnext(&ff)==0) p+=(ff.ff_fsize/1024l)*1024l+1024l;
return p;

}

int ReadBoot(int drive, int byte)
{
char *buf;

buf=(char *)farmalloc(2048);
if (buf==NULL) { printf ("\nNot enough memory!");  return -1; }

absread(drive,1,0,buf);

farfree(buf);
return buf[byte];
}


int WriteBoot(int drive, int byte, char value)
{
char *buf;

buf=(char *)farmalloc(2048);
if (buf==NULL) { printf ("\nNot enough memory!");  return -1; }

absread(drive,1,0,buf);
buf[byte]=value;
abswrite(drive,1,0,buf);

farfree(buf);
return 0;
}


void Start()
{
unsigned int tim,dirnum;
struct ffblk ff;

CUR_DRIVE=getdisk();
if (findfirst("STARDICT.CFG",&ff,FA_ARCH)!=0) { printf("\nПотерян необходимый файл"); exit(0); }
if (ReadPsw(ff.ff_name,PSWNUM1,&tim,PSWNUM2,&dirnum)==-1) { printf("\nОшибка чтения"); exit(0); }
if (tim!=0)
   {
   if (IfSameHard(tim,dirnum)==NO) { printf("\nНелегальная копия"); exit(0); }
   }
else
   {
   if (ReadBoot(CUR_DRIVE,INSTALL_BYTE)!=LEGAL) { printf("\nНелегальная копия!"); exit(0); }
   }
}


void MakeKeyDisk(int qcopies)
{
WriteBoot(CUR_DRIVE,INSTALL_BYTE,LEGAL);
WriteBoot(CUR_DRIVE,INSTALLATION_BYTE,qcopies);
}