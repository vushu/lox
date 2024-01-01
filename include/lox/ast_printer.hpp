#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP
#include "lox/ast.hpp"

class ASTPrinter
{
private:
public:
    // using Expr = std::variant<Unary, Binary, Grouping, Literal, Variable>;
    ASTPrinter() = default;
    ~ASTPrinter() = default;

    void operator(const Unary &unary)
    {
        std::cout << "Got unary" << std::endl;
    }

    void operator(const Binary &binary)
    {
        std::cout << "Got Binary" << std::endl;
    }

    void operator(const Grouping &grouping)
    {
        std::cout << "Got Grouping" << std::endl;
    }

    void operator(const Literal &literal)
    {
        std::cout << "Got Literal" << std::endl;
    }

    void operator(const Variable &variable)
    {
        std::cout << "Got Variable" << std::endl;
    }
};

#endif