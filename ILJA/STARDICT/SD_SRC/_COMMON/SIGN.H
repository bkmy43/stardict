/* ***************************************************************************
                                SIGN.H
    Описание стуктур подписи на дискете дистрибутива StarDict.
    (c) В. Перов.
*************************************************************************** */

#if !defined(_SIGN_H)
#define _SIGN_H

// track enumerate from 0, so the last available track+1 = 80
// sector enumerate from 1

#define    TRACK_NUMBER_first        80
#define    SECTOR_NUMBER_first        1
#define    HEAD_NUMBER_first          0

#define    TRACK_NUMBER_boot          0
#define    SECTOR_NUMBER_boot         1
#define    OFFSET_boot                (512-sizeof(TSignature)-1)

struct TSignature {

    char        chProgram[2];   // "SD"
    unsigned    CRC;
    int         progVersion;    // 0x0002 = v2.00
    int         dataVersion;
    int         numInst;
    char        RegNum[30];
    char        RegNumCoded[30];
};

#endif // of _SIGN_H
