#ifndef UTILITIES
#define UTILITIES

#include <iostream>
#include <string>
#include <set>

#include "struct_nfa.hpp"

using namespace std;

set<string> get_all_inputs(NFA nfa);

std::string setToString(const std::set<std::string>& stringSet);

char find_greatest_key(const map<char, set<string>>& DFA_states_possible);

bool check_if_eCLosure_in_DFA_possible_states(map<char, set<string>> DFA_states_possible, set<string> eClosure);

void displaySet(const std::set<char>& charSet);

void displaySet(const std::set<std::string>& stringSet);

#endif