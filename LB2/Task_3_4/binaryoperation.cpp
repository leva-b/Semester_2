#include "binaryoperation.h"

BinaryOperation::BinaryOperation() {}


BinaryOperation::BinaryOperation(Expression* left, char operation, Expression* right): _left(left), _right(right), operation(operation){}

BinaryOperation::~BinaryOperation(){
    delete _left;
    delete _right;
}

double BinaryOperation::evaluate()const {
    switch(operation){
    case '+':
        return _left->evaluate()+_right->evaluate();
        break;
    case '/':
        return _left->evaluate()/_right->evaluate();
        break;
    case '*':
        return _left->evaluate()*_right->evaluate();
    default:
            return _left->evaluate()-_right->evaluate();
    }

}
