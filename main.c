#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Parser.c"

unsigned char* charger_fichier(const char* filepath) {
    
    FILE* fichier_out = fopen(filepath, "rb");

    //mettre la tete de lecture à la fin de fichier pour récuperer sa position avec ftell= taille du fichier
    fseek(fichier_out, 0, SEEK_END);
    
    
    long size = ftell(fichier_out);
    
    // Revenir au début du fichier
    rewind(fichier_out);

    if (fichier_out == NULL) {
        perror("Erreur ouverture du fichier");
        fclose(fichier_out);
        exit(0);
    }

    unsigned char* tab = (unsigned char*)malloc(size * sizeof(unsigned char));

    if (tab == NULL) {
        perror("Erreur allocation de mémoire");
        exit(0);
    }

    size_t res = fread(tab, sizeof(unsigned char), size, fichier_out);

    if (res != size) {
        perror("Erreur lors du chargement dans le tableau");
        fclose(fichier_out);
        exit(0);
    }

    fclose(fichier_out);
    return tab;
}


int main() {
    // Charger le fichier et obtenir son contenu
    unsigned char* tab = charger_fichier("luac.out");

    // Créer une structure pour le header
    Lua_bytcode Lua_bytcode;


    // Appeler la fonction pour parser le header
    parsing_header(tab, &Lua_bytcode.Lua_header);

    parsing_function_block(tab,&Lua_bytcode);
    // Libérer la mémoire allouée pour le tableau 'tab'
    free(tab);

    return 0;
}
