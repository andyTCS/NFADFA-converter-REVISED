#ifndef FIND_NEXT_STARTING_STATES
#define FIND_NEXT_STARTING_STATES

#include <map>
#include <string>
#include <set>

#include "struct_nfa.hpp"
#include "utilities.hpp"

using namespace std;

// Creates a list of 'auto included' states used for the next eClosure list entry. This will be used to expand the set of NFA transitions
map<string, set<string>> find_next_starting_states(NFA nfa, set<string> eClosure);

#endif