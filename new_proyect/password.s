


org 100h

MAX_INTENTOS  equ 3
LARGO_PASS    equ 5

section .data
password      db '12345'
mensaje1      db 'Ingrese su password de 5 digitos:', 13,10, '$'
mensajeOK     db 13,10, 'Bienvenido al Sistema', 13,10, '$'
mensajeNO     db 13,10, 'Password no valida', 13,10, '$'
mensajeFallo  db 13,10, 'Demasiados intentos. Abortando...', 13,10, '$'
buffer        db 5 dup(0)
intentos      db 0

section .text
start:
    mov byte [intentos], 0

intento:
    mov ah, 9
    mov dx, mensaje1
    int 21h

    mov cx, LARGO_PASS
    mov si, 0

leer_caracter:
    mov ah, 7
    int 21h
    mov [buffer+si], al

    mov dl, '*'
    mov ah, 2
    int 21h

    inc si
    loop leer_caracter


    mov cx, LARGO_PASS
    mov si, 0
    mov di, 0

comparar:
    mov al, [buffer+si]
    mov bl, [password+si]
    cmp al, bl
    jne password_invalida
    inc si
    loop comparar


    mov ah, 9
    mov dx, mensajeOK
    int 21h
    jmp salir

password_invalida:
    mov ah, 9
    mov dx, mensajeNO
    int 21h

    inc byte [intentos]
    cmp byte [intentos], MAX_INTENTOS
    jl intento

    mov ah, 9
    mov dx, mensajeFallo
    int 21h

salir:
    mov ah, 4Ch
    int 21h
