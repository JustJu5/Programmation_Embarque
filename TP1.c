#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

static int ja_pins[8] = {24, 29, 28, 1, 11, 12, 13, 14};

int main(int argc, char *argv[])

{

    int i;
    int valeur = 0;
    int *pins = NULL;

    if (argc != 2) {
        fprintf(stderr, "Usage : %s <nom_du_port>\n", argv[0]);
        fprintf(stderr, "Exemple : %s JA\n", argv[0]);
        return 1;

    }

    if (strcmp(argv[1], "JA") == 0) {
        pins = ja_pins;

    } else {

        fprintf(stderr, "Erreur : port %s non gere.\n", argv[1]);
        fprintf(stderr, "Pour cette question, utiliser le port JA.\n");
        return 1;
    }

    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Erreur : impossible d'initialiser wiringPi.\n");
        return 1;
    }

    for (i = 0; i < 8; i++) {
        pinMode(pins[i], INPUT);
    }

    for (i = 0; i < 8; i++) {
        int bit = digitalRead(pins[i]);
        valeur |= (bit << i);
    }

    printf("Configuration du port %s : 0x%02X\n", argv[1], valeur);
    return 0;
}
 