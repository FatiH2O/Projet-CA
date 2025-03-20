#include "Chunk.hpp"
#include "Instruction.hpp"


void Chunk::addInstruction(const Instruction& instr) {
    instructions.push_back(instr);
}

void Chunk::addConstant(const Constant& constant) {
    constants.push_back(constant);
}

void Chunk::addProto(const Chunk& proto) {
    protos.push_back(proto);
}

void Chunk::addLine(int line) {
    lineNums.push_back(line);
}

void Chunk::addLocal(const Local& local) {
    locals.push_back(local);
}

void Chunk::addUpval(const std::string& upval) {
    upvalues.push_back(upval);
}

const Local* Chunk::findLocal(int pc) const {
    for (const auto& local : locals) {
        if (local.start <= pc && local.end >= pc) {
            return &local;
        }
    }
    return nullptr; // Pas trouvé
}

const Constant& Chunk::getConstant(int index) const {
    return constants.at(index);
}

void Chunk::print() const {
    std::cout << "\n==== [[" << name << "'s constants]] ====\n";
    for (size_t i = 0; i < constants.size(); i++) {
        std::cout << i << ": " << constants[i].toString() << "\n";
    }

    std::cout << "\n==== [[" << name << "'s locals]] ====\n";
    for (size_t i = 0; i < locals.size(); i++) {
        std::cout << "R[" << i << "]: " << locals[i].name << "\n";
    }

    std::cout << "\n==== [[" << name << "'s disassembly]] ====\n";
    for (size_t i = 0; i < instructions.size(); i++) {
        std::cout << "[" << i << "] " 
                  << instructions[i].toString() 
                  << " ; " << instructions[i].getAnnotation(*this) << "\n";
    }

    if (!protos.empty()) {
        std::cout << "\n==== [[" << name << "'s protos]] ====\n";
        for (const auto& proto : protos) {
            proto.print();
        }
    }
}
