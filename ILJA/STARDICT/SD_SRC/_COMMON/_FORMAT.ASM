;********************************************************************
; _FORMAT.ASM : Форматирование указанной дорожки дискеты.
; и еще несколько операций с дискетой.
;
;********************************************************************

.model LARGE,C
.8086

.data

NUM_SECT        equ   18

SectorsDescr    dw    NUM_SECT*4 dup(0)

           extrn    sign:word, buf_512:word

.code

            public   AsmFormatTrack, AsmWriteSignature, AsmReadSignature

AsmReadSignature        proc    C
        uses    es, ds
        arg     drive:word, track:word, head: word

;        mov     ah, 0
;        int     13h

        mov     ax, @data
        mov     ds, ax
        mov     es, ax

        mov     ah, 2h      ; read
        mov     al, 1       ; 1 sector
        mov     dl, byte ptr drive
        mov     dh, byte ptr head

        mov     cx, track
        xchg    ch, cl
        shl     cl, 6
        and     cl, 0C0h
        or      cl, 1

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
        uses    es, ds

        arg     drive:word, track:word, head: word

        mov     ax, @data
        mov     es, ax
        mov     ds, ax
        mov     cx, NUM_SECT
        lea     bx, SectorsDescr
        mov     al, 1
@22:
        mov     dl, byte ptr track
        mov     [bx], dl
        mov     dl, byte ptr head
        mov     [bx+1], dl
        mov     [bx+2], al
        mov     byte ptr [bx+3], 2
        inc     al
        add     bx, 4
        loop    @22

        mov     ah, 05h
        mov     dl, byte ptr drive
        mov     dh, byte ptr head
        mov     cx, track
        xchg    ch, cl
        shl     cl, 6
        push    @data
        pop     es
        lea     bx, SectorsDescr

        int     13h

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

;        mov     ah, 0
;        int     13h
        mov     ax, @data
        mov     ds, ax
        mov     es, ax

        mov     ah, 3h    ; write
        mov     al, 1       ; 1 sector
        mov     dl, byte ptr drive
        mov     dh, byte ptr head

        mov     cx, track
        xchg    ch, cl
        shl     cl, 6
        and     cl, 0C0h
        or      cl, 1

        lea     bx, es:sign    ; buffer

        int     13h
        jc      @7

        mov     ax, 0
        jmp     @8
@7:
        mov     ax, ax
@8:
        ret
endp


        end

