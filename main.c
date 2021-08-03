#include <stdio.h>
#include <stdlib.h>
#include "matriceCreuse.h"

int main(){
    char *nom_fichier = "aft.txt";

    Creuse **matrice = lecture(nom_fichier);
    
    return 0;
}