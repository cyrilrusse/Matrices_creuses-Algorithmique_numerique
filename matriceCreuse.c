#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "matriceCreuse.h"

// struct resultat_t{
//     int joueurcol;
//     int joueurlig;
//     double valeur;
// };

struct creuse_t
{
    int *startCol;
    int *rows;
    float *values;
    unsigned int nz;
    unsigned int *matricule_colonne;
    unsigned int *matricule_different;
};

void quicksort(unsigned int *array1, int p, int r){
    int position_pivot;
    if (p < r)
    {
        position_pivot = partition(array1, p, r);
        quicksort(array1, p, position_pivot - 1);
        quicksort(array1, position_pivot + 1, r);
    }
}

int partition(unsigned int *array1, int p, int r){
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
    int compteur = 0;

    

    if (!(fichier= fopen(nom_fichier, "r")))    return NULL;
    while (fscanf(fichier, "%u", &tampon)!=EOF)   nombre_result++;
    fclose(fichier);
    nombre_result/=3;

    unsigned int *tab_j1 = malloc(sizeof(unsigned int)*nombre_result);
    unsigned int *tab_j2 = malloc(sizeof(unsigned int)*nombre_result);

    if (!(fichier = fopen(nom_fichier, "r")))    return NULL;
    //lecture fichier + tri des 2 tableaux en fonction du tableau des gagnants
    while (fscanf(fichier, "%u", &tampon)!=EOF){
        tab_j1[compteur] = tampon;
        fscanf(fichier, "%u", &tampon);
        tab_j2[compteur] = tampon;
        fscanf(fichier, "%u", &tampon);

        if(tampon == 2){
            tampon = tab_j1[compteur];
            tab_j1[compteur] = tab_j2[compteur];
            tab_j2[compteur] = tampon;
        }
        compteur++;
    }
    quicksort2(tab_j1, tab_j2, 0, nombre_result-1);

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
            nombre_joueur_jamais_gagne++;
        }
    }
    quicksort(tab_joueur_jamais_gagne, 0, nombre_joueur_jamais_gagne-1);

    //tab joueur différent
    unsigned int index_gagnant=0, index_perdant = 0, nombre_joueur_different = 0;
    unsigned int *tab_joueur_different = malloc(sizeof(unsigned int)*(nombre_joueur_gagnant+nombre_joueur_jamais_gagne));
    while(index_gagnant<nombre_joueur_gagnant && index_perdant<nombre_joueur_jamais_gagne){
        if(index_perdant<nombre_joueur_jamais_gagne && tab_joueur_gagnant[index_gagnant]>tab_joueur_jamais_gagne[index_perdant]){
            tab_joueur_different[nombre_joueur_different] = tab_joueur_jamais_gagne[index_perdant];
            index_perdant++;
        }
        else{
            tab_joueur_different[nombre_joueur_different] = tab_joueur_gagnant[index_gagnant];
            index_gagnant++;
        }
        nombre_joueur_different++;
    }

    //remplissage fin tab joueur différent
    if(index_gagnant==nombre_joueur_gagnant){
        while (index_perdant!=nombre_joueur_jamais_gagne){
            tab_joueur_different[nombre_joueur_different] = tab_joueur_jamais_gagne[index_perdant];
            index_perdant++;
            nombre_joueur_different++;
        }
    }
    else{
        while (index_perdant != nombre_joueur_jamais_gagne){
            tab_joueur_different[nombre_joueur_different] = tab_joueur_gagnant[index_gagnant];
            index_gagnant++;
            nombre_joueur_different++;
        }
    }

    //tab nombre victoires joueurs
    unsigned int *tab_victoire = calloc(nombre_joueur_gagnant, sizeof(unsigned int));
    tampon = tab_j1[0];
    index_gagnant = 0;
    for(unsigned int i=0; i<nombre_result; i++){
        if(tampon!=tab_j1[i]){
            index_gagnant++;
            tampon = tab_j1[i];
        }
        tab_victoire[index_gagnant]++;
    }


    //création de la matrice

    Creuse *M = malloc(sizeof(Creuse));
    if(!M)
        return NULL;
    M->matricule_colonne = tab_joueur_gagnant;
    M->matricule_different = tab_joueur_different;
    M->nz = nombre_result;
    M->startCol = malloc(sizeof(int)*(nombre_joueur_gagnant+1));
    M->startCol[nombre_joueur_gagnant] = -1;
    M->rows = malloc(sizeof(int)*nombre_result);
    M->values = malloc(sizeof(float)*nombre_result);

    Creuse *A = malloc(sizeof(Creuse));
    if(!A)
        return NULL;
    A->matricule_colonne = tab_joueur_gagnant;
    A->matricule_different = tab_joueur_different;
    A->nz = nombre_result;
    A->startCol = malloc(sizeof(int) * (nombre_joueur_gagnant + 1));
    A->startCol[nombre_joueur_gagnant] = -1;
    A->rows = malloc(sizeof(int) * nombre_result);
    A->values = malloc(sizeof(float) * nombre_result);

    tampon = INT_MAX;
    unsigned int index_col = 0;
    for(unsigned int i = 0; i<nombre_result;){
        M->startCol[index_col] = i;
        A->startCol[index_col] = i;
        for(unsigned int j = 0; j<tab_victoire[index_col]; j++){
            M->rows[i] = recherche_dico(tab_joueur_different, tab_j2[i], nombre_joueur_different);
            M->values[i] = 1;
            A->rows[i] = recherche_dico(tab_joueur_different, tab_j2[i], nombre_joueur_different);
            A->values[i] = 1./tab_victoire[index_col];
            i++;
        }
        index_col++;
    }

    tab_matrice[0] = A;
    tab_matrice[1] = M;
    // for(unsigned int i=0; i<nombre_joueur_gagnant; i++){
    //     printf("StartCol[%u]=%u\n", i, A->startCol[i]);
    // }
    // for(unsigned int i = 0; i<nombre_result; i++)
    //     printf("rows[%u]=%u\tvalues[%u]=%f\n", i, A->rows[i], i, A->values[i]);

    // for(unsigned int i = 0; i<nombre_joueur_different; i++)
    //     printf("joueur[%u] = %u\n", i, tab_joueur_different[i]);

    // printf("nbr j  %d\n", nombre_joueur_different);
    // for(int k = 0; k<20; k++){
    //     printf("%d \n", tab_joueur_different[k]);
    // }
    return tab_matrice;
}


int recherche_dico(unsigned int* tab, unsigned int elem, unsigned int taille){
    if(taille==0)
        return -1;
    unsigned int born_sup = taille -1;
    unsigned int born_inf = 0;
    unsigned int centre;

    while(born_sup>=born_inf){
        centre = (born_inf + born_sup) / 2;
        if(tab[centre] > elem){
            if(!centre)
                return -1;
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
