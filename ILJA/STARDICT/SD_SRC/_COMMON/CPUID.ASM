TITLE TEST_CPU - Determine Intel Microprocessor Type
;
; MODULE NAME:     TEST_CPU.ASM
; AUTHOR:          Dan Wronski
; DATE:            12/02/94
; VERSION:         1.0
; LANGUAGE:        Turbo/MASM Assembly
; DESCRIPTION:     Determine Intel microprocessor type while running in
;                  8086 Real Address Mode or Virtual 8086 Mode.
; COPYRIGHT:       (C) Dan Wronski 1994
.MODEL large,C
.8086

FLAG_ID_MASK   equ 200000h
FLAG_AC_MASK   equ 40000h
FLAG_NT_MASK   equ 4000h

CPU_ID         MACRO
               db      0Fh                     ; New Pentium CPUID instruction
               db      0A2h
               ENDM

               .DATA

FPU_Status     dw      0
Text_Unknown   db      'Could not recognize the microprocessor in this system.',13,10,'$'
Text_FPU       db      'This system also has a numeric coprocessor.',13,10,'$'
Text_CPU_Start db      'This system has an Intel ',0

Text_8088      db      '8088',0
Text_8086      db      '8086',0
Text_80188     db      '80188',0
Text_80186     db      '80186',0
Text_80286     db      '80286',0
Text_386DX     db      '386DX',0
Text_386SX     db      '386SX',0
Text_486DX     db      '486DX',0
Text_486SX     db      '486SX',0
Text_Old_End   db      ' or compatible microprocessor.',13,10,'$',0
Text_Intel     db      'genuine ',0
Text_Clone     db      'compatible ',0

Text_New_Type  db      'microprocessor:',13,10,'  family='
Text_Family    db      'xh, model='
Text_Model     db      'xh, stepping='
Text_Stepping  db      'xh'
Text_Pentium   db      ' (Pentium)',0
Text_New_End   db      '.',13,10,'$',0

Text_Buffer    db      250 dup (?)             ; Max of three 80-byte lines

.CODE

GetID          proc    C
               mov     ax,@data
               mov     ds,ax                   ; Set DS to our data segment
               mov     es,ax                   ; And ES also
               cld                             ; Clear string direction


; Check if the microprocessor is one of the "old" ones prior to Pentium
;

               lea     di,Text_Buffer          ; Set up initial CPU string
               lea     si,Text_CPU_Start
               call    Copy_String

               call    Check_8086
               jnz     Print_Old               ; Print if 8088/8086/80188/80186

               call    Check_80286
               jnz     Print_Old               ; Print if 80286

               call    Check_80386
               jnz     Print_Old               ; Print if 386DX or 386SX

               call    Check_80486
               jnz     Print_Old               ; Print if 486DX or 486SX
;
; Now we have a "new" microprocessor that supports the CPUID instruction
;

               call    Check_Pentium
               jnz     Print_New               ; Print if we know what it is

               jmp     Print_FPU

Print_New:
               call    Copy_String             ; Copy genuine/compatible text
               lea     si,Text_New_Type        ; Copy cpu type to buffer

               call    Copy_String
               lea     si,Text_New_End         ; Add ending to text string
               call    Copy_String
               jmp     Print_CPU
Print_Old:
               call    Copy_String             ; Copy cpu type to buffer
               lea     si,Text_Old_End         ; Add ending to text string
               call    Copy_String

Print_CPU:
Print_FPU:
               call    Check_Coprocessor       ; Coprocessor in system?
               jnz     Done                    ; No,  we are finish
Done:
               ret

               endp



;-----------------------------------------------------------------------------

; Copy_String - Copy one source string to a target buffer. This routine

;               requires the string direction flag updated via CLD, and

;               the end-of-string character in the source string to be zero.

;               Also this routine does no limit checking on string length.

;   Inputs

;     SI = Source string address

;     DI = Target buffer address

;

;   Outputs

;     AX = Undefined

;     SI = Undefined

;     DI = Address of end-of-string character

;-----------------------------------------------------------------------------

Copy_String    proc

               lodsb                           ; Get next source character

               stosb                           ; Store in target buffer

               or      al,al                   ; Reached end of string?

               jnz     Copy_String             ; No,  get next character

               dec     di                      ; Yes, point to end of string

               ret

Copy_String    endp



