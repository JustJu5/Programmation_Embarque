#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>

int max_val;

int seg_pins[7] = {10, 12, 13, 14, 24, 29, 28};

int digits[10][7] = {
    {1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0},
    {1,1,0,1,1,0,1},
    {1,1,1,1,0,0,1},
    {0,1,1,0,0,1,1},
    {1,0,1,1,0,1,1},
    {1,0,1,1,1,1,1},
    {1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1}
};

void display_digit(int n)
{
    if (n < 0 || n > 9) return;

    for (int i = 0; i < 7; i++) {
        digitalWrite(seg_pins[i], digits[n][i]);
    }
}

void *compteur(void *arg)
{
    for (int i = 0; i <= max_val; i++) {
        display_digit(i % 10);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t th;

    if (argc != 2) {
        return 1;
    }

    max_val = atoi(argv[1]);

    if (wiringPiSetup() == -1) {
        return 1;
    }

    for (int i = 0; i < 7; i++) {
        pinMode(seg_pins[i], OUTPUT);
        digitalWrite(seg_pins[i], 0);
    }

    if (pthread_create(&th, NULL, compteur, NULL) != 0) {
        return 1;
    }

    pthread_join(th, NULL);

    for (int i = 0; i < 7; i++) {
        digitalWrite(seg_pins[i], 0);
    }

    return 0;
}
