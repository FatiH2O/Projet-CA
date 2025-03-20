#pragma once

#include <cstdint>
#include <vector>
#include "Instruction.hpp"
class Instruction;


enum class Opcodes : uint8_t {
    MOVE        = 0,
    LOADK       = 1,
    LOADBOOL    = 2,
    LOADNIL     = 3,
    GETUPVAL    = 4,
    GETGLOBAL   = 5,
    GETTABLE    = 6,
    SETGLOBAL   = 7,
    SETUPVAL    = 8,
    SETTABLE    = 9,
    NEWTABLE    = 10,
    SELF        = 11,
    ADD         = 12,
    SUB         = 13,
    MUL         = 14,
    DIV         = 15,
    MOD         = 16,
    POW         = 17,
    UNM         = 18,
    NOT         = 19,
    LEN         = 20,
    CONCAT      = 21,
    JMP         = 22,
    EQ          = 23,
    LT          = 24,
    LE          = 25,
    TEST        = 26,
    TESTSET     = 27,
    CALL        = 28,
    TAILCALL    = 29,
    RETURN      = 30,
    FORLOOP     = 31,
    FORPREP     = 32,
    TFORLOOP    = 33,
    SETLIST     = 34,
    CLOSE       = 35,
    CLOSURE     = 36,
    VARARG      = 37
};


// Liste des opcodes utilisant RK ou KBx
extern const std::vector<Opcodes> RKBCInstr;
extern const std::vector<Opcodes> RKCInstr;
extern const std::vector<Opcodes> KBx;

// Lua Magic Number
extern const uint8_t LUAMAGIC[4];

// Vérifie si une valeur RK est un K (constante)
bool whichRK(int rk);

// Lire une valeur RK en tant que K
int readRKasK(int rk);

// Fonction pour extraire des bits à partir d'un nombre
uint32_t get_bits(uint32_t num, int p, int s);

// Fonction pour insérer des bits dans un nombre
uint32_t set_bits(uint32_t num, uint32_t data, int p, int s);

// Table des instructions
extern std::vector<Instruction> instr_lookup_tbl;

// Fonction pour décoder une instruction à partir de son encodage binaire
Instruction decode_instr(uint32_t data);

// Fonction pour encoder une instruction en uint32_t
uint32_t encode_instr(const Instruction& instr);
