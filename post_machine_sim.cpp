#include "post_machine_sim.h"

void ReadInputLine(string& line) {
	getline(cin, line);

	/* Fix so that the program can run for both Windows and Linux file formats */
	if(!line.empty() && *line.rbegin() == '\r') {
    	line.erase( line.length()-1, 1);
	}
}

int PostMachineSimulator::TraverseDiagram(int current_node) {

	char symbol, current_queue_symbol = NONE;
	bool transition_found = false;
	string name;
	list<state>::iterator it;

	/*
		If the work queue is empty and we haven't reached the accepting state, reject entry
		If we reached the accepting state, accept entry
	*/ 
	if(flow_diagram[current_node].name.compare(accept_state) == 0) {
		return ACCEPT;
	} else if(work_queue.empty()) {
		return REJECT;
	}

	/* If current state is READ */
	if(flow_diagram[current_node].name.front() == 'R' || flow_diagram[current_node].name.front() == 'r') {
		
		/* Check first element of the queue */
		symbol = work_queue.front();
		for(it = flow_diagram[current_node].flows.begin(); it != flow_diagram[current_node].flows.end(); it++) {
			if((*it).read_symbol == symbol) {
				transition_found = true;
				name = (*it).name;
				current_queue_symbol = (*it).add_symbol;
				break;
			}
		}

		/* If the transition was not found, the machine halts by indefinition */
		if(!transition_found) {
			return REJECT;
		} else {
			/* Remove element from the queue, since it was already read */
			work_queue.pop();
			for(int i = 0; i < flow_diagram.size(); i++) {
				if(name.compare(flow_diagram[i].name) == 0) {
					if(current_queue_symbol != NONE) {
						work_queue.push(current_queue_symbol);
					}
					return TraverseDiagram(i);
				}
			}
		}

	/* If current state is ADD or START */
	} else {
		name = flow_diagram[current_node].flows.front().name;
		current_queue_symbol = flow_diagram[current_node].flows.front().add_symbol;

		for(int i = 0; i < flow_diagram.size(); i++) {
			if(name.compare(flow_diagram[i].name) == 0) {
				if(current_queue_symbol != NONE) {
					work_queue.push(current_queue_symbol);
				}
				return TraverseDiagram(i);
			}
		}
	}
	return REJECT;
}

DiagramState* PostMachineSimulator::BuildDiagram() {
	string state_name, input_entry, transition, error_text;
	int number_states, number_transitions, output, start_index = 0;
	node node_aux;
	state state_aux;
	bool error_found = false, state_found = false;
	char symbol, current_queue_symbol = NONE;

	/* Read the number of states the Post Machine has */
	cin >> number_states;

	/* Read the name of each of these states, storing them in an adjacency list */
	for (int i = 0; i < number_states; i++) {
		cin >> state_name;
		node_aux.name = state_name;
		flow_diagram.push_back(node_aux);
	}

	/* Read the name of the starting state */
	cin >> start_state;

	/* Read the name of the accepting state - we don't have a rejection state, 
	   the machine halts and rejects an input by indefinition */
	cin >> accept_state;

	/* Read the number of transitions present in the diagram */
	cin >> number_transitions;

	/* Read each transition */
	for(int i = 0; i < number_transitions; i++) {
		cin >> transition;
		state_found = false;

		for (int j = 0; j < number_states; j++) {
			/* If the state specified in the transition exists */
			if(flow_diagram[j].name.compare(transition) == 0) {
				state_found = true;

				/* If the transition starts with a READ state */
				if(transition.front() == 'R' || transition.front() == 'r') {
					cin >> symbol;
					ReadInputLine(transition);

					if(transition.compare(start_state) == 0) {
						string message = "[ERROR] Invalid transition: it's not possible to have a transition pointing to the starting state.";
						DiagramState* response = new DiagramState(/*has_error=*/true, message);
						return response;
					} else if(transition.compare(" " + accept_state) != 0 && (transition.at(1) == 'A' || transition.at(1) == 'a')) {
						current_queue_symbol = transition.substr(transition.length()-1, 1).front();
						transition = transition.substr(1, transition.length()-3);
					} else {
						transition = transition.substr(1, transition.length()-1);
					}

					state_aux.name = transition;
					state_aux.read_symbol = symbol;
					state_aux.add_symbol = current_queue_symbol;
					current_queue_symbol = NONE;
					flow_diagram[j].flows.push_back(state_aux);

				/* If the transition starts with a START state */
				} else if(transition.compare(start_state) == 0) {
					ReadInputLine(transition);

					if(transition.at(1) == 'A' || transition.at(1) == 'a') {
						current_queue_symbol = transition.substr(transition.length()-1, 1).front();
						transition = transition.substr(1, transition.length()-3);
					}

					state_aux.name = transition;
					state_aux.read_symbol = NONE;
					state_aux.add_symbol = current_queue_symbol;
					current_queue_symbol = NONE;
					flow_diagram[j].flows.push_back(state_aux);
				}  else if(transition.compare(accept_state) == 0) {
					string message = "[ERROR] Invalid transition: the accepting state cannot have a transition to any other state.";
					DiagramState* response = new DiagramState(/*has_error=*/true, message);
					return response;
				/* If the transition starts with an ADD state */
				} else if(transition.front() == 'A' || transition.front() == 'a') {
					cin >> symbol;
					ReadInputLine(transition);
					if(transition.compare(" " + start_state) == 0) {
						string message = "[ERROR] Invalid transition: it's not possible to have a transition pointing to the starting state.";
						DiagramState* response = new DiagramState(/*has_error=*/true, message);
						return response;
					} else if(transition.at(1) == 'A' || transition.at(1) == 'a') {
						current_queue_symbol = transition.substr(transition.length()-1, 1).front();
						transition = transition.substr(1, transition.length()-3);
					} else {
						transition = transition.substr(1, transition.length()-1);
					}
					state_aux.name = transition;
					state_aux.read_symbol = NONE;
					state_aux.add_symbol = current_queue_symbol;
					current_queue_symbol = NONE;
					flow_diagram[j].flows.push_back(state_aux);
				}
				break;
			}
		}

		if(!state_found) {
			string message = "[ERROR] Invalid transition: the specified state does not exist.";
			DiagramState* response = new DiagramState(/*has_error=*/true, message);
			return response;
		}
	}
	string message = "";
	DiagramState* response = new DiagramState(/*has_error=*/false, message);
	return response;
}

bool PostMachineSimulator::Run(string& input) {
	int start_index;

	for(int i = 0; input[i] != '\0'; i++) {
		work_queue.push(input[i]);
	}

	for(int i = 0; i < flow_diagram.size(); i++) {
		if(flow_diagram[i].name.compare(start_state) == 0) {
			start_index = i;
			break;
		}
	}

	/* Process the input word through the working queue and flow diagram */
	return TraverseDiagram(start_index);
}
