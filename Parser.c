#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*****************************************
 *         Parsing du header             
 * ***************************************/ 

/*****************************************
 *         Lire le fichier            
 * ***************************************/ 

unsigned char* charger_fichier(const char* filepath) {
    unsigned char* tab = (unsigned char*)malloc(12 * sizeof(unsigned char));

    if (tab == NULL) {
        perror("Erreur allocation de mémoire");
        exit(0);
    }

    FILE* fichier_out = fopen(filepath, "rb");

    if (fichier_out == NULL) {
        perror("Erreur ouverture du fichier");
        fclose(fichier_out);
        exit(0);
    }

    size_t res = fread(tab, sizeof(unsigned char), 12, fichier_out);

    if (res != 12) {
        perror("Erreur lors du chargement dans le tableau");
        fclose(fichier_out);
        exit(0);
    }

    fclose(fichier_out);
    return tab;
}

/*****************************************
 *         Lire le header           
 * ***************************************/ 

struct Lua_header {

    unsigned char header_signature[4]; //signature de Lua sur 4 octets
    unsigned int header_sig;         // les 4 octets concaténé pour avoir la signature en entier
    unsigned char version_number;
    unsigned char format_version;
    unsigned char endianess_flag;
    unsigned char size_of_int;
    unsigned char size_of_size_t;
    unsigned char size_of_instructions;
    unsigned char size_of_lua_number;
    unsigned char integral_flag;

    bool fit_for_consumption;
};

// Passer la structure par référence pour modifier directement les valeurs
void parsing_header(unsigned char* tab, struct Lua_header* header) {

    header->fit_for_consumption= true;//initiaiser à true
    // Lecture de la signature
    for (int i = 0; i < 4; i++) {
        header->header_signature[i] = tab[i];
        printf("header sig: %02X\n", tab[i]);
    }
    
    header->header_sig= (header->header_signature[0] << 24)| (header->header_signature[1] << 16)
                                                           | (header->header_signature[2] << 8)
                                                           | header->header_signature[3] ;


    printf("header sig: %02X\n", header->header_sig);

    header->version_number = tab[4];
    printf("version_number: %u\n", tab[4]);

    header->format_version = tab[5];
    printf("format_version: %u\n", tab[5]);

    if (tab[6]==0| tab[6]==1){
        header->endianess_flag=tab[6];
    }else{
        header->fit_for_consumption= false;
    }
    printf("endianess_flag: %u\n", tab[6]);

    header->size_of_int = tab[7];
    printf("size_of_int: %u\n", tab[7]);

    header->size_of_size_t = tab[8];
    printf("size_of_size_t: %u\n", tab[8]);

    header->size_of_instructions = tab[9];
    printf("size_of_instructions: %u\n", tab[9]);

    header->size_of_lua_number = tab[10];
    printf("size_of_lua_number: %u\n", tab[10]);

    header->integral_flag = tab[11];
    printf("integral_flag: %u\n", tab[11]);
}

