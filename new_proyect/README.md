# Implementación de Switch-Case en ARM

Este proyecto demuestra cómo implementar una estructura switch-case en código ensamblador ARM e integrarlo con un programa en C.

## Estructura del Proyecto

- `menu.s`: Contiene la implementación del switch-case en ensamblador ARM
- `main.c`: Programa principal en C que muestra un menú y llama a la función de ensamblador
- `Makefile`: Archivo para compilar el proyecto

## Compilación

Para compilar el proyecto, ejecute:

```
make
```

Esto generará el ejecutable `menu_arm`.

## Ejecución

Para ejecutar el programa:

```
./menu_arm
```

o bien:

```
make run
```

## Explicación del Código ARM

El archivo `menu.s` implementa una función llamada `procesarOpcion` que recibe un número de opción en el registro R0 y realiza diferentes acciones según ese valor:

1. Compara el valor de R0 con diferentes constantes (1, 2, 3, 4)
2. Salta a la etiqueta correspondiente si hay coincidencia
3. Cada etiqueta implementa una acción diferente y devuelve un valor en R0
4. Si no hay coincidencia, salta a la etiqueta por defecto

Este patrón implementa efectivamente una estructura switch-case en ensamblador ARM.

## Notas para Compilación en Windows

Si está utilizando Windows, puede ser necesario ajustar el Makefile o utilizar herramientas como MinGW o WSL para la compilación.