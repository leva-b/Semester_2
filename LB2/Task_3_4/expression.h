#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression
{

public:
    static bool check_equals(Expression const* left, Expression const* right) {
        return *(void**)left == *(void**)right;
    }
    virtual double evaluate() const = 0;
    Expression();
    virtual ~Expression();
};

#endif // EXPRESSION_H
