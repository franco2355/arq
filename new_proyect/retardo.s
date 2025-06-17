


section .text
    global retardo

retardo:

    mov cx, ax
.loop:
    loop .loop
    ret
