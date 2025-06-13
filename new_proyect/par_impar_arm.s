// alternar.s

.global alternar

.text
alternar:
    PUSH {R4, LR}                  // Guardar registros

    LDR R0, =prompt_str
    BL printf                      // Mostrar mensaje

    MOV R4, #0x55                  // Patrón inicial

loop:
    MOV R0, R4
    BL prenderLEDs
    BL espera_tecla
    CMP R0, #0
    BEQ exit

    EOR R4, R4, #0xFF              // Alternar patrón
    B loop

exit:
    POP {R4, PC}                   // Restaurar y regresar

.data
prompt_str: .asciz "Mostrando secuencia Par e Impar...\n"
