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

#include "MyString.h"

#ifndef OPERATOR_H
#define OPERATOR_H

class Operator
{
    char symbol;
    char operation;
    //int instead of bool for upgradeability
    int precedence; // 1 = *, / ; 2 = +, -
    bool associativity; //0 = left, 1 = right

    static MyString operators;

    public:
        Operator();
        virtual ~Operator();
        Operator(char symbol, char operation, bool associativity);
        char getSymbol();
        char getOperation();
        int getPrecedence();
        bool getAssociativity();
        static bool isOperator(MyString ch);
        double calculate(double a, double b);
        void print();
};

#endif // OPERATOR_H
