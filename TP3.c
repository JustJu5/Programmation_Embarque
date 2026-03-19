#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#define I2C_BUS     "/dev/i2c-1"
#define HDC1080_ADDR 0x40
// Registres HDC1080
#define REG_TEMP    0x00
#define REG_HUMID   0x01
#define REG_CONFIG  0x02
#define REG_MANUF   0xFE
#define REG_DEVICE  0xFF
// Lire un registre 16 bits
uint16_t read_register(int fd, uint8_t reg) {
    write(fd, &reg, 1);
    usleep(20000); // attente conversion 20ms
    uint8_t buf[2];
    read(fd, buf, 2);
    return (buf[0] << 8) | buf[1];
}
int main() {
    int fd = open(I2C_BUS, O_RDWR);
    if (fd < 0) {
        perror("Erreur ouverture I2C");
        return 1;
    }
    if (ioctl(fd, I2C_SLAVE, HDC1080_ADDR) < 0) {
        perror("Erreur adresse I2C");
        close(fd);
        return 1;
    }
    // Identifiants
    uint16_t manuf  = read_register(fd, REG_MANUF);
    uint16_t device = read_register(fd, REG_DEVICE);
    uint16_t config = read_register(fd, REG_CONFIG);
    printf("Fabricant      : 0x%04X (attendu 0x5449)\n", manuf);
    printf("ID Produit     : 0x%04X (attendu 0x1050)\n", device);
    printf("Configuration  : 0x%04X\n", config);
    // Température
    uint16_t raw_temp = read_register(fd, REG_TEMP);
    double temperature = ((double)raw_temp / 65536.0) * 165.0 - 40.0;
    printf("Temperature    : %.2f °C\n", temperature);
    // Humidité
    uint16_t raw_humid = read_register(fd, REG_HUMID);
    double humidite = ((double)raw_humid / 65536.0) * 100.0;
    printf("Humidite       : %.2f %%\n", humidite);
    close(fd);
    return 0;
}