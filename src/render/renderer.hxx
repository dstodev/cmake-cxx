#ifndef RENDERER_HXX
#define RENDERER_HXX

#include <unordered_map>

#include <factory.hxx>
#include <square.hxx>

struct SDL_Window;
using SDL_GLContext = void*;  // from SDL_video.h

namespace project {

template <typename T>
class Point;

class EventHandler;
class Player;
class Simulation;

class Renderer
{
public:
	explicit Renderer(EventHandler const& handler);

	Renderer(Renderer const& copy) = delete;
	Renderer(Renderer&& move) noexcept;

	Renderer& operator=(Renderer const& copy) = delete;
	Renderer& operator=(Renderer&& move) noexcept;

	void init(SDL_Window* window);
	void deinit();

	void refresh();
	void render();
	void clear();

	void draw(Simulation const& simulation);

protected:
	void draw(Player const& player, Point<int> const& view_center);
	void draw(Point<int> const& point);

private:
	std::reference_wrapper<EventHandler const> _handler;
	SDL_Window* _window;
	SDL_GLContext _context;

	shader::Factory _shaders;
	vao::Square _squares;
};

}  // namespace project

#endif
