#include "find_next_starting_states.hpp"

// Creates a list of 'auto included' states used for the next eClosure list entry. This will be used to expand the set of NFA transitions
map<string, set<string>> find_next_starting_states(NFA nfa, set<string> eClosure) {
    map<string, set<string>> new_starting_nfa_states;
    set<string> inputs_of_nfa = get_all_inputs(nfa);

    for (const string &input : inputs_of_nfa) {                                     // For each available input
        for (const string &state_transition : eClosure) {                           // For each state transition in the eClosure
            // Check if the state_transition exists in the transitions map
            if (nfa.transitions.find(state_transition) != nfa.transitions.end()) {
                const auto &current_state = nfa.transitions.at(state_transition);   // Starting from the state transition from the eClosure

                if (current_state.find(input) != current_state.end()) {             // Check if the available input leads to another state
                    const auto &x = current_state.at(input);                        // Set of all transitions resulting from the current evaluated input
                    new_starting_nfa_states[input].insert(x.begin(), x.end());      // Store all transitions from the current input
                }
            }
        }
    }
    return new_starting_nfa_states;
}