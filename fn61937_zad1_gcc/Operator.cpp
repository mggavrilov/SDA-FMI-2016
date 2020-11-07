/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 1
 * @compiler GCC
 *
 */

#include "Operator.h"

MyString Operator::operators = "";

Operator::Operator()
{
    //ctor
}

Operator::~Operator()
{
    //dtor
}

Operator::Operator(char symbol, char operation, bool associativity)
{
    this->symbol = symbol;
    this->operation = operation;
    this->associativity = associativity;

    switch(operation)
    {
        //Intentional fall-through
        case '*':
        case '/':
            precedence = 1;
            break;
        case '+':
        case '-':
            precedence = 2;
            break;
    }

    operators += symbol;
}

char Operator::getSymbol()
{
    return symbol;
}

char Operator::getOperation()
{
    return operation;
}

int Operator::getPrecedence()
{
    return precedence;
}

bool Operator::getAssociativity()
{
    return associativity;
}

bool Operator::isOperator(MyString ch)
{
    if(ch.length() != 1)
        return false;

    for(unsigned int i = 0; i < operators.length(); i++)
    {
        MyString temp;
        temp += operators[i];
        if(temp.compare(ch) == 0)
            return true;
    }

    return false;
}

double Operator::calculate(double a, double b)
{
    switch(operation)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            return 0;
    }
}
