#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Const.hpp"
#include "Instruction.hpp"
class Instruction;

class Local;  // Déclaration anticipée pour éviter les problèmes d'inclusion

class Chunk {
public:
    std::vector<Constant> constants;
    std::vector<Instruction> instructions;
    std::vector<Chunk> protos;

    std::string name = "Unnamed proto";
    int first_line = 0;
    int last_line = 0;
    int numUpvals = 0;
    int numParams = 0;
    bool isVarg = false;
    int maxStack = 0;

    std::vector<std::string> upvalues;
    std::vector<int> lineNums;
    std::vector<Local> locals;

    Chunk() = default;

    void addInstruction(const Instruction& instr);
    void addConstant(const Constant& constant);
    void addProto(const Chunk& proto);
    void addLine(int line);
    void addLocal(const Local& local);
    void addUpval(const std::string& upval);

    const Local* findLocal(int pc) const;
    const Constant& getConstant(int index) const;

    void print() const;
};
