/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 1
 * @compiler GCC
 *
 */

#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>

const int DEFAULT_SIZE = 20;

class MyString
{
    char *str;
    unsigned int strlength, maxSize;

    public:
        MyString();
        MyString(const char *s);
        MyString(const MyString& s); //copy constructor
        virtual ~MyString();
        unsigned int length();
        void concat();
        MyString& operator=(const char *s);
        MyString& operator=(const MyString &s);
        MyString& operator+=(const char c);
        MyString& operator+=(const char *s);
        MyString& operator+=(const MyString &s);
        char& operator[](const unsigned int x);
        char* c_str();
        bool compare(const char *s);
        bool compare(const MyString& s);
        friend std::ostream& operator<<(std::ostream& o, const MyString& s);
        friend std::istream& operator>>(std::istream& i, MyString& s);
};

#endif // MYSTRING_H
