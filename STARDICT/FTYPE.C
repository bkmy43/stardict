#include "ftype.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIGNES_SYMBOLS_SHIFT  16
/* extern char inner[29]; */

time_t Ftime=0;


char *CordInts2String(int g, int m, char *s, char rezhim)
					   /* T - широта; N - долгота*/
{
int gr,min;
char z=1; /* 1 - положительное; 0 - отрицательное */

if (g<0) { z=0; gr=-g;  }  else gr=g;
if (m<0) { z=0; min=-m; }  else min=m;

if (z==1 && rezhim=='T') z='N';
else if (z==0 && rezhim=='T') z='S';
else if (z==1  && rezhim=='N') z='E';
else if (z==0 && rezhim=='N') z='W';

if (rezhim=='T') sprintf(s,"%02d%c%02d",gr,z,min);
else sprintf(s,"%03d%c%02d",gr,z,min);

return s;
}


char *CordInts3String(int g, int m, int sg, char *s, char rezhim)
					   /* T - широта; N - долгота*/
{
int deg,min;

deg=g;
min=m;
if (rezhim=='T' && sg=='S' || rezhim=='N' && sg=='W')
   {
   if (deg!=0) deg=-deg; else min=-min;
   }
return(CordInts2String(deg,min,s,rezhim));
}


char *CordDouble2String(double k, char *s, char rezhim)
					   /* T - широта; N - долгота*/
{
char z;
double p;
int m;

if (k<0.0) { p=-k; z=(rezhim=='T') ? 'S' : 'W' ; }
      else { p=k;  z=(rezhim=='T') ? 'N' : 'E' ; }

m=(int)((p-(int)p)*60.0);

if (rezhim=='T') sprintf(s,"%2d%c%02d",(int)p,z,m);
else sprintf(s,"%2d%c%02d",(int)p,z,m);

return s;
}


void CordString2Ints(char *str, int *g, int *m)
{
char *s,c[4]="\x0\x0\x0";
int z;

s=strchr(str,'N');
if (s!=NULL) z=1;
else
   {
   s=strchr(str,'S');
   if (s!=NULL) z=-1;
   else
      {
      s=strchr(str,'E');
      if (s!=NULL) z=1;
      else
	 {
	 s=strchr(str,'W');
	 if (s!=NULL) z=-1;
	 else gerrmes(FT_BADPARAM,"CordString2Double",0,SYSTEM_ERROR);
	 }
      }
   }
strncpy(c,str,(size_t)(s-str));
*g=atoi(c);
strcpy(c,s+1);
*m=atoi(c);
if (z==-1)  { if (*g==0) *m=-(*m);  else *g=-(*g); }
}


void CordString3Ints(char *str, int *g, int *m, int *sg)
{
char *s,c[4]="\x0\x0\x0";

s=strchr(str,'N');
if (s!=NULL) *sg=*s;
else
   {
   s=strchr(str,'S');
   if (s!=NULL) *sg=*s;
   else
      {
      s=strchr(str,'E');
      if (s!=NULL) *sg=*s;
      else
	 {
	 s=strchr(str,'W');
	 if (s!=NULL) *sg=*s;
	 else gerrmes(FT_BADPARAM,"CordString2Double",0,SYSTEM_ERROR);
	 }
      }
   }
strncpy(c,str,(size_t)(s-str));
*g=atoi(c);
strcpy(c,s+1);
*m=atoi(c);
}


void CordString2Double(char *str, double *k)
{
char *s,c[4]="\x0\x0\x0";
int z;

s=strchr(str,'N');
if (s!=NULL) z=1;
else
   {
   s=strchr(str,'S');
   if (s!=NULL) z=-1;
   else
      {
      s=strchr(str,'E');
      if (s!=NULL) z=1;
      else
	 {
	 s=strchr(str,'W');
	 if (s!=NULL) z=-1;
	 else gerrmes(FT_BADPARAM,"CordString2Double",0,SYSTEM_ERROR);
	 }
      }
   }
strncpy(c,str,(size_t)(s-str));
*k=(double)atoi(c);
strcpy(c,s+1);
*k+=(double)(atoi(c))/60.0;
*k=*k*z;
}


time_t GetCurrentTime()
   /*
   возвращает текущее время в секундах
   устанавливает Ftime
   */
{
time_t t;

t=time(NULL);
Ftime=t;
return(t);
}


long GetPassedTime(void)
     /*
     возвращает время (в секундах !), прощедшее с последнего вызова этой
     функции или GetCurrentTime
     устанавливает Ftime
     рекомендуется до первого вызова использовать функцию GetCurrentTime
     */
{
time_t t;
long r;

t=time(NULL);
r=(Ftime==0) ? 1000000l : t-Ftime;
Ftime=t;
return(r);
}

void Seconds2String(long seconds, char *str, char *format)
     /* преобразует количество секунд в строку по форматам
     типа HH:MM:SS или MM:SS и т.п. */
{
int h,m,s;
char *c;

s=(int)(seconds%60);
m=(int)(seconds/60%60);
h=(int)(seconds/3600%24);
strcpy(str,format);
c=strstr(str,"SS");
if (c!=NULL) { *c=s/10+48; *(c+1)=s%10+48; }
c=strstr(str,"MM");
if (c!=NULL) { *c=m/10+48; *(c+1)=m%10+48; }
c=strstr(str,"HH");
if (c!=NULL) { *c=h/10+48; *(c+1)=h%10+48; }
}

