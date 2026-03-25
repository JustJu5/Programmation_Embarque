#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

int segments[7] = {10, 12, 13, 14, 24, 29, 28};

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

int digit_left = 0;
int digit_right = 1;

void display_digit(int num)
{
    for (int i = 0; i < 7; i++)
        digitalWrite(segments[i], digits[num][i]);
}

void clear_segments()
{
    for (int i = 0; i < 7; i++)
        digitalWrite(segments[i], LOW);
}

void disable_digits()
{
    digitalWrite(digit_left, LOW);
    digitalWrite(digit_right, LOW);
}

void show_number(int value, int duration)
{
    int dizaines = value / 10;
    int unites = value % 10;

    for (int t = 0; t < duration * 500; t++) {
        disable_digits();
        display_digit(dizaines);
        digitalWrite(digit_left, HIGH);
        delay(1);

        disable_digits();
        display_digit(unites);
        digitalWrite(digit_right, HIGH);
        delay(1);
    }

    disable_digits();
    clear_segments();
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("Usage: %s <port> <value> <duration>\n", argv[0]);
        return 1;
    }

    int value = atoi(argv[2]);
    int duration = atoi(argv[3]);

    if (value < 0 || value > 99)
        return 1;

    if (wiringPiSetup() == -1)
        return 1;

    for (int i = 0; i < 7; i++) {
        pinMode(segments[i], OUTPUT);
        digitalWrite(segments[i], LOW);
    }

    pinMode(digit_left, OUTPUT);
    pinMode(digit_right, OUTPUT);

    digitalWrite(digit_left, LOW);
    digitalWrite(digit_right, LOW);

    show_number(value, duration);

    return 0;
}
