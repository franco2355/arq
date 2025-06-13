// giro_auto.s
// Simula el patrón de luces secuenciales tipo intermitente moderno

.global giro_auto

.text
giro_auto:
    PUSH {R4, R5, R6, LR}              // Guardar registros

    LDR R0, =prompt_str
    BL printf                          // Mostrar mensaje

    MOV R4, #0x80                      // Posición inicial (bit más a la izquierda)
    MOV R5, #0                         // Acumulador = 0

giro_loop:
    ORR R5, R5, R4                     // Acumulador |= posición
    MOV R0, R5
    BL prenderLEDs                     // Encender LEDs según patrón acumulado

    BL delay_con_teclado              // Esperar con detección de teclado
    CMP R0, #0
    BEQ giro_exit

    LSR R4, R4, #1                     // Desplazar posición a la derecha
    CMP R4, #0
    BNE giro_loop                      // Repetir si aún hay bits a encender

    // Mostrar patrón completo
    MOV R0, R5
    BL prenderLEDs
    BL delay_con_teclado
    CMP R0, #0
    BEQ giro_exit

    // Apagar LEDs
    MOV R0, #0x00
    BL prenderLEDs
    BL delay_con_teclado
    CMP R0, #0
    BEQ giro_exit

    // Reiniciar variables
    MOV R4, #0x80
    MOV R5, #0
    B giro_loop

giro_exit:
    POP {R4, R5, R6, PC}               // Restaurar registros y regresar

.data
prompt_str: .asciz "Mostrando Giño de Auto Secuencial:\n"
