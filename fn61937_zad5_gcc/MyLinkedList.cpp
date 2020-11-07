/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 5
 * @compiler GCC
 *
 */

#include "MyLinkedList.h"

MyLinkedList::MyLinkedList()
{
    first = nullptr;
    last = nullptr;
    size = 0;
}

MyLinkedList::~MyLinkedList()
{
    destroy();
}

MyLinkedList::MyLinkedList(const MyLinkedList &t)
{
    first = copy(t.first);

    Node *temp = first;
    while(temp != nullptr)
    {
        last = temp;
        temp = temp->next;
    }

    size = t.size;
}

MyLinkedList& MyLinkedList::operator=(const MyLinkedList &t)
{
    if (this == &t)
        return *this;

    destroy();
    first = copy(t.first);

    Node *temp = first;
    while(temp != nullptr)
    {
        last = temp;
        temp = temp->next;
    }

    size = t.size;

    return *this;
}

void MyLinkedList::add(uint64_t k)
{
    ++size;

    Node *temp = new Node(nullptr, k);

    if(first == nullptr)
        first = last = temp;
    else
    {
        last->next = temp;
        last = temp;
    }
}

bool MyLinkedList::remove(uint64_t k)
{
    if(first != nullptr)
    {
        Node *traverse = first;
        Node *prev = traverse;

        while(traverse != nullptr)
        {
            if(traverse->data == k)
            {
                --size;

                if(first == last)
                    first = last = nullptr;
                else
                {
                    prev->next = traverse->next;

                    if(traverse == first)
                        first = first->next;
                    else if(traverse == last)
                        last = prev;
                }

                delete traverse;
                return true;
            }

            prev = traverse;
            traverse = traverse->next;
        }
    }

    return false;
}

bool MyLinkedList::search(uint64_t k) const
{
    if(first != nullptr)
    {
        Node *traverse = first;

        while(traverse != nullptr)
        {
            if(traverse->data == k)
                return true;

            traverse = traverse->next;
        }
    }

    return false;
}

uint64_t* MyLinkedList::getall() const
{
    Node *traverse = first;
    uint64_t *arr = new uint64_t[size];
    int counter = 0;

    while(traverse != nullptr)
    {
        arr[counter++] = traverse->data;

        traverse = traverse->next;
    }

    return arr;
}

int MyLinkedList::getSize() const
{
    return size;
}

bool MyLinkedList::isEmpty() const
{
    return size ? false : true;
}

void MyLinkedList::destroy()
{
    Node* temp = first;

    while(temp != nullptr)
    {
        Node* temp2 = temp;
        temp = temp->next;
        delete temp2;
    }

    first = nullptr;
}

MyLinkedList::Node* MyLinkedList::copy(Node *n)
{
    if(n == nullptr)
        return n;
    else
    {
        Node* temp = new Node(copy(n->next), n->data);
        return temp;
    }
}

/********************************************************
*                       Node
*********************************************************/

MyLinkedList::Node::Node(Node *n, uint64_t k)
{
    next = n;
    data = k;
}
