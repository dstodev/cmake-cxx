#ifndef EVENT_HANDLER_HXX
#define EVENT_HANDLER_HXX

#include <SDL_events.h>

namespace project {

class EventHandler
{
public:
	void handle_queued_events();
	void reset();

#define ATTR [[nodiscard]]
	ATTR bool intent_quit() const;
	ATTR bool intent_reset_render() const;

	ATTR int mouse_xpos() const;
	ATTR int mouse_ypos() const;

	ATTR bool intent_escape() const;
	ATTR bool intent_up() const;
	ATTR bool intent_down() const;
	ATTR bool intent_left() const;
	ATTR bool intent_right() const;
	ATTR bool intent_shift() const;
	ATTR bool intent_r() const;

	ATTR bool window_resized() const;
#undef ATTR

private:
	void handle_scancode(SDL_Scancode code, bool pressed);

	SDL_Event event;
	bool event_quit;
	bool event_window_resized;
	bool event_render_targets_reset;
	bool event_render_device_reset;

	int mouse_x;
	int mouse_y;

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
