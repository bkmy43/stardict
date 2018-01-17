/* ***************************************************************************
                            CODING.C
    одирование и раскодирование строк текста следующим методом:
        добавлени€ к байту его индекса в строке;
        цикличексий сдвиг байта на два бита;
        сдвига байта
   вставки синхробитов. :)
    одирование: сдвиг вправо, раскодирование: сдвиг влево,
   сдвиг на 2 бита.
   ‘ункции:
        CodeString,
        DecodeString.
   (c) ¬. ѕеров.
*************************************************************************** */

#include <string.h>

#include "ex.h"
#include "code.h"

typedef unsigned char UBYTE;

static UBYTE* result = 0;

// *************************************************************************
void UnCodeStringShifting(char* dest, char* src, int size, unsigned char xorVal)
{
    unsigned char a;
    for (int i=0; i<size; i++, src++, dest++) {
        a = *src;
        asm {
            mov   al, a
            xor   al, xorVal
            ror   al, 1
            ror   al, 1
            mov   a, al
        }
        *dest = a;
    }
}
void CodeStringShifting(char* dest, char* src, int size, unsigned char xorVal)
{
    unsigned char a;
    for (int i=0; i<size; i++, src++, dest++) {
        a = *src;
        asm {
            mov   al, a
            rol   al, 1
            rol   al, 1
            xor   al, xorVal
            mov   a, al
        }
        *dest = a;
    }
}
unsigned CountCRCword(unsigned char* src, int bytes)
{
    unsigned res = 0;
    bytes &= 0xFFFE;
    for (int i=0; i<bytes; i+=2, src += 2)
        res += (unsigned int) *src;

    return res;
}
static void CopyByteToResult(UBYTE byte, int& index)
{
    if (result == 0) return;
    UBYTE mask = 0x80;
    for (int i=0; i<8; ++i) {
        result[index++] = ( (byte & mask) != 0 );
        mask >>= 1;
    }
}

void CodeString(char* dest, char* src, int size, int numSyncBits)
{
    if (strlen(src) < CODE_MIN_STRINGLEN)
        throw TSTDError("", __LINE__, EC_WRONGREGINFO, 0);
    int resLen = size*(8+numSyncBits)+2;
    result = new UBYTE[resLen];
    int index = 0;
    while (*src && index<resLen) {
        UBYTE cbyte = *src++;
        cbyte += index;
        asm {
            mov     al, cbyte
            ror     al, 1
            ror     al, 1
            mov     cbyte, al
        }
        CopyByteToResult(cbyte, index);
        for (int i=0; i<numSyncBits; ++i)
            result[index++] = 1;
    }
    int dist = 0;
    for (int i=0; i<index && dist<size-1;) {
        UBYTE mask = 0x80;
        *dest = '\0';
        for (int j=0; j<8 && i<index; ++j, ++i) {
            if (result[i]) *dest |= mask;
                      else *dest &= ~mask;
            mask >>= 1;
        }
        if ((unsigned char)*dest>127) *dest &= 0x7F;
        if ((unsigned char)*dest == '\0') *dest = *(dest-1);
        while (*dest != '\0' && (unsigned char)*dest<33) *dest <<=1;
        ++dest; ++dist;
    }

    *dest = '\0';
    if (result) delete[] result;
}

void CodeStringAdvanced(char* dest, char* src, int size)
{
    if (strlen(src) < CODE_MIN_STRINGLEN)
        throw TSTDError("", __LINE__, EC_WRONGREGINFO, 0);

    char* cp = src;
    int sum = strlen(src);

    while (*cp) sum += (unsigned char)*cp++;
    cp = src;
    while (*cp) *cp++ ^= (unsigned char)sum;

    CodeString(dest, src, size, 2);
}
