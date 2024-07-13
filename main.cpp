#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <set>
#include <stack>
#include <queue>

// Converts NFA to DFA by creating an epsilon-closure list and using that list to create a transition table for the DFA.

using namespace std;

string setToString(const std::set<std::string>& stringSet);

struct NFA {
    map<string, map<string, set<string>>> transitions;
    string start_state;
    vector<string> accept_states;
};

NFA nfa_example1(){
    NFA nfa_example1;
    nfa_example1.start_state = "q0";
    nfa_example1.accept_states = {"q0"};

    nfa_example1.transitions["q0"]["epsilon"] = {"q1", "q7"};
    nfa_example1.transitions["q1"]["epsilon"] = {"q2", "q4"};
    nfa_example1.transitions["q2"]["a"] = {"q3"};
    nfa_example1.transitions["q4"]["b"] = {"q5"};
    nfa_example1.transitions["q3"]["epsilon"] = {"q6"};
    nfa_example1.transitions["q5"]["epsilon"] = {"q6"};
    nfa_example1.transitions["q6"]["epsilon"] = {"q1", "q7"};
    nfa_example1.transitions["q7"]["a"] = {"q8"};
    nfa_example1.transitions["q8"]["b"] = {"q9"};
    nfa_example1.transitions["q9"]["b"] = {"q10"};

    return nfa_example1;
}

// Using the NFA and the 'automatically included' states that are inserted into the eClosure list,
// Create the set of NFA states that are to be represented by the DFA state
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

// Creates a list of 'auto included' states used for the next eClosure list entry. This will be used to expand the set of NFA transitions
map<string, set<string>> find_next_starting_states(NFA nfa, set<string> eClosure, char current_DFA_character = '\0') {
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
// -- The following functions below are for displaying and processing the example NFA -- //

void display_nfa_example1_eClosure_states(set<string> e_closure_states){
    cout << "The first eClosure list comprises of states";
    cout << "\n------------------------------------ \n";
    for(string nfa_states : e_closure_states){
        cout << nfa_states << " ";
    }
    cout << "\n__________________________________________\n";
}

map<string, set<string>> display_next_eClosure(NFA nfa_example_1, set<string> e_closure_states){
    map<string, set<string>> new_eClosure_nfa_states = find_next_starting_states(nfa_example_1, e_closure_states);
    cout << "\nThe new states to evaluate are ";
    cout << "\n------------------------------------ \n";
    for(const auto &pair : new_eClosure_nfa_states){
        const string &input = pair.first;
        const auto &transitions = pair.second;
        cout << "Input: " << input << " states - ";
        for(const auto &transition : transitions){
            cout << transition << " ";
        }
        cout << endl;
    }
    cout << "\n__________________________________________\n";

    return new_eClosure_nfa_states;
}

NFA set_up_example_nfa(){
    NFA nfa_example_1 = nfa_example1();
    set<string> starting_state_example_1;
    starting_state_example_1.insert(nfa_example_1.start_state);
    set<string> e_closure_states = create_eClosure(starting_state_example_1 , nfa_example_1);

    display_nfa_example1_eClosure_states(e_closure_states);

    map<string, set<string>> beginning_e_closure_states = display_next_eClosure(nfa_example_1, e_closure_states);

    map<string, set<string>> new_eClosure_nfa_states = find_next_starting_states(nfa_example_1, e_closure_states);

    create_table(nfa_example_1, e_closure_states, beginning_e_closure_states);

    return nfa_example_1;
}

// -- The following functions above are for displaying and processing the example NFA -- //

int main(){
    cout << "Beginning" << endl;

    NFA nfa_example_1 = set_up_example_nfa();
    
    cout << "\nFinishing" << endl;
}

