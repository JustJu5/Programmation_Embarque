#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
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

int current_value = 0;
int max_value = 0;
int finished = 0;

pthread_mutex_t lock;

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

void show_number_once(int value)
{
    int dizaines = value / 10;
    int unites = value % 10;

    disable_digits();
    display_digit(dizaines);
    digitalWrite(digit_right, HIGH);
    delay(1);

    disable_digits();
    display_digit(unites);
    digitalWrite(digit_left, HIGH);
    delay(1);
}

void *thread_counter(void *arg)
{
    for (int i = 0; i <= max_value; i++) {
        pthread_mutex_lock(&lock);
        current_value = i;
        pthread_mutex_unlock(&lock);
        sleep(1);
    }

    pthread_mutex_lock(&lock);
    finished = 1;
    pthread_mutex_unlock(&lock);

    return NULL;
}

void *thread_display(void *arg)
{
    int value;
    int done;

    while (1) {
        pthread_mutex_lock(&lock);
        value = current_value;
        done = finished;
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 200; i++)
            show_number_once(value);

        if (done)
            break;
    }

    for (int i = 0; i < 300; i++)
        show_number_once(current_value);

    disable_digits();
    clear_segments();

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t th_counter, th_display;

    if (argc != 2) {
        printf("Usage: %s <valeur_max>\n", argv[0]);
        return 1;
    }

    max_value = atoi(argv[1]);

    if (max_value < 0 || max_value > 99)
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

    pthread_mutex_init(&lock, NULL);

    pthread_create(&th_counter, NULL, thread_counter, NULL);
    pthread_create(&th_display, NULL, thread_display, NULL);

    pthread_join(th_counter, NULL);
    pthread_join(th_display, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}
