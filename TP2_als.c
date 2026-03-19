// code ALS

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000
int main(void)
{
   unsigned char buffer[2];
   int value;
   if (wiringPiSetup() == -1)
       return 1;
   if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0)
       return 1;
   while (1) {
       buffer[0] = 0x00;
       buffer[1] = 0x00;
       wiringPiSPIDataRW(SPI_CHANNEL, buffer, 2);
       // Extraction de la valeur (les 8 bits du milieu souvent sur ce capteur)
       value = (buffer[0] << 8) | buffer[1];
       printf("Luminosite : %d\n", value);
       delay(500);
   }
   return 0;
}