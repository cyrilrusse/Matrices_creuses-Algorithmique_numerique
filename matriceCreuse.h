typedef struct creuse_t Creuse;

Creuse **lecture(char* nom_fichier);

Creuse *transpose(Creuse *matrice);

float *produitMatriceVecteurDense(Creuse *matrice, float *vecteur_dense);

// unsigned int *union_tab(unsigned int *tab1, unsigned int *tab2, unsigned int *taille_tab1, unsigned int taille_tab2);

Creuse *produitMatriceVecteurCreux(Creuse *matrice, Creuse *vecteur_creux);

void libereCreuse(Creuse *matrice, int avec_index, int avec_matricule_colonne);

float norme2_VecteurCreux(Creuse *vecteur);

float norme2_vecteurDense(float *vecteur, unsigned int taille);

float *puissance(Creuse *matrice);

unsigned int get_nombre_joueur_different(Creuse *matrice);