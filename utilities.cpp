#include "utilities.hpp"

// Returns a set of all inputs used in the NFA that are not epsilon
set<string> get_all_inputs(NFA nfa){
    set<string> all_inputs;
    for(const auto &outer_map: nfa.transitions){
        const string &state = outer_map.first;
        const auto &inner_map = outer_map.second;

        for(const auto &inner_map_elements : inner_map){
            const string &input = inner_map_elements.first;
            
            if(input != "epsilon"){ all_inputs.insert(input); }
        }
    }

    return all_inputs;
}

// Utility function used to display NFA state transitions and such 
std::string setToString(const std::set<std::string>& stringSet) {
    std::string result;
    for (const auto& str : stringSet) {
        result += str;
    }
    return result;
}

// Utility function used to help determine what a new DFA state should be labeled
char find_greatest_key(const map<char, set<string>>& DFA_states_possible) {
    if (DFA_states_possible.empty()) {
        throw runtime_error("The map is empty, no keys to find.");
    }

    char greatest_key = '\0';
    for (const auto& pair : DFA_states_possible) {
        if (pair.first > greatest_key) {
            greatest_key = pair.first;
        }
    }
    return greatest_key;
}

// Utility function used to check if the NFA state transitions was already observed and labeled. Helps to label DFA transitions with existing DFA state
bool check_if_eCLosure_in_DFA_possible_states(map<char, set<string>> DFA_states_possible, set<string> eClosure){
    for(const auto &possible : DFA_states_possible){
        const char &DFA_state = possible.first;
        const auto &NFA_possible = possible.second;
        if(NFA_possible == eClosure){
            return true;
        }
    }
    return false;
}

// Utility function to display a set of chars
void displaySet(const std::set<char>& charSet) {
    std::cout << "{ ";
    for (char c : charSet) {
        std::cout << c << " ";
    }
    std::cout << "}";
}

// Utility function to display a set of strings
void displaySet(const std::set<std::string>& stringSet) {
    std::cout << "{ ";
    for (const std::string& str : stringSet) {
        std::cout << str << " ";
    }
    std::cout << "}";
}