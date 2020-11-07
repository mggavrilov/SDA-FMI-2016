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

#include "MyString.h"
#include <stdexcept>
#include <cstring>

MyString::MyString()
{
    str = new char[DEFAULT_SIZE];
    str[0] = 0;
    strlength = 0;
    maxSize = DEFAULT_SIZE;
}

MyString::MyString(const char *s)
{
    unsigned int len = strlen(s);
    str = new char[len + DEFAULT_SIZE];

    for(unsigned int i = 0; i <= len; ++i)
    {
        str[i] = s[i];
    }

    maxSize = len + DEFAULT_SIZE;
    strlength = len;
}

MyString::MyString(const MyString& s)
{
    str = new char[s.maxSize];
    memcpy(str, s.str, s.maxSize);
    maxSize = s.maxSize;
    strlength = s.strlength;
}

unsigned int MyString::length()
{
    return strlength;
}

void MyString::concat()
{

}

char* MyString::c_str()
{
    return str;
}

bool MyString::compare(const char *s)
{
    for(unsigned int i = 0; i <= strlen(s); ++i)
    {
        if(str[i] != s[i])
            return true;
    }

    return false;
}

bool MyString::compare(const MyString& s)
{
    return this->compare(s.str);
}

MyString& MyString::operator=(const char *s)
{
    unsigned int len = strlen(s);
    str = new char[len + DEFAULT_SIZE];

    for(unsigned int i = 0; i <= len; ++i)
    {
        str[i] = s[i];
    }

    maxSize = len + DEFAULT_SIZE;
    strlength = len;

    return *this;
}

MyString& MyString::operator=(const MyString &s)
{
    str = new char[s.maxSize];
    memcpy(str, s.str, s.maxSize);
    maxSize = s.maxSize;
    strlength = s.strlength;

    return *this;
}

MyString& MyString::operator+=(const char c)
{
    if(strlength + 2 > maxSize)
    {
        char *temp = new char[maxSize + DEFAULT_SIZE];

        for(unsigned int i = 0; i <= strlength; ++i)
        {
            temp[i] = str[i];
        }

        str = temp;
        str[strlength] = c;
        str[strlength + 1] = 0;
        ++strlength;
        maxSize += DEFAULT_SIZE;
    }
    else
    {
        str[strlength] = c;
        str[strlength + 1] = 0;
        ++strlength;
    }

    return *this;
}

MyString& MyString::operator+=(const char *s)
{
    unsigned int len = strlen(s);

    if(strlength + len > maxSize)
    {
        char *temp = new char[maxSize + len + DEFAULT_SIZE];

        for(unsigned int i = 0; i <= strlength; ++i)
        {
            temp[i] = str[i];
        }

        str = temp;

        for(unsigned int i = 0; i <= strlength; ++i)
        {
            str[strlength + i] = s[i];
        }

        strlength += len;
        maxSize += (len + DEFAULT_SIZE);
    }
    else
    {
        for(unsigned int i = 0; i <= strlength; ++i)
        {
            str[strlength + i] = s[i];
        }

        strlength += len;
    }

    return *this;
}

MyString& MyString::operator+=(const MyString &s)
{
    if(strlength + s.strlength > maxSize)
    {
        char *temp = new char[maxSize + s.strlength + DEFAULT_SIZE];
        memcpy(temp, str, maxSize);
        str = temp;
        memcpy(str + strlength, s.str, s.strlength);

        strlength += s.strlength;
        maxSize += (s.strlength + DEFAULT_SIZE);
    }
    else
    {
        memcpy(str + strlength, s.str, s.strlength);
        strlength += s.strlength;
    }

    return *this;
}

char& MyString::operator[](const unsigned int x)
{
    if(x < strlength)
        return str[x];
    else
        throw std::invalid_argument("Invalid index.");
}

std::ostream& operator<<(std::ostream& o, const MyString& s)
{
    o << s.str;
    return o;
}

std::istream& operator>>(std::istream& i, MyString& s)
{
    char temp[100];
    i >> temp;
    unsigned int len = strlen(temp);
    s.str = new char[len + DEFAULT_SIZE];

    for(unsigned int i = 0; i <= len; ++i)
    {
        s.str[i] = temp[i];
    }

    s.maxSize = len + DEFAULT_SIZE;
    s.strlength = len;
    return i;
}

MyString::~MyString()
{
    delete[] str;
}
