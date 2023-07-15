#include "user-intent.hxx"

namespace project {

static user_intent_t UserIntent;

void user_intent_t::clear()
{
	*this = user_intent_t {};
}

}  // namespace project
