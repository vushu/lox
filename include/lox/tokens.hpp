#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <map>
#include <string>
#include <string_view>
#include <variant>

namespace lox
{

    template <class... Ts>
    struct visit_overloader : Ts...
    {
        using Ts::operator()...;
    };

    // Some compilers might require this explicit deduction guide
    template <class... Ts>
    visit_overloader(Ts...) -> visit_overloader<Ts...>;

    using LiteralType = std::variant<std::monostate, double, bool, std::string_view>;
    using EvalResult = std::variant<std::monostate, double, bool, std::string>;
    enum class TokenType
    {
        left_paren_tok,  // 0
        right_paren_tok, // 1
        left_brace_tok,  // 2
        right_brace_tok, // 3
        comma_tok,       // 4
        dot_tok,         // 5
        minus_tok,       // 6
        plus_tok,        // 7
        semicolon_tok,   // 8
        slash_tok,       // 9
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
        var_tok,    // 36
        while_tok,  // 37
        eof_tok     // 38
    };

    inline std::map<TokenType, std::string> token_names =
        {
            {TokenType::left_paren_tok, "left paren"},
            {TokenType::right_paren_tok, "right paren"},
            {TokenType::left_brace_tok, "left brace"},
            {TokenType::right_brace_tok, "right brace"},
            {TokenType::comma_tok, "comma"},
            {TokenType::dot_tok, "dot"},
            {TokenType::minus_tok, "minus"},
            {TokenType::plus_tok, "plus"},
            {TokenType::semicolon_tok, "semicolon"},
            {TokenType::slash_tok, "slash"},
            {TokenType::star_tok, "star"},

            {TokenType::bang_tok, "bang"},
            {TokenType::bang_equal_tok, "bang equal"},
            {TokenType::equal_tok, "equal"},
            {TokenType::equal_equal_tok, "equal equal"},
            {TokenType::greater_tok, "greater"},
            {TokenType::greater_equal_tok, "greater equal"},
            {TokenType::less_tok, "less"},
            {TokenType::less_equal_tok, "less equal"},

            {TokenType::identifier_tok, "identifier"},
            {TokenType::string_tok, "string"},
            {TokenType::number_tok, "number"},

            {TokenType::and_tok, "and"},
            {TokenType::class_tok, "class"},
            {TokenType::else_tok, "else"},
            {TokenType::false_tok, "false"},
            {TokenType::fun_tok, "fun"},
            {TokenType::for_tok, "for"},
            {TokenType::if_tok, "if"},
            {TokenType::nil_tok, "nil"},
            {TokenType::or_tok, "or"},
            {TokenType::print_tok, "print"},
            {TokenType::return_tok, "return"},
            {TokenType::super_tok, "super"},
            {TokenType::this_tok, "this"},
            {TokenType::true_tok, "true"},
            {TokenType::var_tok, "var"},
            {TokenType::while_tok, "while"},
            {TokenType::eof_tok, "end of file"}

    };

    template <typename T>
    auto as_integer(T const value)
        -> typename std::underlying_type<T>::type
    {
        return static_cast<typename std::underlying_type<T>::type>(value);
    }

    inline std::map<std::string_view, TokenType> keywords = {
        {"and", TokenType::and_tok},
        {"class", TokenType::class_tok},
        {"else", TokenType::else_tok},
        {"false", TokenType::false_tok},
        {"for", TokenType::for_tok},
        {"fun", TokenType::fun_tok},
        {"if", TokenType::if_tok},
        {"nil", TokenType::nil_tok},
        {"or", TokenType::or_tok},
        {"print", TokenType::print_tok},
        {"return", TokenType::return_tok},
        {"super", TokenType::super_tok},
        {"this", TokenType::this_tok},
        {"true", TokenType::true_tok},
        {"var", TokenType::var_tok},
        {"while", TokenType::while_tok},
    };

    class Token
    {
        TokenType type_;
        std::string_view lexeme_;
        LiteralType literal_;
        unsigned int line_;

    public:
        Token(TokenType type, std::string_view lexeme, LiteralType literal, unsigned int line)
            : type_(type), lexeme_(lexeme), literal_(literal), line_(line) {}

        [[nodiscard]] auto get_type() const -> const TokenType { return type_; }

        [[nodiscard]] auto get_literal() const -> const LiteralType { return literal_; }

        [[nodiscard]] auto to_string() const -> std::string
        {
            std::string lit, lexeme;
            lexeme = lexeme_;
            std::visit(visit_overloader{[&](const int &v)
                                        { lit = std::to_string(v); },
                                        [&](const double &v)
                                        { lit = std::to_string(v); },
                                        [&](const std::string_view &text)
                                        { lit = text; },
                                        [&](const std::monostate &v)
                                        { lit = "nil"; },
                                        [&](const bool &v)
                                        { lit = std::to_string(v); }},
                       literal_);

            return "TokenType: " + token_names.at(type_) + ", Lexeme: " + lexeme + ", Literal: " + lit;
        } // namespace mirscript
    };
} // namespace mirscript
#endif