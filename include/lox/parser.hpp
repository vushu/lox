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
        auto is_at_end() const -> bool
        {
            return peek().get_type() == TokenType::eof_tok;
        }

        auto peek() const -> const Token &
        {
            return tokens_[current_];
        }

        auto advance() -> const Token &
        {
            if (!is_at_end())
            {
                current_++;
            }
            return previous();
        }

        auto previous() const -> const Token &
        {
            return tokens_[current_ - 1];
        }

        auto check(TokenType type) const -> bool
        {
            if (is_at_end())
            {
                return false;
            }
            return peek().get_type() == type;
        }

        auto match(std::span<TokenType> types) -> bool
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
        }

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

        auto parse() -> std::span<Stmt>;
    };

}
#endif