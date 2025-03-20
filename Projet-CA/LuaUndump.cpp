#include "LuaUndump.hpp"

std::vector<uint8_t> LuaUndump::loadBlock(size_t sz) {
    if (index + sz > bytecode.size()) {
        throw std::runtime_error("Malformed bytecode!");
    }
    std::vector<uint8_t> temp(bytecode.begin() + index, bytecode.begin() + index + sz);
    index += sz;
    return temp;
}

uint8_t LuaUndump::get_byte() {
    return loadBlock(1)[0];
}

uint32_t LuaUndump::get_uint32() {
    std::vector<uint8_t> data = loadBlock(4);
    uint32_t result;
    if (big_endian) {
        result = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    } else {
        result = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
    }
    return result;
}

uint64_t LuaUndump::get_uint(size_t size) {
    std::vector<uint8_t> data = loadBlock(size);
    uint64_t result = 0;
    if (big_endian) {
        for (size_t i = 0; i < size; i++) {
            result = (result << 8) | data[i];
        }
    } else {
        for (size_t i = 0; i < size; i++) {
            result |= (uint64_t(data[i]) << (8 * i));
        }
    }
    return result;
}

double LuaUndump::get_double() {
    std::vector<uint8_t> data = loadBlock(l_number_size);
    double result;
    std::memcpy(&result, data.data(), sizeof(double));
    return result;
}



std::string LuaUndump::get_string() {
    size_t size = get_uint(size_t_size);
    if (size == 0) return "";  // Retourne une chaîne vide, pas un pointeur NULL
    
    std::vector<uint8_t> data = loadBlock(size);
    if (data.empty()) return "";  // Vérification supplémentaire

    return std::string(data.begin(), data.end() - 1);  // Exclut le NULL terminator
}


Chunk LuaUndump::decode_chunk() {
    Chunk chunk;
    chunk.name = get_string();
    chunk.first_line = get_uint(int_size);
    chunk.last_line = get_uint(int_size);
    chunk.numUpvals = get_byte();
    chunk.numParams = get_byte();
    chunk.isVarg = (get_byte() != 0);
    chunk.maxStack = get_byte();

    uint32_t num_instr = get_uint(int_size);

    for (uint32_t i = 0; i < num_instr; i++) {
        chunk.addInstruction(decode_instr(get_uint32()));
    }

    uint32_t num_constants = get_uint(int_size);

    for (uint32_t i = 0; i < num_constants; i++) {
               

        Constant constant= Constant(ConstType::NIL, "");

        uint8_t type = get_byte();
        switch (type) {
            case 0:  constant = Constant(ConstType::NIL, nullptr); break;
            case 1:  constant = Constant(ConstType::BOOL, get_byte() != 0); break;
            case 3:  constant = Constant(ConstType::NUMBER, get_double()); break;
            case 4:  constant = Constant(ConstType::STRING, get_string()); break;
            default: throw std::runtime_error("Unknown Datatype!");
        }
        chunk.addConstant(constant);
    }



    uint32_t num_protos = get_uint(int_size);
    for (uint32_t i = 0; i < num_protos; i++) {
        chunk.addProto(decode_chunk());
    }

    uint32_t num_lines = get_uint(int_size);
    for (uint32_t i = 0; i < num_lines; i++) {
        get_uint(int_size);
    }

    uint32_t num_locals = get_uint(int_size);
    for (uint32_t i = 0; i < num_locals; i++) {
        std::string name = get_string();
        uint32_t start = get_uint(int_size);
        uint32_t end = get_uint(int_size);
        chunk.addLocal(Local(name, start, end));
    }

    uint32_t num_upvalues = get_uint(int_size);
    for (uint32_t i = 0; i < num_upvalues; i++) {
        chunk.addUpval(get_string());
    }



    return chunk;
}

Chunk LuaUndump::decode_bytecode(const std::vector<uint8_t>& rawbytecode) {
    bytecode = rawbytecode;

    if (bytecode.size() < 4 || std::memcmp(bytecode.data(), "\x1bLua", 4) != 0) {
        throw std::runtime_error("Lua Bytecode expected!");
    }

    index = 4;

    uint8_t vm_version = get_byte();
    uint8_t bytecode_format = get_byte();

    big_endian = (get_byte() == 0);

    int_size = get_byte();

    size_t_size = get_byte();

    instr_size = get_byte();

    l_number_size = get_byte();

    integral_flag = get_byte();

    rootChunk = decode_chunk();

    return rootChunk;
}

Chunk LuaUndump::loadFile(const std::string& luaCFile) {
    std::ifstream file(luaCFile, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Failed to open file: " + luaCFile);
    }

    std::vector<uint8_t> bytecode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return decode_bytecode(bytecode);
}

void LuaUndump::print_dissassembly() {
    rootChunk.print();
}
