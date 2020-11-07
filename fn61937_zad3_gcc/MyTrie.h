/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 3
 * @compiler GCC
 *
 */

#ifndef MYTRIE_H
#define MYTRIE_H

struct Node {
    Node *next[27]; //a-z and space
    double mark;
    bool isWord;
};

class MyTrie
{
    public:
        MyTrie();
        void insert(const char *a, double mark);
        bool search(const char *a); //is *a a prefix
        double getMark(const char *a);
        virtual ~MyTrie();

    protected:

    private:
        Node *root;
        void destroy(Node *start);
};

#endif // MYTRIE_H
