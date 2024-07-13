#include "create_table.hpp"

// This function sets up the first line table entry for the first DFA state. Subsequent DFA states and their transitions will automaticcaly be generated afterwards
// DFA states and their processing will be handled via a queue that calls forth functions to find  the next DFA starting states/auto-include states
// Then their expanded NFA transitions that represent the next DFA state transition from the current DFA state being evaluated
map<set<string>, map<set<char>, map<string, map<set<char>, set<string>>>>> create_table(NFA nfa,  set<string> first_state_eClosure, map<string, set<string>> beginning_NFA_states){
    map<set<string>, map<set<char>, map<string, map<set<char>, set<string>>>>> table;
    // NFA state     DFA state       input         DFA transition   NFA transitions

    queue<pair<char, set<string>>> DFA_states_to_process;                                     // Add and process states from a queue
    map<char, set<string>> DFA_states_possible;                                               // Store previously observed DFA states and its NFA state transitions representations here
    
    char starting_DFA_state = 'A';                                                            // Start from DFA state A
    char DFA_state_transition = starting_DFA_state + 1;                                       // State A transitions to state B

    table[{first_state_eClosure}][{starting_DFA_state}];                                      // Example NFA state transitions {0,1,2,4,7} associated with A
    DFA_states_possible.insert(make_pair(starting_DFA_state, first_state_eClosure));
  
    DFA_states_to_process.push(make_pair(starting_DFA_state, first_state_eClosure));
    map<char, set<string>> DFA_states_processed;


    for(const auto& beginning_states : beginning_NFA_states){                                               
        const string &input = beginning_states.first;                                                       // Input that lead to the transitions - a , b
        const auto &transitions = beginning_states.second;                                                  // Set of 'auto included' states

        set<string> e_closure_states = create_eClosure(transitions, nfa);                                   // Create eClosure states from 'auto included' states
                                                                                                            // Which are the fully expanded NFA state transitions frmo those auto included states
        
        table[{first_state_eClosure}][{starting_DFA_state}][input][{DFA_state_transition}] = e_closure_states;// Example : transition_table[{'A'}][b][{'C'}] = {q1 q2 q4 q5 q6 q7}
                                                                                                              // state A on input b goes to state C where C = {q1 q2 q4 q5 q6 q7}

        

        DFA_states_possible.insert(make_pair(DFA_state_transition, e_closure_states));                      // Calculate NFA mappings for corresponding DFA transitions

        if(DFA_states_processed.find(DFA_state_transition) == DFA_states_processed.end()){                  // If the state is not already found in the processed set, queue it
            DFA_states_to_process.push(make_pair(DFA_state_transition, e_closure_states));
        }
        DFA_state_transition++;                                                                             // Increment the next letter for the DFA state transition
    }
    DFA_states_to_process.pop();                                                                            // Remove 'A' from the queue since we processed it
    DFA_states_processed.insert(make_pair(starting_DFA_state, first_state_eClosure));                       // and add 'A' to the set of processed states with its NFA representation
    
    print_table(table);
    cout << "\n_____________________________________________________________________________________\n";

    // At this stage, the first DFA state A has been processed with states B and C queued to be processed next

    while(!DFA_states_to_process.empty()){  // While the stack of states to process isn't empty
        pair<char, set<string>> queue_front = DFA_states_to_process.front(); // Grab the next DFA state to process and pop it
        DFA_states_to_process.pop();


        char dfa_state = queue_front.first;
        set<string> nfa_transitions_for_dfa = queue_front.second;
        set<string> auto_included_nfa_states = create_eClosure(nfa_transitions_for_dfa, nfa);   // 

        // From the NFA state transition that represents the current DFA state, find the next starting states for each input
        map<string, set<string>> next_starting_states_nfa = find_next_starting_states(nfa, auto_included_nfa_states);
        
        // Step through the generated next starting states for each DFA transition on each input
        for(const auto &next_states : next_starting_states_nfa){
            
            const string &input = next_states.first;
            const auto &next_states_nfa = next_states.second;
            set<string> new_nfa_representations = create_eClosure(next_states_nfa, nfa);        // Expand the next states into their entire possible NFA transitions from epsilon inputs
            
            cout << "new: " << setToString(new_nfa_representations) << " test: " << setToString(nfa_transitions_for_dfa) << endl;
            
            // If a set of NFA state transitions that are new have been generated, this represents a new DFA state to add to the table.
            // Create a new DFA state which is the next letter in the alphabet to represent this state
            if(!check_if_eCLosure_in_DFA_possible_states(DFA_states_possible, new_nfa_representations)){
                char next_dfa_state = find_greatest_key(DFA_states_possible);
                next_dfa_state += 1;

                table[nfa_transitions_for_dfa][{dfa_state}][input][{next_dfa_state}] = new_nfa_representations;
                DFA_states_possible.insert(make_pair(next_dfa_state, new_nfa_representations));
                DFA_states_to_process.push(make_pair(next_dfa_state, new_nfa_representations));
                // print_table(table);
            }
            // An existing set of NFA state transitions have been recognized from the set of observed state transitions.
            // Use its existing DFA state representation to fill in the table as a DFA state transition
            else{ 
                char existing_dfa_transition = '\0';
                for(const auto &existing_pair : DFA_states_possible){
                    const char &existing_dfa_state = existing_pair.first;
                    const auto &nfa_transitions = existing_pair.second;
                    

                    if(nfa_transitions == new_nfa_representations){
                        existing_dfa_transition = existing_dfa_state;
                        table[nfa_transitions_for_dfa][{dfa_state}][input][{existing_dfa_transition}] = new_nfa_representations;
                        // print_table(table);
                    }
                }
            }
        }
    }

    print_table(table);
    return table;
}