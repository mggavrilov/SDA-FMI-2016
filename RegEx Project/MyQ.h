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

#ifndef MYQ_H
#define MYQ_H

template <class T>
class MyQ
{
    public:
        MyQ();
        virtual ~MyQ();
        MyQ(const MyQ& other);
        MyQ& operator=(const MyQ& other);
        void push(const T& n);
        const T& pop();
        const T& front() const;
        bool empty() const;
        int size() const;

    protected:

    private:
        struct node
        {
            T data;
            node *next;

            node(node *n, const T& d);
        } *first, *last;

        int currentSize;

        node* copy(node *n);
        void destroy();
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
    destroy();
}

template <class T>
MyQ<T>::MyQ(const MyQ& other)
{
    first = copy(other.first);

    node *temp = first;
    while(temp != nullptr)
    {
        last = temp;
        temp = temp->next;
    }

    currentSize = other.currentSize;
}

template <class T>
MyQ<T>& MyQ<T>::operator=(const MyQ<T>& rhs)
{
    if (this == &rhs)
        return *this;

    destroy();

    first = copy(rhs.first);

    node *temp = first;
    while(temp != nullptr)
    {
        last = temp;
        temp = temp->next;
    }

    currentSize = rhs.currentSize;

    return *this;
}

template <class T>
void MyQ<T>::push(const T& n)
{
    ++currentSize;
    node *temp = new node(nullptr, n);

    if(first == nullptr)
    {
        first = last = temp;
        return;
    }

    last->next = temp;
    last = temp;
}

template <class T>
const T& MyQ<T>::pop()
{
    if(first == nullptr)
        throw std::invalid_argument("Queue empty.");
    else
    {
        --currentSize;
        node *temp = first;
        if(first == last)
            first = last = nullptr;
        else
            first = first->next;

        return temp->data;
    }
}

template <class T>
const T& MyQ<T>::front() const
{
    if(first == nullptr)
        throw std::invalid_argument("Queue empty.");
    else
        return first->data;
}

template <class T>
bool MyQ<T>::empty() const
{
    return currentSize ? false : true;
}

template <class T>
int MyQ<T>::size() const
{
    return currentSize;
}

template <class T>
typename MyQ<T>::node* MyQ<T>::copy(node *n)
{
    if(n == nullptr)
        return n;
    else
    {
        node* temp = new node(copy(n->next), n->data);
        return temp;
    }
}

template <class T>
void MyQ<T>::destroy()
{
    node* temp = first;

    while(temp != nullptr)
    {
        node* temp2 = temp;
        temp = temp->next;
        delete temp2;
    }

    first = nullptr;
}

/********************************************************
*                       Node
*********************************************************/

template <class T>
MyQ<T>::node::node(node *n, const T& d)
{
    next = n;
    data = d;
}

#endif // MYQ_H
