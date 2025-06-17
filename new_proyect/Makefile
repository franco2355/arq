CC = gcc            # Compilador de C
AS = as             # Ensamblador
LD = gcc            # Enlazador

CFLAGS = -Wall -g   # Advertencias y depuración
LDFLAGS =           # Flags de enlace

# Lista de objetos que forman la aplicación
OBJECTS = main.o giro_auto_arm.o par_impar_arm.o

# Regla por defecto: construir el binario
all: menu_arm

# Unir todos los objetos para obtener el ejecutable
menu_arm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o menu_arm

# Cómo compilar los archivos C individuales
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cómo compilar los archivos en ensamblador
%.o: %.s
	$(CC) -c $< -o $@

# Borrar los productos de la compilación
clean:
	rm -f *.o menu_arm

# Ejecutar el programa resultante
run: menu_arm
	./menu_arm
