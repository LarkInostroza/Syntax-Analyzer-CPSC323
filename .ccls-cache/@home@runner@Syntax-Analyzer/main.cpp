#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

//LR parsing table
//'i' represents id
unordered_map<int, unordered_map<char, string>> parsingTable = {
    {0, {{'i', "S5"}, {'(', "S4"}, {'E', "1"}, {'T', "2"}, {'F', "3"}}},
    {1, {{'+', "S6"}, {'$', "acc"}}},
    {2, {{'+', "R2"}, {'*', "S7"}, {')', "R2"}, {'$', "R2"}}},
    {3, {{'+', "R4"}, {'*', "R4"}, {')', "R4"}, {'$', "R4"}}},
    {4, {{'i', "S5"}, {'(', "S4"}, {'E', "8"}, {'T', "2"}, {'F', "3"}}},
    {5, {{'+', "R6"}, {'*', "R6"}, {')', "R6"}, {'$', "R6"}}},
    {6, {{'i', "S5"}, {'(', "S4"}, {'T', "9"}, {'F', "3"}}},
    {7, {{'i', "S5"}, {'(', "S4"}, {'F', "10"}}},
    {8, {{'+', "S6"}, {')', "S11"}}},
    {9, {{'+', "R1"}, {'*', "S7"}, {')', "R1"}, {'$', "R1"}}},
    {10,{{'+',"R3"}, {'*', "R3"}, {')', "R3"}, {'$', "R3"}}},
    {11,{{'+', "R5"}, {'*', "R5"}, {')', "R5"}, {'$', "R5"}}}
};

//Stack, push, pop, peek operations
stack<int> st;
void push(int item) { 
  st.push(item); 
}
int pop() {
    int top = st.top();
    st.pop();
    return top;
}
int peek() { 
  return st.top(); 
}

// Parse the input string
void parseInput(string input) {
    push(0);
    input += "$";
    int pos = 0;
    while (true) {
        int state = peek();
        char symbol = input[pos];
        string action = parsingTable[state][symbol];
        /*
        if (action == "-1") {
            cout << "String is not accepted." << endl;
            return;
        } */
        if (action == "acc") {
            cout << "String is accepted." << endl;
            return;
        } 
        else if (action[0] == 'S') {
            push(symbol);
            push(stoi(action.substr(1)));
            pos++;
        }
        else if (action[0] == 'R') {
            int ruleNum = stoi(action.substr(1));
            int newState = ruleNum;
            //Perform reduction (Needs to be shuffled around)
            switch (ruleNum) {
                case 1:
                    //F -> id
                    pop(); // Pop id
                    pop(); // Pop F
                    push(1); // Push non-terminal T
                    break;
                case 2:
                    //T -> T * F
                    pop(); //Pop F
                    pop(); //Pop *
                    pop(); //Pop T
                    pop(); //Pop F
                    pop(); //Pop *
                    pop(); //Pop T
                    push(1); //Push non-terminal T
                    break;
                case 3:
                    //T -> F
                    pop(); //Pop F
                    pop(); //Pop T
                    push(1); //Push non-terminal T
                    break;
                case 4:
                    //F -> ( E )
                    pop(); //Pop )
                    pop(); //Pop E
                    pop(); //Pop (
                    pop(); //Pop F
                    push(1); //Push non-terminal T
                    break;
                case 5:
                    //E -> E + T
                    pop(); //Pop T
                    pop(); //Pop +
                    pop(); //Pop E
                    pop(); //Pop T
                    push(1); //Push non-terminal T
                    break;
                default:
                    cout << "Error: Invalid reduction rule" << endl;
                    return;
            }
            //Determine new state based on the current state & nonterminal pushed
            //The code below is like putting duct tape because on a leaky pipe. Not a good solution
            auto newStateStr = parsingTable[peek()][action[0]];
            if (!newStateStr.empty()) {
                newState = stoi(newStateStr);
                push(newState); //Push the new state
            } 
            else {
                cout << "Error: Invalid new state" << endl;
                return;
            }
        }

        //ETF not implemented yet
        else if (action[0] == 'E') {
            
        }
        else if (action[0] == 'T') {
            
        }
        else if (action[0] == 'F') {
            
        }
        else {
            cout << "Error: Invalid action" << endl;
            return;
        }
    }
}

int main() {
    vector<string> testCases = {"(id+id)*id$", "id*id$", "(id*)$"};
    for (auto& testCase : testCases) {
        cout << "Input: " << testCase << endl;
        parseInput(testCase);
        while (!st.empty()) st.pop();
    }
    return 0;
}
