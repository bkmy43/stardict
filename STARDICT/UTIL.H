#define OK           0
#define CANT_CREATE  1
#define CANT_WRITE   2
#define CANT_OPEN    3
#define CANT_READ    4

unsigned long diskspace(int drive);
int lastfilechoise(char *spec, char *flnam);
char *Path2Name(char *path, char *name);
int CompareStrings(const char *s1, const char *s2, int length=10000, int case_sense=1);
int CompareSymbols(const char c1, const char c2);
int ftoupper(int c);
