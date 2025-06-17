CC = gcc
AS = as
LD = gcc

CFLAGS = -Wall -g
LDFLAGS =


OBJECTS = main.o giro_auto_arm.o par_impar_arm.o


all: menu_arm


menu_arm: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o menu_arm


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.s
	$(CC) -c $< -o $@


clean:
	rm -f *.o menu_arm


run: menu_arm
	./menu_arm
