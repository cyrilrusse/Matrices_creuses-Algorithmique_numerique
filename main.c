#include <stdio.h>
#include <stdlib.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft_test.txt";

    Creuse **matrice = lecture(nom_fichier);
    float tab[7] = {1,2,5,3,10,7,6};
    float *test_produit = produitMatriceVecteurDense(matrice[1], tab);

    for(unsigned int i=0; i<7; i++){
        printf("tab[%u]:%f\n", i, test_produit[i]);
    }
    
    return 0;
}