#include <stdio.h>
#include <stdlib.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft.txt";

    Creuse **matrice = lecture(nom_fichier);



    Creuse *result = produitMatriceVecteurCreux(matrice[0], NULL);
    // libereCreuse(result, 0, 1);

    libereCreuse(matrice[0], 0, 0);
    libereCreuse(matrice[1], 1, 1);
    free(matrice);

    return 0;
}