/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 4
 * @compiler GCC
 *
 */

#include "MyLinkedList.h"
#include <cstring>

MyLinkedList::MyLinkedList()
{
    first = nullptr;
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

void MyLinkedList::add(char *d)
{
    ++size;

    Node *temp = new Node(nullptr, d);

    if(first == nullptr)
        first = last = temp;
    else
    {
        last->next = temp;
        last = temp;
    }
}

bool MyLinkedList::remove(char *d)
{
    if(first != nullptr)
    {
        Node *traverse = first;
        Node *prev = traverse;

        while(traverse != nullptr)
        {
            if(strcmp(traverse->data, d) == 0)
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

void MyLinkedList::removeall()
{
    destroy();
    size = 0;
}

bool MyLinkedList::search(char *d)
{
    if(first != nullptr)
    {
        Node *traverse = first;

        while(traverse != nullptr)
        {
            if(strcmp(traverse->data, d) == 0)
                return true;

            traverse = traverse->next;
        }
    }

    return false;
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

MyLinkedList::Node::Node(Node *n, char *d)
{
    next = n;

    int length = strlen(d);
    data = new char[length + 1];

    for(int i = 0; i <= length; ++i)
    {
        data[i] = d[i];
    }
}
