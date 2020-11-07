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

#include <iostream>
#include <cstdint>
#include <fstream>
#include "MyHashMap.h"

using namespace std;

int main()
{
    unsigned int nFiles;
    cin >> nFiles;

    if(nFiles < 2)
    {
        cerr << "Can't find intersection of less than 2 files." << endl;
        return 1;
    }

    ifstream *files = new ifstream[nFiles]; //array of files containing uint64_t numbers
    char pathToFile[256];
    ios::pos_type minFileSize; //streampos to find file with smallest size and use it as a base dictionary for the hash
    unsigned int arrPos; //holds the index of the file with the smallest size in the files array

    //find size of first file
    cin >> pathToFile;
    files[0].open(pathToFile, ios::ate | ios::binary); //opens the file and seeks to the end
    minFileSize = files[0].tellg();
    arrPos = 0;
    files[0].clear();
    files[0].seekg(0, ios::beg); //seek to the beginning of the file

    //find minimum file size
    for(unsigned int i = 1; i < nFiles; ++i)
    {
        cin >> pathToFile;
        files[i].open(pathToFile, ios::ate | ios::binary);
        ios::pos_type fileSize = files[i].tellg();
        if(fileSize < minFileSize)
        {
            minFileSize = fileSize;
            arrPos = i;
        }

        files[i].clear();
        files[i].seekg(0, ios::beg);
    }

    MyHashMap hash1 = MyHashMap();

    //smallest file is found and is located on index arrPos; hash it
    if(files[arrPos].is_open())
    {
        while(!files[arrPos].eof())
        {
            uint64_t k;

            files[arrPos].read((char*)&k, 8);

            if(!hash1.search(k))
                hash1.put(k);
        }

        files[arrPos].close();
    }
    else
    {
        cerr << "Couldn't open " << files[arrPos];
        return 1;
    }

    //check every file against the hash to find the intersection of them all
    //create new hash containing only numbers which were found in the previous hash
    //stop if at some point the intersection becomes empty

    MyHashMap hash2 = MyHashMap();

    bool useSecondHash = true;
    uint64_t k;

    for(unsigned int i = 0; i < nFiles; ++i)
    {
        if(i == arrPos)
            continue;

        if(files[i].is_open())
        {
            while(!files[i].eof())
            {
                files[i].read((char*)&k, 8);

                if(useSecondHash)
                {
                    if(hash1.search(k) && !hash2.search(k)) //don't allow repeating numbers
                        hash2.put(k);
                }
                else
                {
                    if(hash2.search(k) && !hash1.search(k)) //don't allow repeating numbers
                        hash1.put(k);
                }
            }

            files[i].close();
        }
        else
        {
            cerr << "Couldn't open " << files[i];
            return 1;
        }

        if(useSecondHash)
        {
            if(hash2.getSize() == 0)
            {
                cerr << "Intersection of selected files is empty." << endl;
                return 1;
            }

            useSecondHash = false;
            hash1.removeall();
        }
        else
        {
            if(hash1.getSize() == 0)
            {
                cerr << "Intersection of selected files is empty." << endl;
                return 1;
            }

            useSecondHash = true;
            hash2.removeall();
        }
    }

    uint64_t *result;
    int arrSize;

    if(useSecondHash)
    {
        arrSize = hash1.getSize();
        result = hash1.getall();
    }
    else
    {
        arrSize = hash2.getSize();
        result = hash2.getall();
    }

    //save the intersection of the files in result.bin

    ofstream resultFile("result.bin", ios::binary);

    for(int i = 0; i < arrSize; ++i)
    {
        resultFile.write((char*)&result[i], 8);
    }

    resultFile.close();

    delete[] result;
    delete[] files;

    return 0;
}
