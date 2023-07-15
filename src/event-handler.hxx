#ifndef EVENT_HANDLER_HXX
#define EVENT_HANDLER_HXX

#include <SDL_events.h>

#include <user-intent.hxx>

namespace project {

class EventHandler
{
public:
	void handle_queued_events();
	void reset();

#define _attr [[nodiscard]]
	_attr user_intent_t intents() const;

	_attr bool intent_quit() const;
	_attr bool intent_reset_render() const;

	_attr bool intent_escape() const;
	_attr bool intent_up() const;
	_attr bool intent_down() const;
	_attr bool intent_left() const;
	_attr bool intent_right() const;
	_attr bool intent_shift() const;
	_attr bool intent_r() const;

	_attr bool window_resized() const;
#undef _attr

private:
	void handle_scancode(SDL_Scancode code, bool pressed);

	SDL_Event event;
	bool event_quit;
	bool event_window_resized;
	bool event_render_targets_reset;
	bool event_render_device_reset;
	bool key_escape;
	bool key_up;
	bool key_w;
	bool key_down;
	bool key_s;
	bool key_left;
	bool key_a;
	bool key_right;
	bool key_d;
	bool key_lshift;
	bool key_rshift;
	bool key_r;
};

}  // namespace project

#endif  // EVENT_HANDLER_HXX
