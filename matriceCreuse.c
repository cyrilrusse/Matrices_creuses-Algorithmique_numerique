#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "matriceCreuse.h"

struct creuse_t
{
    unsigned int *startCol;
    int *rows;
    float *values;
    unsigned int nz;
    unsigned int taille_startCol;
    unsigned int *matricule_colonne;
    unsigned int nombre_joueur_different;
    unsigned int *matricule_different;
};

static void quicksort2(unsigned int *array1, unsigned int *array2, int p, int r);

static int partition2(unsigned int *array1, unsigned int *array2, int p, int r);

static void swap(unsigned int *array, int a, int b);

static void quicksort(unsigned int *array1, int p, int r);

static int partition(unsigned int *array1, int p, int r);

static int recherche_dico(unsigned int *tab, unsigned int elem, unsigned int taille);

static Creuse *produitMatricesCreuses(Creuse *matrice1, Creuse *matrice2);

// static unsigned int *union_tab(unsigned int *tab1, unsigned int *tab2, int taille_tab1, int taille_tab2);

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

int recherche_dico(unsigned int *tab, unsigned int elem, unsigned int taille)
{
    if (taille == 0)
        return -1;
    unsigned int born_sup = taille - 1;
    unsigned int born_inf = 0;
    unsigned int centre;

    while (born_sup >= born_inf){
        centre = (born_inf + born_sup) / 2;
        if (tab[centre] > elem){
            if (!centre)
                return -1;
            born_sup = centre - 1;
        }
        else if (tab[centre] < elem){
            born_inf = centre + 1;
        }
        else
            return centre;
    }
    return -1;
}

