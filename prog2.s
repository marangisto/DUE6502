stdout  .equ    $ff00   ; output channel

start   .org    $0000  ; start at zero
        ldx     #0
loop    lda     text,x
        jsr     chrout    
        inx
        cpx     #12
        bne     loop 
stop    jmp stop
text    .byte  "HELLO WORLD!"

chrout  sta stdout
        rts
 
