#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
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


static void free_echec_allocation_memoire(int value, Creuse **tab_matrice, unsigned int *tabj1, unsigned int *tabj2, unsigned int *tab_joueur_gagnant, unsigned int *tab_joueur_jamais_gagne, unsigned int *tab_joueur_dif, unsigned int *tab_victoire);

static void free_matrice_echec_allocation_memoire(int value_matrice, Creuse *M);

static float norme2_VecteurCreux(Creuse *vecteur);

static float norme2_vecteurDense(float *vecteur, unsigned int taille);



unsigned int getNombreJoueurDifferent(Creuse *matrice){
    return matrice->nombre_joueur_different;
}

void free_echec_allocation_memoire(int value, Creuse **tab_matrice, unsigned int *tabj1, unsigned int *tabj2, unsigned int *tab_joueur_gagnant, unsigned int *tab_joueur_jamais_gagne, unsigned int *tab_joueur_dif, unsigned int *tab_victoire)
{
    if(value >= 0)
        free(tab_matrice);
    if(value >= 1)
        free(tabj1);
    if (value >= 2)
        free(tabj2);
    if (value >= 3)
        free(tab_joueur_gagnant);
    if (value >= 4)
        free(tab_joueur_jamais_gagne);
    if (value >= 5)
        free(tab_joueur_dif);
    if (value >= 6)
        free(tab_victoire);
}

void free_matrice_echec_allocation_memoire(int value_matrice, Creuse *M){
    if(value_matrice>=1)
        free(M->startCol);
    if(value_matrice>=2)
        free(M->rows);
    if(value_matrice>=3)
        free(M->values);
    if(value_matrice>=0)
        free(M);
}

Creuse **lecture(char *nom_fichier){
    Creuse **tab_matrice = malloc(sizeof(Creuse*)*2);
    if(!tab_matrice)
        return NULL;
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
    if(!tab_j1){
        free_echec_allocation_memoire(0, tab_matrice, NULL, NULL, NULL, NULL, NULL, NULL);
        return NULL;
    }
    unsigned int *tab_j2 = malloc(sizeof(unsigned int)*nombre_result);
    if(!tab_j2){
        free_echec_allocation_memoire(1, tab_matrice, tab_j1, NULL, NULL, NULL, NULL, NULL);
        return NULL;
    }

    if (!(fichier = fopen(nom_fichier, "r"))){
        free_echec_allocation_memoire(2, tab_matrice, tab_j1, tab_j2, NULL, NULL, NULL, NULL);
        return NULL;
    }

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

    fclose(fichier);

    unsigned int *tab_joueur_gagnant = malloc(sizeof(unsigned int)*nombre_result);
    if(!tab_joueur_gagnant){
        free_echec_allocation_memoire(2, tab_matrice, tab_j1, tab_j2, NULL, NULL, NULL, NULL);
        return NULL;
    }

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
    if (!tab_joueur_jamais_gagne){
        free_echec_allocation_memoire(3, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, NULL, NULL, NULL);
        return NULL;
    }

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
    if (!tab_joueur_different){
        free_echec_allocation_memoire(4, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, NULL, NULL);
        return NULL;
    }

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
    if (!tab_victoire)
    {
        free_echec_allocation_memoire(5, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, NULL);
        return NULL;
    }

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
    if (!M)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        return NULL;
    }

    M->matricule_colonne = tab_joueur_gagnant;
    M->matricule_different = tab_joueur_different;
    M->nz = nombre_result;
    M->startCol = malloc(sizeof(unsigned int) * nombre_joueur_gagnant);
    if (!M->startCol)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        free_matrice_echec_allocation_memoire(0, M);
        return NULL;
    }
    M->rows = malloc(sizeof(int) * nombre_result);
    if (!M->rows)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        free_matrice_echec_allocation_memoire(1, M);
        return NULL;
    }
    M->values = malloc(sizeof(float)*nombre_result);
    if (!M->values)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        free_matrice_echec_allocation_memoire(2, M);
        return NULL;
    }
    M->taille_startCol = nombre_joueur_gagnant;
    M->nombre_joueur_different = nombre_joueur_different;


    Creuse *A = malloc(sizeof(Creuse));
    if (!A)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        free_matrice_echec_allocation_memoire(3, M);
        return NULL;
    }

    A->matricule_colonne = tab_joueur_gagnant;
    A->matricule_different = tab_joueur_different;
    A->nz = nombre_result;
    A->startCol = malloc(sizeof(unsigned int) * nombre_joueur_gagnant);
    if (!A->startCol)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        free_matrice_echec_allocation_memoire(3, M);
        free_matrice_echec_allocation_memoire(0, A);
        return NULL;
    }
    A->rows = malloc(sizeof(int) * nombre_result);
    if (!A->rows)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        free_matrice_echec_allocation_memoire(3, M);
        free_matrice_echec_allocation_memoire(1, A);
        return NULL;
    }
    A->values = malloc(sizeof(float) * nombre_result);
    if (!A->values)
    {
        free_echec_allocation_memoire(6, tab_matrice, tab_j1, tab_j2, tab_joueur_gagnant, tab_joueur_jamais_gagne, tab_joueur_different, tab_victoire);
        free_matrice_echec_allocation_memoire(3, M);
        free_matrice_echec_allocation_memoire(2, A);
        return NULL;
    }
    A->taille_startCol = nombre_joueur_gagnant;
    A->nombre_joueur_different = nombre_joueur_different;

    unsigned int index_col = 0;
    for(unsigned int i = 0; i<nombre_result;){
        M->startCol[index_col] = i;
        A->startCol[index_col] = i;
        for(unsigned int j = 0; j<tab_victoire[index_col]; j++){
            M->rows[i] = recherche_dico(tab_joueur_different, tab_j2[i], nombre_joueur_different);
            M->values[i] = 1;
            A->rows[i] = M->rows[i];
            A->values[i] = 1./tab_victoire[index_col];
            i++;
        }
        index_col++;
    }

    tab_matrice[0] = transpose(A);
    tab_matrice[1] = transpose(M);

    libereCreuse(A, 0, 1);
    libereCreuse(M, 0, 0);

    free(tab_j1);
    free(tab_j2);
    free(tab_victoire);
    free(tab_joueur_jamais_gagne);


    return tab_matrice;
}

