#include <stdio.h>
#include <stdlib.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft_test.txt";

    Creuse **matrice = lecture(nom_fichier);
    unsigned int tab1[7] = {1,2,3,5,6,7,10};
    unsigned int tab2[3] = {0,5,11};
    unsigned int taille_tab1 = 7;
    unsigned int *pointeur = &taille_tab1;
    unsigned int *tab_union = union_tab(tab1, tab2, pointeur, 3);
    
    printf("taille tab1 : %u\n", taille_tab1);
    for(unsigned int i=0; i<9; i++){
        printf("tab[%u]:%u\n", i, tab_union[i]);
    }
    
    
    return 0;
}