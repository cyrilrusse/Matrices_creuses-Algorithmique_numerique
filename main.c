#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft.txt";

    Creuse **matrice = lecture(nom_fichier);
    unsigned int nombre_joueur_dif = getNombreJoueurDifferent(matrice[0]);

    //Création d'une vecteur remplit de 1 de la taille du nombre de joueur différent
    //afin de faire le produit matriciel de M avec ce vecteur. On obtient un tableau des 
    //victoires des différents joueurs.
    float *vecteur_pour_tab_victoire = malloc(sizeof(float)*nombre_joueur_dif);
    if(!vecteur_pour_tab_victoire){
        libereCreuse(matrice[0], 0, 0);
        libereCreuse(matrice[1], 1, 1);
        free(matrice);
        return -1;
    }
    for(unsigned int i = 0; i<nombre_joueur_dif; i++){
        vecteur_pour_tab_victoire[i] = 1;
    }
    float *tab_victoire = produitMatriceVecteurDense(matrice[1], vecteur_pour_tab_victoire);

    float *vecteur_B_V = puissance(matrice[0]);


    libereCreuse(matrice[0], 0, 1);
    libereCreuse(matrice[1], 1, 1);
    free(matrice);
    free(vecteur_B_V);
    free(tab_victoire);
    free(vecteur_pour_tab_victoire);

    return 0;
}