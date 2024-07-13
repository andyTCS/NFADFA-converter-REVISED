#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <set>
#include <stack>
#include <queue>

#include "struct_nfa.hpp"
#include "utilities.hpp"
#include "create_eClosure.hpp"
#include "create_table.hpp"
#include "find_next_starting_states.hpp"
#include "nfa_example1.hpp"
#include "print_table.hpp"

// Converts NFA to DFA by creating an epsilon-closure list and using that list to create a transition table for the DFA.

using namespace std;


int main(){
    cout << "Beginning" << endl;

    NFA nfa_example_1 = set_up_example_nfa();
    
    cout << "\nFinishing" << endl;
}

