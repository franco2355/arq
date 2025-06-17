// guinio.s
// Muestra un giro secuencial al estilo de los autos actuales

.global guinio

.text
guinio:
    PUSH {R4, R5, R6, LR}              @ Guardar registros usados

    LDR R0, =prompt_str                @ Dirección del texto de aviso
    BL printf                          @ Llamada a printf

    MOV R4, #0x80                      @ Posición inicial del bit
    MOV R5, #0                         @ Valor acumulado de LEDs

giro_loop:
    ORR R5, R5, R4                     @ Activar LED indicado
    MOV R0, R5                         @ Argumento para prenderLEDs
    BL prenderLEDs                     @ Llamar a función de encendido

    BL espera_tecla                   @ Pausa con consulta de teclado
    CMP R0, #0                         @ ¿Presionó 'a'?
    BEQ giro_exit                      @ Salir si se solicitó

    LSR R4, R4, #1                     @ Mover bit a la derecha
    CMP R4, #0                         @ ¿Quedan bits por mostrar?
    BNE giro_loop                      @ Repetir si es así

    @ Al final se encienden todos los LEDs
    MOV R0, R5                         @ Patrón completo
    BL prenderLEDs                     @ Mostrar patrón
    BL espera_tecla                    @ Esperar otra tecla
    CMP R0, #0                         @ ¿Salir?
    BEQ giro_exit

    @ Luego se apagan todos los LEDs
    MOV R0, #0x00                      @ Patrón apagado
    BL prenderLEDs                     @ Apagar luces
    BL espera_tecla                    @ Esperar nuevamente
    CMP R0, #0
    BEQ giro_exit

    @ Reiniciar variables para repetir
    MOV R4, #0x80                      @ Reiniciar posición
    MOV R5, #0                         @ Limpiar acumulador
    B giro_loop                        @ Comenzar nuevamente

giro_exit:
    POP {R4, R5, R6, PC}               @ Recuperar registros y volver

.data
prompt_str: .asciz "Mostrando Giño de Auto Secuencial:\n"
