#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
int test_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 21, 22, 23, 24, 25, 26, 27, 28, 29};
int main(int argc, char *argv[])
{
    int i, pin;
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <wPi>\n", argv[0]);
        return 1;
    }
    pin = atoi(argv[1]);
    if (wiringPiSetup() == -1)
        return 1;
    for (i = 0; i < 22; i++) {
        pinMode(test_pins[i], OUTPUT);
        digitalWrite(test_pins[i], 0);
    }
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);
    printf("Broche wPi %d activee.\n", pin);
    printf("Regarde quel segment s'allume, puis appuie sur Entree.\n");
    getchar();
    digitalWrite(pin, 0);
    return 0;
}