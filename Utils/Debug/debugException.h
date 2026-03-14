#ifndef DEBUG_EXCEPTION_H
#define DEBUG_EXCEPTION_H

#include <stdexcept>
#include <string>
#include <source_location>  // C++20

class debugException : public std::runtime_error {
public:
    debugException(const std::string& message,
                   const std::source_location location = std::source_location::current());
    void log() const;
    const std::source_location& get_location() const;

private:
    std::source_location location_;
};

#endif // DEBUG_EXCEPTION_H
