/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 2
 * @compiler GCC
 *
 */

#include <stdexcept>

#ifndef MYQ_H
#define MYQ_H

template <class T>
class node
{
    public:
        T data;
        node *next;

};

template <class T>
class MyQ
{
    node<T> *first;
    node<T> *last;
    int currentSize;

    public:
        MyQ();
        virtual ~MyQ();
        void push(T const& n);
        T pop();
        T front();
        void removeAt(int n);
        bool empty();
        int size();
        T& operator [](int n);
};

template <class T>
MyQ<T>::MyQ()
{
    currentSize = 0;
    first = nullptr;
    last = nullptr;
}

template <class T>
MyQ<T>::~MyQ()
{
    //dtor
}

template <class T>
void MyQ<T>::push(T const& n)
{
    ++currentSize;
    node<T> *temp = new node<T>;
    temp->data = n;
    temp->next = nullptr;

    if(first == nullptr)
    {
        first = last = temp;
        return;
    }


    last->next = temp;
    last = temp;
}

template <class T>
T MyQ<T>::pop()
{
    if(first == nullptr)
        throw std::invalid_argument("Queue empty.");
    else
    {
        --currentSize;
        node<T> *temp = first;
        if(first == last)
            first = last = nullptr;
        else
            first = first->next;

        return temp->data;
    }
}

template <class T>
T MyQ<T>::front()
{
    if(first == nullptr)
        throw std::invalid_argument("Queue empty.");
    else
        return first->data;
}

template <class T>
void MyQ<T>::removeAt(int n)
{
    if(n < 1 || n > currentSize)
        throw std::invalid_argument("Invalid index.");
    else
    {
        --currentSize;
        node<T> *temp = first;

        if(n == 1)
        {
            first = temp->next;
            delete temp;
        }
        else
        {
            for(int i = 1; i < n - 1; ++i)
                temp = temp->next;

            node<T> *temp2 = temp->next;
            temp->next = temp2->next;

            delete temp2;
        }
    }
}

template <class T>
T& MyQ<T>::operator[](int n)
{
    if(n < 1 || n > currentSize)
        throw std::invalid_argument("Invalid index.");
    else
    {
        node<T> *temp = first;
        for(int i = 1; i < n; ++i)
            temp = temp->next;

        return temp->data;
    }
}

template <class T>
bool MyQ<T>::empty()
{
    return currentSize ? false : true;
}

template <class T>
int MyQ<T>::size()
{
    return currentSize;
}

#endif // MYQ_H
