#ifndef BINARYOPERATION_H
#define BINARYOPERATION_H
#include "expression.h"

class BinaryOperation: public Expression
{
    Expression* _right = nullptr;
    Expression* _left = nullptr;
    char operation;
    protected:
    double evaluate() const override;
public:
    BinaryOperation();
    BinaryOperation(Expression*, char, Expression*);
    ~BinaryOperation();
};

#endif // BINARYOPERATION_H
