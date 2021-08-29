#include "utile.h"

typedef struct creuse_t Creuse;

/**
 * Crée un tableau de 2 matrices, M et A.
 * 
 * PARAMETRES
 * nom_fichier  une chaine de charactere contenant le nom du fichier.
 * 
 * RETURN
 * matrice  un pointeur vers un tableau de 2 struct Creuse. Peut être 
 *          free à l'aide de 'libereCreuse'. Attention, l'index est commun à M et A.
 * 
 * NULL     en cas d'erreur.
 */
Creuse **lecture(char* nom_fichier);

/**
 * Crée un matrice étant la transposée de la matrice donnée en argument.
 * 
 * PARAMETRES
 * matrice  un pointeur vers Creuse, la matrice à transposer.
 * 
 * RETURN 
 * matrice_transposee   un pointeur vers une struct Creuse contenant la transposée de matrice.
 * 
 * NULL     en cas d'erreur.
 */
Creuse *transpose(Creuse *matrice);

/**
 * renvoit un pointeur vers un vecteur de float contenant 
 * le produit entre une matrice Creuse et un vecteur dense.
 * 
 * PARAMETRES
 * 
 * matrice  un pointeur vers une struct Creuse, la matrice avec laquelle faire 
 * le produit matriciel.
 * 
 * vecteur_dense    un pointeur vers un tableau de float, le vecteur dense avec lequel faire
 * le produit matriciel.
 * 
 * RETURN
 * 
 * result_prod  un pointeur vers un tableau de float, 
 * soit le vecteur resultant du produit entre matrice et vecteur_dense.
 * 
 * NULL         en cas d'erreur.
 * 
 */
float *produitMatriceVecteurDense(Creuse *matrice, float *vecteur_dense);

/**
 * renvoit un pointeur vers une struct Creuse contenant 
 * le produit entre une matrice Creuse et un vecteur creux.
 * 
 * PARAMETRES
 * 
 * matrice  un pointeur vers une struct Creuse, la matrice avec laquelle faire 
 * le produit matriciel.
 * 
 * vecteur_creux    un pointeur vers une struct Creuse, le vecteur creux avec lequel faire
 * le produit matriciel.
 * 
 * RETURN
 * 
 * result_prod  un pointeur vers une struct Creuse, 
 * soit le vecteur resultant du produit entre matrice et vecteur_creux.
 * 
 * NULL         en cas d'erreur.
 * 
 */
Creuse *produitMatriceVecteurCreux(Creuse *matrice, Creuse *vecteur_creux);

/**
 * libère la mémoire d'un pointeur vers Creuse
 * 
 * PARAMETRES
 * 
 * matrice  le pointeur à free
 * 
 * avec_index   !=0 pour également free l'index
 *              0 sinon
 *              
 * 
 * avec_matricule_colonne   !=0 pour également free l'index de matricules du vecteur Startcol
 *                          0 sinon
 * 
 */
void libereCreuse(Creuse *matrice, int avec_index, int avec_matricule_colonne);

/**
 * Calcule le vecteur propre associé à la valeur propre de plus grand module de matrice
 * à l'aide de la technique de la puissance.
 * 
 * PARAMETRES
 * 
 * matrice  un pointeur vers une struct Creuse, la matrice dont on va chercher un 
 * vecteur propre avec la technique de la puissance.
 * 
 * 
 * RETURN
 * 
 * un pointeur vers un vecteur de float qui contient le vecteur propre de la 
 * matrice trouvé à l'aide de la technique de la puissance.
 * 
 * NULL         en cas d'erreur.
 * 
 */
float *puissance(Creuse *matrice);

/**
 * Accesseur du champ matricule_different étant l'index de tous 
 * les matricules.
 * 
 * PARAMETRES
 * matrice  un pointeur vers une struct Creuse, la matrice dont on veut 
 * récupérer l'index.
 * 
 * RETURN
 * un pointeur vers l'index de matrice.
 * 
 * NULL     en cas d'erreur.
 * 
 */
unsigned int *getIndex(Creuse *matrice);

/**
 * Accesseur du champ nombre_joueur_different étant le nombre de joueur différent
 * 
 * PARAMETRES
 * matrice  un pointeur vers une struct Creuse, la matrice dont on veut 
 * récupérer cette valeur.
 * 
 * RETURN
 * un entier : la valeur du champ nombre_joueur_different
 * 
 */
unsigned int getNombreJoueurDifferent(Creuse *matrice);
