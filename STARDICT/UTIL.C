#include "util.h"
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <ctype.h>

#include <alloc.h>
#include <stdio.h>

char ALL_SYMBOLS[]=
   {" _-€‚ƒ„…ğ†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œŸABCDEFGHIJKLMNOPQRSTUVWXYZ ¡¢£¤¥ñ¦§¨©ª«¬­®¯àáâãäåæçèéêëìíîïabcdefghijklmnopqrstuvwxyz0123456789!?',.;~`@#$%^&*()|\\/=\"<>"};

/*
void main()
{
char *buf;
long i;
int ret;


buf=(char *)farmalloc(111000l);
if (buf==NULL) return;
for (i=0;i<111000l;i++) *(buf+i)='A';
ret=writemem("test",buf,111000l);
printf("\nret = %i",ret);
farfree(buf);

printf("\nfreedrive=%d",FindFreeDrive(51000000));

}
*/

int writemem(char *flname, char *buf, long bufsize)
{
long i,r,q,rbuf=10000l,f,ret;

f=_creat(flname,FA_ARCH);
if (f==-1) return(CANT_CREATE);

r=bufsize%10000l;
q=(r==0) ? bufsize/10000l : bufsize/10000l+1;

for (i=0;i<q;i++)
   {
   if (r!=0 && i==q-1) rbuf=r;
   ret=write(f,buf+i*10000l,rbuf);
   if (ret==-1) { close(f); return(CANT_WRITE); }
   }
close(f);
return(OK);
}

   	
int readmem(char *flname, char *buf)
{
long i,r,q,rbuf=10000l,f,bufsize;
int ret;

f=_open(flname,O_RDONLY | O_BINARY);
if (f==-1) return(CANT_OPEN);
bufsize=filelength(f);

r=bufsize%10000l;
q=(r==0) ? bufsize/10000l : bufsize/10000l+1;

for (i=0;i<q;i++)
   {
   if (r!=0 && i==q-1) rbuf=r;
   ret=read(f,buf+i*10000l,rbuf);
   if (ret==-1) { close(f); return(CANT_READ); }
   }
close(f);
return(OK);
}


int FindFreeDrive(long freespace)
    /* ¢®§¢à é ¥â ­®¬¥à ¤à ©¢  (  3 - C,  4 - D  etc.),
       £¤¥ ¤®áâ â®ç­® á¢®¡®¤­®£® ¬¥áâ ,   ¨­ ç¥  0  */
{
int i;
for (i=3;i<87;i++)   if (diskspace(i)>freespace) return(i);
return(0);
}



unsigned long diskspace(int drive)
     /*  0 - default; 1 - A etc.;  on error returns  0 */
{
struct dfree df;
getdfree(drive,&df);
if (df.df_sclus==0xFFFF) return (0);
return((unsigned long) df.df_avail * (unsigned long)df.df_bsec
				* (unsigned long)df.df_sclus);
}


int lastfilechoise(char spec[13], char flnam[13])
{
struct ffblk ff;
int r,i;
unsigned da,ti;
r=findfirst(spec,&ff,FA_ARCH);
if (r!=0) return(0);
da=ff.ff_fdate;
ti=ff.ff_ftime;
strncpy(flnam,ff.ff_name,13);
for (i=1;findnext(&ff)==0;i++)
   {
   if (da<ff.ff_fdate || da==ff.ff_fdate && ti<ff.ff_ftime)
	  {
	  strncpy(flnam,ff.ff_name,13);
	  da=ff.ff_fdate;
	  ti=ff.ff_ftime;
	  }
   }
return(21);
}


char *Path2Name(char *path, char *name)
{
int i,j,k=0;
char *s;

for (i=strlen(path)-1;i>0;i--)
   if (path[i]=='\\')
      {
      for (j=i+1;path[j]!=0 && path[j]!='.';j++) name[k++]=path[j];
      name[k]=0;
      return(name);
      }
memccpy(name,path,'.',strlen(path));
if ( (s=strchr(name,'.')) != NULL ) *s=0;
return(name);
}



int CompareStrings(const char *s1, const char *s2, int length, int case_sense)
			      // 0 - case sensitive; 1 - not case sensitive
{
int l1,l2,r;

l1=strlen(s1);
l2=strlen(s2);

for (int i=0; i<l1 && i<l2 && i<length; i++)
   {
   r=(case_sense==0) ? CompareSymbols(s1[i],s2[i])
		     : CompareSymbols(ftoupper(s1[i]),ftoupper(s2[i])) ;
   if (r == '=' ) continue;
   if (r == '>' ) return '>';
   else return '<';
   }
if (i==length) return '=';
if (l1==l2) return '=';
else if (l1>l2) return '>';
else return '<';
}



int CompareSymbols(const char c1, const char c2)
{
if (strchr(ALL_SYMBOLS,c1)<strchr(ALL_SYMBOLS,c2)) return '<';
if (strchr(ALL_SYMBOLS,c1)>strchr(ALL_SYMBOLS,c2)) return '>';
return '=';
}


int ftoupper(int c)
{
if (c>='a' && c<='z') return(toupper(c));
if (c>=' ' && c<='¯') return(c-32);
if (c>='à' && c<='ï') return(c-80);
if (c=='ñ') return('ğ');
return(c);
}


