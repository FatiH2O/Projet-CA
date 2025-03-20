#include "Instruction.hpp"
#include "classe.hpp"

// Liste des opcodes utilisant RK ou KBx
const std::vector<Opcodes> RKBCInstr = { 
    Opcodes::SETTABLE, Opcodes::ADD, Opcodes::SUB, Opcodes::MUL, Opcodes::DIV, 
    Opcodes::MOD, Opcodes::POW, Opcodes::EQ, Opcodes::LT 
};

const std::vector<Opcodes> RKCInstr = { 
    Opcodes::GETTABLE, Opcodes::SELF 
};

const std::vector<Opcodes> KBx = { 
    Opcodes::LOADK, Opcodes::GETGLOBAL, Opcodes::SETGLOBAL 
};

// Lua Magic Number
const uint8_t LUAMAGIC[4] = { 0x1B, 'L', 'u', 'a' };

// Vérifie si une valeur RK est un K (constante)
bool whichRK(int rk) {
    return (rk & (1 << 8)) > 0;
}

// Lire une valeur RK en tant que K
int readRKasK(int rk) {
    return (rk & ~(1 << 8));
}

// Fonction pour extraire des bits à partir d'un nombre
uint32_t get_bits(uint32_t num, int p, int s) {
    return (num >> p) & (~((~0) << s));
}

// Fonction pour insérer des bits dans un nombre
uint32_t set_bits(uint32_t num, uint32_t data, int p, int s) {
    return (num & (~((~((~0) << s)) << p))) | ((data << p) & ((~((~0) << s)) << p));
}

// Table des instructions
std::vector<Instruction> instr_lookup_tbl = {
    {InstructionType::ABC, "MOVE"}, {InstructionType::ABx, "LOADK"}, {InstructionType::ABC, "LOADBOOL"},
    {InstructionType::ABC, "LOADNIL"}, {InstructionType::ABC, "GETUPVAL"}, {InstructionType::ABx, "GETGLOBAL"},
    {InstructionType::ABC, "GETTABLE"}, {InstructionType::ABx, "SETGLOBAL"}, {InstructionType::ABC, "SETUPVAL"},
    {InstructionType::ABC, "SETTABLE"}, {InstructionType::ABC, "NEWTABLE"}, {InstructionType::ABC, "SELF"},
    {InstructionType::ABC, "ADD"}, {InstructionType::ABC, "SUB"}, {InstructionType::ABC, "MUL"},
    {InstructionType::ABC, "DIV"}, {InstructionType::ABC, "MOD"}, {InstructionType::ABC, "POW"},
    {InstructionType::ABC, "UNM"}, {InstructionType::ABC, "NOT"}, {InstructionType::ABC, "LEN"},
    {InstructionType::ABC, "CONCAT"}, {InstructionType::AsBx, "JMP"}, {InstructionType::ABC, "EQ"},
    {InstructionType::ABC, "LT"}, {InstructionType::ABC, "LE"}, {InstructionType::ABC, "TEST"},
    {InstructionType::ABC, "TESTSET"}, {InstructionType::ABC, "CALL"}, {InstructionType::ABC, "TAILCALL"},
    {InstructionType::ABC, "RETURN"}, {InstructionType::AsBx, "FORLOOP"}, {InstructionType::AsBx, "FORPREP"},
    {InstructionType::ABC, "TFORLOOP"}, {InstructionType::ABC, "SETLIST"}, {InstructionType::ABC, "CLOSE"},
    {InstructionType::ABx, "CLOSURE"}, {InstructionType::ABC, "VARARG"}
};

// Fonction pour décoder une instruction à partir de son encodage binaire
Instruction decode_instr(uint32_t data) {
    uint32_t opcode = get_bits(data, 0, 6);
    Instruction instr = instr_lookup_tbl[opcode];  // Copie de l'instruction modèle

    instr.opcode = opcode;
    instr.A = get_bits(data, 6, 8);

    if (instr.type == InstructionType::ABC) {
        instr.B = get_bits(data, 23, 9);
        instr.C = get_bits(data, 14, 9);
    } else if (instr.type == InstructionType::ABx) {
        instr.B = get_bits(data, 14, 18);
    } else if (instr.type == InstructionType::AsBx) {
        instr.B = get_bits(data, 14, 18) - 131071; // Convertit en valeur signée
    }

    return instr;
}

// Fonction pour encoder une instruction en uint32_t
uint32_t encode_instr(const Instruction& instr) {
    uint32_t data = 0;

    data = set_bits(data, instr.opcode, 0, 6);
    data = set_bits(data, instr.A, 6, 8);

    if (instr.type == InstructionType::ABC) {
        data = set_bits(data, instr.B, 23, 9);
        data = set_bits(data, instr.C, 14, 9);
    } else if (instr.type == InstructionType::ABx) {
        data = set_bits(data, instr.B, 14, 18);
    } else if (instr.type == InstructionType::AsBx) {
        data = set_bits(data, instr.B + 131071, 14, 18);
    }

    return data;
}
