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

#ifndef REGEX_H
#define REGEX_H

#define ALPHABET_SIZE 96 //ASCII 32-126 + Epsilon

#include "MyString.h"

class RegEx
{
    public:
        RegEx();
        virtual ~RegEx();
        RegEx(const RegEx& other);
        RegEx& operator=(const RegEx& other);
        bool isOperator(const char c) const;
        bool isParenthesis(const char c) const;
        bool isCharacter(const char c) const;
        void addConcatenation(MyString& s) const;
        void toPostFix(MyString& s) const;
        void toLowerCase(MyString& s) const;
        int countNFAStates(const MyString s) const;
        bool matchString(MyString s) const;
        void init(MyString s);

    protected:
        struct Transition
        {
            int start;
            int end;

            Transition() {}

            Transition(int s, int e)
            {
                start = s;
                end = e;
            }
        };

        struct TransitionList
        {
            int list[2];

            TransitionList()
            {
                list[0] = -1;
                list[1] = -1;
            }

            void addTransition(int t)
            {
                if(list[0] == -1)
                    list[0] = t;
                else
                    list[1] = t;
            }
        };

    private:
        TransitionList **NFATable;
        int m_States; //number of states in NFA
        int startState;
        int finalState;
        MyString regexString;

        void clearTable();
        void copyTable(const RegEx& other);
        void createNFATable(const MyString s);
        void consumeEpsilon(int *current) const;
};

#endif // REGEX_H
