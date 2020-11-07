/**
 *
 * Solution to homework task
 * Data Structures Course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2016/2017
 *
 * @author Martin Gavrilov
 * @idnumber 61937
 * @task 2
 * @compiler GCC
 *
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include "Stack.h"
#include "MyQ.h"

struct Query
{
	char dir[5];
	int floor;
	int time;
};

struct Stop
{
    int time;
    int floor;
    int lastFloor;
    char dir[5];

    Stop() {}

    Stop(int time, int floor, int lastFloor, char dir[])
    {
        this->time = time;
        this->floor = floor;
        this->lastFloor = lastFloor;
        strcpy(this->dir, dir);
    }
};

using namespace std;

//seconds per floor
const int ELEVATOR_SPEED = 5;

int main(int argc, char* argv[])
{
    ifstream queryFile (argv[1]);

    //Queue for queries
    MyQ<Query> queryQ = MyQ<Query>();
    int floors, queries;

    int currentFloor = 1;
    int lastFloor = 1;
    int currentTime = 0;
    char lastDirection[] = "down";

    char dir[5];
    int floor, time;

    if(queryFile.is_open())
    {
        queryFile >> floors >> queries;

        for(int i = 0; i < queries; ++i)
        {
            char cmd[5];
            queryFile >> cmd;

            if(cmd[0] == 'c')
                queryFile >> dir >> floor >> time;
            else
            {
                queryFile >> floor >> time;
                strcpy(dir, "null");
            }

            if(floor > floors || time < 0)
            {
                cerr << "Error";
                return 1;
            }

            Query query;
            strcpy(query.dir, dir);
            query.floor = floor;
            query.time = time;
            queryQ.push(query);
        }

        queryFile.close();
    }
    else
    {
        cerr << "Couldn't open " << argv[1];
        return 1;
    }

    //Stack for floor stops
    Stack<Stop> stack = Stack<Stop>(queries);

    /****************************************************************************************************************
    *
    *                                               Algorithm used
    *
    *****************************************************************************************************************
    *
    * While there are queries left in the queue:
    *   If the stack of stops doesn't contain any stops:
    *       1) Pop the first query off the queue.
    *       2) Calculate the time of arrival for the stop.
    *       3) Push stop to stack.
    *
    *   The stack contains at least one stop:
    *       Search the queue for a query with higher priority than the stack's next stop.
    *           Pop element off queue.
    *           If the queue's popped element is higher priority than the one in the stack:
    *               1) Push it to the stack.
    *               2) Pop all remaining queue queries and push them to the back of the queue (to keep proper order).
    *               3) Search queue for yet another higher priority element.
    *           Else: the queue's popped element isn't higher priority than the one in the stack:
    *               Push it to the back of the queue for future evaluation.
    *       When the search ends and it hasn't found a higher priority element:
    *           Pop one element off the stack and continue search for an element that has
    *           higher priority than stack's next element.
    *       Else: the search ends and it has found a higher priority element:
    *           Continue next while loop iteration and search for yet another higher priority element
    *           (than the last one found).
    *
    * In case that every given query in the queue had higher priority than the previous one:
    *   There are stops remaining in the stack, pop them all.
    *
    ****************************************************************************************************************/

    while(!queryQ.empty())
    {
        if(stack.isEmpty())
        {
            Query tempQuery = queryQ.pop();
            lastFloor = currentFloor;

            //If the next query starts some time after the last one has finished, calculate the gap of time as well
            int gapTime = tempQuery.time > currentTime ? (tempQuery.time - currentTime) : 0;
            //Depends on direction: first case direction is down; second one - up
            int floorDifference = currentFloor > tempQuery.floor ? (currentFloor - tempQuery.floor) : (tempQuery.floor - currentFloor);

            currentTime += (gapTime +  floorDifference * ELEVATOR_SPEED);

            if(currentFloor > tempQuery.floor)
                strcpy(lastDirection, "down");
            else
                strcpy(lastDirection, "up");

            currentFloor = tempQuery.floor;

            Stop stop = Stop(currentTime, currentFloor, lastFloor, lastDirection);
            stack.push(stop);
        }

        bool searchAgain = false;
        int qSize = queryQ.size();

        for(int i = 1; i <= qSize; ++i)
        {
            Query query = queryQ.pop();

            //Pop all remaining queue queries and push them to the back of the queue (to keep proper order).
            if(searchAgain)
            {
                queryQ.push(query);
                continue;
            }

            bool goingUp = query.floor > currentFloor ? true : false;
            int timeLimit = currentTime - (goingUp ? query.floor - currentFloor : currentFloor - query.floor) * ELEVATOR_SPEED;
            bool isFloorValid = false;

            if((lastFloor >= query.floor && query.floor >= currentFloor) || (lastFloor <= query.floor && query.floor <= currentFloor))
                isFloorValid = true;

            if(query.time <= timeLimit && isFloorValid)
            {
                Stop tempStop = stack.peek();

                int floorDifference;
                if(query.floor <= tempStop.floor)
                    floorDifference = tempStop.floor - query.floor;
                else
                    floorDifference = query.floor - tempStop.floor;

                Stop newStop = Stop(tempStop.time - floorDifference * ELEVATOR_SPEED, query.floor, tempStop.lastFloor, tempStop.dir);

                if(!(tempStop.time == newStop.time && tempStop.floor == newStop.floor))
                {
                    lastFloor = newStop.lastFloor;
                    currentTime = newStop.time;
                    currentFloor = newStop.floor;
                    strcpy(lastDirection, newStop.dir);
                    stack.push(newStop);
                }

                //If the search found an element with higher priority, don't pop the stack,
                //search for yet another higher priority element.
                searchAgain = true;
                continue;
            }
            else
            {
                //If the queue's popped element has lower priority than stack's next element,
                //push it to the back of the queue for future evaluation.
                queryQ.push(query);
            }
        }

        //If the queue doesn't contain a higher priority element than stack's next element,
        //pop the stack and start the search again for stack's next element.
        if(!searchAgain)
        {
            Stop temp = stack.pop();
            cout << temp.time << " " << temp.floor << " " << temp.dir << endl;
            if(!stack.isEmpty())
            {
                Stop tempStop = stack.peek();
                lastFloor = tempStop.lastFloor;
                currentTime = tempStop.time;
                currentFloor = tempStop.floor;
                strcpy(lastDirection, tempStop.dir);
            }
        }
    }

    //Pop all remaining stack elements when queue is empty in case
    //any given element in the queue had higher priority than the last one.
    while(!stack.isEmpty())
    {
        Stop temp = stack.pop();
        cout << temp.time << " " << temp.floor << " " << temp.dir << endl;
    }

    return 0;
}
