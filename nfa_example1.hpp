#ifndef NFA_EXAMPLE1
#define NFA_EXAMPLE1

#include <set>
#include <string>
#include <map>
#include <iostream>

#include "struct_nfa.hpp"
#include "create_eClosure.hpp"
#include "find_next_starting_states.hpp"
#include "create_table.hpp"

using namespace std;

// -- The following functions below are for displaying and processing an example NFA -- //

void display_nfa_example1_eClosure_states(set<string> e_closure_states);

map<string, set<string>> display_next_eClosure(NFA nfa_example_1, set<string> e_closure_states);

// The main function for setting up the example NFA
NFA set_up_example_nfa();

#endif