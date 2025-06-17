#include <stdio.h>       // Operaciones de entrada y salida
#include <string.h>      // Manejo de cadenas de texto
#include <stdlib.h>      // Utilidades generales de C
#include <unistd.h>      // Llamadas POSIX

// Solo en la Raspberry se usa la biblioteca EasyPIO
#if defined(__linux__) && defined(__arm__)
#include "EasyPIO.h"     // Configuración de pines GPIO
#endif

// En Windows se utiliza conio.h, en Linux se implementa getch()
#ifdef _WIN32
#include <conio.h>       // Funciones de consola propias de Windows
#else
#include <termios.h>     // Configurar terminales POSIX
#include <unistd.h>      // Funciones POSIX
#include <fcntl.h>       // Control de archivos
// Implementación personalizada de getch
int getch()
{
    struct termios oldattr, newattr;  // Estructuras para configurar la terminal
    int ch;                           // Variable para almacenar el caracter
    tcgetattr(STDIN_FILENO, &oldattr);    // Guardar atributos actuales
    newattr = oldattr;                    // Copiar configuración
    newattr.c_lflag &= ~(ICANON | ECHO);  // Desactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Aplicar cambios
    ch = getchar();                      // Leer un caracter
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // Restaurar atributos
    return ch;                           // Retornar el caracter leído
}

// kbhit comprueba si hay una tecla disponible sin bloquear la ejecución
int kbhit()
{
    struct termios oldt, newt;         // Copias de la configuración del terminal
    int ch;                            // Guarda la tecla leída
    int oldf;                          // Almacena los flags del descriptor

    tcgetattr(STDIN_FILENO, &oldt);        // Guardar atributos actuales
    newt = oldt;                           // Copiarlos
    newt.c_lflag &= ~(ICANON | ECHO);      // Desactivar espera bloqueante
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Aplicar nueva configuración
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  // Obtener flags del descriptor
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Hacer lectura no bloqueante

    ch = getchar();                        // Intentar leer un caracter

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restaurar atributos
    fcntl(STDIN_FILENO, F_SETFL, oldf);       // Restaurar flags

    if (ch != EOF)
    {
        ungetc(ch, stdin);                 // Devolver el caracter al buffer
        return 1;                          // Indicar que hay entrada disponible
    }

    return 0;                              // No se presionó ninguna tecla
}
#endif

int mostrar_menu(void);                      // Prototipo del menú de opciones
int espera_tecla(void);                      // Espera y lee comandos del usuario
void autof(void);                            // Efecto "auto fantástico"
void disp_binary(int);                       // Presenta un número en binario
extern void guinio(void);   // Función ensamblada para la animación de giro
extern void alternar(void); // Rutina ensamblada que invierte los LEDs
void choque(void);                           // Efecto de colisión
// Vector con los pines de la Raspberry conectados a los LEDs
const char ledPins[] = {14, 15, 18, 23, 24, 25, 8, 7};

void prenderLEDs(unsigned char byte)
{
    disp_binary(byte);                    // Mostrar representación binaria
#if defined(__linux__) && defined(__arm__)
    for (int i = 0; i < 8; i++)           // Recorrer cada LED
    {
        int estado = (byte >> i) & 0x01;  // Extraer el bit correspondiente
        digitalWrite(ledPins[i], estado); // Cambiar el estado del LED
    }
#endif
}

void autof()
{
    unsigned char patron[] = {        // Tabla con las posiciones de la luz
        0x80, 0x40, 0x20, 0x10,
        0x08, 0x04, 0x02, 0x01};

    printf("Auto Fantástico: (a para salir, r para más rápido, l para más lento)\n");

    // Variables para controlar la posición actual y la dirección
    int pos = 0;        // Índice dentro del arreglo
    int sentido = 1;    // 1 avanza, -1 retrocede
    while (1)
    {
        prenderLEDs(patron[pos]);      // Encender LED en la posición actual
        if (espera_tecla() == 0)       // Consultar si el usuario quiere salir
            return;

        pos += sentido;                // Avanzar en la dirección indicada
        if (pos == 7)
            sentido = -1;              // Cambiar dirección al llegar al final
        else if (pos == 0)
            sentido = 1;               // Regresar al inicio
    }
}

void choque(void)
{
    printf("Mostrando Choque:\n");     // Encabezado en pantalla
    unsigned char patronImpacto[7] = { // Secuencia de encendido doble
        0x81, 0x42, 0x24, 0x18,
        0x18, 0x24, 0x42};
    // Efecto visual de colisión

    int pos = 0;                         // Posición actual en el patrón
    while (1)
    {
        prenderLEDs(patronImpacto[pos]); // Mostrar la posición actual
        if (espera_tecla() == 0)         // Revisar la entrada del usuario
            return;

        pos = (pos + 1) % 7;            // Avanzar circularmente
    }
}

