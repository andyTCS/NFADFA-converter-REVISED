#include "print_table.hpp"

// Main function that displays the NFA-DFA table conversion
void print_table(map<set<string>, map<set<char>, map<string, map<set<char>, set<string>>>>> table){
    cout << "Displaying table" << endl;
    for(const auto & outer_map : table){                                                                        // For each NFA transitions set and outer middle map
        const set<string> &nfa_representation = outer_map.first;
        const auto &middle_outer_map = outer_map.second;

        cout << "\nNFA representation " << setToString(nfa_representation) << endl;                             // Display the NFA transitions that represent the current DFA key

        for(const auto &middle_outer_map_elements : middle_outer_map){                                          // For each middle map: DFA state and inner middle map
            const set<char> &dfa_representation = middle_outer_map_elements.first;
            const auto &middle_inner_map = middle_outer_map_elements.second;
            
            for(const auto &dfa_states : dfa_representation){
                cout << "    " << dfa_states << endl;                                                           // Display the DFA state of the current DFA key

                for(const auto &middle_inner_map_elements : middle_inner_map){                                  // For each inner middle map: NFA inputs and inner map
                const string &inputs = middle_inner_map_elements.first;
                const auto &inner_map = middle_inner_map_elements.second;

                cout << "        " << inputs << endl;                                                           // Display the input used to get to that NFA/DFA state transition

                for(const auto &inner_map_elements : inner_map){                                                // For each inner map: DFA transition on input and NFA transitions that represent that DFA transition
                    const set<char> &dfa_transition = inner_map_elements.first;
                    const auto &dfa_transition_nfa_representation = inner_map_elements.second;

                    for(const auto & dfa : dfa_transition){

                        cout << "            " << dfa << endl;                                                  // Display the DFA transition

                        cout << "                " << setToString(dfa_transition_nfa_representation) << endl;   // Display the NFA transition
                    }
                    cout << endl;
                }
            }
            }    
        }
    }
}