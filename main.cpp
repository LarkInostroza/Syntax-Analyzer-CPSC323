#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

// LR parsing table
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
    {10, {{'+', "R3"}, {'*', "R3"}, {')', "R3"}, {'$', "R3"}}},
    {11, {{'+', "R5"}, {'*', "R5"}, {')', "R5"}, {'$', "R5"}}}};

// Stack, push, pop, peek operations
stack<int> st;
void push(int item) { st.push(item); }
int pop() {
  int top = st.top();
  st.pop();
  return top;
}
int peek() { return st.top(); }

// Parse the input string
void parseInput(string input) {
  push(0);
  // input += "$"; Taken out since string already includes $
  int pos = 0;

  while (true) {
    int state = peek();
    char symbol = input[pos];
    string action = parsingTable[state][symbol];
    cout << "State : " << state << endl;
    cout << "Symbol: " << symbol << endl;
    if (action == "acc") {
      cout << "String is accepted." << endl;
      return;
    }
    // Shift action based on S#. EX: S5 shifts 5
    else if (action[0] == 'S') {
      char nextSymbol = input[pos];
      push(nextSymbol);
      int newState = stoi(action.substr(1));
      push(newState);
      pos++;
      if (input[pos] == 'd') {
        pos++;
      }
    }

    else if (action[0] == 'R') {
      int ruleNum = stoi(action.substr(1));
      cout << "Rule Reduction: "<< ruleNum << endl;
      // Perform reduction
      switch (ruleNum) {
      case 1:
        // E -> E + T
        pop();     // Pop T
        pop();     // Pop '+'
        pop();     // Pop E
        push('E'); // Push non-term E
        break;
      case 2:
        // E -> T
        pop();     // Pop T
        push('E'); // Push non-term E
        break;
      case 3:
        // T -> T * F
        pop();     // Pop F
        pop();     // Pop '*'
        pop();     // Pop T
        push('T'); // Push non-term T
        break;
      case 4:
        // T -> F
        pop();     // Pop F
        push('T'); // Push non-term T
        break;
      case 5:
        // F -> (E)
        pop();     // Pop ')'
        pop();     // Pop E
        pop();     // Pop '('
        push('F'); // Push non-term F
        break;
      case 6:
        // F -> id
        pop();     // Pop id
        push('F'); // Push non-term F
        break;

      default:
        cout << "Error: Invalid reduction rule: " << ruleNum << endl;
        return;
      }
    }

    // Reduction for E
    else if (action[0] == 'E') {
      cout << "Fired Reduction for E" << endl;
      char nextSymbol = input[pos];
      if (nextSymbol == '+' || nextSymbol == ')' || nextSymbol == '$') {
        pop();     // Pop T
        pop();     // Pop '+'
        pop();     // Pop E
        push('E'); // Push non-term E
      } else {
        cout << "Error: Invalid action E" << endl;
        return;
      }
    }
    // Reduction for T
    else if (action[0] == 'T') {
      char nextSymbol = input[pos];
      if (nextSymbol == '+' || nextSymbol == '*' || nextSymbol == ')' ||
          nextSymbol == '$') {
        pop();     // Pop F
        pop();     // Pop '*'
        pop();     // Pop T
        push('T'); // Push non-term T
      } else {
        cout << "Error: Invalid action T" << endl;
        return;
      }
    }
    // Reduction for F
    else if (action[0] == 'F') {
      char nextSymbol = input[pos];
      cout << "Fired Reduction for F" << endl;
      if (nextSymbol == '+' || nextSymbol == '*' || nextSymbol == ')' ||
          nextSymbol == '$') {
        pop(); // Pop id || ')'
        if (input[pos - 1] == '(') {
          pop(); // Pop '('
          pop(); // Pop E
          pop(); // Pop ')'
        }
        push('F'); // Push non-term F
      } else {
        cout << "Error: Invalid action F" << endl;
        return;
      }
    } else {
      cout << "Error: Invalid action" << endl;
      return;
    }
  }
}

int main() {
  vector<string> testCases = {"(id+id)*id$", "id*id$", "(id*)$"};
  for (auto &testCase : testCases) {
    cout << "Input: " << testCase << endl;
    parseInput(testCase);
    while (!st.empty()) {
      st.pop();
    }
  }

  return 0;
}
