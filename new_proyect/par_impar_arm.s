// alternar.s

.global alternar

.text
alternar:
    PUSH {R4, LR}                  // Salva registros

    LDR R0, =prompt_str
    BL printf                      // Imprime texto

    MOV R4, #0x55                  // Patrón de arranque

loop:
    MOV R0, R4
    BL prenderLEDs
    BL espera_tecla
    CMP R0, #0
    BEQ exit

    EOR R4, R4, #0xFF              // Invierte los bits
    B loop

exit:
    POP {R4, PC}                   // Restaurar y terminar

.data
prompt_str: .asciz "Mostrando secuencia Par e Impar...\n"