;-----------------------------------------------------------------------------

; Check_Prefetch  - Check to see if we can modify an instruction at a specific

;                   displacement after a jump. We can not modify displacement 0

;                   and we assume we can always modify after displacement 64.

;   Inputs

;     AX = instruction displacement (limited to 0 - 64)

;

;   Outputs

;     AX = 0 if could not modify instruction else undefined (ZF matches results)

;-----------------------------------------------------------------------------

Check_Prefetch proc

               push    es                      ; Save registers

               push    si

               push    di

               push    dx

               push    cx

               push    bx



               mov     dx,ax                   ; Save displacement

               or      ax,ax                   ; Zero displacement?

               jz      Finish_Test             ; Yes, no need to test

               cmp     ax,64                   ; Displacement too big?

               ja      Finish_Test             ; Yes, no need to test

;

; We need to align the jump target address on a 16-byte boundary

;

               mov     ax,cs

               mov     es,ax                   ; Load ES with code segment

               lea     bx,cs:Test_Code         ; Get initial target address

               mov     di,bx

               mov     cx,80

               mov     al,90h                  ; Keep code reusable by filling

               rep     stosb                   ;   target area with nop's

               add     bx,15                   ;   Also ensure 4K page is loaded

               and     bx,0FFF0h               ; Align target to 16-byte

               mov     di,bx

               mov     al,0AAh                 ; Get "stosb" instruction

               stosb                           ; Place it at 0 displacement

               dec     di                      ; Restore 0 displacement offset

               add     di,dx                   ; Point to desired displacement

               mov     al,40h                  ; Get "inc ax" op-code to store

               cli                             ; Disable interrupts

               jmp     bx                      ; Flush prefetch queue

Test_Code:

               db      80 dup (90h)            ; string of "nop" instructions

               sti                             ; Restore interrupts

               cmp     al,40h                  ; Modified instruction?

               jne     Finish_Test             ; Yes

               xor     ax,ax                   ; No

Finish_Test:

               pop     bx                      ; Restore registers

               pop     cx

               pop     dx

               pop     di

               pop     si

               pop     es

               or      ax,ax                   ; Set SF to match results

               ret

Check_Prefetch endp



;-----------------------------------------------------------------------------

; Check_Coprocessor - Determine if a numeric coprocessor is in the system

;                     by trying to retrieve the hardware status.

;   Inputs

;     None

;

;   Outputs

;     AX = 0 if coprocessor exists else undefined (ZF matches results)

;-----------------------------------------------------------------------------

Check_Coprocessor proc

               mov     FPU_Status,0FFFFh       ; Reset floating point status

               fninit                          ; Reset floating point hardware

               fnstsw  FPU_Status              ; Get floating point h/w status

               mov     ax,FPU_Status           ; FPU_Status = 0 if h/w exists

               or      ax,ax                   ; Set ZF to match results

               ret

Check_Coprocessor endp



;-----------------------------------------------------------------------------

; Check_XXXXX - Determine if the CPU type is a specific microprocessor.

;               If a match is found then load SI with the CPU text string,

;               otherwise load zero into SI. ZF matches SI contents.

;   Inputs

;     None

;

;   Outputs

;     AX = Undefined

;     SI = Address of CPU text string or zero (ZF matches results)

;-----------------------------------------------------------------------------

Check_8086     proc

               push    di                      ; Save registers

               push    cx

               xor     si,si                   ; Assume no match

;

; First test is whether we can reset the NT flag

;

               pushf                           ; Save original flags

               cli                             ; Disable interrupts

               pushf                           ; Get current flags

               pop     ax

               or      ax,FLAG_NT_MASK         ; Clear only NT flag

               xor     ax,FLAG_NT_MASK

               push    ax

               popf                            ; Update flags

               pushf

               pop     ax                      ; Get "updated" flags

               popf                            ; Restore original flags

                                               ; (and restore interrupts)

               test    ax,FLAG_NT_MASK         ; Changed NT flag?

               jz      Finish_8086             ; Yes, not 8088/8086/80188/80186

               lea     si,Text_8088            ; No, assume we have an 8088

               lea     di,Text_8086            ; Second choice would be 8086

;

; Test whether it's an 8088/8086 or 80188/80186. The later will mask shift

; counts with 1Fh before shifting.

