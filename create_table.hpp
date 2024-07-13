#ifndef CREATE_TABLE
#define CREATE_TABLE

#include <map>
#include <set>
#include <string>
#include <queue>

#include "struct_nfa.hpp"
#include "print_table.hpp"
#include "create_eClosure.hpp"
#include "find_next_starting_states.hpp"
#include "utilities.hpp"

using namespace std;

// This function sets up the first line table entry for the first DFA state. Subsequent DFA states and their transitions will automaticcaly be generated afterwards
// DFA states and their processing will be handled via a queue that calls forth functions to find  the next DFA starting states/auto-include states
// Then their expanded NFA transitions that represent the next DFA state transition from the current DFA state being evaluated
map<set<string>, map<set<char>, map<string, map<set<char>, set<string>>>>> create_table(NFA nfa,  set<string> first_state_eClosure, map<string, set<string>> beginning_NFA_states);

#endif