CC = gcc
AS = as
LD = gcc

CFLAGS = -Wall -g
LDFLAGS = 

# Archivos objeto
OBJECTS = main.o giro_auto_arm.o par_impar_arm.o

# Generar el ejecutable
all: menu_arm

# Enlace final del programa
menu_arm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o menu_arm

# Compilar archivos C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar archivos en ensamblador
%.o: %.s
	$(CC) -c $< -o $@

# Eliminar archivos generados
clean:
	rm -f *.o menu_arm

# Ejecutar la aplicación
run: menu_arm
	./menu_arm

