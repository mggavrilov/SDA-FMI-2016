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

#include <cstring>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Stack.h"
#include "Operator.h"
#include "MyString.h"

using namespace std;

const int MAX_OPERATORS = 50;
const int MAX_PREFIX_POSTFIX = MAX_OPERATORS * 2;
const char DELIMITER = ' '; //character that separates each operator or operand in the prefix notation file
const char DECIMAL_POINT = '.'; //allowed character for a decimal point; (ex. 2,5 will produce an error, use 2.5 instead)

//check if a given number in the prefix file expression is valid;
//prevent parsing garbage values (ex. 10!@&15 where 'atof' would convert it to 10)
bool isValidNumber(MyString n);

int main(int argc, char* argv[])
{
    //file1 = prefix expression
    //file2 = operators
    if(argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <file1> <file2>";
        return 1;
    }

    ifstream prefixFile (argv[1]);
    ifstream operatorFile (argv[2]);

    //Array for all of the operators described in the operator file
    Operator operators[MAX_OPERATORS];
    int operatorCounter = 0;

    if(operatorFile.is_open())
    {
        while(!operatorFile.eof())
        {
            char a, b, c;
            operatorFile >> a >> b >> c;
            if(operatorCounter == MAX_OPERATORS)
            {
                cerr << "Maximum number of operators (" << MAX_OPERATORS << ") exceeded!";
                return 1;
            }

            //Format: SYMBOL OPERATION ASSOCIATIVITY
            operators[operatorCounter++] = Operator(a, b, (c == '0' ? 0 : 1));
        }

        operatorFile.close();
    }
    else
    {
        cerr << "Couldn't open " << argv[2];
        return 1;
    }

    //number of spaces in prefix expression
    //total operators + operands = number of spaces + 1
    int counter = 0;
    //String array that will hold the prefix expression; each operator or operand is saved as a string in it
    MyString prefix[MAX_PREFIX_POSTFIX];
    char line[MAX_PREFIX_POSTFIX];
    if(prefixFile.is_open())
    {
        prefixFile.getline(line, MAX_PREFIX_POSTFIX);

        for(unsigned int i = 0; i < strlen(line); ++i)
        {
            if(line[i] == ' ')
                counter++;
        }

        //Reset file's fstream to beginning because it was already read once
        prefixFile.seekg(0, prefixFile.beg);

        for(int i = 0; i <= counter; ++i)
        {
            //Populate the 'prefix' string array; '>>' eats up the spaces

            prefixFile >> prefix[i];

            //Prevent adding invalid operators (ones that aren't included in the operator file)
            //atof returns 0 if conversion was unsuccessful; check in case operand is a zero
            //(assuming zeros will be passed as '0' instead of 0.00 for example)
            if(!Operator::isOperator(prefix[i]) && (atof(prefix[i].c_str()) == 0 && prefix[i].compare("0") != 0))
            {
                cerr << "Error";
                return 1;
            }

            //Prevent adding invalid operands (ex. 10!@&15 where 'atof' would convert it to 10
            //If token isn't a valid character (absent in file) and not a valid number, the expression is invalid
            if(!Operator::isOperator(prefix[i]) && !isValidNumber(prefix[i]))
            {
                cerr << "Error";
                return 1;
            }
        }

        prefixFile.close();
    }
    else
    {
        cerr << "Couldn't open " << argv[1];
        return 1;
    }

    /***************************************************************************************************************
    *
    *                                   Prefix to postfix notation algorithm
    *
    ****************************************************************************************************************
    *
    * Start reading prefix expression from right to left.
    *   - If the token is an operand, push it to stack.
    *   - If the token is an operator, pop two elements from the stack and concatenate them as follows:
    *           operand1 operand2 operator
    *           (ex. 3 4 +)
    *       Push result to stack.
    *   - Repeat steps until the last token from the prefix expression is read.
    *   - Pop stack's last (and only) element which is the initial prefix notation expression converted to postfix.
    *
    *   If there's a stack under- or overflow, or there's more or less than one element left in the stack after the
    *   last step, the expression is invalid.
    *
    ***************************************************************************************************************/

    Stack<MyString> stack = Stack<MyString>(MAX_OPERATORS);

    for(int i = counter; i >= 0; --i)
    {
        if(!Operator::isOperator(prefix[i]))
            stack.push(prefix[i]);
        else
        {
            MyString operand1 = stack.pop();
            MyString operand2 = stack.pop();
            MyString temp = "";
            temp += operand1;
            temp += ' ';
            temp += operand2;
            temp += ' ';
            temp += prefix[i];
            stack.push(temp);
        }
    }

    MyString tempPostfix;
    //If there isn't exactly one element left on the stack, the prefix notation expression is invalid
    if(stack.isEmpty())
    {
        cerr << "Error";
        return 1;
    }
    else
    {
        tempPostfix = stack.pop();

        if(!stack.isEmpty())
        {
            cerr << "Error";
            return 1;
        }
    }

    Stack<double> stack2 = Stack<double>(MAX_OPERATORS);
    //String array that will hold the postfix expression; each operator or operand is saved as a string in it
    MyString postfix[MAX_PREFIX_POSTFIX];

    //number of spaces in postfix expression
    //tokenize all operators and operands from the expression (delimiter: space)
    //also used to index the postfix string array
    int opCounter = 0;
    for(unsigned int i = 0; i < tempPostfix.length(); ++i)
    {
        if(tempPostfix[i] != DELIMITER)
            postfix[opCounter] += tempPostfix[i];
        else
            opCounter++;
    }

    //Print postfix expression
    for(int i = 0; i <= counter; ++i)
    {
        cout << postfix[i] << " ";
    }

    cout << endl;

    /***************************************************************************************************************
    *
    *                                   Postfix notation evaluation algorithm
    *
    ****************************************************************************************************************
    *
    * Start reading prefix expression from left to right.
    *   - If the token is an operand, push it to stack.
    *   - If the token is an operator, pop two elements from the stack, evaluate them and push result to stack.
    *   - Repeat steps until the last token from the postfix expression is read.
    *   - Pop stack's last (and only) element which is the result from evaluating the postfix notation expression.
    *
    *   If there's a stack under- or overflow, or there's more or less than one element left in the stack after the
    *   last step, the expression is invalid.
    *
    ***************************************************************************************************************/

    for(int i = 0; i <= counter; ++i)
    {
        if(!Operator::isOperator(postfix[i]))
        {
            double temp = atof(postfix[i].c_str());
            stack2.push(temp);
        }

        else
        {
            double result;

            //If the read token is an operator, find Operator object in the 'operators' array and perform calculation
            //j is declared outside of for loop because it's needed in another scope (after the loop)
            int j;
            for(j = 0; j < operatorCounter; ++j)
            {
                MyString tempSymbol;
                tempSymbol += operators[j].getSymbol();
                if(tempSymbol.compare(postfix[i]) == 0)
                {
                    break;
                }
            }

            //Pop two stack elements and send them to calcucate(double, double) member function to be calculated
            result = operators[j].calculate(stack2.pop(), stack2.pop());
            stack2.push(result);
        }
    }

    //another check just in case and for algorithm completeness
    double postfixResult;
    if(stack2.isEmpty())
    {
        cerr << "Error";
        return 1;
    }
    else
    {
        postfixResult = stack2.pop();

        if(!stack2.isEmpty())
        {
            cerr << "Error";
            return 1;
        }
    }

    //Precision of 5 digits after decimal point
    cout.precision(5);
    //Print result from expression evaluation
    cout << fixed << postfixResult;

    return 0;
}

bool isValidNumber(MyString n)
{
    //Decimal point counter; if it exceeds 1, the token is not a valid number
    int dpCounter = 0;
    //Minus counter; if it exceeds 1, the token is not a valid number
    int minusCounter = 0;
    for(unsigned int i = 0; i < n.length(); ++i)
    {
        if(n[i] == DECIMAL_POINT)
            ++dpCounter;

        if(n[i] == '-')
        {
            ++minusCounter;

            //Minus sign can only be in the very beginning of a number; if it's not then token is not a valid number
            if(i != 0)
                return false;
        }

        //If token contains more than 1 dot or one of the token's symbols isn't between 0-9, or a decimal point, or a minus sign, it's invalid
        if(dpCounter > 1  || minusCounter > 1 || (n[i] != DECIMAL_POINT && n[i] != '-' && (n[i] < '0' || n[i] > '9')))
            return false;
    }

    return true;
}
