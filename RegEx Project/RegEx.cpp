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

#include "RegEx.h"
#include "MyQ.h"
#include "MyStack.h"
#include <stdexcept>

RegEx::RegEx()
{
    m_States = 0;
    startState = -1;
    finalState = -1;
    NFATable = nullptr;
}

RegEx::~RegEx()
{
    clearTable();
}

RegEx::RegEx(const RegEx& other)
{
    m_States = other.m_States;
    startState = other.startState;
    finalState = other.finalState;
    regexString = other.regexString;

    if(m_States)
        copyTable(other);
}

RegEx& RegEx::operator=(const RegEx& rhs)
{
    if (this == &rhs)
        return *this;

    clearTable();

    m_States = rhs.m_States;
    startState = rhs.startState;
    finalState = rhs.finalState;
    regexString = rhs.regexString;

    if(m_States)
        copyTable(rhs);

    return *this;
}

void RegEx::clearTable()
{
    for(int i = 0; i < ALPHABET_SIZE; ++i)
    {
        delete NFATable[i];
    }

    delete[] NFATable;
}

void RegEx::copyTable(const RegEx& other)
{
    NFATable = new TransitionList*[ALPHABET_SIZE];

    for(int i = 0; i < ALPHABET_SIZE; ++i)
    {
        NFATable[i] = new TransitionList[m_States];
    }

    for(int i = 0; i < ALPHABET_SIZE; ++i)
    {
        for(int j = 0; j < m_States; ++j)
        {
            NFATable[i][j] = other.NFATable[i][j];
        }
    }
}

bool RegEx::isOperator(const char c) const
{
    return ((c == '*') || (c == '.') || (c == '|'));
}

bool RegEx::isParenthesis(const char c) const
{
    return ((c == '(') || (c == ')'));
}

bool RegEx::isCharacter(const char c) const
{
    return !(isOperator(c) || isParenthesis(c));
}

//Adds '.' symbol where it hasn't been explicitly put in the input regular expression.
void RegEx::addConcatenation(MyString& s) const
{
    if(!s.length())
        return;

    MyString result;

	for(unsigned int i = 0; i < s.length() - 1; ++i)
	{
		char left = s[i];
		char right = s[i + 1];
		result += left;

		if(left == '\\')  //treat \s \d \a \e \\ as a single operand
            continue;

		if((isCharacter(left)) || (left == ')') || (left == '*'))
			if((isCharacter(right)) || (right == '('))
				result += '.';
	}


    result += s[s.length() - 1];

	s = result;
}

/* Shunting-yard algorithm */
/*
 * Source: https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 *
 * Converts given regular expression from infix to postfix (RPN).
 * Detects if the regular expression is valid or not.
 *
 */
void RegEx::toPostFix(MyString& s) const
{
    if(!s.length())
        return;

    MyStack<char> operatorStack;
    MyQ<char> output;

    for(unsigned int i = 0; i < s.length(); ++i)
    {
        if(s[i] == '\\') //treat \s \d \a \e \\ as a single operand
        {
            output.push(s[i]);
            if(s[i + 1] == 's' || s[i + 1] == 'd' || s[i + 1] == 'a' || s[i + 1] == 'e' || s[i + 1] == '\\')
                output.push(s[i + 1]);
            else
                throw std::invalid_argument("Invalid regular expression: Only \\s \\d \\a \\e and \\\\ are allowed.");

            ++i;
        }
        else if(isCharacter(s[i]))
        {
            output.push(s[i]);
        }
        else
        {
            if(s[i] == '(')
            {
                operatorStack.push(s[i]);
            }
            else if(s[i] == ')')
            {
                while(operatorStack.peek() != '(')
                {
                    output.push(operatorStack.pop());

                    if(operatorStack.isEmpty())
                        throw std::invalid_argument("Invalid regular expression: mismatched parenthesis.");
                }

                operatorStack.pop();
            }
            else
            {
                /*ASCII codes:
                *
                * Kleene star       (*) - 42
                * Concatenation     (.) - 46
                * Union             (|) - 124
                *
                * Using ASCII codes to determine precedence*/

                while(!operatorStack.isEmpty() && isOperator(operatorStack.peek()) && s[i] >= operatorStack.peek())
                    output.push(operatorStack.pop());

                operatorStack.push(s[i]);
            }
        }
    }

    while(!operatorStack.isEmpty())
    {
        if(operatorStack.peek() == '(' || operatorStack.peek() == ')')
            throw std::invalid_argument("Invalid regular expression: mismatched parenthesis.");

        output.push(operatorStack.pop());
    }

    s.clear();

    while(!output.empty())
        s += output.pop();
}

