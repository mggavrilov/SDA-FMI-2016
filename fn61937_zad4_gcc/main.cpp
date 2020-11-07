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

#include <iostream>
#include <cstring>
#include <fstream>
#include "MyBST.h"

using namespace std;

struct DataNode
{
    int key;
    char *data;

    DataNode()
    {
        key = 0;
        data = nullptr;
    }

    DataNode(int key, char *data)
    {
        this->key = key;
        this->data = data;
    }
};

int partition(DataNode *arr, int l, int h)
{
    int pivot = arr[h].key;
    int i = l;

    for(int j = l; j < h; j++)
    {
        if(arr[j].key <= pivot)
        {
            DataNode temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
        }
    }

    DataNode temp = arr[i];
    arr[i] = arr[h];
    arr[h] = temp;

    return i;
}

void quicksort(DataNode *arr, int l, int h)
{
    if(l < h)
    {
        int p = partition(arr, l, h);
        quicksort(arr, l, p - 1);
        quicksort(arr, p + 1, h);
    }
}

void binary_insert(const DataNode *arr, MyBST<int> &tree, int start, int end)
{
    if(start > end)
        return;

    int mid = start + ((end - start) / 2);

    tree.add(arr[mid].key, arr[mid].data);

    binary_insert(arr, tree, start, mid - 1);
    binary_insert(arr, tree, mid + 1, end);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
        return 0;

    ifstream binFile (argv[1], ios::binary);

    int elements = 0;
    int maxSize = 100;

    //array which will hold the elements received from the binary file
    DataNode *arr = new DataNode[maxSize];

    if(binFile.is_open())
    {
        while(!binFile.eof())
        {
            int key;
            int dataSize;

            binFile.read((char*)&key, 4);
            binFile.read((char*)&dataSize, 4);

            //string for the corresponding key from the binary file
            char *data = new char[dataSize + 1];

            binFile.read(data, dataSize);
            data[dataSize] = 0; //null-terminate the string

            //resize element array if its capacity is exceeded
            if(elements == maxSize)
            {
                DataNode *temp = new DataNode[elements * 2]; //doubles the size every time it's exceeded
                for(int i = 0; i < elements; ++i)
                {
                    temp[i] = arr[i];
                }

                delete[] arr;
                arr = temp;
                maxSize = elements * 2;
            }

            arr[elements++] = DataNode(key, data);
        }
    }
    else
    {
        cerr << "Couldn't open " << argv[1];
        return 1;
    }

    //Sort elements by key
    quicksort(arr, 0, elements);

    MyBST<int> tree = MyBST<int>();

    //Insert elements in 'binary search order' to guarantee perfectly balanced BST
    binary_insert(arr, tree, 0, elements);

    //BST Queries
    while(true)
    {
        int chars = 0;
        int maxStrSize = 100;

        char cmd[10];
        int key;
        char *str = new char[maxStrSize];

        cin >> cmd;
        cin >> key;

        if(strcmp(cmd, "removeall") == 0)
        {
            cout << tree.removeall(key) << endl;

            char c = cin.peek();

            if(c == 3 || c == 4 || c == 26) // Ctrl+C, Ctrl+D, Ctrl+Z
                break;
        }
        else
        {
            char c;
            bool endOfInput = false;

            while(cin.get(c))
            {
                if(c == '\n')
                    break;

                if(c == 3 || c == 4 || c == 26) // Ctrl+C, Ctrl+D, Ctrl+Z
                {
                    endOfInput = true;
                    break;
                }

                if(chars == maxStrSize - 1)
                {
                    char *temp = new char[chars * 2];
                    for(int i = 0; i < elements; ++i)
                    {
                        temp[i] = str[i];
                    }

                    delete[] str;
                    str = temp;
                    maxStrSize = chars * 2;
                }

                str[chars++] = c;
            }

            str[chars] = 0; //null-terminate the string

            if(strcmp(cmd, "add") == 0)
            {
                tree.add(key, str);
            }
            else if(strcmp(cmd, "remove") == 0)
            {
                if(tree.remove(key, str))
                    cout << "true" << endl;
                else
                    cout << "false" << endl;
            }
            else if(strcmp(cmd, "search") == 0)
            {
                if(tree.search(key, str))
                    cout << "true" << endl;
                else
                    cout << "false" << endl;
            }
            else
            {
                cout << "Invalid operation." << endl;
                return 0;
            }

            if(endOfInput)
                break;
        }
    }

    delete[] arr;

    return 0;
}
