#pragma once

#include <string>
#include <sstream>
#include <variant>

enum class ConstType {
    NIL = 0,
    BOOL = 1,
    NUMBER = 3,
    STRING = 4
};

class Constant {
public:
    ConstType type;
    std::variant<std::string, bool, double> data; // Stocke les différents types de constantes

    Constant();
    Constant(ConstType type, std::variant<std::string, bool, double> data);

    std::string toString() const;
    std::string toCode() const;

private:
    std::string toTypeName() const;
    std::string toValueString() const;
};

class Local {
public:
    std::string name;
    int start;
    int end;

    Local(const std::string& name, int start, int end);
};
