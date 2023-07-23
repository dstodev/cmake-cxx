#ifndef EVENT_HANDLER_HXX
#define EVENT_HANDLER_HXX

#include <SDL_events.h>

#include <point_t.hxx>

namespace project {

class EventHandler
{
public:
	void update_event_state();
	void reset();

#define ATTR [[nodiscard]]
	ATTR bool signal_quit() const;
	ATTR bool signal_refresh_render() const;
	ATTR bool signal_window_resized() const;

	ATTR auto mouse_pos() const -> point_t<int> const&;
	ATTR bool mouse_left() const;
	ATTR bool mouse_right() const;

	ATTR bool intent_escape() const;
	ATTR bool intent_up() const;
	ATTR bool intent_down() const;
	ATTR bool intent_left() const;
	ATTR bool intent_right() const;
	ATTR bool intent_shift() const;

	ATTR bool key_r() const;
#undef ATTR

private:
	void handle_scancode(SDL_Scancode code, bool pressed);

	SDL_Event _event{};

	bool _signal_quit{};
	bool _signal_window_resized{};
	bool _signal_render_targets_reset{};
	bool _signal_render_device_reset{};

	point_t<int> _mouse_pos{};
	bool _mouse_left{};
	bool _mouse_right{};

	bool _key_escape{};
	bool _key_up{};
	bool _key_w{};
	bool _key_down{};
	bool _key_s{};
	bool _key_left{};
	bool _key_a{};
	bool _key_right{};
	bool _key_d{};
	bool _key_lshift{};
	bool _key_rshift{};
	bool _key_r{};
};

}  // namespace project

#endif  // EVENT_HANDLER_HXX
