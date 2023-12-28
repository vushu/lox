#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "lox/tokens.hpp"
#include <string_view>
#include <variant>
#include <vector>
namespace lox {
class Scanner {
private:
    std::string_view source_;
    std::vector<Token> tokens_;

    unsigned int start_ = 0;
    unsigned int current_ = 0;
    unsigned int line_ = 0;

public:
    Scanner(const std::string_view source)
        : source_(source)
    {
    }
    auto scan_tokens() -> std::vector<Token>;

private:
    auto scan_token() -> void;
    auto add_token(TokenType type) -> void;
    auto add_token(TokenType type, std::string_view lexeme, LiteralType literal) -> void;
    auto advance() -> char { return source_[current_++]; }
    auto handle_identifier() -> void;
    auto handle_number() -> void;

    auto is_at_end() -> bool { return current_ >= source_.size(); }
    auto match(char expected) -> bool;
    auto peek() -> char;
    auto peek_next() -> char;
    auto is_digit(char c) -> bool;
    auto is_alpha(char c) -> bool;
    auto is_alpha_numeric(char c) -> bool;
};

}
#endif