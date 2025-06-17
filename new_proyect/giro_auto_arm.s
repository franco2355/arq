


.global guinio

.text
guinio:
    PUSH {R4, R5, R6, LR}

    LDR R0, =prompt_str
    BL printf

    MOV R4, #0x80
    MOV R5, #0

giro_loop:
    ORR R5, R5, R4
    MOV R0, R5
    BL prenderLEDs

    BL espera_tecla
    CMP R0, #0
    BEQ giro_exit

    LSR R4, R4, #1
    CMP R4, #0
    BNE giro_loop


    MOV R0, R5
    BL prenderLEDs
    BL espera_tecla
    CMP R0, #0
    BEQ giro_exit


    MOV R0, #0x00
    BL prenderLEDs
    BL espera_tecla
    CMP R0, #0
    BEQ giro_exit


    MOV R4, #0x80
    MOV R5, #0
    B giro_loop

giro_exit:
    POP {R4, R5, R6, PC}

.data
prompt_str: .asciz "Mostrando Giño de Auto Secuencial:\n"