void libereCreuse(Creuse *matrice, int avec_index, int avec_matricule_colonne){
    if(avec_matricule_colonne && matrice->matricule_colonne)
        free(matrice->matricule_colonne);
    if(matrice->startCol)
        free(matrice->startCol);
    if(matrice->rows)
        free(matrice->rows);
    if (matrice->values)
        free(matrice->values);
    if (avec_index && matrice->matricule_different)
        free(matrice->matricule_different);
    free(matrice);
    matrice = NULL;
}

Creuse *transpose(Creuse *matrice){
    if(!matrice)
        return NULL;

    Creuse *matrice_transposee;
    unsigned int *occurence_ligne, nombre_ligne = 0;

    matrice_transposee = malloc(sizeof(Creuse));
    if(!matrice_transposee)
        return NULL;
    occurence_ligne = calloc(matrice->nombre_joueur_different, sizeof(unsigned int));
    if(!occurence_ligne){
        free(matrice_transposee);
        return NULL;
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
        return NULL;
    }

    matrice_transposee->startCol = malloc(sizeof(unsigned int)*nombre_ligne);
    if (!matrice_transposee->startCol)
    {
        free(matrice_transposee);
        free(occurence_ligne);
        free(rowcount);
        return NULL;
    }

    matrice_transposee->matricule_colonne = malloc(sizeof(unsigned int) * nombre_ligne);
    if(!matrice_transposee->matricule_colonne){
        free_matrice_echec_allocation_memoire(1, matrice_transposee);
        free(occurence_ligne);
        free(rowcount);
        return NULL;
    }

    matrice_transposee->rows = malloc(sizeof(int) * matrice->nz);
    if (!matrice_transposee->rows)
    {
        free(matrice_transposee->matricule_colonne);
        free_matrice_echec_allocation_memoire(1, matrice_transposee);
        free(occurence_ligne);
        free(rowcount);
        return NULL;
    }

    matrice_transposee->values = malloc(sizeof(float) * matrice->nz);
    if (!matrice_transposee->values)
    {
        free(matrice_transposee->matricule_colonne);
        free_matrice_echec_allocation_memoire(2, matrice_transposee);
        free(occurence_ligne);
        free(rowcount);
        return NULL;
    }

    matrice_transposee->startCol[0] = 0;
    matrice_transposee->taille_startCol = nombre_ligne;
    matrice_transposee->nombre_joueur_different = matrice->nombre_joueur_different;
    matrice_transposee->matricule_different = matrice->matricule_different;
    matrice_transposee->nz = matrice->nz;

    //initialisation des matricules correspondants aux colonnes
    for(unsigned int i = 0, j=0; i<nombre_ligne; i++){
        while (occurence_ligne[j]==0) j++;
        rowcount[i]=occurence_ligne[j];
        matrice_transposee->matricule_colonne[i] = j;
        j++;
    }

    //initialisation de startCol
    for(unsigned int i = 1, j=0; i<nombre_ligne; i++){
        while (occurence_ligne[j]==0) j++;
        
        matrice_transposee->startCol[i] = matrice_transposee->startCol[i-1]+occurence_ligne[j];
        j++;
    }

    //initialisation des lignes à -1 afin de savoir par après lesquelles ne sont pas encore remplies
    for(unsigned int i = 0; i<matrice->nz; i++)
        matrice_transposee->rows[i] = -1;


    //initialisation de rows et values
    unsigned int indice_Startcol_transposee = 0, indice_rows_transposee = 0, indice_Startcol = 0;
    for(unsigned int i = 0; i<matrice->nz; i++){

        if(indice_Startcol<matrice->taille_startCol && matrice->startCol[indice_Startcol+1]==i)
            indice_Startcol++;

        indice_Startcol_transposee = recherche_dico(matrice_transposee->matricule_colonne, matrice->rows[i], matrice_transposee->taille_startCol);
        indice_rows_transposee = matrice_transposee->startCol[indice_Startcol_transposee];
        while (matrice_transposee->rows[indice_rows_transposee] != -1) indice_rows_transposee++;
        
        matrice_transposee->rows[indice_rows_transposee] = matrice->matricule_colonne[indice_Startcol];
        matrice_transposee->values[indice_rows_transposee] = matrice->values[i];
    }

    free(rowcount);
    free(occurence_ligne);

    return matrice_transposee;
}

