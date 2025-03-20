#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "Instruction.hpp"
#include "Chunk.hpp"
#include "Const.hpp"

class LuaUndump {
private:
    Chunk rootChunk;
    std::vector<uint8_t> bytecode;
    size_t index = 0;
    bool big_endian;
    uint8_t int_size, size_t_size, instr_size, l_number_size, integral_flag;

    std::vector<uint8_t> loadBlock(size_t sz);
    uint8_t get_byte();
    uint32_t get_uint32();
    uint64_t get_uint(size_t size);
    double get_double();
    std::string get_string();
    Chunk decode_chunk();

public:
    Chunk decode_bytecode(const std::vector<uint8_t>& rawbytecode);
    Chunk loadFile(const std::string& luaCFile);
    void print_dissassembly();
};
