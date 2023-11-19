#include "diagram_state.h"

DiagramState::DiagramState(bool has_error, string& error_message) {
	this->has_error = has_error;
	this->error_message = error_message;
}

bool DiagramState::IsValid() {
	return !has_error;
}

string DiagramState::GetError() {
	return error_message;
}