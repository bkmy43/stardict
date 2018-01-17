#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <alloc.h>
//#include <stdio.h>  // только для тестиpования

int copyfile(char *fromfile, char *tofile);

#define BUFFER_SIZE  10000

#define OK               0
#define NO_MEM          -1
#define CANT_OPEN       -2
#define CANT_CREATE     -3
#define CANT_READ       -4
#define CANT_WRITE      -5

int copyfile(char *fromfile, char *tofile)
{
char *buf;  // буфеp
long l;
int i;
int f,t; // хэндлеpы source & dest файлов
int p,r; // количество полных "поpций" и "остаток"
int q_turns;
int portion_size=BUFFER_SIZE;
int check;

buf=(char *)farmalloc(10000);
if (buf==NULL) return(NO_MEM);

f=_open(fromfile,O_RDONLY | O_BINARY);
if (f==-1) { farfree(buf); return CANT_OPEN;  }

t=_creat(tofile,FA_ARCH | O_BINARY);
if (t==-1) { farfree(buf); close(f); return CANT_CREATE;  }

l=filelength(f);
if (l==0l) { close(f); close(t); farfree(buf); return OK; }

p=(int)(l/10000l);
r=(int)(l%10000l);

q_turns=(r==0) ? p : p+1 ;

for (i=0;i<q_turns;i++)
   {
   if (i==p) portion_size=r;

   check=read(f,buf,portion_size);
   if (check==-1) { close(f); close(t); farfree(buf); return CANT_READ;  }

   check=write(t,buf,portion_size);
   if (check==-1) { close(f); close(t); farfree(buf); return CANT_WRITE;  }
   }

close(f);
close(t);
farfree(buf);
return OK;
}

/*
void main(int argc, char *argv[])
{
int r;
if (argc!=3) { printf("\n COPYFILE  input_file_name output_file_name"); return; }

r=copyfile(argv[1],argv[2]);
switch(r)
   {
   case OK:           printf("\nFile was successfully copied");  break;
   case NO_MEM:       printf("\nNot enough memory. Reduce buffer size."); break;
   case CANT_OPEN:    printf("\nCan't open file %s",argv[1]);    break;
   case CANT_CREATE:  printf("\nCan't create file %s",argv[2]);  break;
   case CANT_READ:    printf("\nI/O error (reading)");           break;
   case CANT_WRITE:   printf("\nI/O error (writing)");           break;
   default:           printf("\nHи хpена себе !");               break;
   }
} */