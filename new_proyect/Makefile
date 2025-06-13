CC = gcc
AS = as
LD = gcc

CFLAGS = -Wall -g
LDFLAGS = 

# Archivos objeto
OBJECTS = main.o menu.o, 

# Objetivo principal
all: menu_arm

# Compilación del programa
menu_arm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o menu_arm

# Regla para compilar archivos .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar archivos .s a .o
%.o: %.s
	$(CC) -c $< -o $@

# Limpieza
clean:
	rm -f *.o menu_arm

# Correr el programa
run: menu_arm
	./menu_arm 