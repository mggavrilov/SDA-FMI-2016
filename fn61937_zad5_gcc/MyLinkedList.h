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

#ifndef MYLINKEDLIST_H
#define MYLINKEDLIST_H

#include <cstdint>

class MyLinkedList
{
    public:
        MyLinkedList();
        virtual ~MyLinkedList();
        MyLinkedList(const MyLinkedList &t);
        MyLinkedList& operator=(const MyLinkedList &t);
        void add(uint64_t k);
        bool remove(uint64_t k);
        bool search(uint64_t k) const;
        int getSize() const;
        bool isEmpty() const;
        uint64_t* getall() const;

    protected:

    private:
        struct Node {
            Node *next;
            uint64_t data;

            Node(Node *n, uint64_t k);
        } *first, *last;

        int size;

        void destroy();
        Node* copy(Node *n);
};

#endif // MYLINKEDLIST_H
