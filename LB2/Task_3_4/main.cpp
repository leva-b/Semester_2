#include <iostream>
#include "expression.h"
#include "binaryoperation.h"
#include "number.h"

int main()
{
    Expression* sube = new BinaryOperation(new Number(4.5), '*', new Number(5));
    // потом используем его в выражении для +
    Expression* expr = new BinaryOperation(new Number(3), '+', sube);
    // вычисляем и выводим результат: 25.5
    std::cout << expr->evaluate() << std::endl;
    // тут освобождаются *все* выделенные объекты
    // (например, sube будет правым операндом expr, поэтому его удалять не нужно)
    delete expr;
    return 0;
}
