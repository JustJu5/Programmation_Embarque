CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lwiringPi
TARGET = pmod_swt_als

all: $(TARGET)

$(TARGET): pmod_swt_als.o
	$(CC) $(CFLAGS) -o $(TARGET) pmod_swt_als.o $(LIBS)

pmod_swt.o: pmod_swt_als.c
	$(CC) $(CFLAGS) -c pmod_swt_als.c
	
clean:
	rm -f *.o $(TARGET)