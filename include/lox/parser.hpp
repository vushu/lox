#ifndef PARSER_HPP
#define PARSER_HPP
#include "ast.hpp"

#include <iostream>
#include <span>
#include <vector>
#include <optional>
#include "lox/lox.hpp"

#include <lox/tokens.hpp>

namespace lox
{
    class Parser
    {
    private:
        std::span<Token> tokens_;
        int current_ = 0;
        std::vector<Expr> expressions_;

        [[nodiscard]] auto is_at_end() const -> bool { return peek().get_type() == TokenType::eof_tok; }

        [[nodiscard]] auto peek() const -> const Token & { return tokens_[current_]; }

        auto advance() -> const Token &
        {
            if (!is_at_end())
            {
                current_++;
            }
            return previous();
        }

        [[nodiscard]] auto previous() const -> const Token & { return tokens_[current_ - 1]; }

        [[nodiscard]] auto check(TokenType type) const -> bool
        {
            if (is_at_end())
            {
                return false;
            }
            return peek().get_type() == type;
        }

        auto match(std::vector<TokenType> types) -> bool
        {
            for (auto type : types)
            {
                if (check(type))
                {
                    advance();
                    return true;
                }
            }
            return false;
        }

        auto consume(TokenType type, std::string_view message) -> const Token &
        {
            if (check(type))
            {
                return advance();
            }
            std::cout << message << " Error" << std::endl;
            throw LoxException("Expected " + std::string{message});
        }

        auto or_expression() -> Expr;
        auto and_expression() -> Expr;
        auto equality() -> Expr;
        auto comparison() -> Expr;
        auto term() -> Expr;
        auto factor() -> Expr;
        auto unary() -> Expr;
        auto primary() -> Expr;
        auto expression() -> Expr;
        auto get_expression() -> Expr;

        auto synchronize() -> void
        {
            advance();
            while (!is_at_end())
            {
                if (previous().get_type() == TokenType::semicolon_tok)
                {
                    return;
                }
            }
            switch (peek().get_type())
            {
            case TokenType::class_tok:
            case TokenType::fun_tok:
            case TokenType::var_tok:
            case TokenType::while_tok:
            case TokenType::print_tok:
            case TokenType::return_tok:
                return;
            default:
                break;
            }
            advance();
        }

    public:
        Parser(std::span<Token> tokens);
        Parser() = default;

        auto parse_expression() -> std::span<Expr>;
    };

}
#endif