#ifndef NUMBER_H
#define NUMBER_H
#include "expression.h"

class Number: public Expression
{
    double number;
public:
    Number(double i);
    double evaluate() const override;
};

#endif // NUMBER_H
