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

#include "MyString.h"

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
    delete[] str;
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
    delete[] str;
    str = new char[s.maxSize];
    memcpy(str, s.str, s.strlength + 1);
    maxSize = s.maxSize;
    strlength = s.strlength;
}

unsigned int MyString::length() const
{
    return strlength;
}

char* MyString::c_str() const
{
    return str;
}

bool MyString::compare(const char *s) const
{
    unsigned int len = strlen(s);

    if(len != strlength)
        return false;

    for(unsigned int i = 0; i < len; ++i)
    {
        if(str[i] != s[i])
            return true;
    }

    return false;
}

bool MyString::compare(const MyString& s) const
{
    return this->compare(s.str);
}

bool MyString::isNumeric() const
{
    int dashCounter = 0;

    for(unsigned int i = 0; i < strlength; ++i)
    {
        if(str[i] == '-')
            ++dashCounter;

        if(dashCounter > 1 || (str[i] != '-' && (str[i] < '0' || str[i] > '9')))
            return false;
    }

    return true;
}

void MyString::clear()
{
    str[0] = 0;
    strlength = 0;
}

MyString& MyString::operator=(const char *s)
{
    unsigned int len = strlen(s);
    delete[] str;
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
    delete[] str;
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

        for(unsigned int i = 0; i < strlength; ++i)
        {
            temp[i] = str[i];
        }

        delete[] str;
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

    if(strlength + len + 1 > maxSize)
    {
        char *temp = new char[maxSize + len + DEFAULT_SIZE];

        for(unsigned int i = 0; i < strlength; ++i)
        {
            temp[i] = str[i];
        }

        delete[] str;
        str = temp;

        for(unsigned int i = 0; i <= len; ++i)
        {
            str[strlength + i] = s[i];
        }

        strlength += len;
        maxSize += (len + DEFAULT_SIZE);
    }
    else
    {
        for(unsigned int i = 0; i <= len; ++i)
        {
            str[strlength + i] = s[i];
        }

        strlength += len;
    }

    return *this;
}

MyString& MyString::operator+=(const MyString &s)
{
    if(strlength + s.strlength + 1 > maxSize)
    {
        char *temp = new char[maxSize + s.strlength + DEFAULT_SIZE];
        memcpy(temp, str, strlength);
        delete[] str;
        str = temp;
        memcpy(str + strlength, s.str, s.strlength + 1);

        strlength += s.strlength;
        maxSize += (s.strlength + DEFAULT_SIZE);
    }
    else
    {
        memcpy(str + strlength, s.str, s.strlength + 1);
        strlength += s.strlength;
    }

    return *this;
}

char& MyString::operator[](const unsigned int x) const
{
    if(x >= 0 && x < strlength)
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
    char c;
    s.strlength = 0;
    s.maxSize = DEFAULT_SIZE;
    s.str = new char[DEFAULT_SIZE];

    while(i.get(c))
    {
        if(c == '\n' || c == '\t' || c == ' ' || c == '\r')
        {
            char p;
            do
            {
                p = i.peek();

                if(p == '\n' || p == '\t' || p == ' ' || p == '\r')
                    i.get(p);
            }
            while(p == '\n' || p == '\t' || p == ' ' || p == '\r');

            break;
        }

        if((c < '0' || c > '9') && (c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && c != '-')
        {
            char p;
            do
            {
                p = i.peek();

                if((p < '0' || p > '9') && (p < 'a' || p > 'z') && (p < 'A' || p > 'Z') && p != '-')
                    i.get(p);
            }
            while((p < '0' || p > '9') && (p < 'a' || p > 'z') && (p < 'A' || p > 'Z') && p != '-');

            continue;
        }

        if(s.strlength + 2 > s.maxSize)
        {
            char *temp = new char[s.maxSize + DEFAULT_SIZE];

            for(unsigned int i = 0; i <= s.strlength; ++i)
            {
                temp[i] = s.str[i];
            }

            delete[] s.str;
            s.str = temp;
            s.str[s.strlength++] = c;
            s.maxSize += DEFAULT_SIZE;
        }
        else
        {
            s.str[s.strlength++] = c;
        }
    }

    s.str[s.strlength] = 0;

    return i;
}

MyString::~MyString()
{
    delete[] str;
}
