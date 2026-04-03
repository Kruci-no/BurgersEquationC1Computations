#include "debugException.h"
#include <iostream>
#include <format>

debugException::debugException(const std::string& message,
                               LogLevel level,
                               const std::source_location location)
    : std::runtime_error(message), level_(level), location_(location) {}

void debugException::log() const {
    std::cerr << std::format("[{}] Error: {}\n", level_to_string(), this->what());
    std::cerr << std::format("File: {}\n", this->get_location().file_name());
    std::cerr << std::format("Function: {}\n", this->get_location().function_name());
    std::cerr << std::format("Line: {}\n", this->get_location().line());
    std::cerr << std::format("Column: {}\n", this->get_location().column());
}

const std::source_location& debugException::get_location() const {
    return location_;
}

LogLevel debugException::get_level() const {
    return level_;
}

std::string_view debugException::level_to_string() const {
    switch (level_) {
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default:                return "UNKNOWN";
    }
}
