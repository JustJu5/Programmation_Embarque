#include <stdio.h>
#include <wiringPi.h>
static int ja_sw_pins[8] = {24, 29, 28, 1, 11, 12, 13, 14};
static int jc_led_pins[8] = {27, 15, 16, 0, 7, 26, 21, 22};
int main(void)
{
   int i;
   if (wiringPiSetup() == -1)
       return 1;
   for (i = 0; i < 8; i++) {
       pinMode(ja_sw_pins[i], INPUT);
       pinMode(jc_led_pins[i], OUTPUT);
       digitalWrite(jc_led_pins[i], 0);
   }
   while (1) {
       for (i = 0; i < 8; i++) {
           int bit = digitalRead(ja_sw_pins[i]);
           digitalWrite(jc_led_pins[i], bit);
       }
       delay(50);
   }
   return 0;
}