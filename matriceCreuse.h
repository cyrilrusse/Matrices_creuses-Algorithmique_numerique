#include "utile.h"

typedef struct creuse_t Creuse;

Creuse **lecture(char* nom_fichier);

Creuse *transpose(Creuse *matrice);

float *produitMatriceVecteurDense(Creuse *matrice, float *vecteur_dense);

Creuse *produitMatriceVecteurCreux(Creuse *matrice, Creuse *vecteur_creux);

void libereCreuse(Creuse *matrice, int avec_index, int avec_matricule_colonne);

float *puissance(Creuse *matrice);

unsigned int *get_index(Creuse *matrice);

unsigned int get_nombre_joueur_different(Creuse *matrice);
