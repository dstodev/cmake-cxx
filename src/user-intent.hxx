#ifndef USER_INTENT_HXX
#define USER_INTENT_HXX

#include <project-api.h>

namespace project {

struct user_intent_t
{
	void clear();

	bool quit;
	bool escape;
	bool up;
	bool down;
	bool left;
	bool right;
	bool shift;
	bool r;
};

PROJECT_API extern user_intent_t UserIntent;

}  // namespace project

#endif  // USER_INTENT_HXX