float *produitMatriceVecteurDense(Creuse *matrice, float *vecteur_dense){
    unsigned int indice_Startcol = 0;
    
    if(matrice==NULL || vecteur_dense==NULL)
        return NULL;
    float *resulat_produit = calloc(matrice->nombre_joueur_different, sizeof(unsigned int));
    if(!resulat_produit)
        return NULL;

    for(unsigned int i = 0; i<matrice->nz; i++){
        if (indice_Startcol < (matrice->taille_startCol-1) && matrice->startCol[indice_Startcol + 1] == i)
            indice_Startcol++;

        resulat_produit[matrice->rows[i]] += vecteur_dense[matrice->matricule_colonne[indice_Startcol]] * matrice->values[i];
    }

    
    return resulat_produit;
}

Creuse *produitMatriceVecteurCreux(Creuse *matrice, Creuse *vecteur_creux){
    if(!matrice || !vecteur_creux)
        return NULL;


    

    if(vecteur_creux->taille_startCol!=1)
        return NULL;
    
    Creuse *result_produit = malloc(sizeof(Creuse));
    if(!result_produit)
        return NULL;

    result_produit->taille_startCol = 1;
    result_produit->startCol = malloc(sizeof(unsigned int));
    if(!result_produit->startCol){
        free(result_produit);
        return NULL;
    }

    result_produit->startCol[0] = 0;
    result_produit->nombre_joueur_different = matrice->nombre_joueur_different;
    result_produit->matricule_different = matrice->matricule_different;

    //déterminer les éléments non nuls de result_produit
    int indice_col_matrice = 0;
    unsigned int taille_col = 0, taille_tab_ligne_nz = 0;
    unsigned int *ptaille_tab_ligne_nz = &taille_tab_ligne_nz;
    int *tab_ligne_non_nulle = NULL, *tab_ligne_intermediaire = NULL;
    for(unsigned int i=0; i<vecteur_creux->nz; i++){
        if ((indice_col_matrice = recherche_dico(matrice->matricule_colonne, vecteur_creux->rows[i], matrice->taille_startCol)) != -1){
            if((unsigned int)indice_col_matrice<(matrice->taille_startCol-1))
                taille_col = matrice->startCol[indice_col_matrice + 1] - matrice->startCol[indice_col_matrice];
            else
                taille_col = matrice->nz - matrice->startCol[indice_col_matrice];
        
            tab_ligne_intermediaire = malloc(sizeof(int)*taille_col);
            for(unsigned int j = 0; j<taille_col; j++){
                tab_ligne_intermediaire[j]=matrice->rows[indice_col_matrice+j];
            }

            tab_ligne_non_nulle = union_tab(tab_ligne_non_nulle, tab_ligne_intermediaire, ptaille_tab_ligne_nz, taille_col);
            if(tab_ligne_non_nulle!=tab_ligne_intermediaire)
                free(tab_ligne_intermediaire);
        }
    }


    result_produit->rows = tab_ligne_non_nulle;
    result_produit->nz = taille_tab_ligne_nz;


    result_produit->values = calloc(taille_tab_ligne_nz, sizeof(float));
    if(!result_produit->values){
        free(result_produit->startCol);
        free(result_produit);
        return NULL;
    }

    int indice_value = 0;
    for(unsigned int i = 0; i<vecteur_creux->nz; i++){
        if ((indice_col_matrice = recherche_dico(matrice->matricule_colonne, vecteur_creux->rows[i], matrice->taille_startCol)) != -1)
        {
            if ((unsigned int)indice_col_matrice < (matrice->taille_startCol - 1))
                taille_col = matrice->startCol[indice_col_matrice + 1] - matrice->startCol[indice_col_matrice];
            else
                taille_col = matrice->nz - matrice->startCol[indice_col_matrice];

            for(unsigned int j=0; j<taille_col; j++){
                indice_value = recherche_dico((unsigned int*)result_produit->rows, matrice->rows[matrice->startCol[indice_col_matrice]+j], result_produit->nz);
                result_produit->values[indice_value] += vecteur_creux->values[i] * matrice->values[matrice->startCol[indice_col_matrice + j]];
            }
        }
    }


    
    return result_produit;
}

