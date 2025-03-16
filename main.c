#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Parser.c"

int main() {
    // Charger le fichier et obtenir son contenu
    unsigned char* tab = charger_fichier("luac.out");

    // Créer une structure pour le header
    struct Lua_header header;

    // Appeler la fonction pour parser le header
    parsing_header(tab, &header);

    // Libérer la mémoire allouée pour le tableau 'tab'
    free(tab);

    return 0;
}