unsigned int *union_tab(unsigned int *tab1, unsigned int *tab2, unsigned int *taille_tab1, unsigned int taille_tab2){
    
    unsigned int nombre_elem_dif = *taille_tab1;
    printf("bite\n");
    for(unsigned int i = 0; i<taille_tab2; i++){
        if(recherche_dico(tab1, tab2[i], *taille_tab1)==-1){
            nombre_elem_dif++;
            printf("nombre elem:%u, %u\n", nombre_elem_dif, tab2[i]);
        }
    }

    unsigned int *tab_union = malloc(sizeof(unsigned int)*nombre_elem_dif);
    if(!tab_union)
        return NULL;

    unsigned int indice_tab1=0, indice_tab2=0, indice_tab=0;
    while ((indice_tab1<*taille_tab1 || indice_tab2<taille_tab2) && indice_tab<nombre_elem_dif){
        if(tab1[indice_tab1]==tab2[indice_tab2]){
            tab_union[indice_tab] = tab1[indice_tab1];
            indice_tab1++;
            indice_tab2++;
        }
        else if(tab1[indice_tab1]<tab2[indice_tab2]){
            tab_union[indice_tab] = tab1[indice_tab1];
            indice_tab1++;
        }
        else if (tab1[indice_tab1]>tab2[indice_tab2]){
            tab_union[indice_tab] = tab2[indice_tab2];
            indice_tab2++;
        }
        indice_tab++;
    }

    *taille_tab1 = nombre_elem_dif;
    
    return tab_union;
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
        while (index_gagnant != nombre_joueur_gagnant){
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


    //transformation tableau de matricule des colonnes en tableau répertoriant l'indice du matricule dans l'index
    for(unsigned int i = 0; i<nombre_joueur_gagnant; i++)
        tab_joueur_gagnant[i] = recherche_dico(tab_joueur_different, tab_joueur_gagnant[i], nombre_joueur_different);

    //création de la matrice

    Creuse *M = malloc(sizeof(Creuse));
    if(!M)
        return NULL;
    M->matricule_colonne = tab_joueur_gagnant;
    M->matricule_different = tab_joueur_different;
    M->nz = nombre_result;
    M->startCol = malloc(sizeof(unsigned int) * nombre_joueur_gagnant);
    M->rows = malloc(sizeof(unsigned int) * nombre_result);
    M->values = malloc(sizeof(float)*nombre_result);
    M->taille_startCol = nombre_joueur_gagnant;
    M->nombre_joueur_different = nombre_joueur_different;


    Creuse *A = malloc(sizeof(Creuse));
    if(!A)
        return NULL;
    A->matricule_colonne = tab_joueur_gagnant;
    A->matricule_different = tab_joueur_different;
    A->nz = nombre_result;
    A->startCol = malloc(sizeof(unsigned int) * nombre_joueur_gagnant);
    A->rows = malloc(sizeof(unsigned int) * nombre_result);
    A->values = malloc(sizeof(float) * nombre_result);
    A->taille_startCol = nombre_joueur_gagnant;
    A->nombre_joueur_different = nombre_joueur_different;

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

    transpose(A);
    transpose(M);

    tab_matrice[0] = A;
    tab_matrice[1] = M;

    // for(unsigned int i=0; i<nombre_joueur_gagnant; i++){
    //     printf("StartCol[%u]=%u\n", i, A->startCol[i]);
    // }
    // for(unsigned int i = 0; i<nombre_result; i++)
    //     printf("rows[%u]=%u\tvalues[%u]=%f\n", i, A->rows[i], i, A->values[i]);

    // for(unsigned int i = 0; i<nombre_joueur_different; i++)
    //     printf("joueur[%u] = %u\n", i, tab_joueur_different[i]);

    // for(unsigned int i = 0; i<nombre_result; i++)
    //     printf("tab1[%u]:%u\ttab2[%u]:%u\n",i, tab_j1[i], i, tab_j2[i]);

    // printf("nbr j  %d\n", nombre_joueur_different);
    // for(int k = 0; k<20; k++){
    //     printf("%d \n", tab_joueur_different[k]);
    // }
    return tab_matrice;
}

int transpose(Creuse *matrice){
    Creuse *matrice_transposee;
    unsigned int *occurence_ligne, nombre_ligne = 0, *starCol_transpose;

    matrice_transposee = malloc(sizeof(Creuse));
    if(!matrice_transposee)
        return -1;
    occurence_ligne = calloc(matrice->nombre_joueur_different, sizeof(unsigned int));
    if(!occurence_ligne){
        free(matrice_transposee);
        return -1;
    }


    for(unsigned int i = 0; i<matrice->nz; i++)
        occurence_ligne[matrice->rows[i]]++;

    for (unsigned int i = 0; i < matrice->nombre_joueur_different; i++){
        if(occurence_ligne[i]!=0)
            nombre_ligne++;
    }

    unsigned int *rowcount = malloc(sizeof(unsigned int)*nombre_ligne);
    if(!rowcount){
        free(matrice_transposee);
        free(occurence_ligne);
        return -1;
    }

    starCol_transpose = malloc(sizeof(unsigned int)*nombre_ligne);
    if(!starCol_transpose){
        free(matrice_transposee);
        free(occurence_ligne);
        free(rowcount);
        return -1;
    }

    unsigned int *matricule_starCol_transpose = malloc(sizeof(unsigned int)*nombre_ligne);
    if(!matricule_starCol_transpose){
        free(matrice_transposee);
        free(occurence_ligne);
        free(starCol_transpose);
        free(rowcount);
        return -1;
    }
    
    for(unsigned int i = 0, j=0; i<nombre_ligne; i++){
        while (occurence_ligne[j]==0) j++;
        rowcount[i]=occurence_ligne[j];
        matricule_starCol_transpose[i] = j;
        j++;
    }

    matrice_transposee->startCol = starCol_transpose;
    matrice_transposee->startCol[0] = 0;

    for(unsigned int i = 1, j=0; i<nombre_ligne; i++){
        while (occurence_ligne[j]==0) j++;
        
        matrice_transposee->startCol[i] = matrice_transposee->startCol[i-1]+occurence_ligne[j];
        j++;
    }

    matrice_transposee->matricule_colonne = matricule_starCol_transpose;
    matrice_transposee->taille_startCol = nombre_ligne;

    // for(unsigned int i = 0; i<matrice_transposee->taille_startCol; i++){
    //     printf("starCol[%d]:%u\tmatriculecolTranspose[%u]:%u\tmatriculecol[%u]:%u\n", i, matrice_transposee->startCol[i], i, matrice_transposee->matricule_colonne[i], i, matrice->matricule_colonne[i]);
    // }

    matrice_transposee->rows = malloc(sizeof(int)*matrice->nz);
    if(!matrice_transposee->rows){
        free(matrice_transposee);
        free(occurence_ligne);
        free(starCol_transpose);
        free(rowcount);
        free(matricule_starCol_transpose);
        return -1;
    }
    
    for(unsigned int i = 0; i<matrice->nz; i++)
        matrice_transposee->rows[i] = -1;

    matrice_transposee->values = malloc(sizeof(float)*matrice->nz);
    if(!matrice_transposee->values){
        free(matrice_transposee->rows);
        free(matrice_transposee);
        free(occurence_ligne);
        free(starCol_transpose);
        free(rowcount);
        free(matricule_starCol_transpose);
    }

    unsigned int indice_Startcol_transposee = 0, indice_rows_transposee = 0, indice_Startcol = 0;
    for(unsigned int i = 0; i<matrice->nz; i++){

        if(indice_Startcol<matrice->taille_startCol && matrice->startCol[indice_Startcol+1]==i)
            indice_Startcol++;

        indice_Startcol_transposee = recherche_dico(matricule_starCol_transpose, matrice->rows[i], matrice_transposee->taille_startCol);
        indice_rows_transposee = matrice_transposee->startCol[indice_Startcol_transposee];
        while (matrice_transposee->rows[indice_rows_transposee] != -1) indice_rows_transposee++;
        


        matrice_transposee->rows[indice_rows_transposee] = matrice->matricule_colonne[indice_Startcol];
        matrice_transposee->values[indice_rows_transposee] = matrice->values[i];
    }

    // for(unsigned int i = 0; i<matrice->nz; i++)
    //     printf("Mt[%u]:%d\tvalues:%f\n", i, matrice_transposee->rows[i], matrice_transposee->values[i]);

    
    return 0;
}

float *produitMatriceVecteurDense(Creuse *matrice, float *vecteur_dense){
    unsigned int indice_Startcol = 0;
    
    if(matrice==NULL || vecteur_dense==NULL)
        return NULL;
    float *resulat_produit = calloc(matrice->nombre_joueur_different, sizeof(unsigned int));
    if(!resulat_produit)
        return NULL;

    for(unsigned int i = 0; i<matrice->nz; i++){
        if (indice_Startcol < matrice->taille_startCol && matrice->startCol[indice_Startcol + 1] == i)
            indice_Startcol++;

        resulat_produit[matrice->rows[i]] += vecteur_dense[matrice->matricule_colonne[indice_Startcol]] * matrice->values[i];
    }

    
    return resulat_produit;
}

Creuse *produitMatricesCreuses(Creuse *matrice1, Creuse *matrice2){

}