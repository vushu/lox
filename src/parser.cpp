#include "lox/parser.hpp"

namespace lox
{
    Parser::Parser(std::span<Token> tokens) : tokens_(tokens) {}
    auto Parser::parse() -> std::span<Stmt> {
        return statements_;
    }
}