float norme2_VecteurCreux(Creuse *vecteur)
{
    float norme = 0;
    for (unsigned int i = 0; i < vecteur->nz; i++)
        norme += vecteur->values[i] * vecteur->values[i];
    return sqrt(norme);
}

float norme2_vecteurDense(float *vecteur, unsigned int taille){
    float norme = 0;
    for (unsigned int i = 0; i < taille; i++)
        norme += vecteur[i] * vecteur[i];
    return sqrt(norme);
}

float *puissance(Creuse *matrice)
{
    Creuse *vecteur_propre_creux = malloc(sizeof(Creuse));
    if(!vecteur_propre_creux){
        free(matrice);
        return NULL;
        }

    float *w_dense = NULL;

    vecteur_propre_creux->startCol = malloc(sizeof(unsigned int));
    if (!vecteur_propre_creux->startCol)
    {
        free(vecteur_propre_creux);
        free(matrice);
        return NULL;
    }
    vecteur_propre_creux->startCol[0] = 0;
    vecteur_propre_creux->rows = malloc(sizeof(int));
    if (!vecteur_propre_creux->rows)
    {
        free_matrice_echec_allocation_memoire(1, vecteur_propre_creux);
        free(matrice);
        return NULL;
    }
    vecteur_propre_creux->rows[0] = 0;
    vecteur_propre_creux->values = malloc(sizeof(unsigned int));
    if (!vecteur_propre_creux->values)
    {
        free_matrice_echec_allocation_memoire(2, vecteur_propre_creux);
        free(matrice);
        return NULL;
    }
    vecteur_propre_creux->values[0] = 1;
    vecteur_propre_creux->nz = 1;
    vecteur_propre_creux->taille_startCol = 1;

    Creuse *w_creux = produitMatriceVecteurCreux(matrice, vecteur_propre_creux);
    if (!w_creux)
    {
        libereCreuse(vecteur_propre_creux, 0, 0);
        free(matrice);
        return NULL;
    }
    libereCreuse(vecteur_propre_creux, 0, 0);
    float norme_w = norme2_VecteurCreux(w_creux);
    float moyenne_pre = 0;
    float moyenne = 0;

    float *z = calloc(matrice->nombre_joueur_different, sizeof(float));
    if (!z)
    {
        libereCreuse(w_creux, 0, 0);
        libereCreuse(vecteur_propre_creux, 0, 0);
        free(matrice);
        return NULL;
    }
    for (unsigned int i = 0; i < w_creux->nz; i++)
    {
        z[w_creux->rows[i]] = w_creux->values[i]/norme_w;
        moyenne += w_creux->values[i];
    }
    moyenne/= w_creux->nz;
    do
    {
        moyenne_pre = moyenne;
        if(w_dense)
            free(w_dense);
        w_dense = produitMatriceVecteurDense(matrice, z);
        if (!w_dense)
        {
            free(z);
            libereCreuse(w_creux, 0, 0);
            libereCreuse(vecteur_propre_creux, 0, 0);
            free(matrice);
            return NULL;
        }
        norme_w = norme2_vecteurDense(w_dense, matrice->nombre_joueur_different);
        moyenne = 0;
        for (unsigned int i = 0; i < matrice->nombre_joueur_different; i++)
        {
            z[i] = w_dense[i]/norme_w;
            moyenne+= w_dense[i];
        }
        moyenne/= matrice->nombre_joueur_different;
    }while(fabs(moyenne - moyenne_pre)>0.0000001);


    float Valeur_E_V = 0;
    

    // for(unsigned int i = 0; i<matrice->nombre_joueur_different; i++)
    //     Valeur_E_V += w_dense[i]/matrice->nombre_joueur_different;    

    // for(unsigned int i = 0; i<matrice->nombre_joueur_different; i++)
    //     w_dense[i] += Valeur_E_V;

    printf("test: %f\n", w_dense[2]/z[2]);

    // libereCreuse(matrice, 0, 1);
    free(z);
    libereCreuse(w_creux, 0, 0);


    return w_dense;
}

unsigned int *getIndex(Creuse *matrice){
    return matrice->matricule_different;
}