//Converts string to lowercase and substitutes its white-spaces with a space character (ASCII 32).
void RegEx::toLowerCase(MyString& s) const
{
    if(!s.length())
        return;

    MyString result;

    for(unsigned int i = 0; i < s.length(); ++i)
    {
        if(s[i] >= 'A' && s[i] <= 'Z')
            result += (s[i] + 32);
        else if(s[i] == '\t' || s[i] == '\v')
            result += ' ';
        else
            result += s[i];
    }

    s = result;
}

//Counts the states which will be generated while converting the postfix regular expression to NFA.
//State count:
//Reading a character: +2
//Concatenation: +0
//Kleene star: +2
//Union: +2
int RegEx::countNFAStates(const MyString s) const
{
    int nStates = 0;

    for(unsigned int i = 0; i < s.length(); ++i)
    {
        if(s[i] != '.')
        {
            if(s[i] == '\\') //treat \s \d \a \e \\ as a single operand
                ++i;

            nStates += 2;
        }
    }

    return nStates;
}

/* Thompson's construction algorithm */
/*
 * Source: https://en.wikipedia.org/wiki/Thompson's_construction
 *
 * Guarantees exactly 1 initial and 1 final state.
 * Guarantees that each state has at most 2 leaving transitions.
 *
 */
void RegEx::createNFATable(const MyString s)
{
    if(!s.length())
        return;

    m_States = countNFAStates(s);
    NFATable = new TransitionList*[ALPHABET_SIZE];
    for(int i = 0; i < ALPHABET_SIZE; ++i)
    {
        NFATable[i] = new TransitionList[m_States];
    }

    MyStack<Transition> stack;
    int counter = -1, x = 0, y = 0;

    int start = 0, end = 1;

    for(unsigned int i = 0; i < s.length(); ++i)
    {
        if(s[i] == '\\')
        {
            x = ++counter;
            y = ++counter;
            Transition t = Transition(x, y);
            stack.push(t);

            switch(s[i + 1])
            {
                case 's':
                {
                    NFATable[0][x].addTransition(y); //space
                    break;
                }
                case 'd':
                {
                    for(int j = '0'; j <= '9'; ++j)
                    {
                        NFATable[j - ' '][x].addTransition(y);
                    }
                    break;
                }
                case 'a':
                {
                    for(int j = 'a'; j <= 'z'; ++j)
                    {
                        NFATable[j - ' '][x].addTransition(y);
                    }
                    break;
                }
                case 'e':
                {
                    NFATable[ALPHABET_SIZE - 1][x].addTransition(y);
                    break;
                }
                case '\\':
                {
                    NFATable[s[i + 1] - ' '][x].addTransition(y);
                    break;
                }
            }

            ++i;
        }
        else if(isCharacter(s[i]))
        {
            x = ++counter;
            y = ++counter;
            Transition t = Transition(x, y);
            stack.push(t);
            NFATable[s[i] - ' '][x].addTransition(y);
        }
        else
        {
            switch(s[i])
            {
                case '*':
                {
                    Transition t = stack.pop();
                    x = ++counter;
                    y = ++counter;
                    Transition temp = Transition(x, y);
                    stack.push(temp);
                    NFATable[ALPHABET_SIZE - 1][t.end].addTransition(t.start);
                    NFATable[ALPHABET_SIZE - 1][t.end].addTransition(y);
                    NFATable[ALPHABET_SIZE - 1][x].addTransition(t.start);
                    NFATable[ALPHABET_SIZE - 1][x].addTransition(y);

                    if(start == t.start)
                        start = x;

                    if(end == t.end)
                        end = y;

                    break;
                }
                case '.':
                {
                    Transition t2 = stack.pop();
                    Transition t1 = stack.pop();
                    Transition temp = Transition(t1.start, t2.end);
                    stack.push(temp);

                    NFATable[ALPHABET_SIZE - 1][t1.end].addTransition(t2.start);

                    if(start == t2.start)
                        start = t1.start;

                    if(end == t1.end)
                        end = t2.end;

                    break;
                }
                case '|':
                {
                    Transition t2 = stack.pop();
                    Transition t1 = stack.pop();
                    x = ++counter;
                    y = ++counter;
                    Transition temp = Transition(x, y);
                    stack.push(temp);

                    NFATable[ALPHABET_SIZE - 1][x].addTransition(t1.start);
                    NFATable[ALPHABET_SIZE - 1][x].addTransition(t2.start);
                    NFATable[ALPHABET_SIZE - 1][t2.end].addTransition(y);
                    NFATable[ALPHABET_SIZE - 1][t1.end].addTransition(y);

                    if(start == t1.start || start == t2.start)
                        start = x;

                    if(end == t1.end || end == t2.end)
                        end = y;
                }
            }
        }
    }

    Transition t = stack.pop();
    startState = t.start;
    finalState = t.end;
}

