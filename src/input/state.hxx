#ifndef VIRTUAL_INPUT_HXX
#define VIRTUAL_INPUT_HXX

#include <project-api.h>

namespace project::input {

struct PROJECT_API State
{
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool esc = false;
};

/// Read the current input state by cloning it.
PROJECT_API State clone();

/// Update the current input state.
PROJECT_API void update(State const& input);

}  // namespace project::input

#endif  // VIRTUAL_INPUT_HXX
