#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#if defined(__linux__) && defined(__arm__)
#include "EasyPIO.h"
#endif


#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

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
extern void guinio(void);
extern void alternar(void);
void choque(void);

const char ledPins[] = {14, 15, 18, 23, 24, 25, 8, 7};

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
        0x80, 0x40, 0x20, 0x10,
        0x08, 0x04, 0x02, 0x01};

    printf("Auto Fantástico: (a para salir, r para más rápido, l para más lento)\n");


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
    unsigned char patronImpacto[7] = {
        0x81, 0x42, 0x24, 0x18,
        0x18, 0x24, 0x42};


    int pos = 0;
    while (1)
    {
        prenderLEDs(patronImpacto[pos]);
        if (espera_tecla() == 0)
            return;

        pos = (pos + 1) % 7;
    }
}






int pedirClave()
{
    const char claveCorrecta[] = "gru11";
    char ingreso[6];
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

        printf("\n");

        if (strcmp(ingreso, claveCorrecta) == 0)
        {
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




int espera_tecla(){
    static int retardo_us = 500000;

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
                return 0;
            if (key == 'r' && retardo_us > 100000)
                retardo_us -= 20000;
            if (key == 'l')
                retardo_us += 20000;
        }
        usleep(10000);
    }
    
    return 1;
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
        return 0;
    }
#if defined(__linux__) && defined(__arm__)
    pioInit();
    int i;
    for (i = 0; i < 8; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }
#endif

    int opcion;
    for (;;)
    {
        prenderLEDs(0x00);
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
