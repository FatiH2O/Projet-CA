#include "Const.hpp"


Constant::Constant(ConstType type, std::variant<std::string, bool, double> data)
    : type(type), data(data) {}

std::string Constant::toString() const {
    std::ostringstream oss;
    oss << "[" << toTypeName() << "] " << toValueString();
    return oss.str();
}

std::string Constant::toCode() const {
    switch (type) {
        case ConstType::STRING:
            return "\"" + std::get<std::string>(data) + "\"";
        case ConstType::BOOL:
            return std::get<bool>(data) ? "true" : "false";
        case ConstType::NUMBER:
            return std::to_string(std::get<double>(data));
        default:
            return "nil";
    }
}

std::string Constant::toTypeName() const {
    switch (type) {
        case ConstType::STRING: return "STRING";
        case ConstType::BOOL: return "BOOL";
        case ConstType::NUMBER: return "NUMBER";
        case ConstType::NIL: return "NIL";
        default: return "UNKNOWN";
    }
}

std::string Constant::toValueString() const {
    switch (type) {
        case ConstType::STRING: return std::get<std::string>(data);
        case ConstType::BOOL: return std::get<bool>(data) ? "true" : "false";
        case ConstType::NUMBER: return std::to_string(std::get<double>(data));
        default: return "nil";
    }
}

Local::Local(const std::string& name, int start, int end)
    : name(name), start(start), end(end) {}
