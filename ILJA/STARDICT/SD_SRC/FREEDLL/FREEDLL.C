#include <toolhelp.h>
#include <windows.h>
//#include <string.h>

int PASCAL WinMain (HINSTANCE,HINSTANCE,LPSTR name,int)
{
  MODULEENTRY entry;
  entry.dwSize=sizeof(MODULEENTRY);

  ::AnsiUpper(name);

/*  ::ModuleFirst(&entry);
  int res = 1;
  while ( (res =strcmpi(entry.szModule, name)) != 0 && ::ModuleNext(&entry) );
  if (res != 0) return 0;*/

  if (ModuleFindName(&entry, name))
    for (int i=0;i<entry.wcUsage;i++) FreeModule(entry.hModule);

  return 0;
}

