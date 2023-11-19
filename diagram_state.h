#include <cstring>
#include <iostream>

using namespace std;

class DiagramState
{
public:
	DiagramState(bool has_error, string& error_message);
	bool IsValid();
	string GetError();
private:
	bool has_error;
	string error_message;
};