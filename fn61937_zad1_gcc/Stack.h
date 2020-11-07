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

//#include <stdexcept>
#include <iostream>

#ifndef STACK_H
#define STACK_H

template <class T>
class Stack {
    int maxSize;
    int top;
    T *elements;

    public:
        Stack();
        virtual ~Stack();
        Stack(int n);
        void push(T const& n);
        T pop();
        bool isEmpty();
};

template <class T>
Stack<T>::Stack()
{
    //ctor
}

template <class T>
Stack<T>::~Stack()
{
    //dtor
}

template <class T>
Stack<T>::Stack(int n)
{
    maxSize = n;
    elements = new T[n];
    top = 0;
}

template <class T>
void Stack<T>::push(T const& n)
{
    if(top < maxSize)
        elements[top++] = n;
    else
    {
        //Stack overflow; expression is invalid
        //throw invalid_argument("Stack full.");
        std::cerr << "Error";
        exit(1);
    }
}

template <class T>
T Stack<T>::pop()
{
    if(top == 0)
    {
        //Stack underflow; expression in invalid
        //throw invalid_argument("Invalid index.");
        std::cerr << "Error";
        exit(1);
    }
    else
        return elements[--top];
}

template <class T>
bool Stack<T>::isEmpty()
{
    return top ? false : true;
}
#endif // STACK_H
