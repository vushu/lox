#include <iostream>
#include "lox/scanner.hpp"
#include <string_view>
#include <vector>
#include <charconv>

namespace lox {

auto Scanner::scan_tokens() -> std::vector<Token>
{
    std::cout << "SCAN TOKENS " << std::endl;
    while (!is_at_end()) {
        start_ = current_;
        scan_token();
    }
    add_token(TokenType::eof_tok);
    return tokens_;
}

auto Scanner::scan_token() -> void
{
    char c = advance();
    switch (c) {
    case '(':
        add_token(TokenType::left_paren_tok);
        break;
    case ')':
        add_token(TokenType::right_paren_tok);
        break;
    case '{':
        add_token(TokenType::left_brace_tok);
        break;
    case '}':
        add_token(TokenType::right_brace_tok);
        break;
    case ',':
        add_token(TokenType::comma_tok);
        break;
    case '.':
        add_token(TokenType::dot_tok);
        break;
    case '-':
        add_token(TokenType::minus_tok);
        break;
    case '+':
        add_token(TokenType::plus_tok);
        break;
    case ';':
        add_token(TokenType::semicolon_tok);
        break;
    case '*':
        add_token(TokenType::star_tok);
        break;
    case '!':
        add_token((match('=') ? TokenType::bang_equal_tok : TokenType::bang_tok));
        break;
    case '=':
        add_token((match('=') ? TokenType::equal_equal_tok : TokenType::equal_tok));
        break;
    case '<':
        add_token((match('=') ? TokenType::less_equal_tok : TokenType::less_tok));
        break;
    case '>':
        add_token((match('=') ? TokenType::greater_equal_tok : TokenType::greater_tok));
        break;
    case '/':
        if (match('/')) {
            // comment until the tne of line
            while (peek() != '\n' && !is_at_end()) {
                advance();
            }
        } else {
            add_token(TokenType::slash);
        }
        break;
    case ' ':
        [[fallthrough]];
    case '\r':
        [[fallthrough]];
    case '\t':
        break;
    case '\n':
        line_++;
        break;
    case '"':
        break;
    default:
        if (is_digit(c)) {
            handle_number();
            break;
        } else if (is_alpha_numeric(c)) {
            handle_identifier();
            break;
        }
        throw "Unexpected character";
        break;
    }
}

auto Scanner::handle_identifier() -> void
{
    while (is_alpha_numeric(peek())) {
        advance();
    }

    auto text = source_.substr(start_, current_ - start_);
    // see if any keywordsj
    auto iter = keywords.find(text);
    auto type = TokenType::identifier_tok;
    if (iter != keywords.end()) {
        type = iter->second;
    }
    add_token(type);
}

auto Scanner::handle_number() -> void
{
    while (is_digit(peek())) {
        advance();
    }
    if (peek() == '.' && is_digit(peek_next())) {
        advance();
        while (is_digit(peek())) {
            advance();
        }
    }

    auto text = source_.substr(start_, current_ - start_);
    double line;
    std::from_chars(text.data(), text.data() + text.size(), line);

    add_token(TokenType::number_tok, text, line);
}

auto Scanner::match(char expected) -> bool
{
    if (is_at_end()) {
        return false;
    }

    if (source_[current_] != expected) {
        return false;
    }
    current_++;
    return true;
}

auto Scanner::peek_next() -> char
{
    if (current_ + 1 >= source_.size()) {
        return '\0';
    }
    return source_.at(current_ + 1);
}

auto Scanner::peek() -> char
{
    if (is_at_end()) {
        return '\0';
    }
    return source_[current_ + 1];
}

auto Scanner::add_token(TokenType type) -> void
{
    auto lexeme = source_.substr(start_, current_ - start_);
    add_token(type, lexeme, lexeme);
}

auto Scanner::add_token(TokenType type, std::string_view lexeme, LiteralType literal) -> void
{
    tokens_.push_back(Token(type, lexeme, literal, line_));
}

auto Scanner::is_digit(char c) -> bool
{
    return c >= '0' && c <= '9';
}

auto Scanner::is_alpha(char c) -> bool
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

auto Scanner::is_alpha_numeric(char c) -> bool
{
    return is_alpha(c) || is_digit(c);
}

}