;

               mov     cl,21h                  ; Can't use 20h or ZF may

               mov     al,1                    ; not get set correctly

               shl     al,cl                   ; Is it an 8088/8086?

               jz      Check_8_Bit_Bus         ; Yes

               lea     si,Text_80188           ; No, assume we have an 80188

               lea     di,Text_80186           ; Second choice would be 80186

;

; Now we need to determine whether it's 8088/80188 (8-bit data bus) or

; 8086/80186 (16-bit data bus). Test whether the microprocessor has the

; 8-bit or 16-bit prefetcher unit by modifying an instruction after a jump.

;

Check_8_Bit_Bus:

               mov     ax,3                    ; Modify displacment 3

               call    Check_Prefetch          ; Did we modify instruction?

               jnz     Finish_8086             ; Yes, use first choice (8-bit)

               mov     si,di                   ; No,  use second choice (16-bit)

Finish_8086:

               pop     cx                      ; Restore registers

               pop     di

               or      si,si                   ; Set ZF to match results

               ret

Check_8086     endp



Check_80286    proc

               xor     si,si                   ; Assume no match

;

; Test whether we can set the NT flag (always zero in Real Mode on 80286)

;

               pushf                           ; Save original flags

               cli                             ; Disable interrupts

               pushf                           ; Get current flags

               pop     ax

               or      ax,FLAG_NT_MASK         ; Set NT flag

               push    ax

               popf                            ; Update flags

               pushf

               pop     ax                      ; Get "updated" flags

               popf                            ; Restore original flags

                                               ; (and restore interrupts)

               test    ax,FLAG_NT_MASK         ; Changed NT flag?

               jnz     Finish_80286            ; Yes, not 80286

               lea     si,Text_80286           ; No,  must be 80286

Finish_80286:

               or      si,si                   ; Set ZF to match results

               ret

Check_80286    endp



;**********************************************************************

; From here to the end of this module is mixed 16-bit and 32-bit code

;**********************************************************************



               .386

Check_80386    proc

               xor     si,si                   ; Assume no match

;

; Test whether we can set the AC flag (always zero on 80386), but we need

; to make sure the stack is properly aligned to avoid an exception on

; 80486 or higher microprocessor.

;

               push    bp                      ; Save register

               mov     bp,sp                   ; Save current stack pointer

               and     sp,0FFFCh               ; Align stack to 32-bit

               push    eax                     ; Save 32-bit register



               pushfd                          ; Save original 32-bit flags

               cli                             ; Disable interrupts

               pushfd                          ; Get current flags

               pop     eax

               or      eax,FLAG_AC_MASK        ; Set AC flag

               push    eax

               popfd                           ; Update flags

               pushfd

               pop     eax                     ; Get "updated" flags

               popfd                           ; Restore original 32-bit flags

                                               ; (and restore interrupts)

               test    eax,FLAG_AC_MASK        ; AC flag still set?

               jnz     short Finish_80386      ; Yes, not an 80386

;

; Now we need to determine whether it's 386SX (16-bit data bus) or

; 386DX (32-bit data bus). It is possible the system could have a

; 16-bit data bus and still have an 386DX (it supports a 16-bit data bus)

; but it's very unlikely because the system performance would be slightly

; worse than having a 386SX. So test whether the microprocessor has the

; 16-bit or 32-bit prefetcher unit by modifying an instruction after a jump.

;

               lea     si,Text_386DX           ; Assume 386DX

               mov     ax,11                   ; Modify displacment 11

               call    Check_Prefetch          ; Did we modify instruction?

               jz      short Finish_80386      ; No,  must have 32-bit bus

               lea     si,Text_386SX           ; Yes, has to be 386SX

                                               ; (or might as well be)

Finish_80386:

               pop     eax                     ; Restore 32-bit register

               mov     sp,bp                   ; Restore stack pointer

               pop     bp                      ; Restore register

               or      si,si                   ; Set ZF to match results

               ret

Check_80386    endp



Check_80486    proc

               xor     si,si                   ; Assume no match

;

; Make sure the stack is aligned to a 32-bit boundary

;

               push    bp                      ; Save register

               mov     bp,sp                   ; Save current stack pointer

               and     sp,0FFFCh               ; Align stack to 32-bit

               push    ebx                     ; Save 32-bit registers

               push    eax

;

; Test whether the CPUID instruction is supported on this microprocessor by

; toggling the ID flag.

