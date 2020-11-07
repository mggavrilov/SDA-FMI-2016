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

#include <iostream>
#include <fstream>
#include <stdlib.h> //atof
#include "MyTrie.h"
#include "MyString.h"

using namespace std;

void trim(MyString &s);

int main(int argc, char* argv[])
{
    if(argc < 3)
        return 0;

    ifstream dictionaryFile (argv[1]);

    MyTrie dictionary = MyTrie();

    MyString word = MyString();
    MyString tempWord = MyString();

    if(dictionaryFile.is_open())
    {
        while(!dictionaryFile.eof())
        {
            dictionaryFile >> tempWord;

            if(!tempWord.isNumeric())
            {
                if(word.length())
                    word += " ";

                word += tempWord;
            }
            else
            {
                dictionary.insert(word.c_str(), atof(tempWord.c_str()));
                word = MyString();
            }
        }
    }
    else
    {
        cerr << "Couldn't open " << argv[1];
        return 1;
    }

    ifstream *text = new ifstream[argc - 2];

    for(int i = 2; i < argc; ++i)
    {
        text[i - 2].open(argv[i]);
    }

    MyString textWord = MyString();
    MyString expression = MyString();
    for(int i = 2; i < argc; ++i)
    {
        double mark = 0;
        int words = 0;

        if(text[i - 2].is_open())
        {
            while(!text[i - 2].eof())
            {
                text[i - 2] >> textWord;
                expression.clear();
                trim(textWord);


                ++words;

                bool isPrefix = dictionary.search(textWord.c_str());
                double isWord = dictionary.getMark(textWord.c_str());

                expression += textWord;

                if(isPrefix && !isWord)
                {
                    while(isPrefix)
                    {
                        text[i - 2] >> textWord;

                        trim(textWord);
                        ++words;
                        MyString temp = MyString();
                        temp = expression;
                        temp += " ";
                        temp += textWord;
                        isPrefix = dictionary.search(temp.c_str());
                        if(isPrefix)
                        {
                            expression = temp;
                        }

                        double isWord = dictionary.getMark(expression.c_str());

                        if(isWord)
                            break;
                    }
                }

                mark += dictionary.getMark(expression.c_str());
            }

            cout << mark << " " << words << endl;

            cout << argv[i] << " " << (mark / words);

            text[i - 2].close();
        }
    }

    return 0;
}

void trim(MyString &s)
{
    int length = s.length();
    MyString temp = MyString(s);
    s.clear();

    for(int i = 0; i < length; ++i)
    {
        if((temp[i] >= 'a' && temp[i] <= 'z') || (temp[i] >= 'A' && temp[i] <= 'Z'))
            s += temp[i];
    }
}
