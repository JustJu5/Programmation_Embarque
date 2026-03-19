CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lwiringPi
TARGET = test_ssd
 
all: $(TARGET)
 
$(TARGET): test_ssd.o
$(CC) $(CFLAGS) -o $(TARGET) test_ssd.o $(LIBS)
 
test_ssd.o: test_ssd.c
$(CC) $(CFLAGS) -c test_ssd.c
 
clean:
rm -f *.o $(TARGET)