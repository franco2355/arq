

.global alternar

.text
alternar:
    PUSH {R4, LR}

    LDR R0, =prompt_str
    BL printf

    MOV R4, #0x55

loop:
    MOV R0, R4
    BL prenderLEDs
    BL espera_tecla
    CMP R0, #0
    BEQ exit

    EOR R4, R4, #0xFF
    B loop

exit:
    POP {R4, PC}

.data
prompt_str: .asciz "Mostrando secuencia Par e Impar...\n"
