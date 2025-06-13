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

int menu(void);
int delay_con_teclado(void);
void autofantastico(void);
void disp_binary(int);
extern void giro_auto_arm(void); // Función implementada en assembly (giro_auto.s)
extern void par_impar(void); // Función implementada en assembly (par_impar.s)
void elchoque(void);
//
// La enumeracion de Pines es de acuerdo a la convencion de BroadCom.
// Pines 14,15,18,23,24,25,8,7 deben conectarse a los 8 LEDs
const char led[] = {14, 15, 18, 23, 24, 25, 8, 7}; // variable global

void prenderLEDs(unsigned char byte)
{
    disp_binary(byte);
#if defined(__linux__) && defined(__arm__)
    for (int i = 0; i < 8; i++)
    {
        int estado = (byte >> i) & 0x01;
        digitalWrite(led[i], estado);
    }
#endif
}

void autofantastico()
{
    unsigned char tabla[] = {
        0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    printf("Auto Fantástico: (a para salir, r para más rápido, l para más lento)\n");

    // Cada valor es un número de 8 bits donde solo un bit está en 1 y los demás en 0.
    // usamos cada valor para prenderlos y solo uno estará encendido a la vez
    while (1)
    {
        for (int i = 0; i < 8; i++)
        {
            prenderLEDs(tabla[i]);
            // Esperar con detección de teclas
            if (delay_con_teclado() == 0)
            {
                return;
            };
        }
        for (int i = 6; i >= 1; i--)
        {
            prenderLEDs(tabla[i]);
            // Esperar con detección de teclas
            if (delay_con_teclado() == 0)
            {
                return;
            };
        }
    }
}

void elchoque(void)
{
    printf("Mostrando Choque:\n");
    unsigned char tabla[7] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42};
    // Dos LEDs encendidos a la vez (dos unos a la vez)

    while (1)
    {
        for (int i = 0; i < 7; i++)
        {
            prenderLEDs(tabla[i]);
            if (delay_con_teclado() == 0)
            {
                return;
            };
        }
    }
}

// Función giro_auto implementada en assembly (giro_auto.s)
// Implementa un patrón de giño secuencial como las luces modernas de auto

// Función par_impar implementada en assembly (par_impar.s)
// La versión en assembly implementa la lógica básica sin control de teclado

int verificarPassword()
{
    const char passwordCorrecto[] = "gru11";
    char input[6]; // 5 dígitos + '\0'
    int intentos = 0;

    while (intentos < 3)
    {
        printf("Ingrese su contrasenia de 5 dígitos (Sin espacios): ");

        for (int i = 0; i < 5; i++)
        {
            char ch = getch();
            input[i] = ch;
            printf("*");
        }
        input[5] = '\0';
        // Esto convierte el arreglo de caracteres en una cadena de texto válida en C
        // para que funciones como strcmp puedan comparar la contraseña ingresada con la correcta.
        printf("\n");

        if (strcmp(input, passwordCorrecto) == 0)
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


// Función auxiliar para delay con detección de teclado desde assembly
// Retorna 1 si debe continuar, 0 si debe salir
int delay_con_teclado(){
    static int delay_us = 500000; // delay estático para assembly

    for (int t = 0; t < delay_us / 10000; t++){
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
            if (key == 'r' && delay_us > 100000)
                delay_us -= 20000;
            if (key == 'l')
                delay_us += 20000;
        }
        usleep(10000);
    }
    
    return 1; // Continuar
}

int menu(void)
{
    int s;
    do
    {
        printf("\n   MENU  \n");
        printf("1) auto fantastico\n");
        printf("2) choque\n");
        printf("3) tira\n");
        printf("4) par e impar\n");
        printf("5) salir\n");
        printf("Elija su opcion: ");
        scanf("%d", &s);
    } while (s < 1 || s > 5);
    return s;
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

    if (!verificarPassword())
    {
        return 0; // Si la contraseña es incorrecta, se sale del programa.
    }
#if defined(__linux__) && defined(__arm__)
    pioInit();
    int i;
    for (i = 0; i < 8; i++)
    {
        pinMode(led[i], OUTPUT); // Configure los 8 pines para los LEDs como salidas en main
    }
#endif

    int choice;
    for (;;)
    {
        prenderLEDs(0x00); // Turn off leds active low
        choice = menu();
        switch (choice)
        {
        case 1:
            autofantastico();
            break;
        case 2:
            elchoque();
            break;
        case 3:
            giro_auto();
            break;
        case 4:
            par_impar();
            break;
        case 5:
            exit(0);
        }
    }
}