CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lwiringPi
TARGET = pmod_ssd
 
all: $(TARGET)
 
$(TARGET): pmod_ssd.o
$(CC) $(CFLAGS) -o $(TARGET) pmod_ssd.o $(LIBS)
 
pmod_ssd.o: pmod_ssd.c
$(CC) $(CFLAGS) -c pmod_ssd.c
 
clean:
rm -f *.o $(TARGET)