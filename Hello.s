UART_SEND_CHR = $EFF0               ; DUE hook for transmit

        .org    $f000  ; start at rom
start   ldx     #0
loop    lda     text,x
        jsr     chrout    
        inx
        cpx     #12
        bne     loop 
stop    jmp stop
text    .byte  "HELLO WORLD!"

chrout  sta UART_SEND_CHR
        rts
 
        .ORG    $FFFC   ; reset vector
        .word   start 
