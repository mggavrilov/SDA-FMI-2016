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

#include <iostream>
#include "MyStack.h"
#include "MyString.h"
#include "MyQ.h"
#include "RegEx.h"
#include <fstream>
#include <io.h>
#include <cstring>

using namespace std;

void matchString(const MyString fileString, RegEx *r)
{
    ifstream iFile (fileString.c_str());

    MyString lineBuffer;
    int lineCounter = 1;

    if(iFile.peek() == -1) // Handle empty files
    {
        if(r->matchString(lineBuffer))
            cout << fileString << ":" << lineCounter << ":" << lineBuffer << endl;

        return;
    }

    if(iFile.is_open())
    {
        while(iFile >> lineBuffer)
        {
            if(r->matchString(lineBuffer))
                cout << fileString << ":" << lineCounter << ":" << lineBuffer << endl;

            ++lineCounter;
        }
    }
    else
    {
        cerr << "Couldn't open " << fileString << "." << endl;
    }
}

void parseDirectory(MyString s, RegEx *r)
{
    _finddata_t file;

    MyString temp = s;
    temp += "\\*.*";
    int found = _findfirst(temp.c_str(), &file);
    if(found != -1)
    {
        int result = 0;
        while(result != -1)
        {
            if(strcmp(file.name, ".") && strcmp(file.name, "..")) //don't check current directory and parent directory entries
            {
                if((file.attrib & _A_SUBDIR) == _A_SUBDIR)
                {
                    MyString s1 = s;
                    s1 += '\\';
                    s1 += file.name;
                    parseDirectory(s1, r);
                }
                else
                {
                    MyString fileString = s;
                    fileString += "\\";
                    fileString += file.name;
                    matchString(fileString, r);
                }
            }

            result = _findnext(found, &file);
        }
        _findclose(found);
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2) //Accept empty regular expression
    {
        cerr << "Usage: " << argv[0] << " <filename> <regular expression>" << endl;
        return 1;
    }

    MyString target = argv[1];
    MyString regex = argv[2];

    /*If you pass "\\" in the console it comes as \ in the program
     * If you pass \\ in the console it comes as \\ in the program
     * If you pass a whole expression like the one in the examples "(((a*).b).(\\|/))" the \\ is recognized as \\
     * So this check is only for occasions when you want to match just the backslash symbol.
     */

    if(regex == "\\")
        regex = "\\\\";

    RegEx *r = new RegEx();
    r->init(regex);
    ifstream i (target.c_str());
    if(!i)
    {
        parseDirectory(target, r);
    }
    else
    {
        i.close();
        matchString(target, r);
    }

    return 0;
}
