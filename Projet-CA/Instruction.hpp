#pragma once

#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Chunk.hpp"
#include "classe.hpp"
class Chunk;

enum class InstructionType {
    ABC,
    ABx,
    AsBx
};

class Instruction {
public:
    InstructionType type;
    std::string name;
    int opcode;
    int A, B, C;

    Instruction(InstructionType type, const std::string& name);

    std::string formatRK(int rk) const;
    std::string toString() const;
    std::string getAnnotation(const Chunk& chunk) const;

private:
    bool whichRK(int rk) const;
    int readRKasK(int rk) const;
    bool isRKBCInstr(int opcode) const;
    bool isRKCInstr(int opcode) const;
    bool isKBx(int opcode) const;
};
