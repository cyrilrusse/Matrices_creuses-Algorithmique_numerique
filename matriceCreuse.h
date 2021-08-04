typedef struct creuse_t Creuse;
// typedef struct resultat_t Resultat;

Creuse **lecture(char* nom_fichier);

void quicksort2(unsigned int *array1, unsigned int *array2, int p, int r);

int partition2(unsigned int *array1, unsigned int *array2, int p, int r);

void swap(unsigned int *array, int a, int b);

void quicksort(unsigned int *array1, int p, int r);

int partition(unsigned int *array1, int p, int r);

int recherche_dico(unsigned int *tab, unsigned int elem, unsigned int taille);

int transpose(Creuse *matrice);