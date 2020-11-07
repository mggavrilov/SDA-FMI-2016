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

#ifndef MYBST_H
#define MYBST_H

#include "MyLinkedList.h"
#include <stdexcept>

//CHECK namespaces thanks to http://stackoverflow.com/a/6536204
namespace CHECK_ASSIGNMENT
{
  class No { bool b[2]; };
  template<typename T, typename Arg> No operator== (const T&, const Arg&);

  bool Check (...);
  No& Check (const No&);

  template <typename T, typename Arg = T>
  struct Exists
  {
    enum { value = (sizeof(Check(*(T*)(0) == *(Arg*)(0))) != sizeof(No)) };
  };
}

namespace CHECK_LESS_THAN
{
  class No { bool b[2]; };
  template<typename T, typename Arg> No operator< (const T&, const Arg&);

  bool Check (...);
  No& Check (const No&);

  template <typename T, typename Arg = T>
  struct Exists
  {
    enum { value = (sizeof(Check(*(T*)(0) < *(Arg*)(0))) != sizeof(No)) };
  };
}

namespace CHECK_MORE_THAN
{
  class No { bool b[2]; };
  template<typename T, typename Arg> No operator> (const T&, const Arg&);

  bool Check (...);
  No& Check (const No&);

  template <typename T, typename Arg = T>
  struct Exists
  {
    enum { value = (sizeof(Check(*(T*)(0) > *(Arg*)(0))) != sizeof(No)) };
  };
}

template <class T>
class MyBST
{
    public:
        MyBST();
        virtual ~MyBST();
        MyBST(const MyBST &t);
        MyBST& operator=(const MyBST &t);
        void add(const T &k, char *info);
        bool remove(const T &k, char *info);
        int removeall(const T &k);
        bool search(const T &k, char *data) const;
        int getSize() const;

    protected:

    private:
        struct Node {
            Node *left;
            Node *right;
            T key;
            MyLinkedList list;

            Node(const T &k, char *info, Node* l = nullptr, Node* r = nullptr);
            Node(const T &k, const MyLinkedList &sourceList, Node* l = nullptr, Node* r = nullptr);
        };

        Node *root;
        int size;

        void add_internal(Node* &start, const T &k, char *info);
        void remove_internal(Node* &start, const T &k, char *info = nullptr);
        Node* getMinimum(Node *traverse) const;
        Node* getMaximum(Node *traverse) const;
        Node* copy(Node *start);
        void destroy(Node *start);
};

template <class T>
MyBST<T>::MyBST()
{
    bool assignment = CHECK_ASSIGNMENT::Exists<T>::value;
    bool lessThan = CHECK_LESS_THAN::Exists<T>::value;
    bool moreThan = CHECK_MORE_THAN::Exists<T>::value;

    if(!(assignment && lessThan && moreThan))
        throw std::invalid_argument("No comparison operators in selected type.");

    root = nullptr;
    size = 0;
}

template <class T>
MyBST<T>::~MyBST()
{
    destroy(root);
}

template <class T>
MyBST<T>::MyBST(const MyBST &t)
{
    root = copy(t.root);
    size = t.size;
}

template <class T>
MyBST<T>& MyBST<T>::operator=(const MyBST<T> &t)
{
    if (this == &t)
        return *this;

    destroy(root);
    root = copy(t.root);
    size = t.size;

    return *this;
}

template <class T>
void MyBST<T>::add(const T &k, char *info)
{
    if(root == nullptr)
        root = new Node(k, info);
    else
        add_internal(root, k, info);

    ++size;
}

template <class T>
void MyBST<T>::add_internal(Node* &start, const T &k, char *info)
{
    if(start == nullptr)
        start = new Node(k, info);
    else if(start->key > k)
        add_internal(start->left, k, info);
    else if(start->key < k)
        add_internal(start->right, k, info);
    else //start->key = k
        start->list.add(info);
}

template <class T>
bool MyBST<T>::remove(const T &k, char *info)
{
    int currentSize = size;
    remove_internal(root, k, info);
    return (size < currentSize);
}

template <class T>
int MyBST<T>::removeall(const T &k)
{
    int currentSize = size;
    remove_internal(root, k);
    return (currentSize - size);

    return 0;
}

template <class T>
void MyBST<T>::remove_internal(Node* &start, const T &k, char *info)
{
    if(start == nullptr)
        return;

    if(start->key == k)
    {
        if(info != nullptr && !start->list.remove(info))
            return;

        if(info != nullptr)
            --size;
        else
        {
            size -= start->list.getSize();
            start->list.removeall();
        }

        if(start->list.isEmpty())
        {
            Node* temp = start;

            if(start->left == nullptr && start->right != nullptr)
            {
                start = start->right;
            }
            else if(start->right == nullptr && start->left != nullptr)
            {
                start = start->left;
            }
            else if(start->left == nullptr && start->right == nullptr)
            {
                start = nullptr;
            }
            else
            {
                Node* minimum = getMinimum(start->right);

                minimum->left = start->left;
                minimum->right = start->right;

                start = minimum;
            }

            delete temp;
        }
    }
    else
    {
        if(start->key > k)
            remove_internal(start->left, k, info);
        else
            remove_internal(start->right, k, info);
    }
}

template <class T>
bool MyBST<T>::search(const T &k, char *info) const
{
    Node* traverse = root;

    while(traverse != nullptr)
    {
        if(traverse->key == k)
        {
            if(traverse->list.search(info))
                return true;
            else
                return true;
        }

        if(traverse->key > k)
            traverse = traverse->left;
        else
            traverse = traverse->right;
    }

    return false;
}

template <class T>
int MyBST<T>::getSize() const
{
    return size;
}

template <class T>
typename MyBST<T>::Node* MyBST<T>::copy(Node *start)
{
    if(start == nullptr)
        return start;
    else
    {
        Node* temp = new Node(start->key, start->list, copy(start->left), copy(start->right));
        return temp;
    }
}

template <class T>
void MyBST<T>::destroy(Node *start)
{
    if(start != nullptr)
    {
        destroy(start->left);
        destroy(start->right);
        delete start;
    }
}

template <class T>
typename MyBST<T>::Node* MyBST<T>::getMinimum(Node *traverse) const
{
    while(traverse->left != nullptr)
        traverse = traverse->left;

    return traverse;
}

template <class T>
typename MyBST<T>::Node* MyBST<T>::getMaximum(Node *traverse) const
{
    while(traverse->right != nullptr)
        traverse = traverse->right;

    return traverse;
}

/********************************************************
*                       Node
*********************************************************/

template <class T>
MyBST<T>::Node::Node(const T &k, char *info, Node* l, Node* r)
{
    left = l;
    right = r;
    key = k;

    list = MyLinkedList();
    list.add(info);
}

template <class T>
MyBST<T>::Node::Node(const T &k, const MyLinkedList &sourceList, Node* l, Node* r)
{
    left = l;
    right = r;
    key = k;
    list = sourceList;
}

#endif // MYBST_H
