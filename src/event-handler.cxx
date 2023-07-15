#include "event-handler.hxx"

#include <log.hxx>

namespace project {

void EventHandler::handle_queued_events()
{
	event_quit = false;
	event_window_resized = false;
	event_render_targets_reset = false;
	event_render_device_reset = false;

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_QUIT: event_quit = true; return;
		case SDL_KEYDOWN: handle_scancode(event.key.keysym.scancode, true); break;
		case SDL_KEYUP: handle_scancode(event.key.keysym.scancode, false); break;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED: event_window_resized = true; break;
			default: break;
			}
			break;
		case SDL_RENDER_TARGETS_RESET:
			log::debug("Render targets reset\n");
			event_render_targets_reset = true;
			break;
		case SDL_RENDER_DEVICE_RESET:
			log::debug("Render device reset\n");
			event_render_device_reset = true;
			break;
		default: break;
		}
	}
}

void EventHandler::reset()
{
	*this = EventHandler {};
}

user_intent_t EventHandler::intents() const
{
	return {
	    intent_quit(),
	    intent_escape(),
	    intent_up(),
	    intent_down(),
	    intent_left(),
	    intent_right(),
	    intent_shift(),
	    intent_r(),
	};
}

bool EventHandler::intent_quit() const
{
	return event_quit;
}

bool EventHandler::intent_reset_render() const
{
	return event_render_targets_reset;
}

bool EventHandler::intent_escape() const
{
	return key_escape;
}

bool EventHandler::intent_up() const
{
	return key_up || key_w;
}

bool EventHandler::intent_down() const
{
	return key_down || key_s;
}

bool EventHandler::intent_left() const
{
	return key_left || key_a;
}

bool EventHandler::intent_right() const
{
	return key_right || key_d;
}

bool EventHandler::intent_shift() const
{
	return key_lshift || key_rshift;
}

bool EventHandler::intent_r() const
{
	return key_r;
}

bool EventHandler::window_resized() const
{
	return event_window_resized;
}

void EventHandler::handle_scancode(SDL_Scancode code, bool pressed)
{
	switch (code) {
	case SDL_SCANCODE_ESCAPE: key_escape = pressed; break;
	case SDL_SCANCODE_UP: key_up = pressed; break;
	case SDL_SCANCODE_W: key_w = pressed; break;
	case SDL_SCANCODE_DOWN: key_down = pressed; break;
	case SDL_SCANCODE_S: key_s = pressed; break;
	case SDL_SCANCODE_LEFT: key_left = pressed; break;
	case SDL_SCANCODE_A: key_a = pressed; break;
	case SDL_SCANCODE_RIGHT: key_right = pressed; break;
	case SDL_SCANCODE_D: key_d = pressed; break;
	case SDL_SCANCODE_LSHIFT: key_lshift = pressed; break;
	case SDL_SCANCODE_RSHIFT: key_rshift = pressed; break;
	case SDL_SCANCODE_R: key_r = pressed; break;
	default: break;
	}
}

}  // namespace project
