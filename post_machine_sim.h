#include "diagram_state.h"
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

/* Definitions */
#define NONE '$'
#define ACCEPT 1
#define REJECT 0

typedef struct state {
	string name;
	char read_symbol;
	char add_symbol;
} state;

typedef struct node {
	string name;
	list<state> flows;
} node;

class PostMachineSimulator
{
public:
	DiagramState* BuildDiagram();
	bool Run(string& input);
private:
	vector<node> flow_diagram;
	queue<char> work_queue;
	string start_state;
	string accept_state;

	int TraverseDiagram(int current_node);
};
