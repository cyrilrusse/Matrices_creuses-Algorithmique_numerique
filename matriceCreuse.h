typedef struct creuse_t Creuse;
// typedef struct resultat_t Resultat;

Creuse **lecture(char* nom_fichier);

int transpose(Creuse *matrice);

float *produitMatriceVecteurDense(Creuse *matrice, float *vecteur_dense);

unsigned int *union_tab(unsigned int *tab1, unsigned int *tab2, unsigned int *taille_tab1, unsigned int taille_tab2);