// La rutina guinio está escrita en giro_auto_arm.s
// Genera un efecto secuencial de intermitentes

// La función alternar (par_impar_arm.s) cambia el patrón sin usar el teclado

int pedirClave()
{
    const char claveCorrecta[] = "gru11"; // Clave válida para acceder
    char ingreso[6];                       // Espacio para los 5 caracteres y el nulo
    int intentos = 0;                      // Conteo de intentos realizados

    while (intentos < 3)                   // Se permiten hasta tres intentos
    {
        printf("Ingrese su contrasenia de 5 dígitos (Sin espacios): ");

        for (int i = 0; i < 5; i++)        // Leer carácter por carácter
        {
            char ch = getch();            // Capturar tecla
            ingreso[i] = ch;              // Guardar en el arreglo
            printf("*");                  // Mostrar asterisco
        }
        ingreso[5] = '\0';               // Final de cadena para strcmp
        // Se convierte el arreglo en una cadena válida para strcmp
        printf("\n");

        if (strcmp(ingreso, claveCorrecta) == 0)
        {
            printf("Bienvenido al Sistema\n\n"); // Acierto
            return 1;
        }
        else
        {
            printf("Password no válida\n\n");    // Error
            intentos++;                           // Incrementar contador
            printf("Intentos restantes: %d\n", 3 - intentos);
        }
    }

    printf("El programa se aborta.\n");    // Se agotaron los intentos
    return 0;                              // Fallo al autenticar
}


// Función auxiliar para la pausa y detección de teclado
// Retorna 1 si debe continuar, 0 si debe salir
int espera_tecla(){
    static int retardo_us = 500000;     // Retardo inicial en microsegundos

    for (int t = 0; t < retardo_us / 10000; t++){ // Bucle de espera fraccionado
#ifdef _WIN32
        if (_kbhit()){
            char key = _getch();       // Lectura sin bloqueo en Windows
        }
#else
        if (kbhit()){
            char key = getch();        // Lectura sin bloqueo en Linux
#endif
            if (key == 'a')
                return 0;             // Salir de la rutina
            if (key == 'r' && retardo_us > 100000)
                retardo_us -= 20000;  // Acelerar la secuencia
            if (key == 'l')
                retardo_us += 20000;  // Disminuir velocidad
        }
        usleep(10000);                  // Espera pequeña antes de repetir
    }
    
    return 1;                          // Mantener la ejecución
}

int mostrar_menu(void)
{
    int op;                              // Almacena la opción elegida
    do
    {
        printf("\n   MENU  \n");
        printf("1) autof\n");
        printf("2) choque\n");
        printf("3) tira\n");
        printf("4) alternar\n");
        printf("5) salir\n");
        printf("Elija su opcion: ");
        scanf("%d", &op);               // Leer la elección
    } while (op < 1 || op > 5);          // Validar que esté en rango
    return op;                           // Devolver la selección
}
void disp_binary(int i)
{
    // Muestra un número de 8 bits usando '*' para encendido y '_' para apagado
    int t;                                 // Variable para recorrer los bits
    for (t = 128; t > 0; t = t / 2)       // Comienza en el bit más significativo
        if (i & t)
            printf("*");                  // Imprime '*' si el bit está a 1
        else
            printf("_");                  // Imprime '_' si el bit vale 0
    printf("\n");                        // Fin de línea
}

int main(void)
{

    if (!pedirClave())                 // Verificar contraseña de acceso
    {
        return 0;                      // Finalizar si es incorrecta
    }
#if defined(__linux__) && defined(__arm__)
    pioInit();                         // Inicializar biblioteca de pines
    int i;
    for (i = 0; i < 8; i++)           // Configurar cada pin como salida
    {
        pinMode(ledPins[i], OUTPUT);   // Pin correspondiente a cada LED
    }
#endif

    int opcion;                        // Guardará la elección del menú
    for (;;)                           // Bucle principal infinito
    {
        prenderLEDs(0x00);            // Apagar todas las luces
        opcion = mostrar_menu();      // Mostrar opciones y leer selección
        switch (opcion)               // Ejecutar la opción elegida
        {
        case 1:
            autof();                  // Secuencia auto fantástico
            break;
        case 2:
            choque();                 // Secuencia de colisión
            break;
        case 3:
            guinio();                 // Efecto de giro en ensamblador
            break;
        case 4:
            alternar();               // Invertir LEDs con ASM
            break;
        case 5:
            exit(0);                  // Salir del programa
        }
    }
}
