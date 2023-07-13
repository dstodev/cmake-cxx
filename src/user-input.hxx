#ifndef USERINPUT_HXX
#define USERINPUT_HXX

#include <project-api.h>

namespace project {

struct user_input_t
{
	bool escape;
	bool up;
	bool w;
	bool down;
	bool s;
	bool left;
	bool a;
	bool right;
	bool d;
	bool lshift;
	bool rshift;
};

PROJECT_API extern user_input_t UserInput;

}  // namespace project

#endif  // USERINPUT_HXX
