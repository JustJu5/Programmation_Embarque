CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lwiringPi
TARGET = pmod_swt

all: $(TARGET)

$(TARGET): pmod_swt.o
	$(CC) $(CFLAGS) -o $(TARGET) pmod_swt.o $(LIBS)

pmod_swt.o: pmod_swt.c
	$(CC) $(CFLAGS) -c pmod_swt.c
	
clean:
	rm -f *.o $(TARGET)