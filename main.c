#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft_test.txt";

    Creuse **matrice = lecture(nom_fichier);

    

    float *vecteur_B_V = puissance(matrice[0]);
    unsigned int indice = 0;
    float min = 0;

    // for(unsigned int i = 0; i<get_nombre_joueur_different(matrice[0]);i++){
    //     // if(min<vecteur_B_V[i]){
    //     //     min = vecteur_B_V[i];
    //     //     indice = i;
    //     //     }
    // }

    // printf("indice: %u et matricule: %u\n", indice, get_index(matrice[0])[indice]);

    libereCreuse(matrice[0], 0, 0);
    libereCreuse(matrice[1], 1, 1);
    free(matrice);
    free(vecteur_B_V);

    return 0;
}