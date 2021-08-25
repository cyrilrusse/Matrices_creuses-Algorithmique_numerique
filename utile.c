#include <stdlib.h>

#include "utile.h"

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

int *union_tab(int *tab1, int *tab2, unsigned int *taille_tab1, unsigned int taille_tab2){

    unsigned int nombre_elem_dif = *taille_tab1;
    if(nombre_elem_dif==0){
        *taille_tab1 = taille_tab2;
        return tab2;
    }

    for(unsigned int i = 0; i<taille_tab2; i++){
        if(recherche_dico((unsigned int*)tab1, tab2[i], *taille_tab1)==-1)
            nombre_elem_dif++;
    }

    int *tab_union = malloc(sizeof(int)*nombre_elem_dif);
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

    free(tab1);
    return tab_union;
}