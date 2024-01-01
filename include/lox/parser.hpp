#ifndef PARSER_HPP
#define PARSER_HPP
#include "ast.hpp"
#include <span>
#include <vector>
namespace lox
{
    class Parser
    {
    private:
        std::span<Token> tokens_;
        int current_ = 0;
        std::vector<Stmt> statements_;

    public:
        Parser(std::span<Token> tokens);
        Parser() = default;

        auto parse() -> std::span<Stmt>;
    };

}
#endif