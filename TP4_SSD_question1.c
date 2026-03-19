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
void display_digit(int num)
{
    for(int i = 0; i < 7; i++)
        digitalWrite(segments[i], digits[num][i]);
}
int main(int argc, char *argv[])
{
    if(argc != 4) {
        printf("Usage: %s <port> <value> <duration>\n", argv[0]);
        return 1;
    }
    int value = atoi(argv[2]);
    int duration = atoi(argv[3]);
    wiringPiSetup();
    for(int i = 0; i < 7; i++) {
        pinMode(segments[i], OUTPUT);
        digitalWrite(segments[i], LOW);
    }
    display_digit(value);
    sleep(duration);
    for(int i = 0; i < 7; i++)
        digitalWrite(segments[i], LOW);
    return 0;
}