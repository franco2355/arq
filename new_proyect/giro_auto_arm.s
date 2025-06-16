// guinio.s
// Muestra un giro secuencial al estilo de los autos actuales

.global guinio

.text
guinio:
    PUSH {R4, R5, R6, LR}              // Guardar estado

    LDR R0, =prompt_str
    BL printf                          // Imprimir texto

    MOV R4, #0x80                      // Comienza en el bit más alto
    MOV R5, #0                         // Inicializa acumulador

giro_loop:
    ORR R5, R5, R4                     // Acumula la posición
    MOV R0, R5
    BL prenderLEDs                     // Enciende LEDs con el valor acumulado

    BL espera_tecla                   // Pausa y revisa el teclado
    CMP R0, #0
    BEQ giro_exit

    LSR R4, R4, #1                     // Mueve la posición a la derecha
    CMP R4, #0
    BNE giro_loop                      // Continúa mientras queden bits

    // Al final se encienden todos
    MOV R0, R5
    BL prenderLEDs
    BL espera_tecla
    CMP R0, #0
    BEQ giro_exit

    // Luego se apagan
    MOV R0, #0x00
    BL prenderLEDs
    BL espera_tecla
    CMP R0, #0
    BEQ giro_exit

    // Vuelve al inicio
    MOV R4, #0x80
    MOV R5, #0
    B giro_loop

giro_exit:
    POP {R4, R5, R6, PC}               // Restaurar y salir

.data
prompt_str: .asciz "Mostrando Giño de Auto Secuencial:\n"
