#ifndef PRINT_TABLE
#define PRINT_TABLE

#include <map>
#include <set>
#include <string>
#include <iostream>

#include "utilities.hpp"

using namespace std;

// Main function that displays the NFA-DFA table conversion
void print_table(map<set<string>, map<set<char>, map<string, map<set<char>, set<string>>>>> table);

#endif