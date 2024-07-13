#ifndef STRUCT_NFA
#define STRUCT_NFA

#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct NFA {
    map<string, map<string, set<string>>> transitions;
    string start_state;
    vector<string> accept_states;
};

#endif