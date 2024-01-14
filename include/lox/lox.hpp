#ifndef LOX_HPP
#define LOX_HPP
#include <exception>
#include <string>
class LoxException : public std::exception
{
private:
    std::string message_;

public:
    LoxException(const std::string &message) : message_(message) {}

    char *what()
    {
        return message_.data();
    }
};
#endif