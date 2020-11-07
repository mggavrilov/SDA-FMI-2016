/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 0
 * @compiler GCC
 *
 */

#include <stdexcept>

#ifndef MYSTACK_H
#define MYSTACK_H

#define DEFAULT_STACK_SIZE 10

template <class T>
class MyStack {
    int maxSize;
    int top;
    T *elements;

    public:
        MyStack();
        virtual ~MyStack();
        MyStack(int n);
        MyStack(const MyStack& other);
        MyStack& operator=(const MyStack& other);
        void push(const T& n);
        const T& pop();
        const T& peek() const;
        bool isEmpty() const;
};

template <class T>
MyStack<T>::MyStack()
{
    maxSize = DEFAULT_STACK_SIZE;
    elements = new T[DEFAULT_STACK_SIZE];
    top = 0;
}

template <class T>
MyStack<T>::MyStack(int n)
{
    maxSize = n;
    elements = new T[n];
    top = 0;
}

template <class T>
MyStack<T>::~MyStack()
{
    delete[] elements;
}

template <class T>
MyStack<T>::MyStack(const MyStack<T>& other)
{
    maxSize = other.maxSize;
    top = other.top;
    elements = new T[other.maxSize];

    for(int i = 0; i < other.top; ++i)
    {
        elements[i] = other.elements[i];
    }
}

template <class T>
MyStack<T>& MyStack<T>::operator=(const MyStack<T>& rhs)
{
    if (this == &rhs) return *this;

    delete[] elements;
    maxSize = rhs.maxSize;
    top = rhs.top;
    elements = new T[rhs.maxSize];

    for(int i = 0; i < rhs.top; ++i)
    {
        elements[i] = rhs.elements[i];
    }

    return *this;
}

template <class T>
void MyStack<T>::push(const T& n)
{
    if(top == maxSize)
    {
        T *temp = elements;
        delete[] elements;
        elements = new T[maxSize * 2 + DEFAULT_STACK_SIZE];
        for(int i = 0; i < top; ++i)
        {
            elements[i] = temp[i];
        }
    }

    elements[top++] = n;
}

template <class T>
const T& MyStack<T>::pop()
{
    if(top == 0)
        throw std::invalid_argument("Stack underflow.");
    else
        return elements[--top];
}

template <class T>
const T& MyStack<T>::peek() const
{
    if(top == 0)
        throw std::invalid_argument("Stack underflow.");
    else
        return elements[top - 1];
}

template <class T>
bool MyStack<T>::isEmpty() const
{
    return top ? false : true;
}

#endif // MYSTACK_H
