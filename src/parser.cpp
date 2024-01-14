#include "lox/parser.hpp"

#include <stdexcept>

#include <lox/ast.hpp>
#include <lox/tokens.hpp>

namespace lox
{
    Parser::Parser(std::span<Token> tokens) : tokens_(tokens) {}

    // This parser expression for now, we will parse statements if desired
    auto Parser::parse_expression() -> std::span<Expr>
    {
        while (!is_at_end())
        {
            expressions_.push_back(expression());
        }
        return expressions_;
    }

    auto Parser::or_expression() -> Expr
    {
        Expr expr = and_expression();
        while (match({TokenType::or_tok}))
        {
            Token op = previous();
            Expr right = and_expression();
            expr = Logical{.left = expr, .right = right, .op = op};
        }
        return expr;
    }

    auto Parser::and_expression() -> Expr
    {
        Expr expr = equality();
        while (match({TokenType::and_tok}))
        {
            Token op = previous();
            Expr right = equality();
            expr = Logical{.left = expr, .right = right, .op = op};
        }
        return expr;
    }

    auto Parser::equality() -> Expr
    {
        Expr expr = comparison();
        while (match({TokenType::bang_equal_tok, TokenType::equal_equal_tok}))
        {
            Token op = previous();
            Expr right = comparison();
            expr = Binary{.left = expr, .right = right, .op = op};
        }
        return expr;
    }

    auto Parser::comparison() -> Expr
    {
        Expr expr = term();
        while (match({TokenType::greater_tok, TokenType::greater_equal_tok, TokenType::less_tok, TokenType::less_equal_tok}))
        {
            Token op = previous();
            Expr right = term();
            expr = Binary{.left = expr, .right = right, .op = op};
        }
        return expr;
    }

    auto Parser::term() -> Expr
    {
        Expr expr = factor();
        while (match({TokenType::minus_tok, TokenType::plus_tok}))
        {
            Token op = previous();
            Expr right = factor();
            expr = Binary{.left = expr, .right = right, .op = op};
        }
        return expr;
    }

    auto Parser::factor() -> Expr
    {
        Expr expr = unary();
        while (match({TokenType::slash_tok, TokenType::star_tok}))
        {
            Token op = previous();
            Expr right = unary();
            expr = Binary{.left = expr, .right = right, .op = op};
        }
        return expr;
    }

    auto Parser::unary() -> Expr
    {
        if (match({TokenType::bang_tok, TokenType::minus_tok}))
        {
            Token op = previous();
            Expr right = unary();
            return Unary{op, right};
        }
        return primary();
    }

    auto Parser::expression() -> Expr { return or_expression(); }

    auto Parser::primary() -> Expr
    {
        if (match({TokenType::false_tok}))
        {
            return Literal{LiteralType(false)};
        }
        if (match({TokenType::true_tok}))
        {
            return Literal{LiteralType(true)};
        }
        if (match({TokenType::nil_tok}))
        {
            return Literal{{}};
        }
        if (match({TokenType::number_tok, TokenType::string_tok}))
        {
            return Literal{previous().get_literal()};
        }

        if (match({TokenType::identifier_tok}))
        {
            return Variable{previous()};
        }

        if (match({TokenType::left_paren_tok}))
        {
            Expr expr = expression();
            consume(TokenType::right_paren_tok, "expected ) after expression.");
            return Grouping{expr};
        }

        std::cout << "I Failed to parse here:  " << tokens_[current_].to_string() << std::endl;
        throw std::invalid_argument("Failed to parse primary");
    }

} // namespace lox