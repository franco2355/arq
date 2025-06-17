// alternar.s

.global alternar              @ Símbolo exportado

.text
alternar:
    PUSH {R4, LR}                  @ Guardar R4 y LR

    LDR R0, =prompt_str            @ Dirección del mensaje
    BL printf                      @ Mostrar texto

    MOV R4, #0x55                  @ Patrón de arranque

loop:
    MOV R0, R4                     @ Valor a mostrar
    BL prenderLEDs                 @ Encender LEDs
    BL espera_tecla                @ Esperar posible salida
    CMP R0, #0                     @ ¿Se presionó 'a'?
    BEQ exit                       @ Terminar si es así

    EOR R4, R4, #0xFF              @ Invertir el patrón
    B loop                         @ Repetir

exit:
    POP {R4, PC}                   @ Restaurar R4 y regresar

.data
prompt_str: .asciz "Mostrando secuencia Par e Impar...\n"
