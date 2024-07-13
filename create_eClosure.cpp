#include "create_eClosure.hpp"

set<string> create_eClosure(set<string> nfa_states, NFA nfa) {
    set<string> e_closure_list;
    stack<string> process_nfa;

    for (const auto& state : nfa_states) { // Add beginning NFA states into e-closure list
        e_closure_list.insert(state);
        process_nfa.push(state); // Push those beginning NFA states to start searching from a stack structure
    }

    while (!process_nfa.empty()) { // While there are elements in the stack, begin the search from the top of the stack
        string current_state = process_nfa.top();
        process_nfa.pop();

        // Check if the current_state exists in the transitions map
        if (nfa.transitions.find(current_state) != nfa.transitions.end()) {
            const auto& transitions = nfa.transitions.at(current_state); // Get the NFA starting from the element at the top of the stack

            if (transitions.find("epsilon") != transitions.end()) { // If there exists an epsilon transition from the current NFA evaluation...
                const auto& transitions_from_epsilon = transitions.at("epsilon"); // Get state transitions that resulted from epsilon inputs

                for (const auto& state_transition : transitions_from_epsilon) { // Traverse each state transition that resulted from epsilon inputs
                    if (e_closure_list.find(state_transition) == e_closure_list.end()) { // If the state transition is not already in the eClosure list
                        e_closure_list.insert(state_transition); // Add the state transition to the eClosure list
                        process_nfa.push(state_transition); // Push the state transition to the stack as the next state to check from
                    }
                }
            }
        }
    }

    return e_closure_list;
}