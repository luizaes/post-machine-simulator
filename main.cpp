#include "post_machine_sim.h"

int main() {

	string input_word;
	bool output;
	auto pm = new PostMachineSimulator();

	DiagramState* diagram = pm->BuildDiagram();

	if(!diagram->IsValid()) {
		cout << diagram->GetError() << endl;
		return 0;
	}

	cout << "[SUCCESS] Post Machine created successfuly." << endl;

	cin >> input_word;

	output = pm->Run(input_word);

	if(output == ACCEPT) {
		cout << "[ACCEPT] The input \"" << input_word << "\" was accepted by the machine." << endl;
	} else {
		cout << "[REJECT] The input \"" << input_word << "\" was rejected by the machine." << endl;
	}

	return 0;
}
