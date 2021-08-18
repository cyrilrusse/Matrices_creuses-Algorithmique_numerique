#include <stdio.h>
#include <stdlib.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft.txt";

    Creuse **matrice = lecture(nom_fichier);

    

    float *vecteur_B_V = puissance(transpose(matrice[0]));

    libereCreuse(matrice[0], 0, 0);
    libereCreuse(matrice[1], 1, 1);
    free(matrice);

    return 0;
}