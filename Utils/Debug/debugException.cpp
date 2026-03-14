#include "debugException.h"
#include <iostream>
debugException::debugException(const std::string& message,
                               const std::source_location location)
    : std::runtime_error(message), location_(location) {}

void debugException::log() const {
    std::cerr << "Error: " << this->what() << '\n';
    std::cerr << "File: " << this->get_location().file_name() << '\n';
    std::cerr << "Function: " << this->get_location().function_name() << '\n';
    std::cerr << "Line: " << this->get_location().line() << '\n';
    std::cerr << "Column: " << this->get_location().column() << '\n';
}
const std::source_location& debugException::get_location() const {
    return location_;

}