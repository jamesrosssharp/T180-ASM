
       
; Viktor T Toths multiplication program 
        ORG $0000

BEGIN:  CLF             ; Initialize
        AND #$0000       ; Load 0 to accumulator
        ST  A,$00FE       ; Set #FE to 0
        LD  A,$00FC       ; Load first argument
        AND #$000F       ; Take low nybble
        ST  A,$00FA       ; Store at working storage
        LD  A,$00FD       ; Load second argument
        AND #$000F       ; Take low nybble
        ST  A,ADDVAL + 1  ; Store as argument to ADD instruction
        LD  A,$00FA       ; Load first argument
        CLF             ;

GETBIT: ROR             ; Take one bit
        ST  A,$00FA       ; Store the rest
        JNC MULT        ; If not 1, no need to add
        LD  A,$00FE       ; Load result
        CLF             ;
ADDVAL: ADD #$0000       ; Add second argument (stored here)
        ST  A,$00FE       ; Store result
MULT:   LD  A,ADDVAL + 1  ; Load second argument
        CLF             ; Multiply by 2
        ROL             ;
        ST  A,ADDVAL + 1  ; Store
        LD  A,$00FA       ; Reload first
        CLF             ;
        ADD #$0000       ; Check for 0
        JNZ GETBIT      ; Repeat loop if necessary
        HLT             ; Result obtained
        JMP BEGIN       ; Restart


