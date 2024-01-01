#ifndef AST_HPP
#define AST_HPP
#include <variant>
#include "tokens.hpp"
#include <memory>
namespace lox
{

    // Value semantic wrapper inspired by rust box type
    template <typename T>
    class Box
    {
        // Wrapper over unique_ptr
        // Is used to make value semantic on top of unique pointer
    private:
        std::unique_ptr<T> impl_;

    public:
        // Automatic construction from a T, not a T*
        // takes value type and basically puts it into a unique_ptr
        Box(T &&obj) : impl_(std::make_unique<T>(std::move(obj))) {}
        Box(const T &obj) : impl_(std::make_unique<T>(std::move(obj))) {}

        // Copy constructor copies T
        Box(const Box &other) : Box(*other.impl_) {}
        Box &operator=(const Box &other)
        {
            *impl_ = *other.impl_;
            return *this;
        }

        ~Box() = default;

        T &operator*() { return *impl_; }
        const T &operator*() const { return *impl_; }

        T *operator->() { return impl_.get(); }
        const T *operator->() const { return impl_.get(); }
    };

    struct Unary;
    struct Binary;
    struct Grouping;
    struct Literal;
    struct Variable;
    struct Expression;
    struct Print;
    struct Var;
    struct Binary;
    struct Grouping;
    struct Literal;
    struct Variable;
    struct Expression;
    struct Print;
    struct Var;

    struct ExprContainer;

    using Expr = std::variant<Box<Unary>, Box<Binary>, Box<Grouping>,Box<Literal>, Box<Variable>>;
    using Stmt = std::variant<Box<Expression>, Box<Print>, Box<Var>>;

    struct Binary
    {
        Expr left, right;
        Token op;
    };

    struct Grouping
    {
        Expr expression;
    };

    struct Literal
    {
        LiteralType value;
    };

    struct Variable
    {
        Token token;
    };

    struct Expression
    {
        Expr expression;
    };

    struct Print
    {
        Expr expression;
    };

    struct Var
    {
        Token token;
        Expr initializer;
    };

    struct Unary
    {
        Token op;
        Expr right;
    };

}
#endif