#include <stdio.h>
#include <stdlib.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft_test.txt";

    Creuse **matrice = lecture(nom_fichier);

    transpose(matrice[0]);

    
    return 0;
}