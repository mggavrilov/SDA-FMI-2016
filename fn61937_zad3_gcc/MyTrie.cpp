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

#include "MyTrie.h"

MyTrie::MyTrie()
{
    root = new Node();
}

MyTrie::~MyTrie()
{
    destroy(root);
}

void MyTrie::destroy(Node *start)
{
    Node *temp = start;

    if(temp != nullptr)
    {
        for(int i = 0; i < 27; ++i)
        {
            Node *temp2 = temp->next[i];

            if(temp2 != nullptr)
                destroy(temp2);
        }
    }

    delete temp;
}

void MyTrie::insert(const char *a, double mark)
{
    Node *temp = root;

    while(*a != '\0')
    {
        if(*a == ' ')
        {
            if(temp->next[26] == nullptr)
            {
                temp->next[26] = new Node();
            }

            temp = temp->next[26];
        }
        else
        {
            bool lowerCase = true;
            bool endOfWord = false;

            if(*(a+1) == ' ' || *(a+1) == '\0')
                endOfWord = true;

            if(*a >= 'A' && *a <= 'Z')
            {
                lowerCase = false;
            }

            if(lowerCase)
            {
                if(temp->next[*a - 'a'] == nullptr)
                {
                    temp->next[*a - 'a'] = new Node();
                }

                temp = temp->next[*a - 'a'];
            }
            else
            {
                char lowerCaseA = (char)(*a + 32);

                if(temp->next[lowerCaseA - 'a'] == nullptr)
                {
                    temp->next[lowerCaseA - 'a'] = new Node();
                }

                temp = temp->next[lowerCaseA - 'a'];
            }

            if(endOfWord)
                temp->isWord = true;
        }

        ++a;
        if(*a == '\0')
        {
            temp->mark = mark;
        }
    }
}

bool MyTrie::search(const char *a)
{
    Node *temp = root;

    while(temp != nullptr && *a != '\0')
    {
        if(*a == ' ')
        {
            temp = temp->next[26];
            ++a;
        }
        else
        {
            bool lowerCase = true;
            if(*a >= 'A' && *a <= 'Z')
            {
                lowerCase = false;
            }

            if(lowerCase)
            {
                temp = temp->next[*a - 'a'];
                ++a;
            }
            else
            {
                char lowerCaseA = (char)(*a + 32);

                temp = temp->next[lowerCaseA - 'a'];
                ++a;
            }
        }
    }

    return temp != nullptr && temp->isWord;
}

double MyTrie::getMark(const char *a)
{
    Node *temp = root;

    while(temp != nullptr && *a != '\0')
    {
        if(*a == ' ')
        {
            temp = temp->next[26];
            ++a;
        }
        else
        {
            bool lowerCase = true;
            if(*a >= 'A' && *a <= 'Z')
            {
                lowerCase = false;
            }

            if(lowerCase)
            {
                temp = temp->next[*a - 'a'];
                ++a;
            }
            else
            {
                char lowerCaseA = (char)(*a + 32);

                temp = temp->next[lowerCaseA - 'a'];
                ++a;
            }
        }
    }

    if(temp != nullptr && temp->mark)
        return temp->mark;
    else
        return 0;
}
