#ifndef CREATE_ECLOSURE
#define CREATE_ECLOSURE

#include <set>
#include <stack>
#include <string>
#include <unordered_map>

#include "struct_nfa.hpp"

using namespace std;

set<string> create_eClosure(set<string> nfa_states, NFA nfa);

#endif