/* **************************************************************************
                             CODE.H
    Описание функции кодирования строчки.
    (c) В. Перов.
************************************************************************** */

#if !defined (_CODE_H)
#define _CODE_H

#define CODE_MIN_STRINGLEN      5

#define DEFAULT_XOR_VALUE       0xC3

void CodeString(char* dest, char* src, int size, int numSyncBits);

inline void CodeUserName(char* dest, char* src, int size)
   { CodeString(dest, src, size, 2); }
inline void CodeRegNum(char* dest, char* src, int size)
   { CodeString(dest, src, size, 3); }

void CodeStringAdvanced(char* dest, char* src, int size);

unsigned CountCRCword(unsigned char*, int bytes);

void CodeStringShifting(char* dest, char* src, int size, unsigned char xorValue=DEFAULT_XOR_VALUE);
void UnCodeStringShifting(char* dest, char* src, int size, unsigned char xorValue=DEFAULT_XOR_VALUE);

#endif
