/* menu.s - Implementación del switch-case en ARM */
.global procesarOpcion
.text

procesarOpcion:
    /* R0 contiene el número de opción del menú */
    
    /* Comparamos R0 con cada caso posible */
    CMP R0, #1              @ Comparamos con la opción 1
    BEQ opcion1             @ Si es igual, saltamos a opcion1
    
    CMP R0, #2              @ Comparamos con la opción 2
    BEQ opcion2             @ Si es igual, saltamos a opcion2
    
    CMP R0, #3              @ Comparamos con la opción 3
    BEQ opcion3             @ Si es igual, saltamos a opcion3
    
    CMP R0, #4              @ Comparamos con la opción 4
    BEQ opcion4             @ Si es igual, saltamos a opcion4
    
    /* Si llegamos aquí, ninguna opción coincidió (equivalente a default) */
    B opcion_default
    
opcion1:
    /* Código para la opción 1 */
    MOV R0, #10             @ Devolver 10 como resultado
    B fin_switch            @ Salir del switch
    
opcion2:
    /* Código para la opción 2 */
    MOV R0, #20             @ Devolver 20 como resultado
    B fin_switch            @ Salir del switch
    
opcion3:
    /* Código para la opción 3 */
    MOV R0, #30             @ Devolver 30 como resultado
    B fin_switch            @ Salir del switch
    
opcion4:
    /* Código para la opción 4 */
    MOV R0, #40             @ Devolver 40 como resultado
    B fin_switch            @ Salir del switch
    
opcion_default:
    /* Código para el caso default */
    MOV R0, #0              @ Devolver 0 para indicar opción inválida
    
fin_switch:
    /* Retornar al código C con el resultado en R0 */
    BX LR 