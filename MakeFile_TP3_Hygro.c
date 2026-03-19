CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lwiringPi
TARGET = pmod_hygro

all: $(TARGET)

$(TARGET): pmod_hygro.o
	$(CC) $(CFLAGS) -o $(TARGET) pmod_hygro.o $(LIBS)

pmod_swt.o: pmod_hygro.c
	$(CC) $(CFLAGS) -c pmod_hygro.c
	
clean:
	rm -f *.o $(TARGET)