/*
char *Kord2String(double kord, char *str)
	преобразует координату планеты в строку типа ГГЗММ
	в качестве З - номер знака в inner
{
int g,m,s;

g=(int)kord;
s=g/30;
m=(int)((kord-g)*60.0);
g=g%30;
sprintf(str,"%2d%c%02d",g,inner[SIGNES_SYMBOLS_SHIFT+s],m);
str[5]=0;
return(str);
} */


char *Date2String(int d, int m, int y, char *str, char *format)
		  /* DD:MM:YY  MM:DD:YYYY */
{
char *s;

strcpy(str,format);
s=strstr(str,"DD");
if (s==NULL) gerrmes(FT_BADFORMAT,"Date2String",1,SYSTEM_ERROR);
*s=d/10+48;
*(s+1)=d%10+48;

s=strstr(str,"MM");
if (s==NULL) gerrmes(FT_BADFORMAT,"Date2String",2,SYSTEM_ERROR);
*s=m/10+48;
*(s+1)=m%10+48;

s=strstr(str,"YYYY");
if (s==NULL)
   {
   s=strstr(str,"YY");
   if (s==NULL) gerrmes(FT_BADFORMAT,"Date2String",3,SYSTEM_ERROR);
   *s=y%100/10+48;
   *(s+1)=y%1000;
   }
else
   {
   *s=y/1000+48;
   *(s+1)=y/100%10+48;
   *(s+2)=y/10%10+48;
   *(s+3)=y%10+48;
   }

return(str);
}


char *Date2StringE(int d, int m, int y, char epoch, char *str, char *format)
		  /* DD:MM:YYE  MM:DD:YYYYE */
{
char *s;

Date2String(d,m,y,str,format);
s=strchr(str,'E');
if (s==NULL) gerrmes(FT_BADFORMAT,"Date2StringE",0,SYSTEM_ERROR);
*s=(epoch=='A') ? '+' : '-' ;
return(str);
}


void String2Date(char *str, int *d, int *m, int *y, char *format)
{
char *s,c[5]="\x0\x0\x0\x0";
int n;

s=strstr(format,"DD");
if (s==NULL) gerrmes(FT_BADFORMAT,"String2Date",1,SYSTEM_ERROR);
n=(int)(s-format);
memcpy(c,str+n,2);
*d=atoi(c);

s=strstr(format,"MM");
if (s==NULL) gerrmes(FT_BADFORMAT,"String2Date",2,SYSTEM_ERROR);
n=(int)(s-format);
memcpy(c,str+n,2);
*m=atoi(c);

s=strstr(format,"YYYY");
if (s==NULL)
   {
   s=strstr(format,"YY");
   if (s==NULL) gerrmes(FT_BADFORMAT,"String2Date",3,SYSTEM_ERROR);
   n=(int)(s-format);
   memcpy(c,str+n,2);
   }
else
   {
   n=(int)(s-format);
   memcpy(c,str+n,4);
   }
*y=atoi(c);
}


void String2DateE(char *str, int *d, int *m, int *y, char *epoch, char *format)
{
char *s;

String2Date(str,d,m,y,format);
s=strchr(str,'-');
if (s==NULL)
   {
   s=strchr(str,'+');
   if (s==NULL) gerrmes(FT_BADFORMAT,"String2DateE",0,SYSTEM_ERROR);
   *epoch='A';
   }
else *epoch='B';
}


void CordString2Int(char *str, int *cord)
{
char *s,c[4]="\x0\x0\x0",m[3]="\x0\x0";
int z;

s=strchr(str,'N');
if (s!=NULL) z=1;
else {
     s=strchr(str,'S');
     if (s!=NULL) z=-1;
     else
	{
	s=strchr(str,'E');
	if (s!=NULL)  z=1;
	else
	   {
	   s=strchr(str,'W');
	   if (s!=NULL)  z=-1;
	   }
	}
     }

memcpy(c,str,(size_t)(s-str));
memccpy(m,s+1,0,3);
s=strchr(m,' ');
if (s==m) strcpy(m,"0"); else *s=0;

*cord=(atoi(str)*60+atoi(m))*z;

}


char *CordInt2String(int cord, char *str, char rezhim)
{
int g,m,z=1,k;

if (cord<0) { z=-1; k=-cord; }
   else k=cord;
g=k/60;
m=k%60;
if (z==-1)  { if (g==0) m=-m; else g=-g; }

return(CordInts2String(g,m,str,rezhim));
}

char *Time2String(int h, int m, char *str, char *format)
{
char *s;

if (h<0 || h>23 || m<0 || m>59)
       gerrmes(FT_BADPARAM,"Time2String",0,SYSTEM_ERROR);

strcpy(str,format);

s=strstr(str,"HH");
if (s==NULL) gerrmes(FT_BADFORMAT,"Time2String",1,SYSTEM_ERROR);
*s=h/10+48;
*(s+1)=h%10+48;

s=strstr(str,"MM");
if (s==NULL) gerrmes(FT_BADFORMAT,"Time2String",2,SYSTEM_ERROR);
*s=m/10+48;
*(s+1)=m%10+48;

return(str);
}


void String2Time(char *str, int *h, int *m, char *format)
{
char *s,c[3]="  ";
int l;


s=strstr(format,"HH");
if (s==NULL) gerrmes(FT_BADFORMAT,"String2Time",1,SYSTEM_ERROR);
l=(int)(s-format);
strncpy(c,str+l,2);
*h=atoi(c);

s=strstr(format,"MM");
if (s==NULL) gerrmes(FT_BADFORMAT,"String2Time",2,SYSTEM_ERROR);
l=(int)(s-format);
strncpy(c,str+l,2);
*m=atoi(c);
}