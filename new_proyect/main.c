#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Importa EasyPIO.h en la raspberry
#if defined(__linux__) && defined(__arm__)
#include "EasyPIO.h"
#endif

// Trae conio.h o lo implementa en linux
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
// Implementación de getch() para sistemas Unix/Linux
int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

// Implementación de kbhit() para sistemas Unix/Linux
int kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

int mostrar_menu(void);
int espera_tecla(void);
void autof(void);
void disp_binary(int);
extern void guinio(void); // Rutina en ensamblador para animación secuencial
extern void alternar(void); // Función implementada en assembly
void choque(void);
//
// La enumeracion de Pines es de acuerdo a la convencion de BroadCom.
// Pines 14,15,18,23,24,25,8,7 deben conectarse a los 8 LEDs
const char ledPins[] = {14, 15, 18, 23, 24, 25, 8, 7}; // Pines conectados a los LEDs

void prenderLEDs(unsigned char byte)
{
    disp_binary(byte);
#if defined(__linux__) && defined(__arm__)
    for (int i = 0; i < 8; i++)
    {
        int estado = (byte >> i) & 0x01;
        digitalWrite(ledPins[i], estado);
    }
#endif
}

void autof()
{
    unsigned char patron[] = {
        0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    printf("Auto Fantástico: (a para salir, r para más rápido, l para más lento)\n");

    // Cada valor es un número de 8 bits donde solo un bit está en 1 y los demás en 0.
    // usamos cada valor para prenderlos y solo uno estará encendido a la vez
    int pos = 0;
    int sentido = 1;
    while (1)
    {
        prenderLEDs(patron[pos]);
        if (espera_tecla() == 0)
            return;

        pos += sentido;
        if (pos == 7)
            sentido = -1;
        else if (pos == 0)
            sentido = 1;
    }
}

void choque(void)
{
    printf("Mostrando Choque:\n");
    unsigned char patronImpacto[7] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42};
    // Dos LEDs encendidos a la vez (dos unos a la vez)

    int pos = 0;
    while (1)
    {
        prenderLEDs(patronImpacto[pos]);
        if (espera_tecla() == 0)
            return;

        pos = (pos + 1) % 7;
    }
}

// Función guinio implementada en assembly (giro_auto_arm.s)
// Implementa un patrón de luces secuenciales tipo intermitente moderno

// Función alternar implementada en assembly (par_impar_arm.s)
// La versión en assembly implementa la lógica básica sin control de teclado

int pedirClave()
{
    const char claveCorrecta[] = "gru11";
    char ingreso[6]; // 5 dígitos + '\0'
    int intentos = 0;

    while (intentos < 3)
    {
        printf("Ingrese su contrasenia de 5 dígitos (Sin espacios): ");

        for (int i = 0; i < 5; i++)
        {
            char ch = getch();
            ingreso[i] = ch;
            printf("*");
        }
        ingreso[5] = '\0';
        // Esto convierte el arreglo de caracteres en una cadena de texto válida en C
        // para que funciones como strcmp puedan comparar la contraseña ingresada con la correcta.
        printf("\n");

        if (strcmp(ingreso, claveCorrecta) == 0)
        {
            // Si la contraseña ingresada es igual a la correcta, se imprime un mensaje de bienvenida.
            printf("Bienvenido al Sistema\n\n");
            return 1;
        }
        else
        {
            printf("Password no válida\n\n");
            intentos++;
            printf("Intentos restantes: %d\n", 3 - intentos);
        }
    }

    printf("El programa se aborta.\n");
    return 0;
}


// Función auxiliar para la pausa y detección de teclado
// Retorna 1 si debe continuar, 0 si debe salir
int espera_tecla(){
    static int retardo_us = 500000; // retardo estático para assembly

    for (int t = 0; t < retardo_us / 10000; t++){
#ifdef _WIN32
        if (_kbhit()){
            char key = _getch();
        }
#else
        if (kbhit()){
            char key = getch();
#endif
            if (key == 'a')
                return 0; // Señal para salir
            if (key == 'r' && retardo_us > 100000)
                retardo_us -= 20000;
            if (key == 'l')
                retardo_us += 20000;
        }
        usleep(10000);
    }
    
    return 1; // Continuar
}

int mostrar_menu(void)
{
    int op;
    do
    {
        printf("\n   MENU  \n");
        printf("1) autof\n");
        printf("2) choque\n");
        printf("3) tira\n");
        printf("4) alternar\n");
        printf("5) salir\n");
        printf("Elija su opcion: ");
        scanf("%d", &op);
    } while (op < 1 || op > 5);
    return op;
}
void disp_binary(int i)
{
    // Recibe un número entero y lo muestra en binario usando * para los bits en 1 y _ para los bits en 0.
    int t;
    for (t = 128; t > 0; t = t / 2)
        if (i & t)
            printf("*");
        else
            printf("_");
    printf("\n");
}

int main(void)
{

    if (!pedirClave())
    {
        return 0; // Si la contraseña es incorrecta, se sale del programa.
    }
#if defined(__linux__) && defined(__arm__)
    pioInit();
    int i;
    for (i = 0; i < 8; i++)
    {
        pinMode(ledPins[i], OUTPUT); // Configure los 8 pines para los LEDs como salidas en main
    }
#endif

    int opcion;
    for (;;)
    {
        prenderLEDs(0x00); // Turn off leds active low
        opcion = mostrar_menu();
        switch (opcion)
        {
        case 1:
            autof();
            break;
        case 2:
            choque();
            break;
        case 3:
            guinio();
            break;
        case 4:
            alternar();
            break;
        case 5:
            exit(0);
        }
    }
}
