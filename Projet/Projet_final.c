#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define HYGRO_DEV   "/dev/i2c-1"
#define HYGRO_ADDR  0x40
#define REG_TEMP    0x00

#define SPI_CHANNEL 1
#define SPI_SPEED   1000000

#define TEMP_LIMIT  30.0
#define HUM_LIMIT   40.0
#define LUX_LIMIT   200

int led_fan    = 27;
int led_pump   = 16;
int led_motor1 = 26;
int led_motor2 = 21;

int read_hygro(float *temp, float *hum)
{
    int fd;
    uint8_t reg;
    uint8_t buf[4];
    uint16_t raw_temp, raw_hum;

    fd = open(HYGRO_DEV, O_RDWR);
    if (fd < 0)
        return -1;

    if (ioctl(fd, I2C_SLAVE, HYGRO_ADDR) < 0) {
        close(fd);
        return -1;
    }

    reg = REG_TEMP;
    if (write(fd, &reg, 1) != 1) {
        close(fd);
        return -1;
    }

    usleep(15000);

    if (read(fd, buf, 4) != 4) {
        close(fd);
        return -1;
    }

    raw_temp = ((uint16_t)buf[0] << 8) | buf[1];
    raw_hum  = ((uint16_t)buf[2] << 8) | buf[3];

    *temp = (raw_temp / 65536.0f) * 165.0f - 40.0f;
    *hum  = (raw_hum / 65536.0f) * 100.0f;

    close(fd);
    return 0;
}

int read_als(int *lux)
{
    unsigned char buffer[2];
    int value;

    buffer[0] = 0x00;
    buffer[1] = 0x00;

    if (wiringPiSPIDataRW(SPI_CHANNEL, buffer, 2) < 0)
        return -1;

    value = (buffer[0] << 8) | buffer[1];
    *lux = value;

    return 0;
}

void update_leds(float temp, float hum, int lux)
{
    if (temp > TEMP_LIMIT)
        digitalWrite(led_fan, HIGH);
    else
        digitalWrite(led_fan, LOW);

    if (hum < HUM_LIMIT)
        digitalWrite(led_pump, HIGH);
    else
        digitalWrite(led_pump, LOW);

    if (lux < LUX_LIMIT) {
        digitalWrite(led_motor1, HIGH);
        digitalWrite(led_motor2, HIGH);
    } else {
        digitalWrite(led_motor1, LOW);
        digitalWrite(led_motor2, LOW);
    }
}

int main(void)
{
    float temp, hum;
    int lux;

    if (wiringPiSetup() == -1)
        return 1;

    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0)
        return 1;

    pinMode(led_fan, OUTPUT);
    pinMode(led_pump, OUTPUT);
    pinMode(led_motor1, OUTPUT);
    pinMode(led_motor2, OUTPUT);

    digitalWrite(led_fan, LOW);
    digitalWrite(led_pump, LOW);
    digitalWrite(led_motor1, LOW);
    digitalWrite(led_motor2, LOW);

    while (1) {
        if (read_hygro(&temp, &hum) != 0) {
            printf("Erreur lecture HYGRO\n");
            sleep(1);
            continue;
        }

        if (read_als(&lux) != 0) {
            printf("Erreur lecture ALS\n");
            sleep(1);
            continue;
        }

        update_leds(temp, hum, lux);

        printf("Temperature: %.2f C | Humidite: %.2f %% | Luminosite: %d | Ventilateur: %s | Pompe: %s | Toit: %s\n",
               temp,
               hum,
               lux,
               (temp > TEMP_LIMIT) ? "ON" : "OFF",
               (hum < HUM_LIMIT) ? "ON" : "OFF",
               (lux < LUX_LIMIT) ? "OUVERT" : "FERME");

        sleep(1);
    }

    return 0;
}
