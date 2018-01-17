;********************************************************************
; _FORMAT.ASM : Форматирование указанной дорожки дискеты.
; и еще несколько операций с дискетой.
;
;********************************************************************

.model LARGE, C
.386

.data

NUM_SECT        equ   17

TIoctlType     struc

rSpclFunc      db    0
bDevType       db    0
rDevAttr       dw    0
wCylCnt        dw    0
bDevMedia      db    0
rDevBPD        db    19h dup(0)
rTrkLayout     db    20*4+2 dup(0)

ends

IoctlFormat     db    0;reserved
                dw    0;head
                dw    0;track
                db    5 dup ('0')

IoctlType       label   TIoctlType
                db      size TIoctlType dup (99h)


           extrn    sign:word, buf_512:word

.code

            public   AsmFormatTrack, AsmWriteSignature, AsmReadSignature

AsmReadSignature        proc    C
        uses    es
        arg     drive:word, track:word, head: word

        mov     ah, 0
        int     13h

        mov     ah, 2h      ; read
        mov     al, 1       ; 1 sector
        mov     dl, byte ptr drive
        mov     dh, byte ptr head

        mov     cx, track
        xchg    ch, cl
        shl     cl, 6
        and     cl, 0C0h
;        inc     cl

        push    @data
        pop     es
        lea     bx, es:buf_512    ; buffer

        int     13h
        jc      @3
        mov     ax, 0
        jmp     @4
@3:
        mov     ax, 1
@4:
        ret
endp

AsmFormatTrack    proc     C
        uses    es

        arg     drive:word, track:word, head: word

; setting up the type of diskette:
        mov     ax, 440dh
        mov     bx, drive
        inc     bx
        mov     cl, 60h
        mov     ch, 08h
        lea     dx, IoctlType
        mov     byte ptr IoctlType, 01h
        int     21h
        jc      @1

        push    @data
        pop     ds
        lea     bx, IoctlType
        mov     [bx].wCylCnt, 80; !!!!!!!!!!!!!!!!!!!!
        lea     bx, [bx].rTrkLayout
        mov     word ptr [bx], NUM_SECT
        add     bx, 2

        mov     cx, NUM_SECT
        xor     ax, ax
        inc     ax
@33:
        mov     [bx], ax
        mov     word ptr [bx+2], 512
        add     bx, 4
        inc     ax
        loop    @33


        mov     ax, 440dh
        mov     bx, drive
        inc     bx
        mov     cl, 40h
        mov     ch, 08h
        lea     dx, IoctlType
        int     21h

        jc      @1


; formatting
        lea     bx, IoctlFormat
        mov     byte ptr [bx], 0
        mov     ax, head
        mov     [bx+1], ax
        mov     ax, track
        mov     [bx+3], ax

        mov     ax, 440dh
        mov     bx, drive
        inc     bx
        mov     cl, 42h
        mov     ch, 08h
        lea     dx, IoctlFormat 
        int     21h

        jc      @1
        xor     ax, ax
        jmp     @2
@1:
        mov     ax, ax
@2:
        ret
endp

AsmWriteSignature        proc    C
        uses    es, ds
        arg     drive:word, track:word, head: word

        mov     ah, 0
        int     13h

        mov     ah, 3h    ; write
        mov     al, 1       ; 1 sector
        mov     dl, byte ptr drive
        mov     dh, byte ptr head

        mov     cx, track
        xchg    ch, cl
        shl     cl, 6
        and     cl, 0C0h
;        inc     cl

        push    @data
        pop     es
        lea     bx, es:sign    ; buffer

        int     13h
        jc      @7

        mov     ax, 0
        jmp     @8
@7:
        mov     ax, 1
@8:
        ret
endp


        end

