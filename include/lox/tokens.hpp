#ifndef TOKENS_HPP
#define TOKENS_HPP
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <variant>
namespace lox {
using LiteralType = std::variant<double, int, std::string_view>;
enum class TokenType {
    left_paren_tok,  // 0
    right_paren_tok, // 1
    left_brace_tok,  // 2
    right_brace_tok, // 3
    comma_tok,       // 4
    dot_tok,         // 5
    minus_tok,       // 6
    plus_tok,        // 7
    semicolon_tok,   // 8
    slash,           // 9
    star_tok,        // 10

    bang_tok,          // 11
    bang_equal_tok,    // 12
    equal_tok,         // 13
    equal_equal_tok,   // 14
    greater_tok,       // 15
    greater_equal_tok, // 16
    less_tok,          // 17
    less_equal_tok,    // 18

    identifier_tok, // 19
    string_tok,     // 20
    number_tok,     // 21

    and_tok,    // 22
    class_tok,  // 23
    else_tok,   // 24
    false_tok,  // 25
    fun_tok,    // 26
    for_tok,    // 27
    if_tok,     // 28
    nil_tok,    // 29
    or_tok,     // 30
    print_tok,  // 31
    return_tok, // 32
    super_tok,  // 33
    this_tok,   // 34
    true_tok,   // 35
    var,        // 36
    while_tok,  // 37
    eof_tok     // 38
};

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// Some compilers might require this explicit deduction guide
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename T>
auto as_integer(T const value) -> typename std::underlying_type<T>::type
{
    return static_cast<typename std::underlying_type<T>::type>(value);
}

inline std::map<std::string_view, TokenType> keywords = {
    { "and", TokenType::eof_tok },
    { "class", TokenType::class_tok },
    { "else", TokenType::else_tok },
    { "false", TokenType::false_tok },
    { "for", TokenType::for_tok },
    { "fun", TokenType::fun_tok },
    { "if", TokenType::if_tok },
    { "nil", TokenType::nil_tok },
    { "or", TokenType::or_tok },
    { "print", TokenType::print_tok },
    { "return", TokenType::return_tok },
    { "super", TokenType::super_tok },
    { "this", TokenType::this_tok },
    { "true", TokenType::true_tok },
    { "var", TokenType::var },
    { "while", TokenType::while_tok },
};

class Token {
    TokenType type_;
    std::string_view lexeme_;
    LiteralType literal_;
    unsigned int line_;

public:
    Token(TokenType type, std::string_view lexeme, LiteralType literal, unsigned int line)
        : type_(type)
        , lexeme_(lexeme)
        , literal_(literal)
        , line_(line)
    {
    }

    auto to_string() -> std::string
    {

        std::string lit, lexeme;
        lexeme = lexeme_;
        std::visit(overloaded {
                       [&](const int& v) { lit = std::to_string(v); },
                       [&](const double& v) { lit = std::to_string(v); },
                       [&](const std::string_view& text) {
                           lit = text;
                       } },
            literal_);
        
        return "TokenType: " + std::to_string(as_integer(type_)) + ", Lexeme: " + lexeme + ", Literal: " + lit;
    }
};
}
#endif