void RegEx::consumeEpsilon(int *current) const
{
    int i = 0;
    int counter = 0;
    int *newStates = new int[m_States];
    for(int j = 0; j < m_States; ++j)
    {
        newStates[j] = -1;
    }

    while(current[i] != -1)
    {
        //Break if final state has already been reached
        if(current[i] == finalState)
        {
            newStates[counter++] = current[i];
            break;
        }

        //Given state's transitions can match 1 of 3 cases:
        // 1) It has exactly one transition with a symbol.
        // 2) It has exactly one transition with epsilon.
        // 3) It has two transitions, both with epsilon.
        //Our goal is to move past cases (2) and (3) and keep only states of type (1) in our current state list.
        //So if we reach a state of type (2) or (3), we advance through the epsilon transition until we reach a state from type (1).
        bool transitionWithSymbol = false;
        for(int j = 0; j < ALPHABET_SIZE - 1; ++j) //check against all symbols except the last one (epsilon)
        {
            if(NFATable[j][current[i]].list[0] != -1 || NFATable[j][current[i]].list[1] != -1) //state has transition with a symbol - case (1)
            {
                //Works with \d and \a even though they generate a lot of transitions in the NFATable
                //because we only need to know if it has a transition with *some* letter and find the next state.
                newStates[counter++] = current[i];
                ++i;
                transitionWithSymbol = true;
                break;
            }
        }

        if(transitionWithSymbol) //already reached state of type (1), so no need to remove any epsilon transitions
            continue;

        //Reached a state of type either (2) or (3), or a final state which has no outbound transitions at all
        if(NFATable[ALPHABET_SIZE - 1][current[i]].list[0] != -1)
            newStates[counter++] = NFATable[ALPHABET_SIZE - 1][current[i]].list[0];

        if(NFATable[ALPHABET_SIZE - 1][current[i]].list[1] != -1)
            newStates[counter++] = NFATable[ALPHABET_SIZE - 1][current[i]].list[1];

        ++i;
    }

    for(int i = 0; i < m_States; ++i)
    {
        current[i] = newStates[i];
    }

    delete[] newStates;

    //Check if current contains only transitions with epsilon. If it does, call consumeEpsilon again.
    int k = 0;
    while(current[k] != -1)
    {
        if(NFATable[ALPHABET_SIZE - 1][current[k]].list[0] != -1)
        {
            consumeEpsilon(current);
        }

        ++k;
    }
}

bool RegEx::matchString(MyString s) const
{
    if(regexString.length() == 0) //Match everything if the given regular expression was empty.
        return true;

    if(NFATable == nullptr)
        throw std::invalid_argument("No regular expression given. Please use init() first.");

    if(s.length() == 0)
    {
        if(NFATable[ALPHABET_SIZE - 1][startState].list[0] == finalState || NFATable[ALPHABET_SIZE - 1][startState].list[1] == finalState) //NFA which recognizes the empty string
            return true;
        else
        {
            //On empty matching string, check for a* which can transition to the final state only with epsilons.
            //This means that the empty string is also accepted when a* regular expression is presented (0 occurrences of a).
            int *temp = new int[m_States];
            for(int i = 0; i < m_States; ++i)
            {
                temp[i] = -1;
            }

            temp[0] = startState;
            consumeEpsilon(temp);
            bool hasFinalState = false;

            for(int i = 0; i < m_States; ++i)
            {
                if(temp[i] == finalState)
                    hasFinalState = true;
            }

            return hasFinalState;
        }
    }

    toLowerCase(s);
    int *current = new int[m_States];
    int *next = new int[m_States];
    for(int i = 0; i < m_States; ++i)
    {
        current[i] = -1;
    }

    current[0] = startState;

    unsigned int k = 0;

    do
    {
        consumeEpsilon(current);

        for(int i = 0; i < m_States; ++i)
        {
            next[i] = -1;
        }

        int j = 0, counter = 0;
        while(current[j] != -1)
        {
            if(NFATable[s[k] - ' '][current[j]].list[0] != -1)
                next[counter++] = NFATable[s[k] - ' '][current[j]].list[0];

            ++j;
        }

        int *temp = current;
        current = next;
        next = temp;

        ++k;

    } while(k < s.length());

    consumeEpsilon(current);

    bool reachedFinalState = false;
    int i = 0;
    while(current[i] != -1)
    {
        if(current[i] == finalState)
            reachedFinalState = true;

        ++i;
    }

    delete[] current;
    delete[] next;

    return reachedFinalState;
}

void RegEx::init(MyString s)
{
    toLowerCase(s);
    addConcatenation(s);
    toPostFix(s);
    createNFATable(s);
    regexString = s;
}
