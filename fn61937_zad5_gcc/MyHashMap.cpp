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

#include "MyHashMap.h"

MyHashMap::MyHashMap()
{
    size = 0;
    maxSize = START_SIZE;
    hashMap = new MyLinkedList[START_SIZE];
}

MyHashMap::~MyHashMap()
{
    delete[] hashMap;
}

MyHashMap::MyHashMap(const MyHashMap& other)
{
    size = other.size;
    maxSize = other.maxSize;
    hashMap = other.hashMap;
}

MyHashMap& MyHashMap::operator=(const MyHashMap& rhs)
{
    if (this == &rhs)
        return *this;

    delete[] hashMap;

    size = rhs.size;
    maxSize = rhs.maxSize;
    hashMap = rhs.hashMap;

    return *this;
}

void MyHashMap::put(uint64_t k)
{
    ++size;
    unsigned int pos = hash(k);
    hashMap[pos].add(k);

    if(size * FULNESS_COEF >= maxSize)
    {
        unsigned int newSize = getNextPrime(maxSize * 2);

        MyLinkedList *oldHashMap = hashMap;

        delete[] hashMap;

        hashMap = new MyLinkedList[newSize];

        unsigned int oldSize = maxSize;

        maxSize = newSize;

        for(unsigned int i = 0; i < oldSize; ++i)
        {
            if(!oldHashMap[i].isEmpty())
            {
                uint64_t *temp = oldHashMap[i].getall();
                unsigned int arrSize = oldHashMap[i].getSize();

                for(unsigned int j = 0; j < arrSize; ++j)
                {
                    put(temp[j]);
                }
            }
        }
    }
}

void MyHashMap::remove(uint64_t k)
{
    unsigned int pos = hash(k);
    if(hashMap[pos].remove(k))
        --size;
}

bool MyHashMap::search(uint64_t k) const
{
    unsigned int pos = hash(k);

    if(hashMap[pos].search(k))
        return true;
    else
        return false;
}

void MyHashMap::removeall(unsigned int m)
{
    if(!isPrime(m))
        m = getNextPrime(m);

    size = 0;
    maxSize = m;
    delete[] hashMap;
    hashMap = new MyLinkedList[m];
}

unsigned int MyHashMap::getSize() const
{
    return size;
}

unsigned int MyHashMap::hash(uint64_t k) const
{
    return k % maxSize;
}

uint64_t* MyHashMap::getall() const
{
    uint64_t *arr = new uint64_t[size];
    int counter = 0;

    for(unsigned int i = 0; i < maxSize; ++i)
    {
        if(!hashMap[i].isEmpty())
        {
            uint64_t *temp = hashMap[i].getall();
            unsigned int arrSize = hashMap[i].getSize();

            for(unsigned int j = 0; j < arrSize; ++j)
            {
                arr[counter++] = temp[j];
            }

            delete[] temp;
        }
    }

    return arr;
}

unsigned int MyHashMap::getNextPrime(unsigned int k) const
{
    int currentNumber = k + 1;

    for(int i = 2; i <= currentNumber / 2; ++i)
    {
        if(currentNumber % i == 0)
        {
            ++currentNumber;
            i = 2;
        }
    }

    return currentNumber;
}

bool MyHashMap::isPrime(unsigned int k) const
{
    for(unsigned int i = 2; i <= k / 2; ++i)
    {
        if(k % i == 0)
            return false;
    }

    return true;
}
