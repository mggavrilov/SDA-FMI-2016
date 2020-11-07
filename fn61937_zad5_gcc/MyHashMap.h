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

#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#define START_SIZE 10007
#define FULNESS_COEF 0.75f

#include <cstdint>
#include "MyLinkedList.h"

class MyHashMap
{
    public:
        MyHashMap();
        virtual ~MyHashMap();
        MyHashMap(const MyHashMap& other);
        MyHashMap& operator=(const MyHashMap& other);
        void put(uint64_t k);
        void remove(uint64_t k);
        bool search(uint64_t k) const;
        void removeall(unsigned int m = START_SIZE);
        unsigned int getSize() const;
        uint64_t* getall() const;

    protected:

    private:
        unsigned int size;
        unsigned int maxSize;
        MyLinkedList *hashMap;

        unsigned int hash(uint64_t k) const;
        unsigned int getNextPrime(unsigned int k) const;
        bool isPrime(unsigned int k) const;
};

#endif // MYHASHMAP_H
