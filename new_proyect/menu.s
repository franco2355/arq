
.global procesarOpcion
.text

procesarOpcion:
    
    
    
    CMP R0, #1
    BEQ opcion1
    
    CMP R0, #2
    BEQ opcion2
    
    CMP R0, #3
    BEQ opcion3
    
    CMP R0, #4
    BEQ opcion4
    
    
    B opcion_default
    
opcion1:
    
    MOV R0, #10
    B fin_switch
    
opcion2:
    
    MOV R0, #20
    B fin_switch
    
opcion3:
    
    MOV R0, #30
    B fin_switch
    
opcion4:
    
    MOV R0, #40
    B fin_switch
    
opcion_default:
    
    MOV R0, #0
    
fin_switch:
    
    BX LR
