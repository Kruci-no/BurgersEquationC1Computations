#ifndef DEBUG_EXCEPTION_H
#define DEBUG_EXCEPTION_H

#include <stdexcept>
#include <string>
#include <source_location>  // C++20
#include <format>          // C++20
#include <string_view>

enum class LogLevel { INFO, WARNING, ERROR, CRITICAL };

class debugException : public std::runtime_error {
public:
    debugException(const std::string& message,
                   LogLevel level = LogLevel::ERROR,
                   const std::source_location location = std::source_location::current());

    void log() const;
    const std::source_location& get_location() const;
    LogLevel get_level() const;
    std::string_view level_to_string() const;

    template<typename... Args>
    static debugException create(std::string_view fmt, LogLevel level, const std::source_location location, Args&&... args) {
        return debugException(std::vformat(fmt, std::make_format_args(args...)), level, location);
    }

private:
    LogLevel level_;
    std::source_location location_;
};

#endif // DEBUG_EXCEPTION_H
