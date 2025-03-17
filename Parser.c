#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    MOVE=0,
    LOADK=1,
    LOADBOOL=2,
    LOADNIL=3,
    GETUPVAL=4,
    GETGLOBAL=5,
    GETTABLE=6,
    SETGLOBAL=7,
    SETUPVAL=8,
    SETTABLE=9,
    NEWTABLE=10,
    SELF=11,
    ADD=12,
    SUB=13,
    MUL=14,
    DIV=15,
    MOD=16,
    POW=17,
    UNM=18,
    NOT=19,
    LEN=20,
    CONCAT=21,
    JMP=22,
    EQ=23,
    LT=24,
    LE=25,
    TEST=26,
    TESTSET=27,
    CALL=28,
    TAILCALL=29,
    RETURN=30,
    FORLOOP=31,
    FORPREP=32,
    TFORLOOP=33,
    SETLIST=34,
    CLOSE=35,
    CLOSURE=36,
    VARARG=37,
}opcode;

typedef enum {
    VARARG_NEEDSARG=4,
    VARARG_ISVARARG=2,
    VARARG_HASARG=1,    
}is_vararg_flag;
/*****************************************
 *         Lire le fichier            
 * ***************************************/ 



/*****************************************
 *         Parsing du header           
 * ***************************************/ 

typedef struct Lua_header {

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
}Lua_header;

// Passer la structure par référence pour modifier directement les valeurs
void parsing_header(unsigned char* tab, Lua_header* header) {

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



/*****************************************
 *     Parsing des Function blocks      
 * ***************************************/ 


typedef struct String{    
    long long unsigned int data_size;//pour etre sur qu'il soit capable de stocker SIZE_t donné par header
    char *data;
}String;


 

typedef struct List_instr{
    int size;

}List_instr;


typedef struct Lua_Fonction_block{
    String source_name;
    void *line_difined;              //void car taille inconnu pour le moment, donné par le header
    void *last_line_defined;
    long long unsigned int number_of_upvalues;
    long long unsigned int number_of_parameters;
    long long unsigned int vararg_flag;
    long long unsigned int max_stack_size;
}Lua_Fonction_block;

typedef struct Lua_bytcode
{
    Lua_header Lua_header;
    Lua_Fonction_block Lua_Fonction_block;
    
}Lua_bytcode;


// Lire le tableau à partir de l'indice 12 car le hearder est de 12 octets.
 void parsing_function_block(unsigned char* tab, Lua_bytcode* Lua_bytcode){

   char hexStr[Lua_bytcode->Lua_header.size_of_size_t];

   for(int i=12, j=0; i< 12+Lua_bytcode->Lua_header.size_of_size_t; i++){
    printf("HELLO %llu\n",  tab[i]);
            hexStr[j++]= tab[i];
            printf("ffHELLO %llu\n",  tab[i]);

     }

   uint64_t hexvalue= strtoll(hexStr, NULL, 16);

   Lua_bytcode->Lua_Fonction_block.source_name.data_size=hexvalue;


   printf("Lua_bytcode->Lua_Fonction_block.source_name.data_size: %llu\n",  Lua_bytcode->Lua_Fonction_block.source_name.data_size);
   printf("Lua_bytcode->Lua_Fonction_block.source_name.data_size: %s\n",  Lua_bytcode->Lua_Fonction_block.source_name.data_size);
   printf("hexvalue: %llu\n",  hexvalue);
   printf("hexvalue: %s\n",  hexvalue);

 }