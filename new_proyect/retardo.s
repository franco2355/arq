; retardo.s
; Funcion de retardo: while(a) a--;

section .text
    global retardo

retardo:
    ; Recibe 'a' en AX (puedes cambiar por CX si prefieres)
    mov cx, ax        ; CX = a
.loop:
    loop .loop        ; Decrementa CX hasta 0
    ret