;

               pushfd                          ; Save original 32-bit flags

               cli                             ; Disable interrupts

               pushfd                          ; Get current flags

               pop     eax

               mov     ebx,eax

               and     ebx,FLAG_ID_MASK        ; Save ID flag

               xor     eax,FLAG_ID_MASK        ; Toggle ID flag

               push    eax

               popfd                           ; Update flags

               pushfd

               pop     eax                     ; Get "updated" flags

               popfd                           ; Restore original 32-bit flags

                                               ; (and restore interrupts)

               and     eax,FLAG_ID_MASK        ; Only want ID flag

               cmp     eax,ebx                 ; ID flag changed?

               jne     short Finish_80486      ; Yes, not an 80486

;

; The only difference between a 486DX and 486SX is whether the numeric

; coprocessor unit is present inside the chip. (The 486SX doesn't have one.)

; So if there is no coprocessor present it has to be a 486SX. But if one

; is present then it could be a 486SX with 487SX or a 486DX. Since installing

; a 487SX disables the 486SX, for all practical purposes a 487SX is a

; 486DX and will therefore be reported as a 486DX.

;

               lea     si,Text_486SX           ; Assume 486SX

               call    Check_Coprocessor       ; Coprocessor in system?

               jnz     short Finish_80486      ; No,  has to be 486SX

               lea     si,Text_486DX           ; Yes, treat as a 486DX

Finish_80486:

               pop     eax                     ; Restore 32-bit registers

               pop     ebx

               mov     sp,bp                   ; Restore stack pointer

               pop     bp                      ; Restore register

               or      si,si                   ; Set ZF to match results

               ret

Check_80486    endp



Check_Pentium  proc

               xor     si,si                   ; Assume no match

;

; Make sure the stack is aligned to a 32-bit boundary

;

               push    bp                      ; Save register

               mov     bp,sp                   ; Save current stack pointer

               and     sp,0FFFCh               ; Align stack to 32-bit

               push    edx                     ; Save 32-bit registers

               push    ecx

               push    ebx

               push    eax

;

; First determine whether we can get the family, model, and stepping level ID

;

               xor    eax,eax                  ; Get ID string

               CPU_ID

               cmp    eax,1                    ; Can we get the ID?

               jb     short Finish_Pentium     ; No, treat as unknown

;

; Check whether this is a "genuine" Intel microprocessor.

;

               lea    si,Text_Clone            ; Assume not genuine Intel

               cmp    ebx,756E6547h            ; 1st part match Intel ID?

               jne    short Check_Family       ; No

               cmp    edx,49656E69h            ; 2nd part match Intel ID?

               jne    short Check_Family       ; No

               cmp    ecx,6C65746Eh            ; 3rd part match Intel ID?

               jne    short Check_Family       ; No

               lea    si,Text_Intel            ; Yes, have the genuine part

;

; Get microprocessor family, model, and stepping level ID

;

Check_Family:

               mov     eax,1                   ; Get family, model, stepping ID

               CPU_ID

               mov     bx,ax                   ; Save family, model, stepping ID

               call    Convert_Hex                     ; Make stepping ID printable

               mov     Text_Stepping,al        ; Place it in text

               mov     al,bl

               shr     al,4                    ; Get model ID

               call    Convert_Hex                     ; Make it printable

               mov     Text_Model,al           ; Place it in text

               mov     al,bh                   ; Get family ID

               call    Convert_Hex                     ; Make it printable

               mov     Text_Family,al          ; Place it in text



               mov     Text_Pentium,' '        ; Make code reuseable

               cmp     al,'5'                  ; Is it a Pentium?

               je      short Finish_Pentium    ; Yes, we are finish

               mov     Text_Pentium,0          ; No,  leave out Pentium text

Finish_Pentium:

               pop     eax                     ; Restore 32-bit registers

               pop     ebx

               pop     ecx

               pop     edx

               mov     sp,bp                   ; Restore stack pointer

               pop     bp                      ; Restore register

               or      si,si                   ; Set ZF to match results

               ret

Check_Pentium  endp


Convert_Hex    proc near

               and     al,0Fh                  ; Only want low nibble

               or      al,30h                  ; Assume 0-9

               cmp     al,39H                  ; Is it?

               jbe     short Skip              ; Yes

               add     al,7                    ; No, adjust for A-F
Skip:
               ret

               endp


               end Main

