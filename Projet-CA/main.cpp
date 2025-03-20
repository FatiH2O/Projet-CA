

#include <iostream>
#include "LuaUndump.hpp"


// et le header??
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <fichier_lua_compilé.luac>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    try {
        LuaUndump luaUndumper;

       

        Chunk chunk = luaUndumper.loadFile(filename);

        std::cout << "=== Disassembly du fichier " << filename << " ===" << std::endl;
        luaUndumper.print_dissassembly();
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// g++ LuaUndump.cpp Chunk.cpp Instruction.cpp Const.cpp classe.cpp  main.cpp -o programme
//./programme luac.out
//luac test.lua


