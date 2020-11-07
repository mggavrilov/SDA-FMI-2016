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

#ifndef MYLINKEDLIST_H
#define MYLINKEDLIST_H

class MyLinkedList
{
    public:
        MyLinkedList();
        virtual ~MyLinkedList();
        MyLinkedList(const MyLinkedList &t);
        MyLinkedList& operator=(const MyLinkedList &t);
        void add(char *d);
        bool remove(char *d);
        void removeall();
        bool search(char *d);
        int getSize() const;
        bool isEmpty() const;

    protected:

    private:
        struct Node {
            Node *next;
            char *data;

            Node(Node *n, char *d);
        } *first, *last;

        int size;

        void destroy();
        Node* copy(Node *n);
};

#endif // MYLINKEDLIST_H
