CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lwiringPi
TARGET = pmod_swt_8ld

all: $(TARGET)

$(TARGET): pmod_swt_8ld.o
	$(CC) $(CFLAGS) -o $(TARGET) pmod_swt_8ld.o $(LIBS)

pmod_swt.o: pmod_swt_8ld.c
	$(CC) $(CFLAGS) -c pmod_swt.c
	
clean:
	rm -f *.o $(TARGET)