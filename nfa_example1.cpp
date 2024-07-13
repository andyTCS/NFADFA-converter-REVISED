#include "nfa_example1.hpp"

// -- The following functions below are for displaying and processing an example NFA -- //

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

// The main function for setting up the example NFA
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