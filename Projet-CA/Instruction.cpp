#include "Instruction.hpp"

Instruction::Instruction(InstructionType type, const std::string& name)
    : type(type), name(name), opcode(-1), A(0), B(0), C(0) {}

std::string Instruction::formatRK(int rk) const {
    if (whichRK(rk)) {
        return "K[" + std::to_string(readRKasK(rk)) + "]";
    } else {
        return "R[" + std::to_string(rk) + "]";
    }
}

std::string Instruction::toString() const {
    std::ostringstream oss;
    oss << std::setw(10) << name;
    std::string regs;

    if (type == InstructionType::ABC) {
        std::string A_str = std::to_string(A);
        std::string B_str = std::to_string(B);
        std::string C_str = std::to_string(C);

        if (isRKBCInstr(opcode)) {
            A_str = "R[" + std::to_string(A) + "]";
            B_str = formatRK(B);
            C_str = formatRK(C);
        } else if (isRKCInstr(opcode)) {
            A_str = "R[" + std::to_string(A) + "]";
            C_str = formatRK(C);
        }

        regs = A_str + " " + B_str + " " + C_str;
    } 
    else if (type == InstructionType::ABx || type == InstructionType::AsBx) {
        std::string A_str = "R[" + std::to_string(A) + "]";
        std::string B_str = std::to_string(B);

        if (isKBx(opcode)) {
            B_str = "K[" + std::to_string(B) + "]";
        }

        regs = A_str + " " + B_str;
    }

    return oss.str() + " : " + regs;
}

std::string Instruction::getAnnotation(const Chunk& chunk) const {
    switch (opcode) {
        case 0:
            return "move R[" + std::to_string(B) + "] into R[" + std::to_string(A) + "]";
        case 1:
            return "load " + chunk.getConstant(B).toCode() + " into R[" + std::to_string(A) + "]";
        case 5:
            return "move _G[" + chunk.getConstant(B).toCode() + "] into R[" + std::to_string(A) + "]";
        case 12:
            return "add " + formatRK(C) + " to " + formatRK(B) + ", place into R[" + std::to_string(A) + "]";
        case 13:
            return "sub " + formatRK(C) + " from " + formatRK(B) + ", place into R[" + std::to_string(A) + "]";
        case 14:
            return "mul " + formatRK(C) + " to " + formatRK(B) + ", place into R[" + std::to_string(A) + "]";
        case 15:
            return "div " + formatRK(C) + " from " + formatRK(B) + ", place into R[" + std::to_string(A) + "]";
        case 21: {
            int count = C - B + 1;
            return "concat " + std::to_string(count) + " values from R[" +
                   std::to_string(B) + "] to R[" + std::to_string(C) +
                   "], store into R[" + std::to_string(A) + "]";
        }
        default:
            return "";
    }
}

bool Instruction::whichRK(int rk) const {
    return (rk & (1 << 8)) > 0;
}

int Instruction::readRKasK(int rk) const {
    return (rk & ~(1 << 8));
}

bool Instruction::isRKBCInstr(int opcode) const {
    return opcode == 9 || opcode == 12 || opcode == 13 || opcode == 14 ||
           opcode == 15 || opcode == 16 || opcode == 17 || opcode == 23 || opcode == 24;
}

bool Instruction::isRKCInstr(int opcode) const {
    return opcode == 6 || opcode == 11;
}

bool Instruction::isKBx(int opcode) const {
    return opcode == 1 || opcode == 5 || opcode == 7;
}
