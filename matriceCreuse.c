#include <stdio.h>
#include <stdlib.h>
#include "matriceCreuse.h"

struct resultat_t{
    int joueurcol;
    int joueurlig;
    double valeur;
};


struct creuse_t
{
    int *startCol;
    int *rows;
    Resultat *values;
    unsigned int nz;
};


void quicksort2(unsigned int *array1, unsigned int *array2, int p, int r)
{
    int position_pivot;
    if (p < r)
    {
        position_pivot = partition2(array1, array2, p, r);
        quicksort2(array1, array2, p, position_pivot - 1);
        quicksort2(array1, array2, position_pivot + 1, r);
    }
}
int partition2(unsigned int *array1, unsigned int *array2, int p, int r)
{
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (array1[j] <= array1[r])
        {
            i++;
            swap(array1, i, j);
            swap(array2, i, j);
        }
    }
    swap(array1, i + 1, r);
    swap(array2, i + 1, r);

    return i + 1;
}
void swap(unsigned int *array, int a, int b)
{
    unsigned int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

Creuse **lecture(char* nom_fichier){
    Creuse **tab_matrice = malloc(sizeof(Creuse*)*2);
    FILE *fichier;
    unsigned int tampon;
    unsigned int nombre_result = 0;
    unsigned int nombre_joueur = 0;
    int i = 0;


    if (!(fichier= fopen(nom_fichier, "r")))    return NULL;
    while (fscanf(fichier, "%u", &tampon)!=EOF)   nombre_result++;
    fclose(fichier);
    nombre_result/=3;

    unsigned int *tab_j1 = malloc(sizeof(unsigned int)*nombre_result);
    unsigned int *tab_j2 = malloc(sizeof(unsigned int)*nombre_result);

    if (!(fichier = fopen(nom_fichier, "r")))    return NULL;

    while (fscanf(fichier, "%u", &tampon)!=EOF){
        tab_j1[i] = tampon;
        fscanf(fichier, "%u", &tampon);
        tab_j2[i] = tampon;
        fscanf(fichier, "%u", &tampon);

        if(tampon == 2){
            tampon = tab_j1[i];
            tab_j1[i] = tab_j2[i];
            tab_j2[i] = tampon;
        }
        i++;
    }
    quicksort2(tab_j1, tab_j2, 0, nombre_result);

    unsigned int *tab_joueur_dif = malloc(sizeof(unsigned int)*nombre_result);
    for(unsigned int j = 0; j<nombre_result; j++){
        if(tampon != tab_j1[j]){
            tab_joueur_dif[nombre_joueur] = tab_j1[j];
            nombre_joueur++;
            tampon = tab_j1[j];
        }
    }
    for(un)

    // printf("nbr j  %d\n", nombre_joueur);
    // for(int k = 0; k<20; k++){
    //     printf("%d \n", tab_joueur_dif[k]);
    // }
    return tab_matrice;
}


int recherche_dico(unsigned int* tab, unsigned int elem, unsigned int taille){
    unsigned int born_sup = taille;
    unsigned int born_inf = 0;
    unsigned int centre;
    while(born_sup>=born_inf){
        centre = (born_inf + born_sup) / 2;
        if(tab[centre] > elem){
            born_sup = centre-1;
        }
        if(tab[centre]<elem){
            born_inf = centre +1;
        }
        else
            return centre;

    }
    return -1;
    
}
