#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

void quicksort(unsigned int *array1, int p, int r)
{
    int position_pivot;
    if (p < r)
    {
        position_pivot = partition(array1, p, r);
        quicksort(array1, p, position_pivot - 1);
        quicksort(array1, position_pivot + 1, r);
    }
}

int partition(unsigned int *array1, int p, int r)
{
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (array1[j] <= array1[r])
        {
            i++;
            swap(array1, i, j);
        }
    }
    swap(array1, i + 1, r);

    return i + 1;
}

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
    unsigned int nombre_joueur_gagnant = 0;
    unsigned int nombre_joueur_jamais_gagne = 0;
    int i = 0;


    if (!(fichier= fopen(nom_fichier, "r")))    return NULL;
    while (fscanf(fichier, "%u", &tampon)!=EOF)   nombre_result++;
    fclose(fichier);
    nombre_result/=3;

    unsigned int *tab_j1 = malloc(sizeof(unsigned int)*nombre_result);
    unsigned int *tab_j2 = malloc(sizeof(unsigned int)*nombre_result);

    if (!(fichier = fopen(nom_fichier, "r")))    return NULL;
    //lecture fichier + tri des 2 tableaux en fonction du tableau des gagnants
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

    unsigned int *tab_joueur_gagnant = malloc(sizeof(unsigned int)*nombre_result);
    tampon = INT_MAX;
    for(unsigned int j = 0; j<nombre_result; j++){
        if(tampon != tab_j1[j]){
            tab_joueur_gagnant[nombre_joueur_gagnant] = tab_j1[j];
            nombre_joueur_gagnant++;
            tampon = tab_j1[j];
        }
    }

    //création tab joueur jamais gagne
    unsigned int *tab_joueur_jamais_gagne = malloc(sizeof(unsigned int)*nombre_result);
    for(unsigned int j = 0; j<nombre_result; j++){
        if (recherche_dico(tab_joueur_gagnant, tab_j2[j], nombre_joueur_gagnant) == -1 && recherche_dico(tab_joueur_jamais_gagne, tab_j2[j], nombre_joueur_jamais_gagne)==-1){
            tab_joueur_jamais_gagne[nombre_joueur_jamais_gagne] = tab_j2[j];
            printf("nombre jamais gangé = %u\n", nombre_joueur_jamais_gagne);
            printf("%u\n", tab_joueur_jamais_gagne[nombre_joueur_jamais_gagne]);
            nombre_joueur_jamais_gagne++;
        }
    }
    quicksort(tab_joueur_jamais_gagne, 0, nombre_joueur_jamais_gagne-1);

    

    unsigned int index_gagnant=0, index_perdant = 0, index_joueur_different = 0;
    unsigned int *tab_joueur_different = malloc(sizeof(unsigned int)*(nombre_joueur_gagnant+nombre_joueur_jamais_gagne));
    while(index_gagnant<nombre_joueur_gagnant || index_perdant<nombre_joueur_jamais_gagne){
        if(index_perdant<nombre_joueur_jamais_gagne && tab_joueur_gagnant[index_gagnant]>tab_joueur_jamais_gagne[index_perdant]){
            tab_joueur_different[index_joueur_different] = tab_joueur_jamais_gagne[index_perdant];
            index_perdant++;
        }
        else{
            tab_joueur_different[index_joueur_different] = tab_joueur_gagnant[index_gagnant];
            index_gagnant++;
            printf("%u\n", index_gagnant);
        }
        index_joueur_different++;
    }


    printf("nombre gagnant : %u\n nombre perdant : %u\n nombre_joueur diff : %u\n",nombre_joueur_gagnant, nombre_joueur_jamais_gagne, index_joueur_different);
    for (int i = 0; i <5; i++)
    {
        printf("tableau[%d] : %u\n", i, tab_joueur_gagnant[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        printf("tableau[%d] : %u\n", i, tab_joueur_jamais_gagne[i]);
    }
    // printf("nbr j  %d\n", nombre_joueur);
    // for(int k = 0; k<20; k++){
    //     printf("%d \n", tab_joueur_dif[k]);
    // }
    return tab_matrice;
}


int recherche_dico(unsigned int* tab, unsigned int elem, unsigned int taille){
    unsigned int born_sup;
    if(taille>0)
        born_sup = taille -1;
    unsigned int born_inf = 0;
    unsigned int centre;
    // for(int i=0; i<5; i++){
    //     printf("tableau[%d] : %u\n", i, tab[i]);
    // }
    // printf("elem : %u\n", elem);
    while(born_sup>born_inf){
        centre = (born_inf + born_sup) / 2;
        // printf("%u\t%u\n", born_sup, born_inf);
        if(tab[centre] > elem){
            born_sup = centre-1;
        }
        else if(tab[centre]<elem){
            born_inf = centre +1;
        }
        else
            return centre;
    }
    return -1;
    
}
